#include "DataSplit.h"
using namespace std;

#pragma execution_character_set("utf-8")//��� qt���������QTextedit��ʾ������������


DataSplit::DataSplit(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    InitUI();
    variableInit();
    SlotLinkInit();
    initcom();
}

DataSplit::~DataSplit()
{
    // �˳��������߳�
    m_serialThread_1.quit();
    m_serialThread_1.wait();

    m_serialThread_2.quit();
    m_serialThread_2.wait();

    delete m_serial_1;
    m_serial_1 = nullptr;

    delete m_serial_2;
    m_serial_2 = nullptr;
}

/// <summary>
/// ��ʼ������
/// </summary>
void DataSplit::InitUI()
{
    //��Ӳ�����
    ui.Rcb_Baudrate->addItem("9600", 9600); 
    ui.Rcb_Baudrate->addItem("115200", 115200);
    ui.Rcb_Baudrate->addItem("614400", 614400);
    ui.Rcb_Baudrate->setCurrentIndex(1);

    ui.Scb_Baudrate->addItem("9600", 9600);
    ui.Scb_Baudrate->addItem("115200", 115200);
    ui.Scb_Baudrate->addItem("614400", 614400);
    ui.Scb_Baudrate->setCurrentIndex(1);

    ui.Sbt_sendData->setVisible(false);
}

/// <summary>
/// ������ʼ��  �ڱ���Ŀ�ж�m_serial_1 ��m_serial_2���źźͲ۶��������ˣ�
/// ���Ƕ�ֻ��������һ���֣�used��������ֻ�з��ͣ�����ֻ�н��ռ�������
/// </summary>
void DataSplit::variableInit()
{
    m_comFlashTimerID = this->startTimer(1000);//�ȴ�1�� 
    m_appPath = QApplication::applicationDirPath();

    //����1�����մ���;����2�����ʹ���
    // 1.�½����ڴ������߳�1
    //m_serialThread_1.setPriority(QThread::TimeCriticalPriority);
    m_serial_1 = new QSerialPort();
    m_serialWorker_1 = new SerialWorker(m_serial_1, m_appPath);
    m_serialWorker_1->moveToThread(&m_serialThread_1);

    // 1.�½����ڴ������߳�2
    m_serialThread_2.setPriority(QThread::TimeCriticalPriority);
    m_serial_2 = new QSerialPort();
    m_serialWorker_2 = new SerialWorker(m_serial_2, m_appPath);
    m_serialWorker_2->moveToThread(&m_serialThread_2);

    // 2.�����źźͲ�1
    connect(&m_serialThread_1, &QThread::finished, m_serialWorker_1, &QObject::deleteLater); // �߳̽������Զ�ɾ������ used
    //connect(this, &DataSplit::serialDataSend, serialWorker1, &SerialWorker::doDataSendWork);// ���߳̽���Ҫ���͵����ݸ����߳�1���з���
    connect(m_serial_1, &QSerialPort::readyRead, m_serialWorker_1, &SerialWorker::doDataReciveWork);//���߳�1�յ�����used
    connect(m_serialWorker_1, &SerialWorker::recvResultToGui, this, &DataSplit::handleResults);//���߳�1���յ������ݴ������߳�
    connect(m_serialWorker_1, &SerialWorker::sendResultMsg, this, &DataSplit::ShowMessageTOEdit);//���߳���Ϣ��ʾ

     // 2.�����źźͲ�2
    connect(&m_serialThread_2, &QThread::finished, m_serialWorker_2, &QObject::deleteLater);// �߳̽������Զ�ɾ������ used
    //connect(this, &DataSplit::serialDataSend, m_serialWorker_2, &SerialWorker::doDataSendWork);// ���߳̽���Ҫ���͵����ݸ����߳�2���з���used
    //connect(m_serial_2, &QSerialPort::readyRead, serialWorker2, &SerialWorker::doDataReciveWork);// ���߳�2�յ�����
    //connect(serialWorker2, &SerialWorker::recvResultToGui, this, &DataSplit::handleResults);// ���߳�2���յ������ݴ������߳�
    connect(m_serialWorker_2,&SerialWorker::sendResultMsg,this,&DataSplit::ShowMessageTOEdit);//���߳���Ϣ��ʾ
    connect(this, &DataSplit::SendFileSignal, m_serialWorker_2, &SerialWorker::DoSendFileSignal);// ���߳�Ҫ�����߳�2�����ļ�
    connect(m_serialWorker_2, &SerialWorker::sendResultToGui, this, &DataSplit::handleResults);//���߳�1���յ������ݴ������߳�

    //TODO������Լ����Լ�����Ҫ�����connectע�͵�
    connect(m_serialWorker_1, &SerialWorker::sendResultToSplit, m_serialWorker_2, &SerialWorker::doDataSendWork);//���߳�1���յ������ݴ������߳�2

    // 3.��ʼ�������߳�1
    m_serialThread_1.start(); // �߳̿�ʼ����
    // 3.��ʼ�������߳�2
    m_serialThread_2.start(); // �߳̿�ʼ����

    // �����߳̽��մ�������
    //connect(&m_serial_1, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);
}

