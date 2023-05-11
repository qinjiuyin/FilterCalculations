#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "chartsthread.h"
#include "serialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    /* 图表相关 */
    ChartsThread *mChartsThread;
    QThread *threadCharts;
    /* 串口相关 */
    void ScanSerialPort();
    int SerialState;
    SerialPort *serial;
    QString strSerialCache;
};
#endif // MAINWINDOW_H
