#ifndef COVERUTIL_H
#define COVERUTIL_H
struct CoverUtil
{
    static int charsToInt(const char *chars);
    static void intToChars(int num, char *chars, int size = 4);
};

#endif