/// <summary>
/// ��ʼ���ۺ���
/// </summary>
void DataSplit::SlotLinkInit()
{
    connect(ui.Rbt_OpenCom, &QPushButton::clicked, this, &DataSplit::OpenRecvCom); 
    connect(ui.Sbt_OpenCom, &QPushButton::clicked, this, &DataSplit::OpenSendCom);
    connect(ui.Sbt_sendData, &QPushButton::clicked, this, &DataSplit::SendData); 
    connect(ui.Rcb_saveFile, &QPushButton::clicked, this, &DataSplit::SaveRecvData);
    connect(ui.pb_clear, &QPushButton::clicked, this, &DataSplit::InfoClear);
}

/// <summary>
/// ��Ϣ��ʾ����
/// </summary>
/// <param name="msg"></param>
/// <param name="colordex"></param>
void DataSplit::ShowMessageTOEdit(QString msg, int colordex)
{
    QString showstr = "";
    QDateTime dateTime = QDateTime::currentDateTime();//��ȡϵͳ��ǰ��ʱ��
    QString timestr1 = dateTime.toString("yyyy-MM-dd hh:mm:ss");//��ʽ��ʱ��
    QString timestr = dateTime.toString("hh:mm:ss");

    //ui.Log_textEdit->setStyleSheet("QTextEdit{padding-top:2px;background:#f7f7f7;border:none;border-radius:5px;font-size:14px;color:#778899;"
    //                                   "font-family:Microsoft YaHei;padding-left:5px;padding-right:5px;}");//�ޱ߿�
   
    showstr = QString("��%1��%2").arg(timestr).arg(msg);
    switch (colordex)
    {
    case -1:
        //showstr = QStringLiteral("<font color = red>��%1��%2</font>").arg(timestr).arg(msg);//����
        ui.Log_textEdit->append("<font color=\"#ff0000\">" + showstr + "</font>");
        break;
    case 0:
        ui.Log_textEdit->append("<font color=\"#949494\">" + showstr + "</font>");
        break;
    case 1:
        ui.Log_textEdit->append("<font color=\"#00aa7f\">" + showstr + "</font>");//��ʾ��ɫ
        break;
    default:
        ui.Log_textEdit->append("<font color=\"#ff0000\">" + showstr + "</font>");//��ʾ��
        break;
    }
    //ui.Log_textEdit->append(showstr);
}


/// <summary>
/// ��Ϣ��ʾ����
/// </summary>
/// <param name="msg"></param>
/// <param name="colordex"></param>
void DataSplit::ShowMessageTOEdit2(QString msg, int colordex)
{
    QString showstr = "";
    QDateTime dateTime = QDateTime::currentDateTime();//��ȡϵͳ��ǰ��ʱ��
    QString timestr1 = dateTime.toString("yyyy-MM-dd hh:mm:ss");//��ʽ��ʱ��
    QString timestr = dateTime.toString("hh:mm:ss");

    //ui.Log_textEdit->setStyleSheet("QTextEdit{padding-top:2px;background:#f7f7f7;border:none;border-radius:5px;font-size:14px;color:#778899;"
    //                                   "font-family:Microsoft YaHei;padding-left:5px;padding-right:5px;}");//�ޱ߿�

    showstr = QString("��%1��%2").arg(timestr).arg(msg);
    switch (colordex)
    {
    case -1:
        //showstr = QStringLiteral("<font color = red>��%1��%2</font>").arg(timestr).arg(msg);//����
        ui.Log_textEdit_2->append("<font color=\"#ff0000\">" + showstr + "</font>");
        break;
    case 0:
        ui.Log_textEdit_2->append("<font color=\"#949494\">" + showstr + "</font>");
        break;
    case 1:
        ui.Log_textEdit_2->append("<font color=\"#00aa7f\">" + showstr + "</font>");//��ʾ��ɫ
        break;
    default:
        ui.Log_textEdit_2->append("<font color=\"#ff0000\">" + showstr + "</font>");//��ʾ��
        break;
    }
    //ui.Log_textEdit->append(showstr);
}

