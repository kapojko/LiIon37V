#include <stdbool.h>
#include <math.h>
#include "MinUnit.h"
#include "LiIon37V.h"

// interpolation with polynom of degree 5
#define COEFF_COUNT 5
#define VOLTAGE_MAX 4.0f
#define VOLTAGE_MIN 3.2f

const float coef[COEFF_COUNT] = { 758465.7587582349f, -440899.8844070132f, 127458.7786068192f, -18326.5357068361f, 1048.7699925162f };
const float intercept = -519074.7161935132f;

static bool approxEqual(float a, float b, float epsilon) {
    return fabsf(a - b) < epsilon;
}

static float clipf(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

static void hsvToRgb(float h, float s, float v, float *r, float *g, float *b) {
    if (s == 0.0f) {
        *r = v;
        *g = v;
        *b = v;
        return;
    }

    int i = (int)(h * 6.0f);
    float f = (h * 6.0f) - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    i = i % 6;

    if (i == 0) {
        *r = v;
        *g = t;
        *b = p;
    } else if (i == 1) {
        *r = q;
        *g = v;
        *b = p;
    } else if (i == 2) {
        *r = p;
        *g = v;
        *b = t;
    } else if (i == 3) {
        *r = p;
        *g = q;
        *b = v;
    } else if (i == 4) {
        *r = t;
        *g = p;
        *b = v;
    } else if (i == 5) {
        *r = v;
        *g = p;
        *b = q;
    } else {
        // Unexpected
        *r = 0.0f;
        *g = 0.0f;
        *b = 0.0f;
    }
}

float LiIon37V_GetBatteryLevel(float voltage) {
    float ret;
    if (voltage > VOLTAGE_MAX) {
        ret = 100.0;
    } else if (voltage < VOLTAGE_MIN) {
        ret = 0.0;
    } else {
        ret = intercept;

        for (int power = 0; power < COEFF_COUNT; ++power) {
            ret += powf(voltage, power + 1) * coef[power];
        }
    }

    return clipf(ret, 0.0f, 100.0f);
}

void LiIon37V_GetLevelColor(float batteryLevel, float *red, float *green, float *blue) {
    if (batteryLevel >= 100.0f) {
        // 100%: green
        *red = 0.0f;
        *green = 1.0f;
        *blue = 0.0f;
    } else if (batteryLevel >= 15.0f) {
        // 100-15%: green to red
        float p = (batteryLevel - 15.0f) / 85.0f; // 0-1.0

        hsvToRgb(p / 3.0f, 1.0f, 1.0f, red, green, blue);
    } else {
        // 15%-0%: red
        *red = 1.0f;
        *green = 0.0f;
        *blue = 0.0f;
    }
}

const char *LiIon37V_UnitTest(void) {
    float chargeLevel;

    // Test battery level for INA voltage
    chargeLevel = LiIon37V_GetBatteryLevel(4.0f);
    mu_assert("Battery level for 4.0 should be 100.0", approxEqual(chargeLevel, 100.0f, 5.0f));

    chargeLevel = LiIon37V_GetBatteryLevel(3.5f);
    mu_assert("Battery level for 3.5 should be 35.0", approxEqual(chargeLevel, 35.0f, 5.0f));

    chargeLevel = LiIon37V_GetBatteryLevel(3.4f);
    mu_assert("Battery level for 3.4 should be 18.0", approxEqual(chargeLevel, 18.0f, 5.0f));

    chargeLevel = LiIon37V_GetBatteryLevel(3.2f);
    mu_assert("Battery level for 3.2 should be 0.0", approxEqual(chargeLevel, 0.0f, 5.0f));

    // TODO: test colors

    return 0;
}
