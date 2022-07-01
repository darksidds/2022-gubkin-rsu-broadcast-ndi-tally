#include "mainwindow.h"
#include "echo_class.h"
//#include "NDIlib_Tally_Echo.cpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    echo_class echo;
    MainWindow w;
    w.echo = &echo;
    echo.setMW(&w);

    w.show();

    return a.exec();
}