/// <summary>
/// ��ʱ����Ӧ�¼�
/// </summary>
/// <param name="event"></param>
void DataSplit::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_comFlashTimerID)
    {
        //initcom();
    }

}

void DataSplit::initcom()
{
    QSerialPort TempSerial;
    ui.Rcb_com->clear();
    ui.Scb_com->clear();
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        TempSerial.setPortName(info.portName());
        //if (TempSerial.open(QIODevice::ReadWrite))
        //{
            QString showName = info.portName();// +":" + info.description();
            ui.Rcb_com->addItem(showName);
            ui.Scb_com->addItem(showName);
        //    TempSerial.close();
        //}
    }
}

/// <summary>
/// �򿪽��մ���
/// </summary>
void DataSplit::OpenRecvCom()
{
    if (m_serial_1->isOpen() == true)
        m_serial_1->close();

    int SelectBaud = 0;
    if (ui.Rbt_OpenCom->text() == QString("�򿪴���"))
    {
        //���ô�����
        m_serial_1->setPortName(ui.Rcb_com->currentText());
        SelectBaud = ui.Rcb_Baudrate->currentData().toInt();
        m_serial_1->setBaudRate(SelectBaud);//���ò�����

        int b = m_serial_1->baudRate();
        m_serial_1->setDataBits(QSerialPort::Data8);//��������λ
        m_serial_1->setParity(QSerialPort::NoParity);//����У��λ
        m_serial_1->setStopBits(QSerialPort::OneStop);//����ֹͣλ
        m_serial_1->setFlowControl(QSerialPort::NoFlowControl);//����������


        //try
        //{
        //    m_serial_1->open(QIODevice::ReadWrite);  //�׳��쳣
        //}
        //catch (exception e)
        //{
        //    QMessageBox::warning(this, "��ʾ", e.what());   //�����쳣��Ȼ��������
        //    ShowMessageTOEdit(ui.Rcb_com->currentText() + " ���ڴ�ʧ��", -1);
        //    return;
        //}

        if (m_serial_1->open(QIODevice::ReadWrite) == false)
        {
            QMessageBox::warning(this, "��ʾ", "���ڴ�ʧ��!");
            ShowMessageTOEdit(ui.Rcb_com->currentText() + " ���ڴ�ʧ��", -1);
            return;
        }
        int bb = m_serial_1->baudRate();
        m_serial_1->clear();
        ui.Rlb_stateT->setText("״̬:�Ѵ�");
        ui.Rlb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_green.png"));
        ShowMessageTOEdit(ui.Rcb_com->currentText() + " �����Ѵ�", 1);
        ui.Rbt_OpenCom->setText("�رմ���");

        ui.Rcb_com->setEnabled(false);
        ui.Rcb_Baudrate->setEnabled(false);
    }
    else
    {
        m_serial_1->clear();
        m_serial_1->close();
        ui.Rbt_OpenCom->setText("�򿪴���");
        ui.Rlb_stateT->setText("״̬:�ѹر�");
        ShowMessageTOEdit(ui.Rcb_com->currentText() + " �����ѹر�", 1);
        ui.Rlb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_red.png"));

        ui.Rcb_com->setEnabled(true);
        ui.Rcb_Baudrate->setEnabled(true);
    }
}


