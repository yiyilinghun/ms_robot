#include "Precompiled.h"
#include "D3DSound.h"

// 解析mp3文件选择使用了libmpg123库中提供的方法，mpg123解码器是全部开源的，可以在http://www.mpg123.de/ 下载获得。
//
//
// 在编译libmpg123的过程中可能会遇到若干问题，这里大致罗列一下，进入ports\MSVC++选择对应的vs版本工程，这里选择了2010版。
//
// 由于工程中在预链接事件中使用了yasm汇编器，所以需要下载配置yasm到vs工程中，在http ://yasm.tortall.net/Download.html 有32位和64位系统对应的vs2010版本。
// 下载后解压，根据readme文档添加配置：将yasm.exe放置vs主目录bin中；将yasm.props、yasm.targets、yasm.xml三个规则文件添加至MSBuild\Microsoft.Cpp\v4.0\BuildCustomizations目录中。
// 选择Debug_X86或Release_x86配置，编译中可能会提示link无法打开输入文件xxx.o，将附加依赖和预链接命令中的.o全部替换为.obj。
// 将生成的libmpg123.lib添加到工程中，并将ports\MSVC++下的mpg123.h也引入到自己的工程中，接下来就可以使用libmpg123相关的方法来解析mp3文件了。
//
//
// 首先需要创建一个mpg123_handle句柄指针，过程如下
int myD3DSound::TestFun__1(LPDIRECTSOUNDBUFFER8 m_pBuffer8)
{
    mpg123_handle* m_mpghandle = NULL;
    int ret = MPG123_OK;
    if ((ret = mpg123_init()) != MPG123_OK || (m_mpghandle = mpg123_new(NULL, &ret)) == NULL)
        return -1;





    long rate;        //频率
    int channels;    //声道数
    int encoding;    //编码格式
    if (mpg123_open(m_mpghandle, "youfile.mp3") != MPG123_OK || mpg123_getformat(m_mpghandle, &rate, &channels, &encoding) != MPG123_OK)
        return -1;






    WAVEFORMATEX wave_format;
    int perbits = 16;    //量化数
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
    frameNum = mpg123_tellframe(m_mpghandle); //获取总帧数
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
////之后打开mp3文件并获取相关信息
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
//// 通过判断encoding来设置量化数，并赋值WAVEFORMATEX结构，代码如下
//void myD3DSound::TestFun__3(int encoding, unsigned short channels, unsigned int rate)
//{
//
//
//}
//
//
//
//
//// 另外再说一下如何获取mp3文件的时长和比特率，mp3文件是由帧所构成的，想要知道播放时长，就可以通过总帧数 * 每一帧的时长来推算获得。
//// 总帧数可通过mpg123_tellframe获取，而每一帧时长 = 每一帧的采样个数 * 每一采样的时长 = 每一帧的采样个数 * （1 / 每一帧的采样频率），
//// 而无论哪种编码的mp3文件每一帧的采样个数都是1152，所以计算时长代码如下：
//
//void myD3DSound::TestFun__4(long fTime, unsigned int rate)
//{
//
//}
//
//
//
//// 而计算比特率，需要区别编码格式，如果是CBR，则比特率是固定的；
//// 如果是VBR，由于比特率不固定，所以只能大概取一个平均比特率，计算公式为 平均比特率 = 文件大小(字节) * 8 / 总时长 / 1000，计算比特率代码如下：
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
//// 将mp3文件解析并创建好缓冲区之后，就可以通过mpg123_read方法将文件数据填充至缓冲区了，代码如下
//
//void myD3DSound::TestFun__6(long lock_pos, long lock_size)
//{
//
//}
//
//
//// 其中lock_pos和lock_size 分别是缓冲区的偏移量和锁定大小。
//// mpg13_read的第2个参数返回实际读取到的数据指针，第3个参数是希望读取的数据长度，第4个参数返回实际读取的数据长度。
//
