#include "CoverUtil.h"

#include <cstring>

int CoverUtil::charsToInt(const char *chars)
{
    int addr = chars[0] & 0xFF;
    addr |= ((chars[1] << 8) & 0xFF00);
    addr |= ((chars[2] << 16) & 0xFF0000);
    addr |= ((chars[3] << 24) & 0xFF000000);
    return addr;
}
void CoverUtil::intToChars(int num, char *chars, int size)
{
    memset(chars, 0, sizeof(char) * size);
    chars[0] = (char)(0xff & num);
    chars[1] = (char)((0xff00 & num) >> 8);
    chars[2] = (char)((0xff0000 & num) >> 16);
    chars[3] = (char)((0xff000000 & num) >> 24);
    return;
}
