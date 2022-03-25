#include "serialworker.h"
#include <QDebug>
#include <QThread>

#pragma execution_character_set("utf-8")//解决 qt中如何设置QTextedit显示中文乱码问题

SerialWorker::SerialWorker(QSerialPort* ser, QString appPath, QObject* parent)
    : QObject(parent),
    serial(ser),
    m_appPath(appPath),
    m_savefile(nullptr),
    m_isSaveFile(false),
    m_MyFrame(nullptr)
{
    m_MyFrame = new FrameParseClass();
}


/// <summary>
/// 数据发送
/// </summary>
/// <param name="data"></param>
void SerialWorker::doDataSendWork(char* data, int sendLen)
{
    int slen = 0;

    if (sendLen <= 0)
    {
        emit sendResultMsg("发送串口发送长度为0！", -1);
        return;
    }

    if (serial->isOpen() == false)
    {
        emit sendResultMsg("发送串口未打开！", -1);
        return;
    }

    //TODO需要进行拆分，然后组成特定的再进行发送(见组包协议)整包拆
    int sNumb = (sendLen + 40) / 41;//每包的有效数据长度41
    char pType = 0;
    char pIndex = 0;
    char pTypeIndex = 0;
    char splitdata[42];
    int sendOneLen = 42;
    for (int i = 0; i < sNumb;i++)
    {
        memset(splitdata,0,42);
        if (sNumb <= 1)
        {
            splitdata[0] = 0;//单包数据
            memcpy(&splitdata[1], data, sendLen);//最后一包有不足的填0
            slen = serial->write(splitdata, 42);//最后一包有不足的填0
            serial->waitForBytesWritten(500);
            if (slen != 42)
            {
                emit sendResultMsg("发送子线程数据发送失败！", -1);
                return;
            }

            emit sendResultToGui(QByteArray(splitdata, slen), 2);

            return;
        }

        //包类型
        if (i == 0)
            pType = 01;
        else if (i != (sNumb - 1))
            pType = 10;    
        else
        {
            pType = 11;
            sendOneLen = sendLen % 41;

            //包序号
            pIndex = i % 64;
            pTypeIndex = ((pType << 6) & 0xff) | (pIndex & 0x3f);
            splitdata[0] = pTypeIndex;
            memcpy(&splitdata[1], &data[i * 41], sendOneLen);//最后一包有不足的填0
            slen = serial->write(splitdata, 42);//最后一包有不足的填0
            serial->waitForBytesWritten(500);
            if (slen != 42)
            {
                emit sendResultMsg("发送子线程数据发送失败！", -1);
                return;
            }

            emit sendResultToGui(QByteArray(splitdata, slen), 2);
            return;
        }
            
        //包序号
        pIndex = i%64; 
        pTypeIndex = ((pType<<6)&0xff)|(pIndex&0x3f);
        splitdata[0] = pTypeIndex;
        memcpy(&splitdata[1], &data[i * 41],41);

        slen = serial->write(splitdata, sendOneLen);
        serial->waitForBytesWritten(500);
        if (slen != 42)
        {
            emit sendResultMsg("发送子线程数据发送失败！", -1);
            return;
        }

        emit sendResultToGui(QByteArray(splitdata, slen), 2);
    }

    //emit sendResultMsg("发送数据完成！", 1);
}

/// <summary>
/// 数据接收
/// </summary>
void SerialWorker::doDataReciveWork()
{
#if 0
    qint64 RecvLen = 0;
    // 1.收到数据
    if (serial->bytesAvailable() <= 0) //判断接收缓冲区是否有数据
    {
        return;
    }
    int numData = serial->bytesAvailable();
    m_buffer.resize(numData);//分配空间
    RecvLen = serial->read(m_buffer.data(), numData);
    
    // 2.进行数据处理
    //TODO需要进行校验收到的数据是否正确，然后再给主线程和串口发送线程（接收拆包协议）
    if (RecvLen > 0)
    {
        //for (; ; )
        //{
        //    int FrameLen = m_MyFrame->FrameParse(buffer.data(), (int)RecvLen);//解帧FrameLength
        //    RecvLen = 0;

        //    if (FrameLen == m_MyFrame->Wait_H) break;
        //    if (FrameLen == m_MyFrame->Error_H) continue;

        //    // 3.将收到数据发送到主线程和串口发送线程
        //    QByteArray array;
        //    array = QByteArray(m_MyFrame->FrameBuffer, m_MyFrame->FrameAllLength);
        //    emit recvResultToGui(array, 1);
        //    emit sendResultToSplit(m_MyFrame->FrameBuffer, m_MyFrame->FrameAllLength);

        //    if (m_isSaveFile == true)
        //    {
        //        TestSaveFile(m_MyFrame->FrameBuffer, m_MyFrame->FrameAllLength);
        //    }
        //}

        for (; ; )
        {
            int FrameLen = m_MyFrame->FrameParse(m_buffer.data(), (int)RecvLen);//解帧FrameLength
            RecvLen = 0;

            if (FrameLen == m_MyFrame->Wait_H) break;
            if (FrameLen == m_MyFrame->Error_H) continue;

            m_PackgNumb++;

            QByteArray array;
            array = QByteArray(m_MyFrame->FrameBuffer, m_MyFrame->FrameAllLength);

            m_sendArray = m_sendArray.append(array);
            
            if (m_PackgNumb == 2)//收到两个D3 （两帧数据后发送）
            {
                
                // 3.将收到数据发送到主线程和串口发送线程
                m_trueSendArray.clear();
                m_trueSendArray = m_sendArray;
                m_sendArray.clear();
                m_PackgNumb = 0;

                emit recvResultToGui(m_trueSendArray, 1);
                emit sendResultToSplit(m_trueSendArray.data(), m_trueSendArray.size());

                if (m_isSaveFile == true)
                {
                    TestSaveFile(m_trueSendArray.data(), m_trueSendArray.size());
                }
            }
        }


    }

#else

    QByteArray array = serial->readAll();
    //m_trueSendArray.clear();

    m_sendArray.append(array);

    if (m_isRun == false)
    {
        m_isRun = true;
        m_recvTimerID_100ms = this->startTimer(100);//等待100毫秒 
    }
   
    
#endif

    //// 3.将收到数据发送到主线程和串口发送线程
    //emit sendResultToGui(buffer, RecvLen);
    //emit sendResultToSplit(buffer.data(), RecvLen);

    //if (m_isSaveFile == true)
    //{
    //    TestSaveFile(buffer.data(), buffer.size());
    //}

}


