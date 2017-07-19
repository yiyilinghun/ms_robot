#include "Precompiled.h"
#pragma warning(disable:4706)
#pragma warning(disable:4302)

/*
 *    Cloud Wu's JPEG decoder
 *
 *            2000/6/19 �� 2 ��
 *
 *        �����Ӯ�����ʵ�����ʹ��, �����ʹ�ô˴����ȫ���򲿷�
 *        ������ Cloud Wu (�Ʒ�)
 *
 *        ��ҵʹ����������ֱ����ϵ
 *
 *        www.codingnow.com
 *        cloudwu@263.net
 *
 *        JPEG ���������� (��Ҫ���)
 */


#include <string.h>
#include <stdlib.h>
 //#include "windsoul.h"
#include "jpegint.h"

short jpeg_ybuf[256];
short jpeg_cbbuf[64];
short jpeg_crbuf[64];
short jpeg_DC[3];

int init_jpeg(void)
{
    //    is555=0;
    //    isMMX=1;
    jpeg_setcolormode(1, 1);
    return 0;
}

void free_table()
{
    int i;
    for (i = 0; i < 8; i++)
        if (jpeg_htable[i].htb) free(jpeg_htable[i].htb);
    for (i = 0; i < 4; i++)
        if (jpeg_qtable[i]) free(jpeg_qtable[i]);
}

void init_table()
{
    memset(jpeg_htable, 0, 8 * sizeof(JPEG_HUFFMANTABLE));
    memset(jpeg_qtable, 0, 4 * sizeof(short *));
}

BMP* Unpak_jpg(unsigned char *inbuf, unsigned int insize)
{
    BMP* tmp;
    BYTE type;
    void* jpeg_buf;
    void* jpeg_buf_begin;
    int HDU, VDU, HY, VY;
    int i, j, jpeg_mode;

    jpeg_buf_begin = jpeg_buf = inbuf;

    if (READ_WORD(jpeg_buf) != 0xd8ff)
    {
        // ���� JPEG �ļ�
        return NULL;
    }

    init_table();

    // ���ļ�ͷ
    do {
        while (READ_BYTE(jpeg_buf) != 0xff);
        while ((type = READ_BYTE(jpeg_buf)) == 0xff);
        switch (type) {
            case 0x01: break;
            case 0xc0:
                if (!(jpeg_buf = read_SOF0(jpeg_buf)))
                    return NULL;
                break;
            case 0xc4:
                if (!(jpeg_buf = read_DHT(jpeg_buf)))
                    return NULL;
                break;
            case 0xda:
                if (!(jpeg_buf = read_SOS(jpeg_buf)))
                    return NULL;
                break;
            case 0xdb:
                if (!(jpeg_buf = read_DQT(jpeg_buf)))
                    return NULL;
                break;
            case 0xd9:
                return NULL;
            case 0xdd: // DRI ֧�� (jpeg_DRI=MCU �����)
                jpeg_buf = read_DRI(jpeg_buf);
                break;
                // EOI ���, �쳣����ͼ��
            default: jpeg_buf = skip_SEG(jpeg_buf);
                break;
        }
        if (((unsigned int)jpeg_buf - (unsigned)jpeg_buf_begin) >= insize) {
            // ���ļ�ͷ����
            return NULL;
        }
    } while (type != 0xda); // ɨ���п�ʼ

    // ��ʼ��

    HY = jpeg_head.component[0].h;
    VY = jpeg_head.component[0].v;
    if (HY != VY || HY > 2) {
        // ��֧�ֵ� MCU (ֻ֧�� YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU ����ģʽ)
        return NULL;
    }

    if (jpeg_head.components == 1) jpeg_mode = 0;
    else {
        int HCb, VCb, HCr, VCr;
        HCb = jpeg_head.component[1].h;
        VCb = jpeg_head.component[1].v;
        HCr = jpeg_head.component[2].h;
        VCr = jpeg_head.component[2].v;
        if ((HCb | VCb | HCr | VCr) != 1) {
            // ��֧�ֵ� MCU (ֻ֧�� YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU ����ģʽ)
            return NULL;
        }
        if (HY == 2) jpeg_mode = 1;
        else jpeg_mode = 2;
    }

    HDU = (jpeg_head.width + HY * 8 - 1)&(-1 << (HY + 2));
    VDU = (jpeg_head.height + VY * 8 - 1)&(-1 << (VY + 2));
    tmp = create_bitmap(HDU, VDU);

    tmp->w = jpeg_head.width, tmp->h = jpeg_head.height;
    //    tmp=screen;

// ����
    jpeg_DC[0] = jpeg_DC[1] = jpeg_DC[2] = 0;
    jpeg_preprocess((LPBYTE)jpeg_buf);
    jpeg_bit = 0;
    jpeg_stream = (LPBYTE)jpeg_buf;

    if (jpeg_DRI > 0) {
        int dri = 0;
        switch (jpeg_mode) {
            case 0: // �Ҷ� Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        if (dri == jpeg_DRI) {
                            dri = 0;
                            jpeg_stream += (jpeg_bit + 7) / 8;
                            jpeg_bit = 0;
                            while (*jpeg_stream != 0xff) ++jpeg_stream;
                            ++jpeg_stream;
                            jpeg_DC[0] = 0;
                        }
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        Y2RGB(&tmp->line[i][j], tmp->pitch);
                        ++dri;
                    }
                break;
            case 1: // YYYYCbCr Jpeg
                for (i = 0; i < VDU; i += 16)
                    for (j = 0; j < HDU; j += 16) {
                        if (dri == jpeg_DRI) {
                            dri = 0;
                            jpeg_stream += (jpeg_bit + 7) / 8;
                            jpeg_bit = 0;
                            while (*jpeg_stream != 0xff) ++jpeg_stream;
                            ++jpeg_stream;
                            jpeg_DC[0] = jpeg_DC[1] = jpeg_DC[2] = 0;
                        }
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_ybuf + 64, 0);
                        jpeg_decode_DU(jpeg_ybuf + 128, 0);
                        jpeg_decode_DU(jpeg_ybuf + 192, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr411(&tmp->line[i][j], tmp->pitch);
                        ++dri;
                    }
                break;
            case 2: // YCbCr Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        if (dri == jpeg_DRI) {
                            dri = 0;
                            jpeg_stream += (jpeg_bit + 7) / 8;
                            jpeg_bit = 0;
                            while (*jpeg_stream != 0xff) ++jpeg_stream;
                            ++jpeg_stream;
                            jpeg_DC[0] = jpeg_DC[1] = jpeg_DC[2] = 0;
                        }
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr111(&tmp->line[i][j], tmp->pitch);
                        ++dri;
                    }
                break;
        }
    }
    else {

        switch (jpeg_mode) {
            case 0: // �Ҷ� Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        Y2RGB(&tmp->line[i][j], tmp->pitch);
                    }
                break;
            case 1: // YYYYCbCr Jpeg
                for (i = 0; i < VDU; i += 16)
                    for (j = 0; j < HDU; j += 16) {
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_ybuf + 64, 0);
                        jpeg_decode_DU(jpeg_ybuf + 128, 0);
                        jpeg_decode_DU(jpeg_ybuf + 192, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr411(&tmp->line[i][j], tmp->pitch);
                    }
                break;
            case 2: // YCbCr Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr111(&tmp->line[i][j], tmp->pitch);
                    }
                break;
        }
    }
    free_table();
    return tmp;
}

