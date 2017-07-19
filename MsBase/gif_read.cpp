#include "Precompiled.h"
//#include <stdio.h>
//#include <io.h>
//#include <string.h>
#include "gif_read.h"

#define M_TRAILER     0x3B
#define M_IMAGE       0x2C
#define M_EXTENSION   0x21

static int gif_width, gif_height;
static gif_buf *gif_input;
static char *gifmarker = "GIF";
static int gif_gctdef = 0;
static int gif_resolution = 0;
static int gif_colors = 0;
static int gif_background = 0;
static int gif_aspectratio = 0;
static int *gif_pal;
static int gif_palmask;
static unsigned char *gif_data;
static unsigned char *sdata;
static int gif_interlaced = 0;
static int bitpos, maxbits;
static unsigned char bitbuf[256];
static int initcodesize, codesize, codemask, gif_ipixel, gif_iindex,
gif_ipassnr, gif_ipassinc;
static int transenable = 0;
static int transcoloridex;
/* prototypes */
void readColormap(int);
int int_gif_readchar(gif_buf* ip)
{
    unsigned char buffer = 0;
    int ret;
    if (ip->fd > 0 && ip->flag == 0)
    {
        ret = read(ip->fd, &buffer, 1);
        if (ret != 1)
            return -1;
    }
    if (ip->flag == 1)
    {
        if (ip->bufpos == ip->buflen)
            return -1;
        buffer = *ip->buf++;
        gif_input->bufpos++;
    }
    return buffer;
}

static int gif_readword(void)
{
    return (int_gif_readchar(gif_input) | (int_gif_readchar(gif_input) << 8)) & 0xFFFF;
}

void readColormap(int colors)
{
    int i;

    for (i = 0; i < colors; i++)
        gif_pal[i] = gif_readword() | (int_gif_readchar(gif_input) << 16);
}

static int readCode(void)
{
    int bits, size;
    int i;

    bits = size = 0;

    while (size < codesize) {
        if (bitpos >= maxbits) {
            if ((maxbits = int_gif_readchar(gif_input)) == EOF) return -1;
            maxbits <<= 3;
            /*      fread(bitbuf,1,(size_t)(maxbits>>3),giffile);*/
            for (i = 0; i < maxbits >> 3; i++) {
                bitbuf[i] = (unsigned char)int_gif_readchar(gif_input);
            }
            bitpos = 0;
        }
        bits = bits | (((bitbuf[bitpos >> 3] >> (bitpos & 7)) & 1) << size);
        bitpos++; size++;
    }

    return bits & codemask;
}

static void saveColor(int nr)
{
    if (sdata - gif_data >= gif_width*gif_height) {
        /* printf("Error: data overflow.\n"); */
        return;
    }
    *sdata++ = (unsigned char)nr;

    if (gif_interlaced) {
        if ((++gif_ipixel) >= gif_width) {
            gif_ipixel = 0;
            gif_iindex += gif_ipassinc;
            if (gif_iindex >= gif_height) {
                switch (++gif_ipassnr) {
                    case 1:
                        gif_ipassinc = 8;
                        gif_iindex = 4;
                        break;
                    case 2:
                        gif_ipassinc = 4;
                        gif_iindex = 2;
                        break;
                    case 3:
                        gif_ipassinc = 2;
                        gif_iindex = 1;
                        break;
                }
            }
            sdata = gif_data + gif_iindex*gif_width;
        }
    }
}

static int readImage(void)
{
    int leftoffs, topoffs, info, CC, EOFC, savecode,
        code, lastcode = 0, lastchar = 0, curcode, freecode, initfreecode, outcodenr;
    int prefix[4096];
    unsigned char suffix[4096], output[4096];

    leftoffs = gif_readword(); topoffs = gif_readword();
    gif_width = gif_readword(); gif_height = gif_readword();

    gif_data = (unsigned char *)malloc((size_t)(gif_width*gif_height));

    if (!gif_data) {
        printf("Out of memory.\n");
        return 0;
    }

    info = int_gif_readchar(gif_input);
    gif_interlaced = info & 0x40;
    if (info & 0x80) readColormap((int)1 << ((info & 7) + 1));

    /*  if (leftoffs||topoffs) {
      printf("Warning: image offset (%i, %i)\n",leftoffs,topoffs);
      return 1;
      }*/

    sdata = gif_data;
    initcodesize = int_gif_readchar(gif_input);
    bitpos = maxbits = 0;
    CC = 1 << initcodesize;
    EOFC = CC + 1;
    freecode = initfreecode = CC + 2;
    codesize = ++initcodesize;
    codemask = (1 << codesize) - 1;
    gif_iindex = gif_ipassnr = 0;
    gif_ipassinc = 8;
    gif_ipixel = 0;

    if ((code = readCode()) >= 0)
        while (code != EOFC) {
            if (code == CC) {
                codesize = initcodesize;
                codemask = (1 << codesize) - 1;
                freecode = initfreecode;
                lastcode = lastchar = code = readCode();
                if (code < 0) break;
                saveColor(code);
            }
            else {
                outcodenr = 0; savecode = code;
                if (code >= freecode) {
                    if (code > freecode) break;
                    output[(outcodenr++) & 0xFFF] = (unsigned char)lastchar;
                    code = lastcode;
                }
                curcode = code;

                while (curcode > gif_palmask) {
                    if (outcodenr > 4095) break;
                    output[(outcodenr++) & 0xFFF] = suffix[curcode];
                    curcode = prefix[curcode];
                }

                output[(outcodenr++) & 0xFFF] = lastchar = curcode;

                while (outcodenr > 0)
                    saveColor(output[--outcodenr]);

                prefix[freecode] = lastcode;
                suffix[freecode++] = lastchar;
                lastcode = savecode;

                if (freecode > codemask) {
                    if (codesize < 12) {
                        codesize++;
                        codemask = (codemask << 1) | 1;
                    }
                }
            }
            if ((code = readCode()) < 0) break;
        }
    return 1;
}

