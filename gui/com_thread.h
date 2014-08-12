#ifndef COM_THREAD_H
#define COM_THREAD_H

#include <QThread>
#include <QTimer>
#include <QMessageBox>
#include "modbus.h"

class com_thread : public QThread
{
    Q_OBJECT
public:
    explicit com_thread(QThread *parent = 0);

    void run();

signals:
    void update_gui(void);

public slots:
    void tmieout_task(void);

private:
};

#endif // COM_THREAD_H
