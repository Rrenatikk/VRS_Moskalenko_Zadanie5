#ifndef LPS22HB_H_
#define LPS22HB_H_

#include <stdint.h>
#include "i2c.h" // Configuring Our Callback Types

// I2C address of the LPS22HB sensor on the IKS01A2 board (7-bit)
#define LPS22HB_ADDR            0x5D

// Register Addresses
#define LPS22HB_WHO_AM_I_REG    0x0F
#define LPS22HB_WHO_AM_I_VAL    0xB1  // Expected response from the sensor

#define LPS22HB_CTRL_REG1       0x10
#define LPS22HB_PRESS_OUT_XL    0x28  // Start of pressure registers (3 bytes: XL, L, H)
#define LPS22HB_TEMP_OUT_L      0x2B  // Start of temperature registers (2 bytes: L, H)

// Sensor Configuration Structure
typedef struct {
    I2C_Write_Callback write_cb;
    I2C_Read_Callback  read_cb;
    float reference_pressure; // For post-processing (relative elevation)
} LPS22HB_t;

// Driver Function Prototypes
uint8_t LPS22HB_Init(LPS22HB_t *dev, I2C_Write_Callback write_cb, I2C_Read_Callback read_cb);
float LPS22HB_ReadTemperature(LPS22HB_t *dev);
float LPS22HB_ReadPressure(LPS22HB_t *dev);
void LPS22HB_SetReferencePressure(LPS22HB_t *dev);
float LPS22HB_GetRelativeAltitude(LPS22HB_t *dev);

#endif /* LPS22HB_H_ */
