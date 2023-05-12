#ifndef CHARTSTHREAD_H
#define CHARTSTHREAD_H

#include <QMainWindow>
#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QQueue>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class ChartsThread : public QObject
{
    Q_OBJECT
public:
    explicit ChartsThread(QObject *parent = nullptr);
    void SettingMessage(QChartView *view);
    void SettingData(const quint16 data);
    void stop();
    void free();
    /* 数据传递 */
    bool dataReceived = false;
    QWaitCondition condition;
    QMutex mutex;
    /* 图表对象 */
    QChart* m_chart;
signals:
    void SerialsendData(char* data, int length);
public slots:
    void ThreadRunning();
private:
    /* 线程相关 */
    int ThreadID;
    bool m_bStopped;
    QMutex m_mutex;
    /* 线程传递数据 */
    quint16 data;
    /* 横纵坐标轴对象 */
    QValueAxis *m_axisX, *m_axisY;
    /* 横纵坐标最大显示范围 */
    const int AXIS_MAX_X = 10, AXIS_MAX_Y = 20;
    /* 曲线图对象 */
    QLineSeries* m_lineSeries;
};

#endif // CHARTSTHREAD_H
