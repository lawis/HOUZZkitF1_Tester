#include "dtMacros.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

NS_DT_BEGIN
// void log(const char * format, ...)
// {
//     dot2d::Director::getInstance()->loger()->println("------------------------");
//     char loc_buf[64];
//     char * temp = loc_buf;
//     va_list arg;
//     va_list copy;
//     va_start(arg, format);
//     va_copy(copy, arg);
//     int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
//     va_end(copy);
//     if(len < 0) {
//         va_end(arg);
//         return;
//     };
//     if(len >= sizeof(loc_buf)){
//         temp = (char*) malloc(len+1);
//         if(temp == NULL) {
//             va_end(arg);
//             return;
//         }
//         len = vsnprintf(temp, len+1, format, arg);
//     }
//     va_end(arg);
//     size_t n = 0;
//     const uint8_t *buffer = (uint8_t*)temp;
//     while(len--) {
//         n += dot2d::Director::getInstance()->loger()->write(*buffer++);
//     }
//     len = (int)n;
//     if(temp != loc_buf){
//         free(temp);
//     }
//     return;
// }

NS_DT_END
