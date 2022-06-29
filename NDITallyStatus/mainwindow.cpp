#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "tally_echo.cpp"
#include "qeventloop.h"
#include "qwindowdefs.h"
#include <QTimer>

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
    recv_desc.bandwidth = NDIlib_recv_bandwidth_lowest;
    QString ndi_name = StringValue();
    const QByteArray str = ndi_name.toUtf8();
    recv_desc.source_to_connect_to.p_ndi_name = str.constData();

    NDIlib_recv_instance_t pNDI_recv = NDIlib_recv_create_v3(&recv_desc);

    NDI_tally.on_program = "on_program";
    NDIlib_recv_set_tally(pNDI_recv, &NDI_tally);

    i = 0;
    flag = 0;

    QEventLoop loop;
    QTimer timer;

    if (!flag)
    {
        NDIlib_metadata_frame_t metadata;
        NDIlib_video_frame_v2_t frame;

        tally_echo(ui, &flag, &i, &pNDI_recv, &recv_desc, &metadata, &frame);
    }
    timer.start(500);
    loop.exec();
}


void MainWindow::on_pushButton_2_clicked()
{
    flag = 1;
}


QString MainWindow::StringValue()
{
    return ui->lineEdit->text();
}


