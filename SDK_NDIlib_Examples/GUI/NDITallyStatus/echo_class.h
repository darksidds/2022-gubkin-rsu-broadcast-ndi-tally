#ifndef ECHO_CLASS_H
#define ECHO_CLASS_H

#include <Processing.NDI.Lib.h>
#include <QObject>

class MainWindow;


class EchoClass : public QObject
{
    Q_OBJECT

public:
    EchoClass(QObject *parent = nullptr);

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
    void initEchoClass();
    void endEchoClass();

private:
    NDIlib_recv_create_v3_t recv_desc;
    NDIlib_tally_t NDI_tally;
    NDIlib_recv_instance_t pNDI_recv;

};

#endif // ECHO_CLASS_H
