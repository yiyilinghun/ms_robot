typedef struct _GIF_BUF
{
    unsigned char* buf;
    int fd;
    int flag;/*0:file  1:buf*/
    int buflen;
    int bufpos;
}gif_buf;

extern int GIF_OpenStream(gif_buf*);
extern void GIF_CloseStream(void);
extern unsigned int GIF_GetPixel(int, int, int);
extern int GIF_GetWidth(void);
extern int GIF_GetHeight(void);
extern void GIF_CloseStream();
extern int GIF_Decode(void*, int, int);
extern int GIF_GetTransColor(int*);

