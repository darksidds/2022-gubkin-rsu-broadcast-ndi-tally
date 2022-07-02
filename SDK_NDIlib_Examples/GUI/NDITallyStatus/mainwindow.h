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
    QThread *thread;

    void metadataCaptured();
    void videoCaptured();
    void nothingCaptured();
    void Initialized();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void textAppend(QString text);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
