#include "I2C_Driver.h"

void I2C_Init(void) {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);         
  Wire1.begin(I2C2_SDA_PIN, I2C2_SCL_PIN);                       
}


bool I2C1_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length)
{
  Wire.beginTransmission(Driver_addr);
  Wire.write(Reg_addr); 
  if ( Wire.endTransmission(true)){
    printf("The I2C transmission fails. - I2C Read\r\n");
    return -1;
  }
  Wire.requestFrom(Driver_addr, Length);
  for (int i = 0; i < Length; i++) {
    *Reg_data++ = Wire.read();
  }
  return 0;
}
bool I2C1_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length)
{
  Wire.beginTransmission(Driver_addr);
  Wire.write(Reg_addr);       
  for (int i = 0; i < Length; i++) {
    Wire.write(*Reg_data++);
  }
  if ( Wire.endTransmission(true))
  {
    printf("The I2C transmission fails. - I2C Write\r\n");
    return -1;
  }
  return 0;
}

bool I2C2_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length)
{
  Wire1.beginTransmission(Driver_addr);
  Wire1.write(Reg_addr); 
  if ( Wire1.endTransmission(true)){
    printf("The I2C transmission fails. - I2C Read\r\n");
    return -1;
  }
  Wire1.requestFrom(Driver_addr, Length);
  for (int i = 0; i < Length; i++) {
    *Reg_data++ = Wire1.read();
  }
  return 0;
}
bool I2C2_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length)
{
  Wire1.beginTransmission(Driver_addr);
  Wire1.write(Reg_addr);       
  for (int i = 0; i < Length; i++) {
    Wire1.write(*Reg_data++);
  }
  if ( Wire1.endTransmission(true))
  {
    printf("The I2C transmission fails. - I2C Write\r\n");
    return -1;
  }
  return 0;
}