#include "Qt5Demo.h"
#include "wave.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Qt5Demo w;
    //w.show();

    wave w;
    w.show();

    return a.exec();
}
