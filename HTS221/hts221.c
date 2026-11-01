#include "hts221.h"

uint8_t HTS221_Init(HTS221_t *dev, I2C_Write_Callback write_cb, I2C_Read_Callback read_cb) {
    dev->write_cb = write_cb;
    dev->read_cb = read_cb;

    uint8_t who_am_i = 0;

    // 1. Connection Test
    if (dev->read_cb(HTS221_ADDR, HTS221_WHO_AM_I_REG, &who_am_i, 1) != 0) {
        return 1;
    }
    if (who_am_i != HTS221_WHO_AM_I_VAL) {
        return 2;
    }

    // 2. Turn on the sensor (PD=1), BDU=1, ODR=1Hz
    // 1000 0101 = 0x85
    uint8_t ctrl_reg1 = 0x85;
    if (dev->write_cb(HTS221_ADDR, HTS221_CTRL_REG1, &ctrl_reg1, 1) != 0) {
        return 1;
    }

    // 3. Reading the humidity calibration data
    uint8_t buf[2];

    dev->read_cb(HTS221_ADDR, HTS221_H0_RH_X2, buf, 1);
    dev->H0_rh = (float)buf[0] / 2.0f;

    dev->read_cb(HTS221_ADDR, HTS221_H1_RH_X2, buf, 1);
    dev->H1_rh = (float)buf[0] / 2.0f;

    dev->read_cb(HTS221_ADDR, HTS221_H0_T0_OUT_L, buf, 2);
    dev->H0_T0_out = (int16_t)((buf[1] << 8) | buf[0]);

    dev->read_cb(HTS221_ADDR, HTS221_H1_T0_OUT_L, buf, 2);
    dev->H1_T0_out = (int16_t)((buf[1] << 8) | buf[0]);

    return 0; // Success
}

float HTS221_ReadHumidity(HTS221_t *dev) {
    uint8_t buf[2];

    if (dev->read_cb(HTS221_ADDR, HTS221_HUMIDITY_OUT_L, buf, 2) == 0) {
        int16_t h_raw = (int16_t)((buf[1] << 8) | buf[0]);

        // Linear Interpolation
        float humidity = dev->H0_rh + ((float)(h_raw - dev->H0_T0_out) * (dev->H1_rh - dev->H0_rh)) / (float)(dev->H1_T0_out - dev->H0_T0_out);

        if (humidity > 100.0f) humidity = 100.0f;
        if (humidity < 0.0f) humidity = 0.0f;

        return humidity;
    }
    return 0.0f;
}
