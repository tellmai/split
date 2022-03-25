/*====================================================
文件名:		ParseClass.cpp
功能简述:		帧解析类，实现数据帧的帧头解析
版本号:		V1.00
=====================================================*/

#include "FrameParseClass.h"

/*******************************************************************************
函数说明:	构造函数
参   数:	无
返   回:	无
示   例：	无
*******************************************************************************/
FrameParseClass::FrameParseClass()
{
    RecvIndex		= 0;//数据接收索引
    RemainIndex		= 0;//未处理数据长度
    ProIndex		= 0;//数据处理位置
    FrameLength		= 0;//帧长度
    FrameState		= 0;//帧状态
    FrameBegIndex	= 0;//一帧有效的数据起始位置

    //帧头
    Frame_h[0] = 0xD3;

    //数据文件帧头
    FileFrame_h[0] = 0x55;
    FileFrame_h[1] = 0xAA;
    FileFrame_h[2] = 0x55;
    FileFrame_h[3] = 0xAA;

    ReadOffset = 0;
    memset(RecvBuffer,0,BUFFERLEN);
    memset(FrameBuffer,0,FRAMEMAXLEN);
}


/**************************************************************
函数功能：比较字符串
函数参数：char *pBuffer1,字符串1
        char *pBuffer2,字符串2
        int length，长度
函数返回：0，相等，其他不等
**************************************************************/
int FrameParseClass::My_Strncmp(char *pBuffer1, char *pBuffer2, int length)
{
    int i=0;

    for(i=0; i<length; i++)
    {
        if(pBuffer1[i] != pBuffer2[i])
        {
            return 1;
        }
    }

    return 0;
}


/*******************************************************************************
函数说明:复位帧参数
参   数:
返   回:
*******************************************************************************/
void FrameParseClass::FrameParaReset(void)
{
    FrameBegIndex = 0;
    FrameState    = 0;
    FrameLength   = 0;
    FrameAllLength = 0;
    memset(FrameBuffer,0,FRAMEMAXLEN);
}




