#ifndef ECHO_CLASS_H
#define ECHO_CLASS_H

#include "mainwindow.h"
#include <QThread>
#include <Processing.NDI.Lib.h>
#include <cstddef>


class echo_class : public QThread
{
    Q_OBJECT

public:
    echo_class(QObject *parent = nullptr);

    int i;
    bool flag;

signals:
    void initialized();
    void metadataCaptured();
    void videoCaptured();
    void nothingCaptured();

public slots:
    void end_echo_class();
    void init_echo_class(QString name);
    void echoing();
    void setMW(MainWindow* mainwindow);

protected:
    void run() override;

private:
    NDIlib_recv_create_v3_t recv_desc;
    NDIlib_tally_t NDI_tally;
    NDIlib_recv_instance_t pNDI_recv;

    MainWindow *mw;

};

#endif // ECHO_CLASS_H
