#ifdef TESTING

#include <stdio.h>
#include "LiIon37V.h"

int main(void) {
    const char *unitTestResult = LiIon37V_UnitTest();
    if (unitTestResult == 0) {
        printf("UNIT TEST PASSED\n");
        return 0;
    } else {
        printf("UNIT TEST FAILED: %s\n", unitTestResult);
        return 1;
    }
}

#endif