/* output size will be ignored */
int GIF_OpenStream(gif_buf* ip)
{
    int i, marker, gifctlex;

    gif_data = NULL;
    gif_pal = NULL;
    gif_width = gif_height = 0;
    gif_input = ip;

    for (i = 0; i < 3; i++)
        if (int_gif_readchar(gif_input) != gifmarker[i])
            return 0;

    // skip version info for now
    for (i = 0; i < 3; i++)
        int_gif_readchar(gif_input);

    gif_width = gif_readword();
    gif_height = gif_readword();

    i = int_gif_readchar(gif_input);
    gif_gctdef = i & 0x80;
    gif_resolution = ((i >> 4) & 7) + 1;
    gif_colors = (int)1 << ((i & 7) + 1);
    gif_palmask = gif_colors - 1;

    gif_background = int_gif_readchar(gif_input);
    gif_aspectratio = int_gif_readchar(gif_input);

    if (gif_aspectratio) {
        printf("Hmm: ASPECT RATIO given ! What to do ?\n");
        return 0;
    }

    gif_pal = (int*)malloc(256 * sizeof(int));
    if (gif_gctdef) readColormap(gif_colors);

    do {
        if ((marker = int_gif_readchar(gif_input)) == EOF) break;

        switch (marker) {
            case M_IMAGE:
                if (readImage() == 0)
                    return 0;
                int_gif_readchar(gif_input);
                break;
            case M_EXTENSION:
                gifctlex = int_gif_readchar(gif_input);
                i = int_gif_readchar(gif_input);
                while (i)
                {
                    while (i--)
                    {
                        if (i == 4)
                        {
                            if ((int_gif_readchar(gif_input) & 0x01) != 0)
                                transenable = 1;
                            else
                                transenable = 0;
                            continue;
                        }
                        if (i == 1)
                        {
                            transcoloridex = int_gif_readchar(gif_input);
                            continue;
                        }
                        int_gif_readchar(gif_input);
                    }
                    i = int_gif_readchar(gif_input);
                }
                break;
            case M_TRAILER:
                break;
            default:
                i = int_gif_readchar(gif_input);
                while (i) {
                    while (i--) int_gif_readchar(gif_input);
                    i = int_gif_readchar(gif_input);
                }
                break;
        }
    } while (marker != M_TRAILER);

    return 1;
}

void GIF_CloseStream(void)
{
    if (gif_pal != NULL)
    {
        free(gif_pal);
        gif_pal = NULL;
    }
    if (gif_data != NULL)
    {
        free(gif_data);
        gif_data = NULL;
    }
    if (gif_input > 0)
    {
        if (gif_input->fd > 0) close(gif_input->fd);
        free(gif_input);
        gif_input = NULL;
    }
}

unsigned int GIF_GetPixel(int x, int y, int col)
{
    int color;

    if ((x >= gif_width) || (y >= gif_height))
        return 0x000000;

    color = gif_pal[gif_data[y*gif_width + x]];

    return col ? (color & 0xFF00) | ((color >> 16) & 0xFF) | ((color & 0xFF) << 16) :
        (((color & 0xFF) + ((color >> 8) & 0xFF) + ((color >> 16) & 0xFF)) / 3) * 0x010101;
}

int GIF_GetWidth(void)
{
    return gif_width;
}

int GIF_GetHeight(void)
{
    return gif_height;
}

int GIF_Decode(void* buf, int buflen, int isfile)
{
    int fd;
    gif_input = (gif_buf*)malloc(sizeof(gif_buf));
    if (!gif_input)
        return 0;
    if (isfile)
    {
        fd = open((const char*)buf, O_BINARY | O_RDONLY);
        if (fd < 0)
            return 0;
        gif_input->fd = fd;
        gif_input->flag = 0;
    }
    else
    {
        gif_input->buf = (unsigned char*)malloc(buflen);
        memcpy(gif_input->buf, buf, buflen);
        gif_input->flag = 1;
        gif_input->bufpos = 0;
        gif_input->buflen = buflen;
    }

    if (GIF_OpenStream(gif_input) == 1)
        return 1;
    else
        return 0;
}
int GIF_GetTransColor(int* transcolor)
{
    if (!transenable)
        return 0;
    *transcolor = gif_pal[transcoloridex];
    return 1;
}

