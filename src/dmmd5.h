
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __DMMD5_H_INCLUDE__
#define __DMMD5_H_INCLUDE__

#include <string>
#include <cstddef>
/* Constants for MD5Transform routine.
*/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
   Rotation is separate from addition to prevent recomputation.
   */
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (unsigned int)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

/* MD5 Class. */
class CDMMD5
{
public:
    CDMMD5(){ MD5Init ();}
    ~CDMMD5(){}

    inline void MD5Update (unsigned char *input, size_t inputLen)
    {
        unsigned int i, index, partLen;

        /* Compute number of bytes mod 64 */
        index = (unsigned int)((this->count[0] >> 3) & 0x3F);

        /* Update number of bits */
        if ((this->count[0] += ((unsigned int)inputLen << 3))
                < ((unsigned int)inputLen << 3))
            this->count[1]++;
        this->count[1] += ((unsigned int)inputLen >> 29);

        partLen = 64 - index;

        /* Transform as many times as possible.
        */
        if (inputLen >= partLen) {
            MD5_memcpy((unsigned char*)&this->buffer[index], 
                    (unsigned char*)input, partLen);
            MD5Transform (this->state, this->buffer);

            for (i = partLen; i + 63 < inputLen; i += 64)
                MD5Transform (this->state, &input[i]);

            index = 0;
        }
        else
            i = 0;

        /* Buffer remaining input */
        MD5_memcpy ((unsigned char*)&this->buffer[index], (unsigned char*)&input[i], inputLen-i);
    }

    inline void MD5Final (unsigned char digest[16])
    {
        unsigned char bits[8];
        unsigned int index, padLen;

        /* Save number of bits */
        Encode (bits, this->count, 8);

        /* Pad out to 56 mod 64.
        */
        index = (unsigned int)((this->count[0] >> 3) & 0x3f);
        padLen = (index < 56) ? (56 - index) : (120 - index);
        MD5Update ( PADDING, padLen);

        /* Append length (before padding) */
        MD5Update (bits, 8);
        /* Store state in digest */
        Encode (digest, this->state, 16);

        /* Zeroize sensitive information.
        */
        MD5_memset ((unsigned char*)this, 0, sizeof (*this));
        this->MD5Init();
    }

    inline void MD5Format(unsigned char format[33])
    {
        unsigned char digest[16] = { 0 };

        MD5Final(digest);
        Binary2Hex(digest, sizeof(digest), format);
    }

    static inline void GetMD5(unsigned char* input, size_t len, unsigned char md5Val[33])
    {
        CDMMD5 ctx;
        ctx.MD5Update(input, len);
        ctx.MD5Final(md5Val);
    }

    static inline std::string GetMD5(const std::string& input)
    {
        unsigned char format[33] = { 0 };

        CDMMD5 ctx;
        ctx.MD5Update((unsigned char*)(input.c_str()), input.size());
        ctx.MD5Format(format);

        return (char*)format;
    }

    static inline std::string GetMD5(unsigned char* input, size_t len)
    {
        unsigned char format[33] = { 0 };

        CDMMD5 ctx;
        ctx.MD5Update(input, len);
        ctx.MD5Format(format);

        return (char*)format;
    }

