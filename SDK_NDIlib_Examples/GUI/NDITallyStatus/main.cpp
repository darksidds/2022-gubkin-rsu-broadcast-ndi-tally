#include "mainwindow.h"
#include "echo_class.h"

#include <QApplication>
#include <QThread>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow();
    w->show();

    QThread* thread = new QThread();
    w->thread = thread;
    echo_class* echo = new echo_class();
    echo->mw = w;
    echo->moveToThread(thread);

    QObject::connect(echo, &echo_class::ismetadataCaptured, w, &MainWindow::metadataCaptured);
    QObject::connect(echo, &echo_class::isvideoCaptured, w, &MainWindow::videoCaptured);
    QObject::connect(echo, &echo_class::isnothingCaptured, w, &MainWindow::nothingCaptured);

    QObject::connect(thread, &QThread::started, echo, &echo_class::init_echo_class);
    QObject::connect(echo, &echo_class::isInitialized, w, &MainWindow::Initialized);
    QObject::connect(echo, &echo_class::isInitialized, echo, &echo_class::echoing);



    return a.exec();
}
