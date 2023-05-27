#pragma once

#include <QWidget>
#include "ui_wave.h"
#include "qvector.h"
#include <QtCharts/qchartview.h>
#include <QtCharts/qsplineseries.h>
#include <qvalueaxis.h>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#include <qqueue.h>
#include <qtimer.h>

class wave : public QWidget
{
	Q_OBJECT

public:
	wave(QWidget *parent = nullptr);
	~wave();

public:
	QVector<quint32> ch1, ch2, ch3;

	QChart* ch1_chart;
	QSplineSeries* ch1_lineSeries;
	QValueAxis* ch1_axisX;
	QValueAxis* ch1_axisY;

	QChart* ch2_chart;
	QSplineSeries* ch2_lineSeries;
	QValueAxis* ch2_axisX;
	QValueAxis* ch2_axisY;

	QChart* ch3_chart;
	QSplineSeries* ch3_lineSeries;
	QValueAxis* ch3_axisX;
	QValueAxis* ch3_axisY;

	QSerialPort serial;
	QQueue<QByteArray> queue;
	QTimer timer;
	QFile file;
	QByteArray buf;
public slots:
	void on_pbStart_clicked();


public:
	void initDraw();
	void DrawLine();

private:
	//Ui::waveClass ui;
	Ui::Form ui;
};
