#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>

#include <QSerialPort>
#include <QSerialPortInfo>

#include "chartsthread.h"

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    int OpenSerial(QString* pStr, ChartsThread *mChartsThread);
    void CloseSerial(void);
signals:
    void SerialsendData(char* data, int length);
public slots:
    int SerialWrites(char* pszData, int nCount);
private slots:
    void readSerialData(void);
private:
    quint16 calculateModbusCRC16(char *data, int length);
    QSerialPort serialPort;
    ChartsThread *mChartsThread;
};

#endif // SERIALPORT_H
