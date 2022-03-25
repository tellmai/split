#ifndef FRAMEPARSECLASS_H
#define FRAMEPARSECLASS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <QByteArray>



class FrameParseClass
{
private:
    /***********************状态机转换************************************/
    // States of packet decoder state machine
    static const int FRAME_HEAD	= 0;
    static const int FRAME_LENGTH	= 1;
    static const int FRAME_TRAIL	= 2;


    static const int BUFFERLEN		= (1024*1024);//
    static const int FRAMEMAXLEN	= (64*1024);//

    static const int FRAME_HEAD_LENGTH_BYTE = 2;//帧头  //帧长度的字节数
    static const int FRAME_STRUCT_BYTE = 12;//帧头+帧长度+帧尾+校验和的字节数
    static const int FRAME_STRUCT_BIG_BYTE = 6;//帧头+保留位+信息长度位+CRC
    
    static const int FILE_ALL_LEN = 304;//数据文件每帧数据的总长度
    static const int FILE_HEAD_LEN = 4;//数据文件每帧数据的帧头长度
    char FileFrame_h[4];//文件解析的帧头

    char Frame_h[1];//帧头
    char Frame_t[3];//帧尾

    unsigned int RecvIndex;//数据接收索引
    unsigned int RemainIndex;//未处理数据长度
    unsigned int ProIndex;//数据处理位置

    unsigned int FrameState;//帧状态
    unsigned int FrameBegIndex;//一帧有效的数据起始位置

    char RecvBuffer[BUFFERLEN];//数据接收缓冲区


    unsigned long ReadOffset;//取数据偏移


    int StatusFrameParse(void);
    int FileStatusFrameParse(void);
    int My_Strncmp(char *pBuffer1,char *pBuffer2,int length);
    void FrameParaReset(void);

    /**************************************************************
    函数功能:	校验和计算函数
    函数参数:	const char *pFrameBuffer，帧内容缓冲区
    int FrameLen，帧内容长度
    函数返回:	命令包数据长度
    **************************************************************/
    char CalculateCheckSum(const char *pFrameBuffer, int FrameLen);

public:
    FrameParseClass();

    static const int Wait_H			= 1;//
    static const int True_H			= 0;//
    static const int Error_H		= -1;//


    unsigned int FrameLength;//帧长度(有效数据长度)
    unsigned int FrameAllLength;//帧长度(完整帧长度)

    char FrameBuffer[FRAMEMAXLEN];//帧数据缓冲区

    /*******************************************************************************
    函数说明:	解析帧数据
    参   数:	char *pBuffer，需要解析的缓冲区
            int BufLength，缓冲区数据长度
    返   回:	解析状态：Wait_H：等待下一个数据，Error_H：解析错误，其他表示帧长度
    *******************************************************************************/
    int FrameParse(char *pBuffer, int BufLength);



    /**************************************************************
    函数功能:	帧封装函数
    函数参数:	int CmdType,命令类型
            char *pBuffer，帧参数缓冲区
            int FrameLen，帧参数长度
            char *pFrameBuffer,完整帧缓冲区
    函数返回:	完整帧的长度
    **************************************************************/
    int FramePack(char CmdType, char *pBuffer, int FrameLen, char *pFrameBuffer);



    /*******************************************************************************
    函数说明:	解析帧数据
    参   数:	char *pBuffer，需要解析的缓冲区
            int BufLength，缓冲区数据长度
    返   回:	解析状态：Wait_H：等待下一个数据，Error_H：解析错误，其他表示帧长度
    *******************************************************************************/
    int FileFrameParse(char* pBuffer, int BufLength);



    /*******************************************************************************
    函数说明:	从完整帧缓冲区顺序取出数据，取出类型可为char、short、int、double等
    函数参数: char *pBuffer,输出的数据，为NULL，则直接返回值，否则将数据放入pBuffer
            int Length,要转换数据长度，字节
    函数返回:	数据
    函数示例：double SkySpeed = GetFrameData<double>(NULL,8);
            short PDOPValue = GetFrameData<short>(NULL,2);
            BOOL state = GetFrameData<BOOL>(pBuffer,1024);
    *******************************************************************************/
    template <typename T>
    T GetFrameData(char *pBuffer, int Length)
    {
        T BackVal = 0;
        char TempChar[8];

        //超出范围，退出
        if (ReadOffset + Length > FRAMEMAXLEN)
        {
            BackVal = 0;
            return BackVal;
        }

        //通过pBuffer读取数据
        if (pBuffer != NULL)
        {
            memcpy(pBuffer, FrameBuffer + ReadOffset, Length);
            ReadOffset += Length;

            BackVal = 1;
            return BackVal;
        }


        //若长度大于8字节，则退出
        if (Length > 8)
        {
            BackVal = 0;
            return BackVal;
        }

        memset(TempChar, 0, 8);
        memcpy(TempChar, FrameBuffer + ReadOffset, Length);
        ReadOffset += Length;

        BackVal = (((T *)TempChar)[0]);

        return BackVal;
    }


    /*******************************************************************************
    函数说明:	从完整帧缓冲区指定位置取出数据，取出类型可为char、short、int、double等
    函数参数: int Postion,指定取数位置，从0开始
            char *pBuffer,输出的数据，为NULL，则直接返回值，否则将数据放入pBuffer
            int Length,要转换数据长度，字节
    函数返回:	数据
    函数示例：double SkySpeed = GetFrameData<double>(0,NULL,8);
            short PDOPValue = GetFrameData<short>(10,NULL,2);
            BOOL state = GetFrameData<BOOL>(10,pBuffer,1024);
    *******************************************************************************/
    template <typename T>
    T GetFrameData(int Postion,char *pBuffer, int Length)
    {
        T BackVal = 0;
        char TempChar[8];

        //超出范围，退出
        if (Postion + Length > FRAMEMAXLEN)
        {
            BackVal = 0;
            return BackVal;
        }

        //通过pBuffer读取数据
        if (pBuffer != NULL)
        {
            memcpy(pBuffer, FrameBuffer + Postion, Length);
            Postion += Length;

            BackVal = 1;
            return BackVal;
        }


        //若长度大于8字节，则退出
        if (Length > 8)
        {
            BackVal = 0;
            return BackVal;
        }

        memset(TempChar, 0, 8);
        memcpy(TempChar, FrameBuffer + Postion, Length);
        Postion += Length;

        BackVal = (((T *)TempChar)[0]);

        return BackVal;
    }
};













#endif // FRAMEPARSECLASS_H
