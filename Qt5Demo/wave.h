#pragma once

#include <QWidget>
#include "ui_wave.h"

class wave : public QWidget
{
	Q_OBJECT

public:
	wave(QWidget *parent = nullptr);
	~wave();

public slots:
	void on_pb_Start_clicked();

private:
	//Ui::waveClass ui;
	Ui::Form ui;
};
