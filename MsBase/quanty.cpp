#include "Precompiled.h"
#pragma warning(disable:4302)
/*
 *	Cloud Wu's JPEG decoder
 *
 *			2000/3/4 �� 1 ��
 *
 *		�����Ӯ�����ʵ�����ʹ��, �����ʹ�ô˴����ȫ���򲿷�
 *		������ Cloud Wu (�Ʒ�)
 *
 *		��ҵʹ����������ֱ����ϵ
 *
 *		www.codingnow.com
 *		cloudwu@263.net
 *
 *		��ȡ Quantize Table
 */

//#include <string.h>
//#include <stdlib.h>
#include "jpegint.h"

short *jpeg_qtable[4];

void* read_DQT(void *stream)
{
    WORD seg_size;
    int i;
    void *stream_end;
    short *qtb;
    BYTE qtb_id;
    seg_size = READ_MWORD(stream);
    stream_end = (void*)((unsigned)stream + seg_size - 2);
    while (stream < stream_end) {

        qtb_id = READ_BYTE(stream);
        if (qtb_id & 0x10) {
            qtb_id &= 3;
            jpeg_qtable[qtb_id] = qtb = (short*)malloc(256);
            for (i = 0; i < 64; i++) {
                short tmp;
                tmp = READ_MWORD(stream);
                qtb[i] = tmp << 4;
            }
        }
        else {
            qtb_id &= 3;
            jpeg_qtable[qtb_id] = qtb = (short*)malloc(256);
            for (i = 0; i < 64; i++)
                qtb[i] = (READ_BYTE(stream)) << 4;
        }
    }

    if (stream != stream_end)	return NULL;
    return stream_end;
}