/*******************************************************************************
函数说明:解析帧数据
参   数:
返   回:	解析状态：Wait_H：等待下一个数据，Error_H：解析错误，其他表示帧长度
示   例：	无
*******************************************************************************/
int FrameParseClass::StatusFrameParse(void)
{
    int ReturnStat = 0;//#define Wait_H   (1)  //#define True_H  (0)  //#define Error_H (-1)

    unsigned int intTemp, i, dataNum;
    unsigned char unCharTemp[2] = {0};
    unsigned int Sum = 0;
    char RecvCheckCRC24 = 0;//接收到的校验和
    char CalculateCheckCRC24 = 0;//计算的校验和
    int msgLen = 0;//数据信息长度 + 数据标识长度
    char holdbit = -1;//保留位
    intTemp = 0;

    RemainIndex = (RecvIndex-ProIndex+BUFFERLEN)%BUFFERLEN;//更新未处理数据
    dataNum = RemainIndex;//用临时变量记录剩余长度

    switch(FrameState)
    {
    case   FRAME_HEAD:
        if(dataNum>=1)
        {
            for(i=0;i<dataNum;i++)
            {
                intTemp = ProIndex;//获取未处理数据的起始位置

                //获取帧头数据
                memset(unCharTemp, 0, sizeof(unCharTemp));
                unCharTemp[0] = RecvBuffer[intTemp];
                intTemp = (intTemp+1)%BUFFERLEN;

                //判断帧头
                if (My_Strncmp((char *)unCharTemp, Frame_h, sizeof(Frame_h)) == 0)
                {
                    FrameBegIndex = ProIndex;//若帧头正确，则保持一帧有效的数据起始位置
                    ProIndex = (ProIndex+1)%BUFFERLEN;//更新数据处理位置
                    FrameState++;//解析状态指向下一状态
                    dataNum = dataNum-i-1;//获取帧去掉帧头剩余长度
                    break;
                }
                else
                {
                    ProIndex = (ProIndex+1)%BUFFERLEN;//若帧头不正确，更新数据处理位置
                }

                if(i==(dataNum-1))//数据处理完成，则返回
                {
                    return(Wait_H);
                }
            }
        }
        else//若数据小于帧头长度，则返回
        {
            return(Wait_H);
        }

    case   FRAME_LENGTH:
        if(dataNum<2)//小于保留为和信息长度所占节数，则返回
            return(Wait_H);

        intTemp = ProIndex;//获取未处理数据的起始位置

        //获取帧数据长度
        memset(unCharTemp, 0, sizeof(unCharTemp));
        unCharTemp[0] = RecvBuffer[intTemp];
        intTemp = (intTemp+1)%BUFFERLEN;
        unCharTemp[1] = RecvBuffer[intTemp];
        intTemp = (intTemp+1)%BUFFERLEN;

        holdbit = (unCharTemp[0] >> 2)&0xff;//注意是左边移动还是右边移动
        if (holdbit != 0)
        {
            FrameParaReset();
            return(Error_H);
        }
        msgLen = (unCharTemp[0] & 0x3) | (unCharTemp[1]); //数据信息长度 + 数据标识长度

        FrameLength = msgLen;//
        FrameAllLength = FrameLength + FRAME_STRUCT_BIG_BYTE;//(全长) 
        if(FrameAllLength > FRAMEMAXLEN)//帧最大FRAMEMAXLEN字节
        {
            FrameParaReset();
            return(Error_H);
        }

        if (FrameAllLength < FRAME_STRUCT_BIG_BYTE)
        {
            FrameParaReset();
            return(Error_H);
        }

        ProIndex = (ProIndex+2)%BUFFERLEN;//更新数据处理位置
        FrameState++;//解析状态指向下一状态
        dataNum -= 2; //获取帧去掉帧头、保留位和信息长度位的剩余长度

    case   FRAME_TRAIL://校验和
        if (dataNum < (FrameLength + 3))//若剩余数据小于帧长+CRC位，则返回 （3 = CRC的长度）
            return(Wait_H);

        //帧尾的校验和
        intTemp = (intTemp + FrameLength)%BUFFERLEN;
        RecvCheckCRC24 = RecvBuffer[intTemp];

        ////计算校验和
        //intTemp = FrameBegIndex;//获取该帧的起始位置（这里记录的是LEN开始的位置，因为校验和是LEN到PAYLOAD的累加和的低8位）
        //intTemp = (intTemp + FRAME_HEAD_LENGTH_BYTE) % BUFFERLEN;
        //for (i = 0; i<(FrameLength + 1); i++)//取数据及校验和计算 (有效长度 + len的长度)
        //{
        //    Sum += RecvBuffer[intTemp];
        //    intTemp = (intTemp+1)%BUFFERLEN;
        //}
        //CalculateCheckSum = Sum % 256;

        //if (CalculateCheckSum == RecvCheckSum)//校验和通过
        if (true)//校验和通过
        {
            //取帧
            //intTemp = FrameBegIndex;//获取该帧的起始位置
            //for(i=0; i<FrameLength; i++)//取数据及校验和计算
            //{
            //	FrameBuffer[i] = RecvBuffer[intTemp];
            //	intTemp = (intTemp+1)%BUFFERLEN;
            //}

            //dataNum -= (FrameLength + 1);

            //取帧
            intTemp = FrameBegIndex;//获取该帧的起始位置
            if((intTemp + FrameAllLength) <= BUFFERLEN)
            {
                memcpy(FrameBuffer,&RecvBuffer[intTemp],FrameAllLength);
            }
            else
            {
                memcpy(FrameBuffer,&RecvBuffer[intTemp],BUFFERLEN-intTemp);
                memcpy(FrameBuffer+(BUFFERLEN-intTemp),RecvBuffer,FrameAllLength-(BUFFERLEN-intTemp));
            }

            ProIndex = (ProIndex + (FrameLength + 1)) % BUFFERLEN;//更新数据处理位置

            FrameBegIndex = 0;
            FrameState    = 0;

            ReadOffset = 0;
            return FrameAllLength;
        }
        else//校验和未通过
        {
            FrameParaReset();

            //将已处理数据位置退帧长度字节数(需要改不)
            for(i=0; i<4; i++)
            {
                ProIndex = (ProIndex == 0) ? (BUFFERLEN-1) : (ProIndex -1);
            }
            return(Error_H);
        }

    default:
        ReturnStat = Error_H;
        FrameParaReset();
        break;
    }

    return ReturnStat;
}



