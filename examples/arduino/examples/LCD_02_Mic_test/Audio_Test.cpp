#include "Audio_Test.h"

#include "Audio_ES8311.h"
#include "Button_Driver.h"
#include "I2C_Driver.h"
#include "I2S_Driver.h"
#include "MIC_MSM.h"
#include "music.h"

#include "Arduino.h"
#include "esp_heap_caps.h"

// Audio configuration parameters
static const uint32_t MUSIC_SAMPLE_RATE = 24000;    // Sample rate for built-in music playback
static const uint32_t RECORD_SAMPLE_RATE = 16000;   // Sample rate for microphone recording
static const uint32_t RECORD_MAX_MS = 5000;         // Maximum recording duration in milliseconds (5 seconds)
static const size_t RECORD_MAX_BYTES = RECORD_SAMPLE_RATE * 2 * sizeof(int16_t) * RECORD_MAX_MS / 1000; // Total recording buffer size
static const uint8_t OUTPUT_VOLUME = 75;            // ES8311 hardware volume (0-100, optimal linear range 70-80)

// FreeRTOS task handles
static TaskHandle_t music_task_handle = NULL;        // Handle for music playback task
static TaskHandle_t record_task_handle = NULL;       // Handle for voice recording task

// Audio state flags (volatile for cross-task communication)
static volatile bool music_active = false;           // True when music is currently playing
static volatile bool music_paused = false;           // True when music playback is paused
static volatile bool music_stop_requested = false;   // Flag to request music task termination
static volatile bool recording_active = false;       // True when recording is in progress
static volatile bool record_stop_requested = false;  // Flag to request recording task termination
static volatile bool recording_playback_active = false; // True when playing back recorded audio

// Audio data buffers
static uint8_t * record_buffer = NULL;               // Buffer to store recorded PCM data (PSRAM preferred)
static size_t recorded_bytes = 0;                     // Number of valid bytes in record buffer

