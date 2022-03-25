#include "DataSplit.h"
using namespace std;

#pragma execution_character_set("utf-8")//解决 qt中如何设置QTextedit显示中文乱码问题


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
    // 退出串口子线程
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
/// 初始化界面
/// </summary>
void DataSplit::InitUI()
{
    //添加波特率
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
/// 变量初始化  在本项目中对m_serial_1 和m_serial_2的信号和槽都连接上了，
/// 但是都只用了其中一部分（used），发送只有发送，接收只有接收即可满足
/// </summary>
void DataSplit::variableInit()
{
    m_comFlashTimerID = this->startTimer(1000);//等待1秒 
    m_appPath = QApplication::applicationDirPath();

    //串口1，接收串口;串口2，发送串口
    // 1.新建串口处理子线程1
    //m_serialThread_1.setPriority(QThread::TimeCriticalPriority);
    m_serial_1 = new QSerialPort();
    m_serialWorker_1 = new SerialWorker(m_serial_1, m_appPath);
    m_serialWorker_1->moveToThread(&m_serialThread_1);

    // 1.新建串口处理子线程2
    m_serialThread_2.setPriority(QThread::TimeCriticalPriority);
    m_serial_2 = new QSerialPort();
    m_serialWorker_2 = new SerialWorker(m_serial_2, m_appPath);
    m_serialWorker_2->moveToThread(&m_serialThread_2);

    // 2.连接信号和槽1
    connect(&m_serialThread_1, &QThread::finished, m_serialWorker_1, &QObject::deleteLater); // 线程结束，自动删除对象 used
    //connect(this, &DataSplit::serialDataSend, serialWorker1, &SerialWorker::doDataSendWork);// 主线程将需要发送的数据给子线程1进行发送
    connect(m_serial_1, &QSerialPort::readyRead, m_serialWorker_1, &SerialWorker::doDataReciveWork);//子线程1收到数据used
    connect(m_serialWorker_1, &SerialWorker::recvResultToGui, this, &DataSplit::handleResults);//子线程1将收到的数据传给主线程
    connect(m_serialWorker_1, &SerialWorker::sendResultMsg, this, &DataSplit::ShowMessageTOEdit);//子线程信息显示

     // 2.连接信号和槽2
    connect(&m_serialThread_2, &QThread::finished, m_serialWorker_2, &QObject::deleteLater);// 线程结束，自动删除对象 used
    //connect(this, &DataSplit::serialDataSend, m_serialWorker_2, &SerialWorker::doDataSendWork);// 主线程将需要发送的数据给子线程2进行发送used
    //connect(m_serial_2, &QSerialPort::readyRead, serialWorker2, &SerialWorker::doDataReciveWork);// 子线程2收到数据
    //connect(serialWorker2, &SerialWorker::recvResultToGui, this, &DataSplit::handleResults);// 子线程2将收到的数据传给主线程
    connect(m_serialWorker_2,&SerialWorker::sendResultMsg,this,&DataSplit::ShowMessageTOEdit);//子线程信息显示
    connect(this, &DataSplit::SendFileSignal, m_serialWorker_2, &SerialWorker::DoSendFileSignal);// 主线程要求子线程2发送文件
    connect(m_serialWorker_2, &SerialWorker::sendResultToGui, this, &DataSplit::handleResults);//子线程1将收到的数据传给主线程

    //TODO该软件自己发自己收需要把这个connect注释掉
    connect(m_serialWorker_1, &SerialWorker::sendResultToSplit, m_serialWorker_2, &SerialWorker::doDataSendWork);//子线程1将收到的数据传给子线程2

    // 3.开始运行子线程1
    m_serialThread_1.start(); // 线程开始运行
    // 3.开始运行子线程2
    m_serialThread_2.start(); // 线程开始运行

    // 在主线程接收串口数据
    //connect(&m_serial_1, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);
}

/// <summary>
/// 初始化槽函数
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
/// 信息显示界面
/// </summary>
/// <param name="msg"></param>
/// <param name="colordex"></param>
void DataSplit::ShowMessageTOEdit(QString msg, int colordex)
{
    QString showstr = "";
    QDateTime dateTime = QDateTime::currentDateTime();//获取系统当前的时间
    QString timestr1 = dateTime.toString("yyyy-MM-dd hh:mm:ss");//格式化时间
    QString timestr = dateTime.toString("hh:mm:ss");

    //ui.Log_textEdit->setStyleSheet("QTextEdit{padding-top:2px;background:#f7f7f7;border:none;border-radius:5px;font-size:14px;color:#778899;"
    //                                   "font-family:Microsoft YaHei;padding-left:5px;padding-right:5px;}");//无边框
   
    showstr = QString("【%1】%2").arg(timestr).arg(msg);
    switch (colordex)
    {
    case -1:
        //showstr = QStringLiteral("<font color = red>【%1】%2</font>").arg(timestr).arg(msg);//乱码
        ui.Log_textEdit->append("<font color=\"#ff0000\">" + showstr + "</font>");
        break;
    case 0:
        ui.Log_textEdit->append("<font color=\"#949494\">" + showstr + "</font>");
        break;
    case 1:
        ui.Log_textEdit->append("<font color=\"#00aa7f\">" + showstr + "</font>");//显示绿色
        break;
    default:
        ui.Log_textEdit->append("<font color=\"#ff0000\">" + showstr + "</font>");//显示红
        break;
    }
    //ui.Log_textEdit->append(showstr);
}


/// <summary>
/// 信息显示界面
/// </summary>
/// <param name="msg"></param>
/// <param name="colordex"></param>
void DataSplit::ShowMessageTOEdit2(QString msg, int colordex)
{
    QString showstr = "";
    QDateTime dateTime = QDateTime::currentDateTime();//获取系统当前的时间
    QString timestr1 = dateTime.toString("yyyy-MM-dd hh:mm:ss");//格式化时间
    QString timestr = dateTime.toString("hh:mm:ss");

    //ui.Log_textEdit->setStyleSheet("QTextEdit{padding-top:2px;background:#f7f7f7;border:none;border-radius:5px;font-size:14px;color:#778899;"
    //                                   "font-family:Microsoft YaHei;padding-left:5px;padding-right:5px;}");//无边框

    showstr = QString("【%1】%2").arg(timestr).arg(msg);
    switch (colordex)
    {
    case -1:
        //showstr = QStringLiteral("<font color = red>【%1】%2</font>").arg(timestr).arg(msg);//乱码
        ui.Log_textEdit_2->append("<font color=\"#ff0000\">" + showstr + "</font>");
        break;
    case 0:
        ui.Log_textEdit_2->append("<font color=\"#949494\">" + showstr + "</font>");
        break;
    case 1:
        ui.Log_textEdit_2->append("<font color=\"#00aa7f\">" + showstr + "</font>");//显示绿色
        break;
    default:
        ui.Log_textEdit_2->append("<font color=\"#ff0000\">" + showstr + "</font>");//显示红
        break;
    }
    //ui.Log_textEdit->append(showstr);
}

/// <summary>
/// 定时器响应事件
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
/// 打开接收串口
/// </summary>
void DataSplit::OpenRecvCom()
{
    if (m_serial_1->isOpen() == true)
        m_serial_1->close();

    int SelectBaud = 0;
    if (ui.Rbt_OpenCom->text() == QString("打开串口"))
    {
        //设置串口名
        m_serial_1->setPortName(ui.Rcb_com->currentText());
        SelectBaud = ui.Rcb_Baudrate->currentData().toInt();
        m_serial_1->setBaudRate(SelectBaud);//设置波特率

        int b = m_serial_1->baudRate();
        m_serial_1->setDataBits(QSerialPort::Data8);//设置数据位
        m_serial_1->setParity(QSerialPort::NoParity);//设置校验位
        m_serial_1->setStopBits(QSerialPort::OneStop);//设置停止位
        m_serial_1->setFlowControl(QSerialPort::NoFlowControl);//设置流控制


        //try
        //{
        //    m_serial_1->open(QIODevice::ReadWrite);  //抛出异常
        //}
        //catch (exception e)
        //{
        //    QMessageBox::warning(this, "提示", e.what());   //捕获异常，然后程序结束
        //    ShowMessageTOEdit(ui.Rcb_com->currentText() + " 串口打开失败", -1);
        //    return;
        //}

        if (m_serial_1->open(QIODevice::ReadWrite) == false)
        {
            QMessageBox::warning(this, "提示", "串口打开失败!");
            ShowMessageTOEdit(ui.Rcb_com->currentText() + " 串口打开失败", -1);
            return;
        }
        int bb = m_serial_1->baudRate();
        m_serial_1->clear();
        ui.Rlb_stateT->setText("状态:已打开");
        ui.Rlb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_green.png"));
        ShowMessageTOEdit(ui.Rcb_com->currentText() + " 串口已打开", 1);
        ui.Rbt_OpenCom->setText("关闭串口");

        ui.Rcb_com->setEnabled(false);
        ui.Rcb_Baudrate->setEnabled(false);
    }
    else
    {
        m_serial_1->clear();
        m_serial_1->close();
        ui.Rbt_OpenCom->setText("打开串口");
        ui.Rlb_stateT->setText("状态:已关闭");
        ShowMessageTOEdit(ui.Rcb_com->currentText() + " 串口已关闭", 1);
        ui.Rlb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_red.png"));

        ui.Rcb_com->setEnabled(true);
        ui.Rcb_Baudrate->setEnabled(true);
    }
}


/// <summary>
/// 打开发送串口
/// </summary>
void DataSplit::OpenSendCom()
{
    if (m_serial_2->isOpen() == true)
        m_serial_2->close();

    int SelectBaud = 0;
    if (ui.Sbt_OpenCom->text() == QString("打开串口"))
    {
        //设置串口名
        m_serial_2->setPortName(ui.Scb_com->currentText());
        SelectBaud = ui.Scb_Baudrate->currentData().toInt();
        m_serial_2->setBaudRate(SelectBaud);//设置波特率

        int b = m_serial_2->baudRate();
        m_serial_2->setDataBits(QSerialPort::Data8);//设置数据位
        m_serial_2->setParity(QSerialPort::NoParity);//设置校验位
        m_serial_2->setStopBits(QSerialPort::OneStop);//设置停止位
        m_serial_2->setFlowControl(QSerialPort::NoFlowControl);//设置流控制

        if (m_serial_2->open(QIODevice::ReadWrite) == false)
        {
            QMessageBox::warning(this, "提示", "串口打开失败!");
            ShowMessageTOEdit(ui.Scb_com->currentText() + " 串口打开失败", -1);
            return;
        }
        int bb = m_serial_2->baudRate();
        m_serial_2->clear();
        ui.Slb_stateT->setText("状态:已打开");
        ui.Slb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_green.png"));
        ShowMessageTOEdit(ui.Scb_com->currentText() + " 串口已打开", 1);
        ui.Sbt_OpenCom->setText("关闭串口");

        ui.Scb_com->setEnabled(false);
        ui.Scb_Baudrate->setEnabled(false);
    }
    else
    {
        m_serial_2->clear();
        m_serial_2->close();
        ui.Sbt_OpenCom->setText("打开串口");
        ui.Slb_stateT->setText("状态:已关闭");
        ShowMessageTOEdit(ui.Scb_com->currentText() + " 串口已关闭", 1);
        ui.Slb_state->setPixmap(QPixmap(":/DataSplit/image_1/Connection statuP_red.png"));

        ui.Scb_com->setEnabled(true);
        ui.Scb_Baudrate->setEnabled(true);
    }
}

/// <summary>
/// 保存文件
/// </summary>
void DataSplit::SaveRecvData()
{
    bool isSave = ui.Rcb_saveFile->checkState();
    m_serialWorker_1->SetSaveIndex(isSave);
}

/// <summary>
/// 清空信息
/// </summary>
void DataSplit::InfoClear()
{
    ui.Log_textEdit->clear();
    ui.Log_textEdit_2->clear();
}

/// <summary>
/// 接收子线程串口接收到的数据和发送子线程发送的数据，主线程根据具体情况判断是否需要显示
/// </summary>
/// <param name="rbuffer"></param>
/// <param name="comIndex"></param>
void DataSplit::handleResults(QByteArray rbuffer,int comIndex)
{
    bool isShow = false;
    QString bufferStr = "";
    //接收的数据
    if (comIndex == 1)
    {
        bufferStr = "接收数据：";
        isShow = ui.Rcb_showRData->checkState();

        if (isShow == true)
        {
            bufferStr = bufferStr + ByteArrayToHexString(rbuffer);
            ShowMessageTOEdit(bufferStr, 1);
        }
    }
        
    //发送的数据
    if (comIndex == 2)
    {
        bufferStr = "发送数据：";
        isShow = ui.Scb_showRData->checkState();
        if (isShow == true)
        {
            bufferStr = bufferStr + ByteArrayToHexString(rbuffer);
            ShowMessageTOEdit2(bufferStr, 1);
        }
    }

    //qDebug() << "主线程收到结果数据：" << rbuffer << "线程ID：" << QThread::currentThreadId();
}

/// <summary>
/// 将字节序列转换为对应的16进制字符串
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
/// 发送串口发送数据（测试使用）
/// </summary>
void DataSplit::SendData()
{
#if 0
    //获取界面上的数据并转换成utf8格式的字节流
    QByteArray data;
    for (int i = 0; i < 10; i++) data[i] = i;
    //m_serial_1.write(data);// 在主线程发送
    emit serialDataSend(data);// 在子线程发送
    qDebug() << "主线程发送信号，线程ID：" << QThread::currentThreadId();

#else
    //发送文件
    QString openFilePath = m_appPath + "/xpFileTest.ais";
    emit SendFileSignal(openFilePath);
    ShowMessageTOEdit("文件发送中，请稍等......", 1);


#endif // 1
}

