#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Processing.NDI.Lib.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString StringValue();

    int flag = 0;
    int i = 0;

    NDIlib_recv_create_v3_t recv_desc;
//    NDIlib_recv_instance_t pNDI_recv = NDIlib_recv_create_v3(&recv_desc);
    NDIlib_tally_t NDI_tally;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
