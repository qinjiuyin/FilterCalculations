#include "serialport.h"
#include <QMessageBox>


SerialPort::SerialPort(QObject* parent) : QObject(parent)
{

}

int SerialPort::OpenSerial(QString* pStr, ChartsThread* mChartsThread)
{
	this->mChartsThread = mChartsThread;
	if (serialPort.isOpen())
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
	if (serialPort.open(QIODevice::ReadWrite))
	{
		QObject::connect(&serialPort, &QSerialPort::readyRead, this, &SerialPort::readSerialData);
		QObject::connect(this, &SerialPort::SerialsendData, this, &SerialPort::SerialWrites, Qt::QueuedConnection);
		return 0;
	}
	else
	{
		QMessageBox::warning(NULL, "Tips", "Serial Open failed");
		return 1;
	}
}

void SerialPort::CloseSerial(void)
{
	serialPort.clear();
	serialPort.close();
}

//这里数据接收包位 02 hh ll 03 crch crcl
//               起始 数据高 数据低 结束 crc高 crc低
void SerialPort::readSerialData(void)
{
	QByteArray array = serialPort.readAll(); // 读取串口数据
	char* buf;//只是一个指针
	int len;//buf的长度
	buf = array.data();
	len = array.size();
	quint16 crcRevice = buf[4] & 0xff;
	quint16 crcCal = this->calculateModbusCRC16(buf, 4);
	crcRevice = (crcRevice << 8)&0xff00;
	crcRevice += buf[5] & 0xff;
	if (len == 6 && buf[0] == 0x02 && buf[3] == 0x03 && crcRevice == crcCal)
	{
		quint16 data = (buf[1] & 0xff) << 8 | (buf[2] & 0xff);
		/* 将数据传递给处理线程 */
		//qDebug("readSerialData : %d\n", data);
		this->mChartsThread->SettingData(data);
		this->mChartsThread->mutex.lock();
		this->mChartsThread->dataReceived = true;
		this->mChartsThread->condition.wakeOne();
		this->mChartsThread->mutex.unlock();
	}
	serialPort.clear();
}

quint16 SerialPort::calculateModbusCRC16(char* data, int length)
{
	quint16 crc = 0xFFFF;

	for (int i = 0; i < length; ++i)
	{
		crc ^= (quint16)data[i];

		for (int j = 0; j < 8; ++j)
		{
			if (crc & 0x0001)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}

	// 转换字节顺序
	crc = (crc >> 8) | (crc << 8);

	return crc;
}

int SerialPort:: SerialWrites(char *pszData, int nCount)
{
	qint64 bytesWritten = serialPort.write(pszData, nCount);
	if (bytesWritten == -1) {
		qDebug("Failed to write to serial port\n");
	}
    return 0;
}