static void stop_music(void)
{
  music_stop_requested = true;
  music_paused = false;
  while (music_active) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

static void write_all_i2s(const uint8_t * data, size_t len)
{
  size_t written = 0;
  while (written < len) {
    size_t n = i2s.write(data + written, len - written);
    if (n == 0) {
      vTaskDelay(pdMS_TO_TICKS(1));
    } else {
      written += n;
    }
  }
}

static void music_task(void * parameter)
{
  (void)parameter;

  music_active = true;
  music_stop_requested = false;
  music_paused = false;

  Audio_PA_EN();
  Audio_SetSampleRate(MUSIC_SAMPLE_RATE);

  int16_t stereo_chunk[256 * 2];
  size_t sample_index = 0;

  while (sample_index < AUDIO_SAMPLES && !music_stop_requested) {
    while (music_paused && !music_stop_requested) {
      vTaskDelay(pdMS_TO_TICKS(20));
    }

    size_t frames = 0;
    while (frames < 256 && sample_index < AUDIO_SAMPLES) {
      int16_t sample = (int16_t)audio_data[sample_index++];
      stereo_chunk[frames * 2] = sample;
      stereo_chunk[frames * 2 + 1] = 0;
      frames++;
    }

    write_all_i2s((const uint8_t *)stereo_chunk, frames * 2 * sizeof(int16_t));
  }

  music_active = false;
  music_paused = false;
  music_stop_requested = false;
  music_task_handle = NULL;
  vTaskDelete(NULL);
}

/**
 * @brief Start music playback or toggle pause/resume state
 * Prevents music playback during recording or recording playback
 */
static void start_or_toggle_music(void)
{
  if (recording_active || recording_playback_active) {
    return;
  }

  if (music_active) {
    music_paused = !music_paused;
    Serial.println(music_paused ? "Music paused" : "Music resumed");
    return;
  }

  xTaskCreatePinnedToCore(
    music_task,
    "MusicTask",
    4096,
    NULL,
    4,
    &music_task_handle,
    1
  );
}

/**
 * @brief Play back the most recently recorded audio data
 * Does nothing if no valid recording exists in the buffer
 */
static void play_recording(void)
{
  if (recorded_bytes == 0 || record_buffer == NULL) {
    Serial.println("No recording data");
    return;
  }

  recording_playback_active = true;
  Audio_PA_EN();
  Audio_SetSampleRate(RECORD_SAMPLE_RATE);
  write_all_i2s(record_buffer, recorded_bytes);
  recording_playback_active = false;
  Serial.println("Recording playback done");
}

static void record_task(void * parameter)
{
  (void)parameter;

  recorded_bytes = 0;
  record_stop_requested = false;
  Audio_PA_DIS();
  Audio_SetSampleRate(RECORD_SAMPLE_RATE);
  MIC_SetSampleRate(RECORD_SAMPLE_RATE);

  uint32_t start_ms = millis();
  while (!record_stop_requested && (millis() - start_ms) < RECORD_MAX_MS && recorded_bytes < RECORD_MAX_BYTES) {
    size_t space = RECORD_MAX_BYTES - recorded_bytes;
    size_t to_read = space > 1024 ? 1024 : space;
    size_t n = i2s.readBytes((char *)record_buffer + recorded_bytes, to_read);
    if (n > 0) {
      recorded_bytes += n;
    } else {
      vTaskDelay(pdMS_TO_TICKS(5));
    }
  }

  recording_active = false;
  record_stop_requested = false;
  Serial.printf("Recorded %lu bytes\r\n", (unsigned long)recorded_bytes);

  play_recording();

  record_task_handle = NULL;
  vTaskDelete(NULL);
}

/**
 * @brief Start a new voice recording session
 * Stops any ongoing music playback first
 * Allocates recording buffer in PSRAM if available
 */
static void start_recording(void)
{
  if (recording_active || recording_playback_active) {
    return;
  }

  stop_music();

  if (record_buffer == NULL) {
    record_buffer = (uint8_t *)heap_caps_malloc(RECORD_MAX_BYTES, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (record_buffer == NULL) {
      record_buffer = (uint8_t *)heap_caps_malloc(RECORD_MAX_BYTES, MALLOC_CAP_8BIT);
    }
  }

  if (record_buffer == NULL) {
    Serial.println("Record buffer alloc failed");
    return;
  }

  Serial.println("Recording start");
  recording_active = true;
  BaseType_t created = xTaskCreatePinnedToCore(
    record_task,
    "RecordTask",
    4096,
    NULL,
    5,
    &record_task_handle,
    1
  );
  if (created != pdPASS) {
    recording_active = false;
    record_task_handle = NULL;
    Serial.println("Record task create failed");
  }
}
/**
 * @brief Request to stop the ongoing recording session
 * Sets the stop flag which is checked in the recording task loop
 */
static void stop_recording(void)
{
  if (recording_active) {
    Serial.println("Recording stop requested");
    record_stop_requested = true;
  }
}

static void handle_button_event(Status_Button event)
{
  switch (event) {
    case Click:
      start_or_toggle_music();
      break;
    case LongPressStart:
      start_recording();
      break;
    case LongPressStop:
      stop_recording();
      break;
    default:
      break;
  }
}

void Audio_Test_Init(void)
{
  I2C_Init();
  if (!I2S_Init(RECORD_SAMPLE_RATE)) {
    Serial.println("I2S init failed, audio disabled");
    return;
  }
  if (!Audio_Init(RECORD_SAMPLE_RATE)) {
    Serial.println("ES8311 init failed, audio disabled");
    return;
  }
  Volume_adjustment(OUTPUT_VOLUME);
  if (!MIC_Init(RECORD_SAMPLE_RATE)) {
    Serial.println("ES7210 init failed, record disabled");
  }
  Button_Init();

  Serial.printf("Audio volume=%u\r\n", OUTPUT_VOLUME);
  Serial.println("BOOT click: play/pause music, long press: record, release or 5s: playback");
}

void Audio_Test_Loop(void)
{
  Status_Button event = BOOT_KEY_State;
  if (event != None) {
    BOOT_KEY_State = None;
    handle_button_event(event);
  }
}
