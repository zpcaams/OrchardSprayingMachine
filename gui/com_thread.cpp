#include "com_thread.h"

com_thread::com_thread(QThread *parent) :
    QThread(parent)
{
}

void com_thread::run()
{
    QTimer * timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(tmieout_task()), Qt::DirectConnection);
    timer->setInterval(500);
    timer->start();   // puts one event in the threads event queue
    exec();
    timer->stop();
}

void com_thread::tmieout_task(void)
{
    emit update_gui();
}
