#include "chartsthread.h"


ChartsThread::ChartsThread(QObject *parent) : QObject(parent)
{
    m_bStopped = false;
}



void ChartsThread :: ThreadRunning()
{
    ThreadID =  (int)QThread::currentThreadId();
    while (!m_bStopped) {
      int count = m_lineSeries->points().size();                  // 获得当前数据序列点数
      if(count > AXIS_MAX_X)
      {
          m_chart->axisX()->setMax(count);                        // 更新X轴范围
      }
      m_lineSeries->append(QPointF(count, rand() % AXIS_MAX_Y));  // 更新显示（随机生成10以内的一个数）
      QThread::msleep(800);
    }
}

void ChartsThread ::SettingMessage(QChartView *view)
{
       // 创建横纵坐标轴并设置显示范围
       //
       m_axisX = new QValueAxis();
       m_axisY = new QValueAxis();
       m_axisX->setTitleText("X-label");
       m_axisY->setTitleText("Y-label");
       m_axisX->setMin(0);
       m_axisY->setMax(0);
       m_axisX->setMax(AXIS_MAX_X);
       m_axisY->setMax(AXIS_MAX_Y);

       m_lineSeries = new QLineSeries();                             // 创建曲线绘制对象
       m_lineSeries->setPointsVisible(true);                         // 设置数据点可见
       m_lineSeries->setName("原始数据");                            // 图例名称

       m_chart = new QChart();                                        // 创建图表对象
       m_chart->addAxis(m_axisX, Qt::AlignLeft);                      // 将X轴添加到图表上
       m_chart->addAxis(m_axisY, Qt::AlignBottom);                    // 将Y轴添加到图表上
       m_chart->addSeries(m_lineSeries);                              // 将曲线对象添加到图表上
       m_chart->setAnimationOptions(QChart::SeriesAnimations);        // 动画：能使曲线绘制显示的更平滑，过渡效果更好看

       m_lineSeries->attachAxis(m_axisX);                             // 曲线对象关联上X轴，此步骤必须在m_chart->addSeries之后
       m_lineSeries->attachAxis(m_axisY);                             // 曲线对象关联上Y轴，此步骤必须在m_chart->addSeries之后

       view->setChart(m_chart);
       view->setRenderHint(QPainter::Antialiasing);                     // 设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑
}

void ChartsThread :: free()
{
    delete m_axisX;
    delete m_axisY;
    delete m_lineSeries;
    delete m_chart;
}

void ChartsThread :: stop()
{
    qDebug("Charts Stop Thread :  %d\n",ThreadID);
    QMutexLocker locker(&m_mutex);
    m_bStopped = true;
}
