#ifndef MB_GUI_H
#define MB_GUI_H

#include <QWidget>
#include <QThread>
#include <QTimer>
#include "modbus.h"
#include "./qcustomplot/qcustomplot.h"

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

    void mb_com_slot();

private:
    Ui::mb_gui *ui;
    modbus_t * m_modbus;
    QTimer mb_com_timer;
    QCustomPlot *customPlot;
};

#endif // MB_GUI_H
