#include "Qt5Demo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    //Ìí¼Ó¸ö×¢ÊÍ 
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    Qt5Demo w;
    w.show();

    //wave w;
    //w.show();

    return a.exec();
}
