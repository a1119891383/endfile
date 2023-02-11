#ifndef FILEUNTIL_H
#define FILEUNTIL_H
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
struct FileUntil
{
    static void setNonBlock(int sock);
};
#endif