/// <summary>
/// 定时器响应事件
/// </summary>
/// <param name="event"></param>
void SerialWorker::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_recvTimerID_100ms)
    {
        m_trueSendArray = m_sendArray;
        m_sendArray.clear();

        emit recvResultToGui(m_trueSendArray, 1);
        emit sendResultToSplit(m_trueSendArray.data(), m_trueSendArray.size());

        if (m_isSaveFile == true)
        {
            TestSaveFile(m_trueSendArray.data(), m_trueSendArray.size());
        }
        m_isRun = false;
        killTimer(m_recvTimerID_100ms);
    }

}

/// <summary>
/// 保存接收到的数据
/// </summary>
/// <param name="dataBuf"></param>
/// <param name="len"></param>
//long static allLenn = 0;
//QString savePath = "C:/Users/Administrator/Desktop/FileTestsave2022.ais";
//QFile* m_file = new QFile(savePath);
void SerialWorker::TestSaveFile(char* dataBuf, int len)
{
    int slen = 0;
    long allLenn = 0;
    QString saveLenstr = "";
    QString timestr;
    QString saveFilePath;
    timestr = QDateTime::currentDateTime().toString("hh:mm:ss");//获取系统当前的时间

    if (m_savefile == nullptr)
    {
        saveFilePath = m_appPath + "/RecvRtkData.txt";
        m_savefile = new QFile(saveFilePath);
    }
    QString hexStr = ByteArrayToHexString(QByteArray(dataBuf, len));//char*转为QByteArray要注意转换长度
    QString writeStr = QString("[%1]%2").arg(timestr).arg(hexStr);

    m_savefile->open(QIODevice::WriteOnly | QIODevice::Append);
    slen = m_savefile->write(writeStr.toLatin1().data());
    m_savefile->write("\r\n");
    m_savefile->write("\r\n");
    allLenn += slen;
    if (slen != writeStr.size())
    {
        emit sendResultMsg("存储文件错误！", -1);
    }
    m_savefile->close();

    //saveLenstr = QString("存储长度：%1").arg(allLenn);
    //emit sendResultMsg(saveLenstr, 1);
}


/// <summary>
/// 将字节序列转换为对应的16进制字符串
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
QString SerialWorker::ByteArrayToHexString(QByteArray data)
{
    QString ret(data.toHex().toUpper());
    int len = ret.length() / 2;
    for (int i = 1; i < len; i++)
    {
        ret.insert(2 * i + i - 1, " ");
    }

    return ret;
}

/// <summary>
/// 将16进制字符串转换为对应的字节序列
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
QByteArray SerialWorker::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach(QString s, sl) {
        if (!s.isEmpty())
        {
            char c = s.toInt(&ok, 16) & 0xFF;
            if (ok) {
                ret.append(c);
            }
            else {
                qDebug() << "非法的16进制字符：" << s;
                //QMessageBox::warning(0, tr("错误："), QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
    //qDebug()<<ret;
    return ret;
}

/// <summary>
/// 主线程设置保存数据成文件标志
/// </summary>
/// <param name=""></param>
void SerialWorker::SetSaveIndex(bool isSave)
{
    m_isSaveFile = isSave;
}

/// <summary>
/// 发送文件
/// </summary>
/// <param name="isSave"></param>
void SerialWorker::DoSendFileSignal(QString sendPath)
{
    int readedLen = 0;
    long allLen = 0;
    long writeLen = 0;
    long allwriteLen = 0;
    static char* validBuffer = new char[1024];;//下次发送文件也不需要重新申请内存
    QString allLenStr = "";

    QFile* m_UpSaveFile = new QFile(sendPath);
    if (m_UpSaveFile->open(QIODevice::ReadOnly) == false)
    {
        emit sendResultMsg("xpFileTest.ais文件打开失败！", -1);
        return;
    }

    if (m_UpSaveFile->isOpen())
    {
        for (;;)
        {
            readedLen = m_UpSaveFile->read(validBuffer, 250);//每次发送250字节
            if (readedLen > 0)
            {
                allLen += readedLen;
                //serial->clear();
                writeLen = serial->write(validBuffer, readedLen);// 在主线程发送
                serial->waitForBytesWritten(500);
                //serial->flush();

                if (writeLen != readedLen)
                {
                    emit sendResultMsg("数据发送失败！", -1);
                    return;
                }
                allwriteLen += writeLen;
                readedLen = 0;
                writeLen = 0;
            }
            else
            {
                //关闭文件
                m_UpSaveFile->close();
                delete m_UpSaveFile;
                m_UpSaveFile = nullptr;
                allLenStr = QString("发送文件总长度:%1").arg(allwriteLen);
                emit sendResultMsg(allLenStr, 1);
                emit sendResultMsg("文件数据发送完成！", 1);
                return;
            }
        }
    }
}