#pragma once
/*
 *    Cloud Wu's JPEG decoder
 *
 *            2000/3/4 �� 1 ��
 *
 *        �����Ӯ�����ʵ�����ʹ��, �����ʹ�ô˴����ȫ���򲿷�
 *        ������ Cloud Wu (�Ʒ�)
 *
 *        ��ҵʹ����������ֱ����ϵ
 *
 *        www.codingnow.com
 *        cloudwu@263.net
 *
 *        JPEG �ڲ�ͷ�ļ�
 */


 // һ��ɫ������� (��ʱ�����޸�)
typedef unsigned short int PIXEL;

// ��Ϸ����ʹ�õĲ�ѹ����λͼ�ṹ
typedef struct {
    int w, h, pitch;      //λͼ��͸��Լ�ÿ��ʵ���ֽ���
    int cl, ct, cr, cb;    //λͼ���þ��ε����ϽǶ�������
    //���þ��εĿ�͸�
    PIXEL *line[1];     //����λͼʱ��̬�����С
} BMP;
typedef BMP* lpBMP;

BMP* load_jpg(char *filename);
BMP* Unpak_jpg(unsigned char *inbuf, unsigned int insize);

// �ͷŷ�ѹ����λͼ
#define destroy_bitmap(a) free(a)

int init_jpeg(void);
#define free_jpeg NULL
#define active_jpeg NULL

