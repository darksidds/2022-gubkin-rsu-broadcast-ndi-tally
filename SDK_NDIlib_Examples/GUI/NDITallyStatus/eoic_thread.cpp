#include "eoic_thread.h"
#include "echo_class.h"

eoic_thread::eoic_thread(QObject *parent)
    : QThread{parent}
{

}


void eoic_thread::WorkPlease()
{
    echo_class::echoing()
}
