#include "MCES.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MCES w;
    w.show();
    return a.exec();
}
