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
    EchoClass* echo = new EchoClass();
    echo->mw = w;
    echo->moveToThread(thread);
    w->echo = echo;

    QObject::connect(echo, &EchoClass::ismetadataCaptured, w, &MainWindow::metadataCaptured);
    QObject::connect(echo, &EchoClass::isvideoCaptured, w, &MainWindow::videoCaptured);
    QObject::connect(echo, &EchoClass::isnothingCaptured, w, &MainWindow::nothingCaptured);

    QObject::connect(thread, &QThread::started, echo, &EchoClass::initEchoClass);
    QObject::connect(echo, &EchoClass::isInitialized, w, &MainWindow::Initialized);
    QObject::connect(echo, &EchoClass::isInitialized, echo, &EchoClass::echoing);

    QObject::connect(thread, &QThread::finished, echo, &EchoClass::endEchoClass);
    //QObject::connect(thread, &QThread::finished, echo, &EchoClass::deleteLater);
    //QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    return a.exec();
}
