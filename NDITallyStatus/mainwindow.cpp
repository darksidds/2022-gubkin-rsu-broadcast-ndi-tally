#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "echo_class.h"


//void tally_echo(QString ndi_name, Ui::MainWindow& ui, int& flag, int& i, NDIlib_recv_instance_t& pNDI_recv, NDIlib_recv_create_v3_t& recv_desc);


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
    QThread* thread = new QThread();
    echo_class* tally_echo = new echo_class();
    tally_echo->init_echo_class(MainWindow::StringValue());

    connect(thread, &QThread::started, tally_echo, &echo_class::echoing);
    connect(tally_echo, &echo_class::end_echo_class, thread, &QThread::quit);

    thread->start();

}


void MainWindow::on_pushButton_2_clicked()
{
//    echo_class::flag = 1;
}


QString MainWindow::StringValue()
{
    return ui->lineEdit->text();
}

void MainWindow::textAppend(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}


