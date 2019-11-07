#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
#ifdef _WIN32
#include <Windows.h>
int GetColumnWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE out;
     
    if (!(out = GetStdHandle(STD_OUTPUT_HANDLE)) ||
        !GetConsoleScreenBufferInfo(out, &info))
        return 80;
    return info.dwSize.X;
}//GetColumnWidth
#else
int GetColumnWidth() {return 80;}
#endif
 
int main()
{
    const char *s = "Hey, I'm centered!";
 
    const int total_width = 80;
    const int s_width = strlen(s);
    const int field_width = (total_width - s_width) / 2 + s_width;
 
    printf("%*s\n", field_width, s);
 
    return 0;
}//main