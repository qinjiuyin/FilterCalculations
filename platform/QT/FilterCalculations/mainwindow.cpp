#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>  //ini file header
#include <QFileInfo>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 串口启动 */
    SerialState = 0;
    ScanSerialPort();
    serial = new SerialPort;
    //启动图表线程
    mChartsThread = new ChartsThread;
    threadCharts = new QThread;
    connect(threadCharts, SIGNAL(started()), mChartsThread, SLOT(ThreadRunning()));
    mChartsThread->SettingMessage(ui->graphicsView);
    mChartsThread->moveToThread (threadCharts);
    threadCharts->start();
}

MainWindow::~MainWindow()
{
    //关闭图表线程
    mChartsThread->stop();
    threadCharts->quit();
    threadCharts->wait();
    mChartsThread->free();
    delete mChartsThread;
    delete threadCharts;
    /* 关闭串口 */
    serial->CloseSerial();
    serial->CloseSerialThread();
    serial->quit();
    serial->wait();
    delete serial;

    delete ui;
}

void MainWindow::ScanSerialPort(void)
{
    //如果串口是打开的状态 不再扫描串口
    if (SerialState)
        return;
    int i = 0, m = 0, n = 0;
    //获取记录上次打开的串口
    QString filePath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(filePath, QSettings::IniFormat);
    QFile file(filePath);
    if (file.exists())
    {
        strSerialCache = settings.value("com").toString();
        m = 1;
    }
    // 获取所有可用串口的信息
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    // 将串口名称添加到下拉列表中
    ui->comboBox->clear();
    for (const QSerialPortInfo &port : ports) {
        ui->comboBox->addItem(port.portName());
        if (m == 1)
        {
            if (strSerialCache == port.portName())
            {
                i = n;
            }
        }
        n++;
    }
    ui->comboBox->setCurrentIndex(i);
}

void MainWindow::on_pushButton_clicked()
{
    ScanSerialPort();
}

void MainWindow::on_pushButton_2_clicked()
{
    //打开或关闭串口
    if (SerialState == 0)
    {
        //获取当前选择串口
        QString strSerial = ui->comboBox->currentText();
        //真正打开串口
        if (!serial->OpenSerial(&strSerial))
        {
            serial->start();
            serial->ReStart();
            //记录打开的串口
            if (strSerial != strSerialCache)
            {
                QString filePath = QCoreApplication::applicationDirPath() + "/config.ini";
                QSettings settings(filePath, QSettings::IniFormat);
                settings.setIniCodec(QTextCodec::codecForName("UTF-8"));//修改编码
                settings.setValue("com", strSerial);
            }
            //标志位
            SerialState = 1;
            QString str = QString::fromLocal8Bit("关闭串口");
            ui->pushButton_2->setText(str);
        }
    }
    else
    {
        serial->CloseSerial();
        serial->CloseSerialThread();
        serial->quit();
       // serial->wait();
        SerialState = 0;
        QString str = QString::fromLocal8Bit("打开串口");
        ui->pushButton_2->setText(str);
    }
}
