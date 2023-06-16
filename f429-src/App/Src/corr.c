#include "adm.h"
#include "cmsis_gcc.h"

void corr(int16_t* vectorX, int16_t* vectorY, int16_t* vectorCorr, uint32_t longitud)
{
    uint32_t i, n, l;
    int16_t sum, sum2;

    for (l = 0; l < longitud; l++)
    {
        sum = 0;
        sum2 = 0;
        i = 0;
        for (n = l; n < longitud; n++)
        {
            sum += vectorX[n] * vectorY[i];
            sum2 += vectorX[i] * vectorY[n];
            i++;
        }
        vectorCorr[l + longitud - 1] = sum;
        vectorCorr[longitud - l - 1] = sum2;
    }
}
