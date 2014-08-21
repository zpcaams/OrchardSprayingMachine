#ifndef MB_GUI_H
#define MB_GUI_H

#include <QWidget>
#include <QThread>
#include <QTimer>
#include <QVector>
#include <QFile>
#include <QFileDialog>
#include "modbus.h"
#include "mb_data_show.h"

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

    void mb_com_slot();

    void on_saveButton_clicked();

private:
    Ui::mb_gui *ui;
    modbus_t * m_modbus;
    QTimer mb_com_timer;
    QCustomPlot *customPlot;
    mb_data_show sensor;
    QVector<int> sensor_left_low, sensor_left_mid, sensor_left_high;
};

#endif // MB_GUI_H
