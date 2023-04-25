#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt5Demo.h"

class Qt5Demo : public QMainWindow
{
    Q_OBJECT

public:
    Qt5Demo(QWidget *parent = nullptr);
    ~Qt5Demo();

private:
    Ui::Qt5DemoClass ui;
};
