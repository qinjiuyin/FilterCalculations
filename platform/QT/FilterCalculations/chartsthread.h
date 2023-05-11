#ifndef CHARTSTHREAD_H
#define CHARTSTHREAD_H

#include <QMainWindow>
#include <QObject>
#include <QThread>
#include <QMutex>

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
    void Starts();
    void stop();
    void free();
    /* 图表对象 */
    QChart* m_chart;
public slots:
    void ThreadRunning();
private:

    /* 线程相关 */
    int ThreadID;
    bool m_bStopped;
    QMutex m_mutex;
    /* 横纵坐标轴对象 */
    QValueAxis *m_axisX, *m_axisY;
    /* 横纵坐标最大显示范围 */
    const int AXIS_MAX_X = 10, AXIS_MAX_Y = 10;
    /* 曲线图对象 */
    QLineSeries* m_lineSeries;
};

#endif // CHARTSTHREAD_H
