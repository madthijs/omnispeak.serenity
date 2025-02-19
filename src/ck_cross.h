// Tools and definitions assisting with general cross-platform support

#ifndef CK_CROSS_H
#define CK_CROSS_H

#include <stdint.h>
#include <stdio.h>

#ifdef WITH_SDL
#include "SDL.h"
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define CK_CROSS_IS_BIGENDIAN
#elif (SDL_BYTEORDER == SDL_LIL_ENDIAN)
#define CK_CROSS_IS_LITTLEENDIAN
#else
#error "ck_cross.h - Unknown platform endianness!"
#endif
#else // WITH_SDL
#if defined(__LITTLE_ENDIAN__) || (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define CK_CROSS_IS_LITTLEENDIAN
#elif defined(__BIG_ENDIAN__) || (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define CK_CROSS_IS_BIGENDIAN
#else
#error "cl_cross.h - Couldn't determine platform endianness!"
#endif
#endif

#define CK_Cross_Swap16(x) ((uint16_t)(((uint16_t)(x) << 8) | ((uint16_t)(x) >> 8)))

#define CK_Cross_Swap32(x) ((uint32_t)(((uint32_t)(x) << 24) | (((uint32_t)(x) << 8) & 0x00FF0000) | (((uint32_t)(x) >> 8) & 0x0000FF00) | ((uint32_t)(x) >> 24)))

#ifdef CK_CROSS_IS_LITTLEENDIAN
#define CK_Cross_SwapLE16(x) (x)
#define CK_Cross_SwapLE32(x) (x)
#else
#define CK_Cross_SwapLE16(x) CK_Cross_Swap16(x)
#define CK_Cross_SwapLE32(x) CK_Cross_Swap32(x)
#endif

typedef enum CK_Log_Message_Class_T
{
	CK_LOG_MSG_NORMAL,
	CK_LOG_MSG_WARNING,
	CK_LOG_MSG_ERROR
} CK_Log_Message_Class_T;

// Used for debugging
void CK_Cross_LogMessage(CK_Log_Message_Class_T msgClass, const char *format, ...);

// Emulates the functionality of the "puts" function in text mode
void CK_Cross_puts(const char *str);

// More standard C functions emulated,
// taking English locale into account (and more, but NOT all)
int CK_Cross_toupper(int c);
// A bit less standard, but still done assuming English locale
int CK_Cross_strcasecmp(const char *s1, const char *s2);

// The C standard library doesn't have an implementation of min/max, which is sad.
#define CK_Cross_max(x, y) ((x) < (y) ? (y) : (x))
#define CK_Cross_min(x, y) ((x) > (y) ? (y) : (x))

#endif
