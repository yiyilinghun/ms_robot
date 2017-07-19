#include "Precompiled.h"
#pragma warning(disable:4302)
/*
 *	Cloud Wu's JPEG decoder
 *
 *			2000/6/19 �� 2 ��
 *
 *		�����Ӯ�����ʵ�����ʹ��, �����ʹ�ô˴����ȫ���򲿷�
 *		������ Cloud Wu (�Ʒ�)
 *
 *		��ҵʹ����������ֱ����ϵ
 *
 *		www.codingnow.com
 *		cloudwu@263.net
 *
 *		��ȡ SEGMENT
 */

#include "jpegint.h"

#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

 // ����һ�������ݵĶ�
void *skip_SEG(void *stream)
{
    void *stream_end;
    WORD seg_size;

    seg_size = READ_MWORD(stream);
    stream_end = (void*)((unsigned)stream + seg_size - 2);
    return stream_end;
}


/*
SOF0: Start Of Frame 0:
~~~~~~~~~~~~~~~~~~~~~~~

  - $ff, $c0 (SOF0)
  - ���� (���ֽ�, ���ֽ�), 8+components*3
  - ���ݾ��� (1 byte) ÿ������λ��, ͨ���� 8 (����������֧�� 12 �� 16)
  - ͼƬ�߶� (���ֽ�, ���ֽ�), �����֧�� DNL �ͱ��� >0
  - ͼƬ��� (���ֽ�, ���ֽ�), �����֧�� DNL �ͱ��� >0
  - components ����(1 byte), �Ҷ�ͼ�� 1, YCbCr/YIQ ��ɫͼ�� 3, CMYK ��ɫͼ
    �� 4
  - ÿ�� component: 3 bytes
     - component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q)
     - ����ϵ�� (bit 0-3 vert., 4-7 hor.)
     - quantization table ��
*/

JPEG_SOF0 jpeg_head;

void *read_SOF0(void *stream)
{
    void *stream_end;
    WORD seg_size;
    BYTE id, q, i;
    seg_size = READ_MWORD(stream);
    stream_end = (void*)((unsigned)stream + seg_size - 2);
    jpeg_head.colordepth = READ_BYTE(stream);
    jpeg_head.height = READ_MWORD(stream);
    jpeg_head.width = READ_MWORD(stream);
    if ((jpeg_head.components = READ_BYTE(stream)) > 3)
        return NULL;	// ��֧�� 3 ���������
    for (i = 0; i < jpeg_head.components; i++) {
        if ((id = READ_BYTE(stream)) > 3) return NULL; //��֧�ֵ�ģʽ
        if (--id < 0) return NULL;
        q = READ_BYTE(stream);
        if ((jpeg_head.component[id].v = q & 0xf) > 2)
            return NULL; // ��֧�ֳ��� 2 �Ĳ�����
        if ((jpeg_head.component[id].h = q >> 4) > 2)
            return NULL; // ��֧�ֳ��� 2 �Ĳ�����
        jpeg_head.component[id].qtb = READ_BYTE(stream) & 3;
    }
    return stream_end;
}

/*
SOS: Start Of Scan:
~~~~~~~~~~~~~~~~~~~

  - $ff, $da (SOS)
  - ���� (���ֽ�, ���ֽ�), ������ 6+2*(ɨ���������������)
  - ɨ��������������� (1 byte), ���� >= 1 , <=4 (�����Ǵ��) ͨ���� 3
  - ÿ�����: 2 bytes
     - component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q), �� SOF0
     - ʹ�õ� Huffman ��:
    - bit 0..3: AC table (0..3)
    - bit 4..7: DC table (0..3)
  - ���� 3 bytes (???)
*/

void *read_SOS(void *stream)
{
    void *stream_end;
    WORD seg_size;
    BYTE i, id, com, q;
    seg_size = READ_MWORD(stream);
    stream_end = (void*)((unsigned)stream + seg_size - 2);
    if ((com = READ_BYTE(stream)) != jpeg_head.components)
        return NULL; // �����Ŀ����

    for (i = 0; i < com; i++) {
        if ((id = READ_BYTE(stream)) > 3) return NULL; //��֧�ֵ�ģʽ
        if (--id < 0) return NULL;
        q = READ_BYTE(stream);
        jpeg_head.component[id].act = (q & 3) | 4;
        jpeg_head.component[id].dct = (q >> 4) & 3;
    }
    return stream_end;
}

/*
DRI: Define Restart Interval:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  - $ff, $dd (DRI)
  - ���� (���ֽ�, ���ֽ�), ������ 4
  - MCU ��ĵ�Ԫ�е����¿�ʼ��� (���ֽ�, ���ֽ�),
    ��˼��˵, ÿ n �� MCU �����һ�� RSTn ���.
    ��һ������� RST0, Ȼ���� RST1 ��, RST7 ���ٴ� RST0 �ظ�
*/

WORD jpeg_DRI = 0;

void *read_DRI(void *stream)
{
    void *stream_end;
    WORD seg_size;
    seg_size = READ_MWORD(stream);
    stream_end = (void*)((unsigned)stream + seg_size - 2);
    jpeg_DRI = READ_MWORD(stream);
    return stream_end;
}
