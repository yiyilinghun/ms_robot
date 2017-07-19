#include "Precompiled.h"
//#include <stdlib.h>
//#include <memory.h>
#include "jpegint.h"

// ������ѹ����λͼ
BMP *create_bitmap(int w, int h)
{
    BMP *tmp;
    PIXEL *t;
    int i, bmpw = w;
    if (w & 3) w = (w + 3) & 0xfffffffc;    //��֤����,����ÿ�ж���4 ��������
    tmp = (BMP *)malloc(sizeof(BMP) + (h - 1) * sizeof(PIXEL *) + w*h*SIZEOFPIXEL);
    if (!tmp) return NULL;
    tmp->w = tmp->cr = bmpw;
    tmp->pitch = w*SIZEOFPIXEL;
    tmp->h = tmp->cb = h;
    tmp->cl = tmp->ct = 0;
    t = tmp->line[0] = (PIXEL *)((char *)tmp + sizeof(BMP) + (h - 1) * sizeof(PIXEL *));
    for (i = 1; i < h; i++)
        tmp->line[i] = (t += w);
    return tmp;
}