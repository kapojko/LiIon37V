#include <stdbool.h>
#include <math.h>
#include "MinUnit.h"
#include "BatteryLI37V.h"

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

float BatteryLI37V_GetRemainingPct(float voltage) {
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

const char *BatteryLI37V_UnitTest(void) {
    float chargeLevel;

    // Test battery level for INA voltage
    chargeLevel = BatteryLI37V_GetRemainingPct(4.0f);
    mu_assert("Battery remaining for 4.0 should be 100.0", approxEqual(chargeLevel, 100.0f, 5.0f));

    chargeLevel = BatteryLI37V_GetRemainingPct(3.5f);
    mu_assert("Battery remaining for 3.5 should be 35.0", approxEqual(chargeLevel, 35.0f, 5.0f));

    chargeLevel = BatteryLI37V_GetRemainingPct(3.4f);
    mu_assert("Battery remaining for 3.4 should be 18.0", approxEqual(chargeLevel, 18.0f, 5.0f));

    chargeLevel = BatteryLI37V_GetRemainingPct(3.2f);
    mu_assert("Battery remaining for 3.2 should be 0.0", approxEqual(chargeLevel, 0.0f, 5.0f));

    return 0;
}
