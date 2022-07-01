#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "echo_class.h"

#include <QTimer>


MainWindow::MainWindow(QWidget *parent, echo_class *e_c)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    echo = e_c;

}

MainWindow::~MainWindow()
{
    delete ui;


}


void MainWindow::on_pushButton_clicked()
{

    echo->start();

}


void MainWindow::on_pushButton_2_clicked()
{

}


QString MainWindow::StringValue()
{
    return ui->lineEdit->text();
}

void MainWindow::textAppend(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}


