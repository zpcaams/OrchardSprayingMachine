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
    void add_new_data(int left_low, int left_mid, int left_high);
    void clear_data(void);

private slots:
  void mousePress();
  void mouseWheel();

private:
    Ui::mb_data_show *ui;
};

#endif // MB_DATA_SHOW_H