    static inline void Binary2Hex(const unsigned char* in,
        size_t len, unsigned char* out, bool isLower = true) {
        static const unsigned short hex_lut[] = {
            0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730,
            0x3830, 0x3930, 0x4130, 0x4230, 0x4330, 0x4430, 0x4530, 0x4630,
            0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,
            0x3831, 0x3931, 0x4131, 0x4231, 0x4331, 0x4431, 0x4531, 0x4631,
            0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632, 0x3732,
            0x3832, 0x3932, 0x4132, 0x4232, 0x4332, 0x4432, 0x4532, 0x4632,
            0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533, 0x3633, 0x3733,
            0x3833, 0x3933, 0x4133, 0x4233, 0x4333, 0x4433, 0x4533, 0x4633,
            0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734,
            0x3834, 0x3934, 0x4134, 0x4234, 0x4334, 0x4434, 0x4534, 0x4634,
            0x3035, 0x3135, 0x3235, 0x3335, 0x3435, 0x3535, 0x3635, 0x3735,
            0x3835, 0x3935, 0x4135, 0x4235, 0x4335, 0x4435, 0x4535, 0x4635,
            0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736,
            0x3836, 0x3936, 0x4136, 0x4236, 0x4336, 0x4436, 0x4536, 0x4636,
            0x3037, 0x3137, 0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737,
            0x3837, 0x3937, 0x4137, 0x4237, 0x4337, 0x4437, 0x4537, 0x4637,
            0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738,
            0x3838, 0x3938, 0x4138, 0x4238, 0x4338, 0x4438, 0x4538, 0x4638,
            0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739,
            0x3839, 0x3939, 0x4139, 0x4239, 0x4339, 0x4439, 0x4539, 0x4639,
            0x3041, 0x3141, 0x3241, 0x3341, 0x3441, 0x3541, 0x3641, 0x3741,
            0x3841, 0x3941, 0x4141, 0x4241, 0x4341, 0x4441, 0x4541, 0x4641,
            0x3042, 0x3142, 0x3242, 0x3342, 0x3442, 0x3542, 0x3642, 0x3742,
            0x3842, 0x3942, 0x4142, 0x4242, 0x4342, 0x4442, 0x4542, 0x4642,
            0x3043, 0x3143, 0x3243, 0x3343, 0x3443, 0x3543, 0x3643, 0x3743,
            0x3843, 0x3943, 0x4143, 0x4243, 0x4343, 0x4443, 0x4543, 0x4643,
            0x3044, 0x3144, 0x3244, 0x3344, 0x3444, 0x3544, 0x3644, 0x3744,
            0x3844, 0x3944, 0x4144, 0x4244, 0x4344, 0x4444, 0x4544, 0x4644,
            0x3045, 0x3145, 0x3245, 0x3345, 0x3445, 0x3545, 0x3645, 0x3745,
            0x3845, 0x3945, 0x4145, 0x4245, 0x4345, 0x4445, 0x4545, 0x4645,
            0x3046, 0x3146, 0x3246, 0x3346, 0x3446, 0x3546, 0x3646, 0x3746,
            0x3846, 0x3946, 0x4146, 0x4246, 0x4346, 0x4446, 0x4546, 0x4646
        };

        for (size_t i = 0; i < len; ++i) {
            *reinterpret_cast<unsigned short*>(out) = hex_lut[(in[i])];
            if (isLower)
            {
                *reinterpret_cast<unsigned char*>(out) = tolower(*reinterpret_cast<unsigned char*>(out));
                *reinterpret_cast<unsigned char*>(out + 1) = tolower(*reinterpret_cast<unsigned char*>(out + 1));
            }

            out += sizeof(unsigned short);
        }
    }

    static inline std::string Binary2Hex(unsigned char *buf, size_t len)
    {
        std::string s;
        s.resize(2 * len);
        Binary2Hex(buf, len, (unsigned char *)(s.data()));

        return s;
    }
private:
    unsigned int state[4];         /* state (ABCD) */
    unsigned int count[2];         /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];           /* input buffer */
    unsigned char PADDING[64];          /* What? */

private:
    inline void MD5Init ()
    {
        this->count[0] = this->count[1] = 0;
        /* Load magic initialization constants.*/
        this->state[0] = 0x67452301;
        this->state[1] = 0xefcdab89;
        this->state[2] = 0x98badcfe;
        this->state[3] = 0x10325476;

        MD5_memset(PADDING, 0, sizeof(PADDING));
        MD5_memset(buffer, 0, sizeof(buffer));
        *PADDING=0x80;
        //PADDING = {
        //0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }

    inline void MD5Transform (unsigned int state[4], unsigned char* block)
    {
        unsigned int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

        Decode (x, block, 64);

        /* Round 1 */
        FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
        FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
        FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
        FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
        FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
        FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
        FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
        FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
        FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
        FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
        FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
        FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
        FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
        FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
        FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
        FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

        /* Round 2 */
        GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
        GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
        GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
        GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
        GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
        GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
        GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
        GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
        GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
        GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
        GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
        GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
        GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
        GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
        GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
        GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

        /* Round 3 */
        HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
        HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
        HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
        HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
        HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
        HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
        HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
        HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
        HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
        HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
        HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
        HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
        HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
        HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
        HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
        HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

        /* Round 4 */
        II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
        II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
        II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
        II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
        II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
        II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
        II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
        II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
        II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
        II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
        II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
        II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
        II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
        II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
        II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
        II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;

        /* Zeroize sensitive information.
        */
        MD5_memset ((unsigned char*)x, 0, sizeof (x));
    }

    inline void MD5_memcpy (unsigned char* output, unsigned char* input, size_t len)
    {
        unsigned int i;

        for (i = 0; i < len; i++)
            output[i] = input[i];
    }

    inline void Encode (unsigned char *output, unsigned int *input, size_t len)
    {
        unsigned int i, j;

        for (i = 0, j = 0; j < len; i++, j += 4)
        {
            output[j] = (unsigned char)(input[i] & 0xff);
            output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
            output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
            output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
        }
    }

    inline void Decode (unsigned int *output, unsigned char *input, size_t len)
    {
        unsigned int i, j;

        for (i = 0, j = 0; j < len; i++, j += 4)
            output[i] = ((unsigned int)input[j]) | (((unsigned int)input[j+1]) << 8) |
                (((unsigned int)input[j+2]) << 16) | (((unsigned int)input[j+3]) << 24);
    }

    inline void MD5_memset (unsigned char* output,int value, size_t len)
    {
        unsigned int i;

        for (i = 0; i < len; i++)
            ((char *)output)[i] = (char)value;
    }
};

#endif // __DMMD5_H_INCLUDE__