/*******************************************************************************
函数说明:	解析帧数据
参   数:	char *pBuffer，需要解析的缓冲区
        int BufLength，缓冲区数据长度
返   回:	解析状态：Wait_H：等待下一个数据，Error_H：解析错误，其他表示帧长度
示   例：	无
*******************************************************************************/
int FrameParseClass::FrameParse(char *pBuffer, int BufLength)
{
    if ((pBuffer != NULL) && (BufLength > 0))
    {
        if (BufLength > BUFFERLEN)
        {
            FrameParaReset();
            return Error_H;
        }

        if (RecvIndex + BufLength < BUFFERLEN)
        {
            memcpy(RecvBuffer + RecvIndex, pBuffer, BufLength);
            RecvIndex += BufLength;
        }
        else
        {
            memcpy(RecvBuffer + RecvIndex, pBuffer, BUFFERLEN - RecvIndex);
            memcpy(RecvBuffer, pBuffer + (BUFFERLEN - RecvIndex), BufLength - (BUFFERLEN - RecvIndex));
            RecvIndex = BufLength - (BUFFERLEN - RecvIndex);
        }
    }

    return StatusFrameParse();
}



/**************************************************************
函数功能:	校验和计算函数
函数参数:	const char *pFrameBuffer，帧内容缓冲区
        int FrameLen，帧内容长度
函数返回:	命令包数据长度
**************************************************************/
char FrameParseClass::CalculateCheckSum(const char *pFrameBuffer, int FrameLen)
{
    int i=0;
    unsigned int FrameSum = 0;
    char FrameSum2  = 0;
    char Sumcheak = 0;

    //计算校验和
    for(i=0; i<FrameLen; i++)
    {
        FrameSum += pFrameBuffer[i];
        FrameSum2 += pFrameBuffer[i];
    }

    Sumcheak = FrameSum & (char(0xff));
    char Sumcheak1 = FrameSum %256;

    return Sumcheak;
}





/**************************************************************
函数功能:	帧封装函数
函数参数:	int CmdType,命令类型
            char *pBuffer，帧参数缓冲区
            int FrameLen，（包序号加有效数据的长度）
            char *pFrameBuffer,完整帧缓冲区
函数返回:	完整帧的长度
**************************************************************/ 
int FrameParseClass::FramePack(char CmdType, char *pBuffer, int FrameLen, char *pFrameBuffer)
{
    int SendCmdLen = 0;
    char CheckSum = 0;
    int Offset = 0;

    if(pFrameBuffer == NULL)
    {
        return 0;
    }

    memcpy(pFrameBuffer + Offset, Frame_h, sizeof(Frame_h));//帧头 2字节
    Offset += sizeof(Frame_h);

    SendCmdLen = (FrameLen + FRAME_STRUCT_BIG_BYTE);

    int Len = FrameLen + 1;//2021.11.18改的协议(需要包括MSG的长度（+1）)
    memcpy(pFrameBuffer + Offset, &Len, sizeof(char));//帧长度（是MSG到PAYLOAD的长度） 1字节
    Offset += sizeof(char);

    memcpy(pFrameBuffer + Offset, &CmdType, sizeof(char));//帧标识符
    Offset += sizeof(char);

    if(FrameLen > 0)
    {
        memcpy(pFrameBuffer + Offset, pBuffer, FrameLen);//帧序号和数据
        Offset += FrameLen;
    }

    CheckSum = CalculateCheckSum(pFrameBuffer + FRAME_HEAD_LENGTH_BYTE, SendCmdLen - 3);//计算校验和（帧长度到数据）

    pFrameBuffer[Offset] = CheckSum;

    return SendCmdLen;

}









//注意
//以下是文件u盘李文件解析，以上是串口接收数据解析




