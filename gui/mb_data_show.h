#ifndef MB_DATA_SHOW_H
#define MB_DATA_SHOW_H

#include <QWidget>
#include "./qcustomplot/qcustomplot.h"

namespace Ui {
class mb_data_show;
}

class mb_data_show : public QWidget
{
    Q_OBJECT

public:
    explicit mb_data_show(QWidget *parent = 0);
    ~mb_data_show();

    void setupRealtimeDataDemo();
    void realtimeDataSlot();

private:
    Ui::mb_data_show *ui;
};

#endif // MB_DATA_SHOW_H
