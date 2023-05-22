#pragma once

#include <QWidget>
#include "ui_wave.h"
#include "qvector.h"
#include <QtCharts/qchartview.h>
#include <QtCharts/qsplineseries.h>
#include <qvalueaxis.h>

class wave : public QWidget
{
	Q_OBJECT

public:
	wave(QWidget *parent = nullptr);
	~wave();

public:
	QVector<quint32> ch1, ch2, ch3;

	QChart* ch1_chart;
	QSplineSeries* ch1_lineSeries_A;
	QSplineSeries* ch1_lineSeries_B;
	QValueAxis* ch1_axisX;
	QValueAxis* ch1_axisY;

	QChart* ch2_chart;
	QSplineSeries* ch2_lineSeries_A;
	QSplineSeries* ch2_lineSeries_B;
	QValueAxis* ch2_axisX;
	QValueAxis* ch2_axisY;

	QChart* ch3_chart;
	QSplineSeries* ch3_lineSeries_A;
	QSplineSeries* ch3_lineSeries_B;
	QValueAxis* ch3_axisX;
	QValueAxis* ch3_axisY;

public slots:
	void on_pbStart_clicked();

private:
	//Ui::waveClass ui;
	Ui::Form ui;
};
