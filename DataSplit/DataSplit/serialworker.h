#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QSerialPort>
#include <QFile>
#include <QDateTime>
#include <QEvent>

#include "FrameParseClass.h"

class SerialWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorker(QSerialPort *ser, QString appPath, QObject *parent = nullptr);


protected:
    virtual void timerEvent(QTimerEvent* event);//定时器检测事件

signals:
    void sendResultToGui(QByteArray rbuffer, int comIndex);
    void recvResultToGui(QByteArray rbuffer, int comIndex);
    void sendResultToSplit(char* data, int rLen);
    void sendResultMsg(QString msg, int colordex);

public slots:
    void doDataSendWork(char* data, int sendLen);
    void doDataReciveWork();
    void DoSendFileSignal(QString isSave);

private:
    QSerialPort *serial;

    void TestSaveFile(char* dataBuf, int len);

    bool m_isSaveFile;
    QString m_appPath;
    QFile* m_savefile;
    FrameParseClass* m_MyFrame;

    QByteArray m_buffer;

    QByteArray m_sendArray;
    QByteArray m_trueSendArray;
    int m_PackgNumb = 0;

    int m_recvTimerID_100ms;
    bool m_isRun = false;


public:

    QString ByteArrayToHexString(QByteArray data);
    QByteArray HexStringToByteArray(QString HexString);
    void SetSaveIndex(bool isSave);
   
};

#endif // SERIALWORKER_H