/// <summary>
/// �򿪷��ʹ���
/// </summary>
void DataSplit::OpenSendCom()
{
    if (m_serial_2->isOpen() == true)
        m_serial_2->close();

    int SelectBaud = 0;
    if (ui.Sbt_OpenCom->text() == QString("�򿪴���"))
    {
        //���ô�����
        m_serial_2->setPortName(ui.Scb_com->currentText());
        SelectBaud = ui.Scb_Baudrate->currentData().toInt();
        m_serial_2->setBaudRate(SelectBaud);//���ò�����

        int b = m_serial_2->baudRate();
        m_serial_2->setDataBits(QSerialPort::Data8);//��������λ
        m_serial_2->setParity(QSerialPort::NoParity);//����У��λ
        m_serial_2->setStopBits(QSerialPort::OneStop);//����ֹͣλ
        m_serial_2->setFlowControl(QSerialPort::NoFlowControl);//����������

        if (m_serial_2->open(QIODevice::ReadWrite) == false)
        {
            QMessageBox::warning(this, "��ʾ", "���ڴ�ʧ��!");
            ShowMessageTOEdit(ui.Scb_com->currentText() + " ���ڴ�ʧ��", -1);
            return;
        }
        int bb = m_serial_2->baudRate();
        m_serial_2->clear();
        ui.Slb_stateT->setText("״̬:�Ѵ�");
        ui.Slb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_green.png"));
        ShowMessageTOEdit(ui.Scb_com->currentText() + " �����Ѵ�", 1);
        ui.Sbt_OpenCom->setText("�رմ���");

        ui.Scb_com->setEnabled(false);
        ui.Scb_Baudrate->setEnabled(false);
    }
    else
    {
        m_serial_2->clear();
        m_serial_2->close();
        ui.Sbt_OpenCom->setText("�򿪴���");
        ui.Slb_stateT->setText("״̬:�ѹر�");
        ShowMessageTOEdit(ui.Scb_com->currentText() + " �����ѹر�", 1);
        ui.Slb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_red.png"));

        ui.Scb_com->setEnabled(true);
        ui.Scb_Baudrate->setEnabled(true);
    }
}

/// <summary>
/// �����ļ�
/// </summary>
void DataSplit::SaveRecvData()
{
    bool isSave = ui.Rcb_saveFile->checkState();
    m_serialWorker_1->SetSaveIndex(isSave);
}

/// <summary>
/// �����Ϣ
/// </summary>
void DataSplit::InfoClear()
{
    ui.Log_textEdit->clear();
    ui.Log_textEdit_2->clear();
}

/// <summary>
/// �������̴߳��ڽ��յ������ݺͷ������̷߳��͵����ݣ����̸߳��ݾ�������ж��Ƿ���Ҫ��ʾ
/// </summary>
/// <param name="rbuffer"></param>
/// <param name="comIndex"></param>
void DataSplit::handleResults(QByteArray rbuffer,int comIndex)
{
    bool isShow = false;
    QString bufferStr = "";
    //���յ�����
    if (comIndex == 1)
    {
        bufferStr = "�������ݣ�";
        isShow = ui.Rcb_showRData->checkState();

        if (isShow == true)
        {
            bufferStr = bufferStr + ByteArrayToHexString(rbuffer);
            ShowMessageTOEdit(bufferStr, 1);
        }
    }
        
    //���͵�����
    if (comIndex == 2)
    {
        bufferStr = "�������ݣ�";
        isShow = ui.Scb_showRData->checkState();
        if (isShow == true)
        {
            bufferStr = bufferStr + ByteArrayToHexString(rbuffer);
            ShowMessageTOEdit2(bufferStr, 1);
        }
    }

    //qDebug() << "���߳��յ�������ݣ�" << rbuffer << "�߳�ID��" << QThread::currentThreadId();
}

/// <summary>
/// ���ֽ�����ת��Ϊ��Ӧ��16�����ַ���
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
QString DataSplit::ByteArrayToHexString(QByteArray data)
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
/// ���ʹ��ڷ������ݣ�����ʹ�ã�
/// </summary>
void DataSplit::SendData()
{
#if 0
    //��ȡ�����ϵ����ݲ�ת����utf8��ʽ���ֽ���
    QByteArray data;
    for (int i = 0; i < 10; i++) data[i] = i;
    //m_serial_1.write(data);// �����̷߳���
    emit serialDataSend(data);// �����̷߳���
    qDebug() << "���̷߳����źţ��߳�ID��" << QThread::currentThreadId();

#else
    //�����ļ�
    QString openFilePath = m_appPath + "/xpFileTest.ais";
    emit SendFileSignal(openFilePath);
    ShowMessageTOEdit("�ļ������У����Ե�......", 1);


#endif // 1
}

