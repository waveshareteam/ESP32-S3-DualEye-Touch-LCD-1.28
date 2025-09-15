#include "I2S_Driver.h"

static esp_err_t on_sr_fill_My(void *arg, void *out, size_t len, size_t *bytes_read, uint32_t timeout_ms) {
  return ((ESP_SR_Class_My *)arg)->_fill_My(out, len, bytes_read, timeout_ms);
}

static void on_sr_event_My(void *arg, sr_event_t event, int command_id, int phrase_id) {
  ((ESP_SR_Class_My *)arg)->_sr_event_My(event, command_id, phrase_id);
}

ESP_SR_Class_My::ESP_SR_Class_My() : cb(NULL), i2s(NULL) {}

ESP_SR_Class_My::~ESP_SR_Class_My() {
  end_My();
}

void ESP_SR_Class_My::onEvent_My(sr_cb event_cb) {
  cb = event_cb;
}

bool ESP_SR_Class_My::begin_My(I2SClass &_i2s, const sr_cmd_t *sr_commands, size_t sr_commands_len, sr_channels_t rx_chan, sr_mode_t mode) {
  i2s = &_i2s;
  esp_err_t err = sr_start(on_sr_fill_My, this, rx_chan, mode, sr_commands, sr_commands_len, on_sr_event_My, this);
  return (err == ESP_OK);
}

bool ESP_SR_Class_My::end_My(void) {
  return sr_stop() == ESP_OK;
}

bool ESP_SR_Class_My::setMode_My(sr_mode_t mode) {
  return sr_set_mode(mode) == ESP_OK;
}

bool ESP_SR_Class_My::pause_My(void) {
  return sr_pause() == ESP_OK;
}

bool ESP_SR_Class_My::resume_My(void) {
  return sr_resume() == ESP_OK;
}

void ESP_SR_Class_My::_sr_event_My(sr_event_t event, int command_id, int phrase_id) {
  if (cb) {
    cb(event, command_id, phrase_id);
  }
}
// esp_err_t ESP_SR_Class_My::_fill_My(void *out, size_t len, size_t *bytes_read, uint32_t timeout_ms) {
//   if (i2s == NULL) {
//     return ESP_FAIL;
//   }
//   i2s->setTimeout(timeout_ms);
//   *bytes_read = i2s->readBytes((char *)out, len);
//   return (esp_err_t)i2s->lastError();
// }


// 立方插值函数 (Cubic Interpolation)
float cubic_interpolate(float p0, float p1, float p2, float p3, float t) {
    float a0, a1, a2, a3;
    a0 = p3 - p2 - p0 + p1;
    a1 = p0 - p1 - a0;
    a2 = p2 - p0;
    a3 = p1;

    return a0 * t * t * t + a1 * t * t + a2 * t + a3;
}

// 高精度重采样函数（立方插值） (High-precision resampling with cubic interpolation)
void resample_audio(char* in_data, size_t in_len, uint32_t in_rate, char* out_data, size_t out_len, uint32_t out_rate) {
  // 计算输入和输出采样率的比例 (Calculate the sample rate ratio)
  float sample_rate_ratio = (float)in_rate / (float)out_rate;

  // // 打印输入数据预览
  // printf("Input data preview:\n");
  // for (size_t i = 0; i < in_len && i < 50*sample_rate_ratio; ++i) {  // 限制打印前10个值
  //     printf("%d ", (unsigned char)in_data[i]);
  // }
  // printf("\n");

  for (size_t i = 0; i < out_len; ++i) {
    // 计算对应输入数据的索引 (Calculate the corresponding index in the input data)
    float index_in_input = i * sample_rate_ratio;
    size_t int_index = (size_t)index_in_input;

    // 使用立方插值法 (Cubic interpolation)
    if (int_index >= 1 && int_index + 2 < in_len) {
        // 获取四个邻近的数据点进行立方插值 (Get four neighboring data points for cubic interpolation)
        float t = index_in_input - int_index;  // 小数部分，用于插值 (Fractional part for interpolation)
        
        // 使用 `char` 类型数据进行立方插值
        out_data[i] = (char)cubic_interpolate((float)(in_data[int_index - 1]), 
                                              (float)(in_data[int_index]), 
                                              (float)(in_data[int_index + 1]), 
                                              (float)(in_data[int_index + 2]), t);
    } else {
        // 如果无法进行立方插值，使用直接值 (If cubic interpolation isn't possible, use the nearest value)
        out_data[i] = in_data[int_index];
    }
  }
  // // 打印输出数据预览
  // printf("Output data preview:\n");
  // for (size_t i = 0; i < out_len && i < 50; ++i) {  // 限制打印前10个值
  //     printf("%d ", (unsigned char)out_data[i]);
  // }
  // printf("\n");
}

// I2S 读取填充函数 (Filling function for I2S)
esp_err_t ESP_SR_Class_My::_fill_My(void *out, size_t len, size_t *bytes_read, uint32_t timeout_ms) {
  if (i2s == NULL) {
    return ESP_FAIL;
  }
  i2s->setTimeout(timeout_ms);
  
  uint32_t current_sample_rate = GetInputSampleRate();  // 这里你可以根据你的需求获取当前 I2S 采样率
  size_t input_len = (size_t)(len * (current_sample_rate / 16000.0));  // 计算重采样后的数据长度
  size_t output_len = len;  // 输出长度

  // 动态分配内存给输入和输出缓冲区
  char* input_data = (char*)malloc(input_len * sizeof(char));  // 动态分配输入缓冲区
  char* output_data = (char*)malloc(output_len * sizeof(char));  // 动态分配输出缓冲区
  
  if (input_data == NULL || output_data == NULL) {
    free(input_data);  // 错误时释放已分配的内存
    free(output_data);
    return ESP_FAIL;  // 内存分配失败
  }

  *bytes_read = i2s->readBytes((char *)input_data, input_len);  // 从 I2S 读取音频数据
  if (*bytes_read == 0) {
    free(input_data);  // 读取失败时释放内存
    free(output_data);
    return ESP_FAIL;  // 没有数据读取到，直接返回失败
  }

  // 调用重采样函数，将输入数据重采样为 16000 Hz
  size_t new_output_len = (size_t)((*bytes_read) / (float)input_len * output_len);
  resample_audio(input_data, *bytes_read, current_sample_rate, output_data, new_output_len, 16000);

  *bytes_read = new_output_len;  // 更新已读取的字节数

  // 将重采样后的数据复制回输出缓冲区
  memcpy(out, output_data, new_output_len * sizeof(char));

  // 释放动态分配的内存
  free(input_data);
  free(output_data);

  return ESP_OK;
}
ESP_SR_Class_My ESP_SR_My;