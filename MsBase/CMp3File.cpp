#include "Precompiled.h"
#include "D3DSound.h"

// ����mp3�ļ�ѡ��ʹ����libmpg123�����ṩ�ķ�����mpg123��������ȫ����Դ�ģ�������http://www.mpg123.de/ ���ػ�á�
//
//
// �ڱ���libmpg123�Ĺ����п��ܻ������������⣬�����������һ�£�����ports\MSVC++ѡ���Ӧ��vs�汾���̣�����ѡ����2010�档
//
// ���ڹ�������Ԥ�����¼���ʹ����yasm�������������Ҫ��������yasm��vs�����У���http ://yasm.tortall.net/Download.html ��32λ��64λϵͳ��Ӧ��vs2010�汾��
// ���غ��ѹ������readme�ĵ�������ã���yasm.exe����vs��Ŀ¼bin�У���yasm.props��yasm.targets��yasm.xml���������ļ������MSBuild\Microsoft.Cpp\v4.0\BuildCustomizationsĿ¼�С�
// ѡ��Debug_X86��Release_x86���ã������п��ܻ���ʾlink�޷��������ļ�xxx.o��������������Ԥ���������е�.oȫ���滻Ϊ.obj��
// �����ɵ�libmpg123.lib��ӵ������У�����ports\MSVC++�µ�mpg123.hҲ���뵽�Լ��Ĺ����У��������Ϳ���ʹ��libmpg123��صķ���������mp3�ļ��ˡ�
//
//
// ������Ҫ����һ��mpg123_handle���ָ�룬��������
int myD3DSound::TestFun__1(LPDIRECTSOUNDBUFFER8 m_pBuffer8)
{
    mpg123_handle* m_mpghandle = NULL;
    int ret = MPG123_OK;
    if ((ret = mpg123_init()) != MPG123_OK || (m_mpghandle = mpg123_new(NULL, &ret)) == NULL)
        return -1;





    long rate;        //Ƶ��
    int channels;    //������
    int encoding;    //�����ʽ
    if (mpg123_open(m_mpghandle, "youfile.mp3") != MPG123_OK || mpg123_getformat(m_mpghandle, &rate, &channels, &encoding) != MPG123_OK)
        return -1;






    WAVEFORMATEX wave_format;
    int perbits = 16;    //������
    if ((encoding & MPG123_ENC_16) == MPG123_ENC_16)
        perbits = 16;
    else if ((encoding & MPG123_ENC_32) == MPG123_ENC_32)
        perbits = 32;
    else
        perbits = 8;

    wave_format.wFormatTag = WAVE_FORMAT_PCM;
    wave_format.nChannels = channels;
    wave_format.nSamplesPerSec = rate;
    wave_format.wBitsPerSample = perbits;
    wave_format.nBlockAlign = perbits / 8 * channels;
    wave_format.nAvgBytesPerSec = rate * perbits / 8 * channels;







    long    frameNum;
    //long    fileTime;
    mpg123_seek(m_mpghandle, 0, SEEK_END);
    frameNum = mpg123_tellframe(m_mpghandle); //��ȡ��֡��
    long fTime = (long)(frameNum * 1152 / rate);





    mpg123_frameinfo mpginfo;
    int bitrate;
    long filesize;
    FILE* tmpfp = NULL;
    if (mpg123_info(m_mpghandle, &mpginfo) != MPG123_OK)
        return -1;
    if (mpginfo.layer != 3)
        return -1;
    if (mpginfo.vbr == MPG123_CBR)
        bitrate = mpginfo.bitrate;
    else if (mpginfo.vbr == MPG123_VBR)
    {
        if (fopen_s(&tmpfp, "youfile.mp3", "rb") == 0)
        {
            fseek(tmpfp, 0, SEEK_END);
            filesize = ftell(tmpfp);
            fclose(tmpfp);
            tmpfp = NULL;
            bitrate = (filesize * 8) / (fTime * 1000);
        }
    }

    return 0;







    //auto lock_pos = 0;
    //auto lock_size = filesize;

    //void* buf = NULL;
    //DWORD buf_len = 0;
    //unsigned char* _buffer;
    //size_t outsize;
    //_buffer = (unsigned char*)malloc(lock_size * sizeof(unsigned char));
    //if (SUCCEEDED(m_pBuffer8->Lock(lock_pos, lock_size, &buf, &buf_len, NULL, NULL, 0)))
    //{
    //    mpg123_read(m_mpghandle, _buffer, lock_size, &outsize);
    //    memcpy(buf, _buffer, outsize);
    //    m_pBuffer8->Unlock(buf, buf_len, NULL, 0);
    //}
}

//
////֮���mp3�ļ�����ȡ�����Ϣ
//int myD3DSound::TestFun__2()
//{
//
//
//    return 0;
//}
//
//
//
//
//// ͨ���ж�encoding������������������ֵWAVEFORMATEX�ṹ����������
//void myD3DSound::TestFun__3(int encoding, unsigned short channels, unsigned int rate)
//{
//
//
//}
//
//
//
//
//// ������˵һ����λ�ȡmp3�ļ���ʱ���ͱ����ʣ�mp3�ļ�����֡�����ɵģ���Ҫ֪������ʱ�����Ϳ���ͨ����֡�� * ÿһ֡��ʱ���������á�
//// ��֡����ͨ��mpg123_tellframe��ȡ����ÿһ֡ʱ�� = ÿһ֡�Ĳ������� * ÿһ������ʱ�� = ÿһ֡�Ĳ������� * ��1 / ÿһ֡�Ĳ���Ƶ�ʣ���
//// ���������ֱ����mp3�ļ�ÿһ֡�Ĳ�����������1152�����Լ���ʱ���������£�
//
//void myD3DSound::TestFun__4(long fTime, unsigned int rate)
//{
//
//}
//
//
//
//// ����������ʣ���Ҫ��������ʽ�������CBR����������ǹ̶��ģ�
//// �����VBR�����ڱ����ʲ��̶�������ֻ�ܴ��ȡһ��ƽ�������ʣ����㹫ʽΪ ƽ�������� = �ļ���С(�ֽ�) * 8 / ��ʱ�� / 1000����������ʴ������£�
//
//
//int myD3DSound::TestFun__5(long fTime)
//{
//
//
//    return 0;
//}
//
//
//
//// ��mp3�ļ������������û�����֮�󣬾Ϳ���ͨ��mpg123_read�������ļ�����������������ˣ���������
//
//void myD3DSound::TestFun__6(long lock_pos, long lock_size)
//{
//
//}
//
//
//// ����lock_pos��lock_size �ֱ��ǻ�������ƫ������������С��
//// mpg13_read�ĵ�2����������ʵ�ʶ�ȡ��������ָ�룬��3��������ϣ����ȡ�����ݳ��ȣ���4����������ʵ�ʶ�ȡ�����ݳ��ȡ�
//
