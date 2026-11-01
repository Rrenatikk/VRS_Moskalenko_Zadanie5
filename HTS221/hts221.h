#ifndef HTS221_H_
#define HTS221_H_

#include <stdint.h>
#include "i2c.h"

// I2C address of sensor HTS221 (7-bit)
#define HTS221_ADDR             0x5F

// Registers
#define HTS221_WHO_AM_I_REG     0x0F
#define HTS221_WHO_AM_I_VAL     0xBC

#define HTS221_CTRL_REG1        0x20
#define HTS221_HUMIDITY_OUT_L   0x28 // Start of humidity data
#define HTS221_TEMP_OUT_L       0x2A // Start of temperature data

// Humidity Calibration Registers
#define HTS221_H0_RH_X2         0x30
#define HTS221_H1_RH_X2         0x31
#define HTS221_H0_T0_OUT_L      0x36
#define HTS221_H1_T0_OUT_L      0x3A

typedef struct {
    I2C_Write_Callback write_cb;
    I2C_Read_Callback  read_cb;

    // Calibration values for humidity
    float   H0_rh, H1_rh;
    int16_t H0_T0_out, H1_T0_out;
} HTS221_t;

uint8_t HTS221_Init(HTS221_t *dev, I2C_Write_Callback write_cb, I2C_Read_Callback read_cb);
float HTS221_ReadHumidity(HTS221_t *dev);
// It's not necessary to write a temperature function for it, since we're getting the temperature from the LPS22HB.

#endif /* HTS221_H_ */
