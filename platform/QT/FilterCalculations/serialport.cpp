#include "serialport.h"
#include <QObject>

SerialPort::SerialPort(QObject *parent)
    : QThread(parent)
{
     m_bStopped = false;
}

void SerialPort::run(void)
{
    ThreadID =  (int)QThread::currentThreadId();
    // 从串口读取数据
    while(!m_bStopped)
    {
       if (serialPort.bytesAvailable() > 0) {
           readSerialData();
       }
       QThread::msleep(600);
    }
}


int SerialPort::OpenSerial(QString* pStr)
{
    if(serialPort.isOpen())
     {
        serialPort.close();
     }
    serialPort.clear(); // 清除系统内已有的接收内容
    // 设置串口参数
    serialPort.setPortName(*pStr);      // 设置串口名称
    serialPort.setBaudRate(115200);      // 设置波特率
    serialPort.setDataBits(QSerialPort::Data8);      // 设置数据位
    serialPort.setParity(QSerialPort::NoParity);     // 设置校验位
    serialPort.setStopBits(QSerialPort::OneStop);    // 设置停止位
    serialPort.setFlowControl(QSerialPort::NoFlowControl);  // 设置流控制
    // 打开串口
    if(serialPort.open(QIODevice::ReadWrite))
    {
       // QObject::connect(&serialPort, &QSerialPort::readyRead , this , &SerialPort::readSerialData);
        return 0;
    }
    else
    {
		qDebug("Failed to open serial port\n");
        return 1;
    }
}

void SerialPort::CloseSerial(void)
{
    serialPort.clear();
    serialPort.close();
    dataList.clear();
}

void SerialPort::readSerialData(void)
{
    QByteArray array = serialPort.readAll(); // 读取串口数据
    char *buf;//只是一个指针
    int len;//buf的长度
    buf = array.data();
    len = array.size();
	if (((buf[0] & 0xff) == 0xff) && ((buf[3] & 0xff) == 0xff) && len == 4)
    {
        quint16 data = (buf[1]&0xff)<<8|(buf[2]&0xff);
       // qDebug("readSerialData : %d\n", data);
        dataList.append(data);
    }
	serialPort.clear();
}

int SerialPort::GetValueReal(quint16* data)
{
    if (!dataList.isEmpty()) {
        *data = dataList.first();
        dataList.removeFirst();
        return 0;
    }
    return 1;
}

int SerialPort::ValueCacheNumber(void)
{
	return dataList.size();
}

void SerialPort::CloseSerialThread(void)
{
    qDebug("Serial Stop Thread :  %d\n",ThreadID);
    QMutexLocker locker(&m_mutex);
    m_bStopped = true;
}

void SerialPort::ReStart(void)
{
    m_bStopped = false;
}