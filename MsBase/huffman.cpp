#include "Precompiled.h"
#pragma warning(disable:4302)
#pragma warning(disable:4244)
/*
 *	Cloud Wu's JPEG decoder
 *
 *			2000/3/4 �� 1 �� 2003/1/21 bug fix
 *
 *		�����Ӯ�����ʵ�����ʹ��, �����ʹ�ô˴����ȫ���򲿷�
 *		������ Cloud Wu (�Ʒ�)
 *
 *		��ҵʹ����������ֱ����ϵ
 *
 *		www.codingnow.com
 *		cloudwu@263.net
 *
 *		Huffman Table �Ľ���
 */

 //#include <string.h>
 //#include <stdlib.h>
#include "jpegint.h"

JPEG_HUFFMANTABLE jpeg_htable[8];

void* read_DHT(void *stream)
{
    WORD seg_size;
    int i, j, p, code, si;
    void *stream_end;
    JPEG_HUFFMANCODE *htb;
    BYTE htb_id, *code_len;
    seg_size = READ_MWORD(stream);
    stream_end = (void*)((unsigned)stream + seg_size - 2);
    while (stream < stream_end) {

        // ȡ��Ϣ��
        htb_id = READ_BYTE(stream);
        htb_id = (htb_id & 0x10) ? (htb_id & 3 | 4) : (htb_id & 3);

        // ͳ��HTB����,�����ڴ�
        code_len = (BYTE *)stream;
        for (i = 0; i < 16; i++)
            jpeg_htable[htb_id].num += code_len[i];
        stream = (void *)((BYTE *)stream + 16);
        htb = jpeg_htable[htb_id].htb = (JPEG_HUFFMANCODE*)malloc(jpeg_htable[htb_id].num * sizeof(JPEG_HUFFMANCODE));

        // ��������
        for (i = 0, p = 0; i < 16; i++)
            for (j = 0; j < code_len[i]; j++)
                htb[p].num = READ_BYTE(stream), htb[p++].len = i + 1;

        for (si = htb[0].len, i = code = 0; i < p; code <<= 1, si++)
            while (htb[i].len == si && i < p)
                htb[i++].code = code++;
    }

    if (stream != stream_end)	return NULL;
    return stream_end;
}


