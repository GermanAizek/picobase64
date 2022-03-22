
#ifdef __cplusplus

#include <array>
#include <cstdint>
#include <iostream>

#define ArrayChar(N) std::array<char,N>
typedef std::string  picostr;

#ifdef __clang__
#define PicoConst    constexpr
typedef size_t       picouint;
#else
#define PicoConst    const
typedef int          picouint;
#endif

#else

#include <stdint.h>
#include <string.h>

#define ArrayChar(N) char[N]
typedef const char* picostr;

void Resize(char* str, size_t curSize, size_t newSize)
{
    // allocate new array and copy in data
    char *newArray = new char[newSize];
    memcpy(newArray, str, curSize);

    // delete old array
    delete [] str;

    // swap pointers and new size
    str = newArray;
    curSize = newSize;
}

#endif

void EncodeChunk(const uint8_t *in, size_t inLen, uint8_t *out) noexcept
{
    /*
      translation table
    */
    alignas(16) PicoConst
    ArrayChar(64) tTable = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    /*
      process bulks
    */
    picouint inLongLen = (inLen - 2);
    picouint i;

    /*
      process blocks
    */
    const uint8_t *it = in;
    uint8_t *ot = out;

    for (i = 0; i < inLongLen; i += 3) {
      ot[0] = tTable[((it[0]) >> 2)];
      ot[1] = tTable[(((it[0] & 0x03) << 4) | (((it[1]) >> 4)))];
      ot[2] = tTable[(((it[1] & 0x0f) << 2) | (((it[2]) >> 6)))];
      ot[3] = tTable[(it[2] & 0x3f)];
      it += 3;
      ot += 4;
    }

    // check for pad
    inLongLen = (inLen - i);
    if (inLongLen > 0) {
      ot[0] = tTable[(it[0]) >> 2];
      char sym = '=';
      picouint ot0 = ((it[0] & 0x3) << 4);
      if (inLongLen == 1) {
        ot[1] = tTable[ot0];
        ot[2] = sym;
        ot[3] = sym;
      } else {
        ot[1] = tTable[((ot0 | ((it[1] & 0xf0) >> 4)))];
        ot[2] = tTable[((it[1] & 0x0f)) << 2];
        ot[3] = sym;
      }
    }
}

size_t DecodeChunk(const char *in, size_t inLen,uint8_t *out) noexcept
{
    const ArrayChar(256) valTable = {
        /* ASCII table */
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63, 52, 53, 54, 55, 56, 57,
        58, 59, 60, 61, 64, 64, 64, 64, 64, 64, 64, 0,  1,  2,  3,  4,  5,  6,
        7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, 64, 64, 64, 64, 64, 64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
        37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64};

    int i;

  // disable code duplication
  #define __FBASE_INIT_DECODE_SEGMENT                                            \
    const uint8_t *it = reinterpret_cast<const uint8_t *>(&in[i]);               \
    uint8_t *ot = &out[oIndex];                                                  \
    uint8_t v0 = valTable[it[0]];                                                \
    uint8_t v1 = valTable[it[1]];                                                \
    uint8_t v2 = valTable[it[2]];                                                \
    uint8_t v3 = valTable[it[3]];


     // check size
    if (inLen == 0 || (inLen % 4) != 0) {
      return 0;
    }

    // remove pads from bulk process
    int inLongLen = (inLen - 4);
    uint32_t oIndex = 0;

    for (i = 0; i < inLongLen; i += 4, oIndex += 3) {
      __FBASE_INIT_DECODE_SEGMENT
      ot[0] = ((v0 << 2) | (v1 >> 4)); // 6 bytes from v0 + 2 byte from v1
      ot[1] = ((v1 << 4) | (v2 >> 2)); // 4 byte from v1 and 4 byte from v2
      ot[2] = ((v2 << 6) | (v3));      // 2 byte from v2 and 6 byte from v3
    }

    // check for last segment
    {
      __FBASE_INIT_DECODE_SEGMENT
      uint32_t padCnt = 0;
      ot[0] = ((v0 << 2) | (v1 >> 4)); // 6 bytes from v0 + 2 byte from v1
      if (v2 != 64) {
        ot[1] = ((v1 << 4) | (v2 >> 2));
      } else {
        ot[1] = ((v1 << 4)); // 4 byte from v1 + pad
        padCnt++;
      }
      if (v3 != 64) {
        ot[2] = ((v2 << 6) | (v3));
      } else {
        ot[2] = ((v2 << 6)); // 2 byte from v2 + pad
        padCnt++;
      }
      oIndex += (3 - padCnt);
    }
    out[oIndex] = 0;
    return oIndex;
}

#ifdef __clang__
inline
PicoConst
#endif
size_t GetEncodeLen(size_t inLen) noexcept
{
    return ((inLen + 2) / 3) * 4;
}

size_t GetDecodeExpectedLen(size_t inLen) noexcept
{
    return ((inLen + 3) / 4) * 3;
}

picostr b64encode(const picostr &bytes) noexcept
{
#ifdef __cplusplus
    size_t strLen = bytes.length();
#else
    uint32_t eLen = GetEncodeLen(strlen(bytes));
#endif
    picostr out;
    out.resize(GetEncodeLen(strLen));
    EncodeChunk((const uint8_t*)&bytes[0], strLen, (uint8_t*)&out[0]);
    return out;
}

picostr b64decode(const picostr &base64) noexcept
{
    picostr out;

#ifdef __cplusplus
    uint32_t eLen = GetDecodeExpectedLen(base64.length());
    out.resize(eLen);
#else
    uint32_t eLen = GetDecodeExpectedLen(strlen(base64));
    Resize(out, strlen(out), eLen);
#endif
    eLen = DecodeChunk(base64.c_str(), base64.length(), reinterpret_cast<uint8_t*>(&out[0]));
    out.resize(eLen);
    return out;
}
