#pragma once

using std::ifstream;

// Constants for MD5Transform routine.
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


// F, G, H and I are basic MD5 functions.
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))


// ROTATE_LEFT rotates x left n bits.
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))


// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4. Rotation is separate from addition to prevent recomputation.
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
}

#define BUFFER_SIZE 1024
static const char HEX[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
static const Byte PADDING[64] = { 0x80 };

/* MD5 declaration. */
class MD5
{
public:
    DINLINE static mstr _GETMD5(const mstr& str) { mstr md5; MD5(str, md5); return md5; }
    DINLINE static mstr _GETMD5(LPCVOID input, size_t length) { mstr md5; MD5(input, length, md5); return md5; }
public:
    // Default construct.
    DINLINE MD5()
    {
        reset();
    }


    // Construct a MD5 object with a input buffer.
    DINLINE MD5(LPCVOID input, size_t length)
    {
        reset();
        update(input, length);
    }
    DINLINE MD5(LPCVOID input, size_t length, OUT mstr& md5)
    {
        reset();
        update(input, length);
        toString(md5);
    }

    // Construct a MD5 object with a mstr.
    DINLINE MD5(const mstr& str)
    {
        reset();
        update(str);
    }
    DINLINE MD5(const mstr& str, OUT mstr& md5)
    {
        reset();
        update(str);
        toString(md5);
    }


    // Construct a MD5 object with a file.
    DINLINE MD5(ifstream& in)
    {
        reset();
        update(in);
    }


    // Updating the context with a input buffer.
    DINLINE void update(LPCVOID input, size_t length)
    {
        update((const LPBYTE)input, length);
    }


    // Updating the context with a mstr.
    DINLINE void update(const mstr& str)
    {
        update((const LPBYTE)str.c_str(), str.length());
    }


    // Updating the context with a file.
    DINLINE void update(ifstream& in)
    {
        if (!in)
        {
            return;
        }

        std::streamsize length;
        char buffer[BUFFER_SIZE];
        while (!in.eof())
        {
            in.read(buffer, BUFFER_SIZE);
            length = in.gcount();
            if (length > 0)
            {
                update(buffer, (size_t)length);
            }
        }
        in.close();
    }


    // Return the message-digest
    DINLINE const LPBYTE digest()
    {
        if (!_finished)
        {
            _finished = true;
            final();
        }
        return _digest;
    }


    /* Convert digest to mstr value */
    DINLINE mstr toString()
    {
        return bytesToHexString(digest(), 16);
    }
    /* Convert digest to mstr value */
    DINLINE void toString(mstr& md5)
    {
        bytesToHexString(digest(), 16, md5);
    }

    // Reset the calculate state
    DINLINE void reset()
    {
        _finished = false;
        // reset number of bits.
        _count[0] = _count[1] = 0;
        // Load magic initialization constants.
        _state[0] = 0x67452301;
        _state[1] = 0xefcdab89;
        _state[2] = 0x98badcfe;
        _state[3] = 0x10325476;
    }

private:
    // MD5 block update operation. Continues an MD5 message-digest operation, processing another message block, and updating the context.
    DINLINE void update(const LPBYTE input, size_t length)
    {
        DWORD i, index, partLen;

        _finished = false;

        /* Compute number of bytes mod 64 */
        index = (DWORD)((_count[0] >> 3) & 0x3f);

        /* update number of bits */
        if ((_count[0] += ((DWORD)length << 3)) < ((DWORD)length << 3))
            _count[1]++;
        _count[1] += ((DWORD)length >> 29);

        partLen = 64 - index;

        /* transform as many times as possible. */
        if (length >= partLen) {

            memcpy(&_buffer[index], input, partLen);
            transform(_buffer);

            for (i = partLen; i + 63 < length; i += 64)
                transform(&input[i]);
            index = 0;

        }
        else {
            i = 0;
        }

        /* Buffer remaining input */
        memcpy(&_buffer[index], &input[i], length - i);
    }


    // MD5 finalization. Ends an MD5 message-_digest operation, writing the the message _digest and zeroizing the context.
    DINLINE void final()
    {
        Byte bits[8];
        DWORD oldState[4];
        DWORD oldCount[2];
        DWORD index, padLen;

        /* Save current state and count. */
        memcpy(oldState, _state, 16);
        memcpy(oldCount, _count, 8);

        /* Save number of bits */
        encode(_count, bits, 8);

        /* Pad out to 56 mod 64. */
        index = (DWORD)((_count[0] >> 3) & 0x3f);
        padLen = (index < 56) ? (56 - index) : (120 - index);
        update(PADDING, padLen);

        /* Append length (before padding) */
        update(bits, 8);

        /* Store state in digest */
        encode(_state, _digest, 16);

        /* Restore current state and count. */
        memcpy(_state, oldState, 16);
        memcpy(_count, oldCount, 8);
    }

    // MD5 basic transformation. Transforms _state based on block.
    DINLINE void transform(const LPBYTE block)
    {
        DWORD a = _state[0], b = _state[1], c = _state[2], d = _state[3], x[16];

        decode(block, x, 64);

        /* Round 1 */
        FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
        FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
        FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
        FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
        FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
        FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
        FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
        FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
        FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
        FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
        FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
        FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
        FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
        FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
        FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
        FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

                                                /* Round 2 */
        GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
        GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
        GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
        GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
        GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
        GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
        GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
        GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
        GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
        GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
        GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
        GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
        GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
        GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
        GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
        GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

                                                /* Round 3 */
        HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
        HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
        HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
        HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
        HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
        HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
        HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
        HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
        HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
        HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
        HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
        HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
        HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
        HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
        HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
        HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

                                               /* Round 4 */
        II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
        II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
        II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
        II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
        II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
        II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
        II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
        II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
        II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
        II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
        II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
        II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
        II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
        II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
        II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
        II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

        _state[0] += a;
        _state[1] += b;
        _state[2] += c;
        _state[3] += d;
    }


    // Encodes input (DWORD) into output (Byte). Assumes length is a multiple of 4.
    DINLINE void encode(const LPDWORD input, LPBYTE output, size_t length)
    {
        for (size_t i = 0, j = 0; j < length; i++, j += 4)
        {
            output[j] = (Byte)(input[i] & 0xff);
            output[j + 1] = (Byte)((input[i] >> 8) & 0xff);
            output[j + 2] = (Byte)((input[i] >> 16) & 0xff);
            output[j + 3] = (Byte)((input[i] >> 24) & 0xff);
        }
    }


    // Decodes input (Byte) into output (DWORD). Assumes length is a multiple of 4.
    DINLINE void decode(const LPBYTE input, LPDWORD output, size_t length)
    {
        for (size_t i = 0, j = 0; j < length; i++, j += 4)
        {
            output[i] = ((DWORD)input[j]) | (((DWORD)input[j + 1]) << 8) | (((DWORD)input[j + 2]) << 16) | (((DWORD)input[j + 3]) << 24);
        }
    }


    // Convert Byte array to hex mstr.
    DINLINE mstr bytesToHexString(const LPBYTE input, size_t length)
    {
        mstr str;
        str.reserve(length << 1);
        for (size_t i = 0; i < length; i++)
        {
            int t = input[i];
            int a = t / 16;
            int b = t % 16;
            str.append(1, HEX[a]);
            str.append(1, HEX[b]);
        }
        return str;
    }
    DINLINE void bytesToHexString(const LPBYTE input, size_t length, mstr& md5)
    {
        md5.reserve(length << 1);
        for (size_t i = 0; i < length; i++)
        {
            int t = input[i];
            int a = t / 16;
            int b = t % 16;
            md5.append(1, HEX[a]);
            md5.append(1, HEX[b]);
        }
    }

    /* class uncopyable */
    DINLINE MD5(const MD5&);
    DINLINE MD5& operator=(const MD5&);
private:
    DWORD _state[4];    /* state (ABCD) */
    DWORD _count[2];    /* number of bits, modulo 2^64 (low-order word first) */
    Byte _buffer[64];   /* input buffer */
    Byte _digest[16];   /* message digest */
    bool _finished;     /* calculate finished ? */
};

#define GETSTRMD5(str) MD5::_GETMD5(str)
#define GETDATAMD5(input, length) MD5::_GETMD5(input, length)
