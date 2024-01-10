#include "SDTMaroc.h"

String NumberFormat(int32_t n, uint8_t l)
{
    String res;
    if (l <= 0)
    {
        return res;
    }
    char numChar[10];
    memset(numChar, 0, 10);
    if (n < pow(10, l))
    {
        res += String(n);
    }
    else if (n >= pow(10, 6))
    {
        int32_t i = n / 1000000;
        int32_t d = n % 1000000;
        uint8_t id = GetNumberDigits(i);
        uint8_t dd = GetNumberDigits(d);
        d = d / pow(10, (6 - l + id + 1));
        if (d > 0)
        {
            switch (6 - dd)
            {
            case 0:
                sprintf(numChar, "%d.%d", i, d);
                break;
            case 1:
                sprintf(numChar, "%d.0%d", i, d);
                break;
            case 2:
                sprintf(numChar, "%d.00%d", i, d);
                break;
            default:
                break;
            }
        }
        else
        {
            sprintf(numChar, "%d", i);
        }
        res += String(numChar) + "M";
    }
    else if (n > pow(10, 3))
    {
        int32_t i = n / 1000;
        int32_t d = n % 1000;
        uint8_t id = GetNumberDigits(i);
        uint8_t dd = GetNumberDigits(d);
        d = d / pow(10, (3 - l + id + 1));
        if (d > 0)
        {
            switch (3 - dd)
            {
            case 0:
                sprintf(numChar, "%d.%d", i, d);
                break;
            case 1:
                sprintf(numChar, "%d.0%d", i, d);
                break;
            case 2:
                sprintf(numChar, "%d.00%d", i, d);
                break;
            default:
                break;
            }
        }
        else
        {
            sprintf(numChar, "%d", i);
        }
        res += String(numChar) + "K";
    }
    else
    {
        res += String(n);
    }
    return res;
}

String split(String origin, String symbol, int index)
{
    int target;
    String temps[origin.length()];
    int i = 0;
    do
    {
        target = origin.indexOf(symbol);
        if (target != -1)
        {
            temps[i] = origin.substring(0, target);
            origin = origin.substring(target + symbol.length(), origin.length());
            i++;
        }
        else
        {
            if (origin.length() > 0)
                temps[i] = origin;
        }
    } while (target >= 0);
    if (index > i)
        return "";
    return temps[index];
}