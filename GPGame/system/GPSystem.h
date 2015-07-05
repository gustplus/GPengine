#ifndef _GPSystem_
#define _GPSystem_

#define swatch32(x) x = ((long)(\
                    (((long)(x) & (long)0x000000ff) << 24) | \
                    (((long)(x) & (long)0x0000ff00) << 8) | \
                    (((long)(x) & (long)0x00ff0000) >> 8) | \
                    (((long)(x) & (long)0xff000000) >> 24)))

#define swatch16(x) \
x = ((short)( \
(((short)(x) & (short)0x00ff) << 8) | \
(((short)(x) & (short)0xff00) >> 8))) \

typedef enum{
    SYSTEM_BIG_ENDIAN,
    SYSTEM_LITTLE_ENDIAN
}SystemEndian;

SystemEndian getEndian()
{
    union{
        short s;
        char c;
    } endianTest;
    endianTest.s = 1;
    if (endianTest.c)
        return SYSTEM_LITTLE_ENDIAN;
    return SYSTEM_BIG_ENDIAN;
}

bool isBigEndian()
{
    unsigned short test = 0x1122;
    if(*( (unsigned char*) &test ) == 0x11)
        return true;
    else
        return false;
}

bool is64Bit()
{
    if (sizeof(int) == 8)
        return true;
    return false;
}

#endif /* defined(__OpenGLES3__GPSystem__) */
