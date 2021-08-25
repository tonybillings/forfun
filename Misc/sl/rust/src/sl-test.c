/**************************************************************************************************
 INCLUDES
**************************************************************************************************/

#include <stdio.h>

/**************************************************************************************************
 FUNCTIONS
**************************************************************************************************/

void sl_initialize()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("sl_initialize() called.\r\n");
}

int sl_draw(int y, int x, char* str)
{
    printf("sl_draw() called with these args: y(%s)  x(%s)  str(%s)", y, x, str);
    return 0;
}

void sl_wait()
{
    printf("sl_wait() called.");
}

void sl_finalize()
{
    printf("sl_finalize() called.");
}

int sl_get_cols()
{
    printf("sl_get_cols() called. Returning 10...");
    return 10;
}

int sl_get_lines()
{
    printf("sl_get_lines() called. Returning 10...");
    return 10;
}

