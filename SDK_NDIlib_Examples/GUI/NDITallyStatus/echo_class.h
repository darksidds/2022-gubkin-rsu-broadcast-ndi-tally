#ifndef ECHO_CLASS_H
#define ECHO_CLASS_H

#include "mainwindow.h"
#include <Processing.NDI.Lib.h>


class echo_class : public QObject
{
    Q_OBJECT

public:
    echo_class(QObject *parent = nullptr);

    int i;
    bool flag;
    MainWindow *mw;

signals:
    void isInitialized();
    void ismetadataCaptured();
    void isvideoCaptured();
    void isnothingCaptured();

public slots:
    void echoing();
    void init_echo_class();
    void end_echo_class();

private:
    NDIlib_recv_create_v3_t recv_desc;
    NDIlib_tally_t NDI_tally;
    NDIlib_recv_instance_t pNDI_recv;

};

#endif // ECHO_CLASS_H