/*******************************************************************************
函数说明:数据文件解析函数
参   数:
返   回:	解析状态：Wait_H：等待下一个数据，Error_H：解析错误，其他表示帧长度
示   例：
日   期:2021.11.09
*******************************************************************************/
int FrameParseClass::FileStatusFrameParse(void)
{
    int ReturnStat = 0;//#define Wait_H   (1)  //#define True_H  (0)  //#define Error_H (-1)

    unsigned int intTemp, i, dataNum;
    unsigned char FileunCharTemp[4] = { 0 };
    unsigned int Sum = 0;
    char RecvCheckSum = 0;//接收到的校验和
    char CalculateCheckSum = 0;//计算的校验和

    RemainIndex = (RecvIndex - ProIndex + BUFFERLEN) % BUFFERLEN;//更新未处理数据
    dataNum = RemainIndex;//用临时变量记录剩余长度

    switch (FrameState)
    {
    case   FRAME_HEAD:
        if (dataNum >= 4)
        {
            for (i = 0; i < dataNum - 3; i++)
            {
                intTemp = ProIndex;//获取未处理数据的起始位置

                //获取帧头数据
                memset(FileunCharTemp, 0, sizeof(FileunCharTemp));
                FileunCharTemp[0] = RecvBuffer[intTemp];
                intTemp = (intTemp + 1) % BUFFERLEN;
                FileunCharTemp[1] = RecvBuffer[intTemp];
                intTemp = (intTemp + 1) % BUFFERLEN;
                FileunCharTemp[2] = RecvBuffer[intTemp];
                intTemp = (intTemp + 1) % BUFFERLEN;
                FileunCharTemp[3] = RecvBuffer[intTemp];
                intTemp = (intTemp + 1) % BUFFERLEN;

                //判断帧头
                if (My_Strncmp((char*)FileunCharTemp, FileFrame_h, sizeof(FileFrame_h)) == 0)
                {
                    FrameBegIndex = ProIndex;//若帧头正确，则保持一帧有效的数据起始位置
                    ProIndex = (ProIndex + 4) % BUFFERLEN;//更新数据处理位置
                    FrameState++;//解析状态指向下一状态
                    dataNum = dataNum - i - 4;//获取帧去掉帧头剩余长度
                    break;
                }
                else
                {
                    ProIndex = (ProIndex + 1) % BUFFERLEN;//若帧头不正确，更新数据处理位置
                }

                if (i == (dataNum - 4))//数据处理完成，则返回
                {
                    return(Wait_H);
                }
            }
        }
        else//若数据小于帧头长度，则返回
        {
            return(Wait_H);
        }

    case   FRAME_LENGTH:
        if (dataNum < FILE_ALL_LEN - FILE_HEAD_LEN)//若数荬度小于帧长度字节数，则返回
            return(Wait_H);

        FrameAllLength = FILE_ALL_LEN;//（每一帧的数据长度是300的数据 + 4的帧头）

         //取帧
        intTemp = FrameBegIndex;//获取该帧的起始位置
        if ((intTemp + FrameAllLength) <= BUFFERLEN)
        {
            memcpy(FrameBuffer, &RecvBuffer[intTemp], FrameAllLength);
        }
        else
        {
            memcpy(FrameBuffer, &RecvBuffer[intTemp], BUFFERLEN - intTemp);
            memcpy(FrameBuffer + (BUFFERLEN - intTemp), RecvBuffer, FrameAllLength - (BUFFERLEN - intTemp));
        }

        ProIndex = (ProIndex + (FILE_ALL_LEN - FILE_HEAD_LEN)) % BUFFERLEN;//更新数据处理位置

        FrameBegIndex = 0;
        FrameState = 0;

        ReadOffset = 0;
        return FrameAllLength;

    default:
        ReturnStat = Error_H;
        FrameParaReset();
        break;
    }

    return ReturnStat;
}



/*******************************************************************************
函数说明:	文件 解析帧数据
参   数:	char *pBuffer，需要解析的缓冲区
        int BufLength，缓冲区数据长度
返   回:	解析状态：Wait_H：等待下一个数据，Error_H：解析错误，其他表示帧长度
示   例：
日   期:2021.11.09
*******************************************************************************/
int FrameParseClass::FileFrameParse(char* pBuffer, int BufLength)
{
    if ((pBuffer != NULL) && (BufLength > 0))
    {
        if (BufLength > BUFFERLEN)
        {
            FrameParaReset();
            return Error_H;
        }

        if (RecvIndex + BufLength < BUFFERLEN)
        {
            memcpy(RecvBuffer + RecvIndex, pBuffer, BufLength);
            RecvIndex += BufLength;
        }
        else
        {
            memcpy(RecvBuffer + RecvIndex, pBuffer, BUFFERLEN - RecvIndex);
            memcpy(RecvBuffer, pBuffer + (BUFFERLEN - RecvIndex), BufLength - (BUFFERLEN - RecvIndex));
            RecvIndex = BufLength - (BUFFERLEN - RecvIndex);
        }
    }

    return FileStatusFrameParse();
}
