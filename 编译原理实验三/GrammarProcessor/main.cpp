#include "widget.h"
#include <QDebug>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.resize(1600,900);
    w.show();
    return a.exec();
}
