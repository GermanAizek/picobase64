
#ifdef __cplusplus

#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>

#define ArrayChar(N) std::array<char,N>
typedef std::string_view  picostr;

#ifdef __clang__
#define PicoConst    constexpr const
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
    PicoConst
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

    for (i = 0; i < inLongLen; i += 12) { // Process 4 chunks (12 bytes) at a time
      ot[0] = tTable[((it[0]) >> 2)];
      ot[1] = tTable[(((it[0] & 0x03) << 4) | (((it[1]) >> 4)))];
      ot[2] = tTable[(((it[1] & 0x0f) << 2) | (((it[2]) >> 6)))];
      ot[3] = tTable[(it[2] & 0x3f)];

      ot[4] = tTable[((it[3]) >> 2)];
      ot[5] = tTable[(((it[3] & 0x03) << 4) | (((it[4]) >> 4)))];
      ot[6] = tTable[(((it[4] & 0x0f) << 2) | (((it[5]) >> 6)))];
      ot[7] = tTable[(it[5] & 0x3f)];

      ot[8] = tTable[((it[6]) >> 2)];
      ot[9] = tTable[(((it[6] & 0x03) << 4) | (((it[7]) >> 4)))];
      ot[10] = tTable[(((it[7] & 0x0f) << 2) | (((it[8]) >> 6)))];
      ot[11] = tTable[(it[8] & 0x3f)];

      ot[12] = tTable[((it[9]) >> 2)];
      ot[13] = tTable[(((it[9] & 0x03) << 4) | (((it[10]) >> 4)))];
      ot[14] = tTable[(((it[10] & 0x0f) << 2) | (((it[11]) >> 6)))];
      ot[15] = tTable[(it[11] & 0x3f)];

      it += 12;
      ot += 16;
    }

    // Process remaining chunks (0 to 3 chunks)
    for (; i < inLongLen; i += 3) {
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
    PicoConst
    ArrayChar(256) valTable = {
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
        64, 64, 64, 64
    };

    picouint i;
    const uint8_t *it;
    uint8_t *ot;
    uint8_t v[4];

    // Process 4 chunks (16 bytes) at a time
    picouint inLongLen = (inLen - 16);
    picouint oIndex = 0;

    for (i = 0; i < inLongLen; i += 16) {
      it = reinterpret_cast<const uint8_t *>(&in[i]);
      ot = &out[oIndex];

      v[0] = valTable[it[0]]; v[1] = valTable[it[1]]; v[2] = valTable[it[2]]; v[3] = valTable[it[3]];
      ot[0] = ((v[0] << 2) | (v[1] >> 4));
      ot[1] = ((v[1] << 4) | (v[2] >> 2));
      ot[2] = ((v[2] << 6) | (v[3]));

      v[0] = valTable[it[4]]; v[1] = valTable[it[5]]; v[2] = valTable[it[6]]; v[3] = valTable[it[7]];
      ot[3] = ((v[0] << 2) | (v[1] >> 4));
      ot[4] = ((v[1] << 4) | (v[2] >> 2));
      ot[5] = ((v[2] << 6) | (v[3]));

      v[0] = valTable[it[8]]; v[1] = valTable[it[9]]; v[2] = valTable[it[10]]; v[3] = valTable[it[11]];
      ot[6] = ((v[0] << 2) | (v[1] >> 4));
      ot[7] = ((v[1] << 4) | (v[2] >> 2));
      ot[8] = ((v[2] << 6) | (v[3]));

      v[0] = valTable[it[12]]; v[1] = valTable[it[13]]; v[2] = valTable[it[14]]; v[3] = valTable[it[15]];
      ot[9] = ((v[0] << 2) | (v[1] >> 4));
      ot[10] = ((v[1] << 4) | (v[2] >> 2));
      ot[11] = ((v[2] << 6) | (v[3]));

      oIndex += 12;
    }

    // Process remaining chunks (0 to 3 chunks)
    for (; i < (inLen - 4); i += 4, oIndex += 3) {
      it = reinterpret_cast<const uint8_t *>(&in[i]);
      ot = &out[oIndex];
      v[0] = valTable[it[0]]; v[1] = valTable[it[1]]; v[2] = valTable[it[2]]; v[3] = valTable[it[3]];

      ot[0] = ((v[0] << 2) | (v[1] >> 4));
      ot[1] = ((v[1] << 4) | (v[2] >> 2));
      ot[2] = ((v[2] << 6) | (v[3]));
    }

    // check for last segment
    {
      it = reinterpret_cast<const uint8_t *>(&in[i]);
      ot = &out[oIndex];
      v[0] = valTable[it[0]];
      v[1] = valTable[it[1]];
      v[2] = valTable[it[2]];
      v[3] = valTable[it[3]];

      picouint padCnt = 0;
      ot[0] = ((v[0] << 2) | (v[1] >> 4)); // 6 bytes from v0 + 2 byte from v1
      if (v[2] != 64) {
        ot[1] = ((v[1] << 4) | (v[2] >> 2));
      } else {
        ot[1] = ((v[1] << 4)); // 4 byte from v1 + pad
        padCnt++;
      }
      if (v[3] != 64) {
        ot[2] = ((v[2] << 6) | (v[3]));
      } else {
        ot[2] = ((v[2] << 6)); // 2 byte from v2 + pad
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

#ifdef __clang__
inline
PicoConst
#endif
size_t GetDecodeExpectedLen(size_t inLen) noexcept
{
    return ((inLen + 3) / 4) * 3;
}

auto b64encode(picostr bytes) noexcept
{
#ifdef __cplusplus
    auto strLen = bytes.length();
#else
    uint32_t eLen = GetEncodeLen(strlen(bytes));
#endif
    std::string out;
    out.resize(GetEncodeLen(strLen));
    EncodeChunk((const uint8_t*)&bytes[0], strLen, (uint8_t*)&out[0]);
    return out;
}

auto b64decode(picostr base64) noexcept
{
  std::string out;

#ifdef __cplusplus
    auto eLen = GetDecodeExpectedLen(std::string_view(base64).length());
    out.resize(eLen);
#else
    uint32_t eLen = GetDecodeExpectedLen(strlen(base64));
    Resize(out, strlen(out), eLen);
#endif
    eLen = DecodeChunk(base64.data(), base64.length(), reinterpret_cast<uint8_t*>(&out[0]));
    out.resize(eLen);
    return out;
}
