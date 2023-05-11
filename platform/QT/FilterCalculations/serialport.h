#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSerialPort>

#include <QObject>
#include <QList>
#include <QThread>
#include <QMutex>

namespace Serial {
class SerialPort;
}

class SerialPort : public QThread
{
     Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    void run();
    int OpenSerial(QString* pStr);
    void CloseSerial(void);
    int GetValueReal(quint16* dat);
	int ValueCacheNumber();
    void CloseSerialThread();
    void ReStart(void);
private slots:
    void readSerialData(void);

private:
    /* 线程相关 */
    int ThreadID;
    bool m_bStopped;
    QMutex m_mutex;
    /* 内容 */
    QSerialPort serialPort;
    QList<quint16> dataList;
};

#endif // SERIALPORT_H
