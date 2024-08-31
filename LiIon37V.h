#ifndef LIION37V_H
#define LIION37V_H

float LiIon37V_GetBatteryLevel(float voltage);
void LiIon37V_GetLevelColor(float batteryLevel, float *red, float *green, float *blue);

const char *LiIon37V_UnitTest(void);

#endif // LIION37V_H
