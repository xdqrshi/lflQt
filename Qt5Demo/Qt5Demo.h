#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt5Demo.h"
#include <qudpsocket.h>
#include <QDebug>
#include "qtimer.h"
#include "qvector.h"
#include <QtCharts/qchartview.h>
#include <QtCharts/qsplineseries.h>
#include <qvalueaxis.h>

class Qt5Demo : public QMainWindow
{
    Q_OBJECT

public:
    Qt5Demo(QWidget *parent = nullptr);
    ~Qt5Demo();

public:
    int ParseMifFile(QString fileName, QVector<quint32>& vec);
    
public:
    QUdpSocket* udpSocket;
    QHostAddress targetAddress;
    quint16 targetPort;
    QByteArray sendData;
    QTimer *timer;
    quint32 cnt;
    QString fileName;
    QString filePath;
    QVector<quint32> vMifValue;

public slots:
    void on_pbSendConfig_clicked();
    void on_pbWavePreview_clicked();
    void on_pbPowerOn_clicked();
    void on_pbStartWork_clicked();
    void on_pbMifOpen_clicked();
    void on_pbOpenDevice_clicked();
    void on_Timeout();
    void on_cbCh2_clicked(bool checked);
public:
    QChart* m_chart;
    QSplineSeries* m_lineSeries_A;
    QSplineSeries* m_lineSeries_B;
    QValueAxis* m_axisX;
    QValueAxis* m_axisY;


public:
    quint32 m_Fclk;
    quint32 m_DDSTime;
    quint32 m_t1, m_t2, m_t3, m_t4, m_t5, m_t6, m_t7, m_t8;
    quint32 m_PhaseDelay;
    quint32 m_CycleTimes;
    quint32 m_CycleDelay;
    quint32 m_PowerOut;
    quint32 m_Fctrl, m_Pctrl;
    quint32 m_FramHead, m_FramTail;
    quint32 m_ModeVal;

public:
    void ParaInit();


private:
    Ui::Qt5DemoClass ui;
};
