#pragma once



#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include<QPushButton>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QFile>
#include <QDateTime>

#include <QtWidgets/QWidget>
#include "ui_DataSplit.h"
#include "serialworker.h"

class DataSplit : public QWidget
{
    Q_OBJECT

public:
    DataSplit(QWidget *parent = Q_NULLPTR);
    ~DataSplit();

protected:
    virtual void timerEvent(QTimerEvent* event);//��ʱ������¼�

private:
    Ui::DataSplitClass ui;


    QString m_appPath;
    QSerialPort* m_serial_1;   // ��������
    QThread m_serialThread_1; // �������߳�
    SerialWorker* m_serialWorker_1;

    QSerialPort* m_serial_2;   // ��������
    QThread m_serialThread_2; // �������߳�
    SerialWorker* m_serialWorker_2;


    int m_comFlashTimerID;

    void ShowMessageTOEdit(QString msg, int colordex);
    void ShowMessageTOEdit2(QString msg, int colordex);
    QString ByteArrayToHexString(QByteArray data);
    void InfoClear();

public:
    void InitUI();
    void variableInit();
    void SlotLinkInit();
    void initcom();
    void OpenSendCom();
    void OpenRecvCom();
    void SaveRecvData();
    void SendData();

signals:
    void serialDataSend(char* data,int sendLen);
    void SendFileSignal(QString sendPath);

public slots:
    void handleResults(QByteArray rbuffer, int comIndex);

};
