#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "echo_class.h"

#include <QTimer>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    thread->start();
}


void MainWindow::on_pushButton_2_clicked()
{
    echo->flag = 1;
    thread->quit();
}


QString MainWindow::StringValue()
{
    return ui->lineEdit->text();
}


void MainWindow::textAppend(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}


void MainWindow::metadataCaptured()
{
    textAppend("metadata captured");
}


void MainWindow::videoCaptured()
{
    textAppend("video captured");
}


void MainWindow::nothingCaptured()
{
    textAppend("nothing parsed");
}


void MainWindow::Initialized()
{
    textAppend("initialized");
}


