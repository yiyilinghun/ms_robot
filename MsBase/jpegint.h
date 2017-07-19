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
 *		JPEG �ڲ�ͷ�ļ�
 */

#ifndef _JPEG_INTERNAL_
#define _JPEG_INTERNAL_

#include "stdio.h"
#include "jpeg.h"

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

#pragma pack (push)
#pragma pack (1)

typedef struct {
    WORD code;	// huffman ����
    BYTE len;	// ���볤��
    BYTE num;	// ������ֽ�
} JPEG_HUFFMANCODE;

typedef struct {
    BYTE v;
    BYTE h;
    BYTE qtb;
    BYTE act;
    BYTE dct;
} JPEG_COMPONENT;

typedef struct {
    BYTE colordepth;
    WORD height;
    WORD width;
    BYTE components;
    JPEG_COMPONENT component[3];
} JPEG_SOF0;

#pragma pack (pop)

typedef struct {
    JPEG_HUFFMANCODE *htb;
    int num;
} JPEG_HUFFMANTABLE;

/*************************************************************************************/

#ifdef _DEBUG
#ifndef DEBUGINFOON
#define DEBUGINFOON
#endif
#endif

#ifdef DEBUGINFOON
#define DEBUGINFO debuginfohead(__FILE__,__LINE__);debuginfo
#else
#define DEBUGINFO
#endif

//extern int is555;
//extern int isMMX;

// һ��ɫ������� (��ʱ�����޸�)
//typedef unsigned short int PIXEL;

// һ��ɫ����ֽڳ��� (��ʱ�����޸�)
#define SIZEOFPIXEL 2

// �ļ���(��������·��) ��󳤶�
#define PATHMAXLEN 80

// ���ݰ��ļ�����������
#define FILEBUFFERNUM 8
/*
// ��Ϸ����ʹ�õĲ�ѹ����λͼ�ṹ
typedef struct {
    int w,h,pitch;      //λͼ��͸��Լ�ÿ��ʵ���ֽ���
    int cl,ct,cr,cb;    //λͼ���þ��ε����ϽǶ�������
    //���þ��εĿ�͸�
    PIXEL *line[1];     //����λͼʱ��̬�����С
} BMP;
*/
#pragma pack (push)
#pragma pack (2)

typedef struct {
    FILE *file;				// ʵ���ļ�ָ��
    unsigned short filenum; // �����ж��ٸ�С�ļ�
    struct {
        unsigned id;		// �����ļ��� ID 32bit
        unsigned offset;	// �ļ�������ļ���ʼ��ƫ����
        unsigned len;		// ��ѹ��ĳ���
    } block[1];
} DATAFILE;

typedef struct {
    FILE *datafile;			//�����ļ�ָ��(�����ǵ������ļ�����)
    unsigned offset;		//��һ���ƫ����,0 ��ʾ�ǵ����ļ�
    unsigned len;			//�ļ�����
    unsigned pos;			//��ָ���λ��(������λ��)
    unsigned char *buf;		//������ָ��(����̶�Ϊ64K)
    unsigned datapos;		//��ѹ�������λ��
    unsigned short block[1];//ÿ��ĳ���
} PACKFILE;

#pragma pack (pop)

PACKFILE *pack_fopen(char *);
unsigned pack_fread(void *buffer, unsigned size, PACKFILE *f);
void pack_fclose(PACKFILE *);

unsigned filename2id(char *);

// ������ѹ����λͼ
BMP *create_bitmap(int w, int h);

int jpg_decompress(void *src, unsigned src_len, void *dst);

/*************************************************************************************/

extern JPEG_HUFFMANTABLE jpeg_htable[8];
extern short *jpeg_qtable[4];
extern JPEG_SOF0 jpeg_head;
extern short jpeg_DC[3];
extern BYTE jpeg_bit, *jpeg_stream;
extern BYTE jpeg_zigzag[64];
extern WORD jpeg_DRI;

//#define READ_MWORD(a,stream) \
//	__asm mov esi,stream	\
//	__asm xor eax,eax		\
//	__asm lodsw				\
//    __asm xchg al,ah		\
//	__asm mov a,ax			\
//	__asm mov stream,esi	


//#define READ_BYTE(stream) (*((BYTE*)stream)++)
//#define READ_WORD(stream) (*((WORD*)stream)++)
typedef void* LPVOID;
inline BYTE READ_BYTE(LPVOID& stream)
{
    LPBYTE& xTemp = (BYTE*&)stream;
    return *xTemp++;
}
inline WORD READ_WORD(LPVOID& stream)
{
    LPWORD& xTemp = (WORD*&)stream;
    return *xTemp++;
}

inline WORD READ_MWORD(LPVOID& stream)
{
    BYTE a = READ_BYTE(stream);
    BYTE b = READ_BYTE(stream);
    return (a * 256) + b;
}

void* read_DQT(void *stream);
void* read_DHT(void *stream);
void *read_SOF0(void *stream);
void *read_DRI(void *stream);
void *read_SOS(void *stream);
void *skip_SEG(void *stream);

// Ԥ����, ȥ�� FF
void jpeg_preprocess(BYTE *stream);

// �������� jpeg_stream ����һ�� DU �� buf
void jpeg_decode_DU(short *buf, int com);

void jpeg_idct8x8aan(short *src_result);
void jpeg_idct2d(short *buf);
extern void(*jpeg_idct) (short *buf);
void Y2RGB(WORD *bmppixel, int pitch);
extern void(*YCbCr411)(WORD *bmppixel, int pitch);
void YCbCr111(WORD *bmppixel, int pitch);
void jpeg_setcolormode(int is555, int isMMX);

#endif