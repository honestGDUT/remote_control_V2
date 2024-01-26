#include "at_cmd.h"
#include "stdio.h"
#include <string.h>
#include <stdarg.h>


void atprintf(const char* msg, ...)
{
    char logbuf[128];
    va_list valist;
    va_start(valist, msg);
    vsprintf(logbuf, msg, valist);
    HAL_UART_Transmit(&huart1, (uint8_t*)logbuf, strlen(logbuf), 1000);
}