BMP *load_jpg(char *filename)
{
    BMP *tmp;
    PACKFILE *f;
    BYTE type;
    void *jpeg_buf, *jpeg_buf_begin;
    int HDU, VDU, HY, VY;
    int i, j, jpeg_mode;

    if ((f = pack_fopen(filename)) == NULL) {
        //       DEBUGINFO(1,"�޷���JPEG�ļ� %s",filename);
        return NULL;
    }

    jpeg_buf_begin = jpeg_buf = malloc(f->len);
    pack_fread(jpeg_buf, f->len, f);
    pack_fclose(f);

    if (READ_WORD(jpeg_buf) != 0xd8ff) {
        // ���� JPEG �ļ�
        free(jpeg_buf);
        return NULL;
    }

    init_table();

    // ���ļ�ͷ

    do {
        while (READ_BYTE(jpeg_buf) != 0xff);
        while ((type = READ_BYTE(jpeg_buf)) == 0xff);
        switch (type) {
            case 0x01: break;
            case 0xc0: if (!(jpeg_buf = read_SOF0(jpeg_buf))) {
                free(jpeg_buf_begin);
                return NULL;
            }
                       break;
            case 0xc4: if (!(jpeg_buf = read_DHT(jpeg_buf))) {
                free(jpeg_buf_begin);
                return NULL;
            }
                       break;
            case 0xda: if (!(jpeg_buf = read_SOS(jpeg_buf))) {
                free(jpeg_buf_begin);
                return NULL;
            }
                       break;
            case 0xdb: if (!(jpeg_buf = read_DQT(jpeg_buf))) {
                free(jpeg_buf_begin);
                return NULL;
            }
                       break;
            case 0xd9: free(jpeg_buf_begin);
                return NULL;
            case 0xdd: // DRI ֧�� (jpeg_DRI=MCU �����)
                jpeg_buf = read_DRI(jpeg_buf);
                break;
                // EOI ���, �쳣����ͼ��
            default: jpeg_buf = skip_SEG(jpeg_buf);
                break;
        }
        if (((unsigned)jpeg_buf - (unsigned)jpeg_buf_begin) >= f->len) {
            // ���ļ�ͷ����
            free(jpeg_buf_begin);
            return NULL;
        }
    } while (type != 0xda); // ɨ���п�ʼ

// ��ʼ��

    HY = jpeg_head.component[0].h;
    VY = jpeg_head.component[0].v;
    if (HY != VY || HY > 2) {
        // ��֧�ֵ� MCU (ֻ֧�� YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU ����ģʽ)
        free(jpeg_buf_begin);
        return NULL;
    }

    if (jpeg_head.components == 1) jpeg_mode = 0;
    else {
        int HCb, VCb, HCr, VCr;
        HCb = jpeg_head.component[1].h;
        VCb = jpeg_head.component[1].v;
        HCr = jpeg_head.component[2].h;
        VCr = jpeg_head.component[2].v;
        if ((HCb | VCb | HCr | VCr) != 1) {
            // ��֧�ֵ� MCU (ֻ֧�� YDU, YDU*4 CbDU CrDU, YDU CbDU CrDU ����ģʽ)
            free(jpeg_buf_begin);
            return NULL;
        }
        if (HY == 2) jpeg_mode = 1;
        else jpeg_mode = 2;
    }

    HDU = (jpeg_head.width + HY * 8 - 1)&(-1 << (HY + 2));
    VDU = (jpeg_head.height + VY * 8 - 1)&(-1 << (VY + 2));
    tmp = create_bitmap(HDU, VDU);

    tmp->w = jpeg_head.width, tmp->h = jpeg_head.height;
    //    tmp=screen;

    // ����

    jpeg_DC[0] = jpeg_DC[1] = jpeg_DC[2] = 0;
    jpeg_preprocess((LPBYTE)jpeg_buf);
    jpeg_bit = 0;
    jpeg_stream = (LPBYTE)jpeg_buf;

    if (jpeg_DRI > 0) {
        int dri = 0;
        switch (jpeg_mode) {
            case 0: // �Ҷ� Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        if (dri == jpeg_DRI) {
                            dri = 0;
                            jpeg_stream += (jpeg_bit + 7) / 8;
                            jpeg_bit = 0;
                            while (*jpeg_stream != 0xff) ++jpeg_stream;
                            ++jpeg_stream;
                            jpeg_DC[0] = 0;
                        }
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        Y2RGB(&tmp->line[i][j], tmp->pitch);
                        ++dri;
                    }
                break;
            case 1: // YYYYCbCr Jpeg
                for (i = 0; i < VDU; i += 16)
                    for (j = 0; j < HDU; j += 16) {
                        if (dri == jpeg_DRI) {
                            dri = 0;
                            jpeg_stream += (jpeg_bit + 7) / 8;
                            jpeg_bit = 0;
                            while (*jpeg_stream != 0xff) ++jpeg_stream;
                            ++jpeg_stream;
                            jpeg_DC[0] = jpeg_DC[1] = jpeg_DC[2] = 0;
                        }
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_ybuf + 64, 0);
                        jpeg_decode_DU(jpeg_ybuf + 128, 0);
                        jpeg_decode_DU(jpeg_ybuf + 192, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr411(&tmp->line[i][j], tmp->pitch);
                        ++dri;
                    }
                break;
            case 2: // YCbCr Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        if (dri == jpeg_DRI) {
                            dri = 0;
                            jpeg_stream += (jpeg_bit + 7) / 8;
                            jpeg_bit = 0;
                            while (*jpeg_stream != 0xff) ++jpeg_stream;
                            ++jpeg_stream;
                            jpeg_DC[0] = jpeg_DC[1] = jpeg_DC[2] = 0;
                        }
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr111(&tmp->line[i][j], tmp->pitch);
                        ++dri;
                    }
                break;
        }
    }
    else {

        switch (jpeg_mode) {
            case 0: // �Ҷ� Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        Y2RGB(&tmp->line[i][j], tmp->pitch);
                    }
                break;
            case 1: // YYYYCbCr Jpeg
                for (i = 0; i < VDU; i += 16)
                    for (j = 0; j < HDU; j += 16) {
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_ybuf + 64, 0);
                        jpeg_decode_DU(jpeg_ybuf + 128, 0);
                        jpeg_decode_DU(jpeg_ybuf + 192, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr411(&tmp->line[i][j], tmp->pitch);
                    }
                break;
            case 2: // YCbCr Jpeg
                for (i = 0; i < VDU; i += 8)
                    for (j = 0; j < HDU; j += 8) {
                        jpeg_decode_DU(jpeg_ybuf, 0);
                        jpeg_decode_DU(jpeg_cbbuf, 1);
                        jpeg_decode_DU(jpeg_crbuf, 2);
                        YCbCr111(&tmp->line[i][j], tmp->pitch);
                    }
                break;
        }
    }

    free_table();
    free(jpeg_buf_begin);
    return tmp;
}
