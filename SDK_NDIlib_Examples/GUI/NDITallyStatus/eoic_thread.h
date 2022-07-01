#ifndef EOIC_THREAD_H
#define EOIC_THREAD_H

#include <QThread>

class eoic_thread : public QThread
{
    Q_OBJECT

public:
    explicit eoic_thread(QObject *parent = nullptr);
    void WorkPlease();

protected:
    void run() override;

};

#endif // EOIC_THREAD_H
