#include "lps22hb.h"

#include <math.h>

// Sensor Initialization
uint8_t LPS22HB_Init(LPS22HB_t *dev, I2C_Write_Callback write_cb, I2C_Read_Callback read_cb) {
    dev->write_cb = write_cb;
    dev->read_cb = read_cb;
    dev->reference_pressure = 1013.25f; // default value

    uint8_t who_am_i = 0;

    // 1. Checking WHO_AM_I
    if (dev->read_cb(LPS22HB_ADDR, LPS22HB_WHO_AM_I_REG, &who_am_i, 1) != 0) {
        return 1; // Communication error I2C
    }
    if (who_am_i != LPS22HB_WHO_AM_I_VAL) {
        return 2; // Incorrect sensor reading
    }

    // CTRL_REG1 configuration
	uint8_t ctrl1_cfg = 0x32;
	if (dev->write_cb(LPS22HB_ADDR, LPS22HB_CTRL_REG1, &ctrl1_cfg, 1) != 0) {
		return 1; // Write error
	}

    return 0; // Success
}

float LPS22HB_ReadTemperature(LPS22HB_t *dev) {
    uint8_t raw_data[2];

    if (dev->read_cb(LPS22HB_ADDR, LPS22HB_TEMP_OUT_L, raw_data, 2) == 0) {
        int16_t temp_raw = (int16_t)((raw_data[1] << 8) | raw_data[0]);
        // The correct formula for LPS22HB: T = raw_value / 100.0
        return (float)temp_raw / 100.0f;
    }
    return 0.0f;
}

// Pressure reading
float LPS22HB_ReadPressure(LPS22HB_t *dev) {
    uint8_t raw_data[3];

    // Multi-byte read of 3 bytes starting at PRESS_OUT_XL
    if (dev->read_cb(LPS22HB_ADDR, LPS22HB_PRESS_OUT_XL, raw_data, 3) == 0) {
        // We construct a 24-bit number and convert it to a signed 32-bit number
        int32_t press_raw = (int32_t)((raw_data[2] << 16) | (raw_data[1] << 8) | raw_data[0]);

        // The sensor outputs pressure in 2-s complement format.
		// If the 24th bit is 1, the number is negative (we expand the sign)
        if (press_raw & 0x00800000) {
            press_raw |= 0xFF000000;
        }

        // Formula from the datasheet: P = raw_value / 4096
        return (float)press_raw / 4096.0f;
    }
    return 0.0f;
}

// Set the current pressure as sea level (call once at startup)
void LPS22HB_SetReferencePressure(LPS22HB_t *dev) {
    dev->reference_pressure = LPS22HB_ReadPressure(dev);
}

// Calculation of Relative Height (Post-Processing)
float LPS22HB_GetRelativeAltitude(LPS22HB_t *dev) {
    float current_pressure = LPS22HB_ReadPressure(dev);

    if (current_pressure == 0.0f || dev->reference_pressure == 0.0f) {
        return 0.0f; // Division-by-zero protection
    }

    return 44330.0f * (1.0f - powf((current_pressure / dev->reference_pressure), (1.0f / 5.255f)));
}
