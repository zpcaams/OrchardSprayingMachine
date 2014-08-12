#ifndef MB_GUI_H
#define MB_GUI_H

#include <QWidget>
#include <QThread>
#include <QTimer>
#include "modbus.h"
#include "com_thread.h"

namespace Ui {
class mb_gui;
}

class mb_gui : public QWidget
{
    Q_OBJECT

public:
    explicit mb_gui(QWidget *parent = 0);
    ~mb_gui();

private slots:
    void on_commButton_clicked();
    void on_connButton_clicked();

    void update_gui();

private:
    Ui::mb_gui *ui;
    modbus_t * m_modbus;
    com_thread* _com_thread;
};

#endif // MB_GUI_H
