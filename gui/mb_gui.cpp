
#include <QtWidgets/QMessageBox>
#include "mb_gui.h"
#include "ui_mb_gui.h"

mb_gui::mb_gui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mb_gui)
{
    ui->setupUi(this);
}

mb_gui::~mb_gui()
{
    delete ui;
}

void mb_gui::on_connButton_clicked()
{
    if (ui->connButton->text()=="close com"){
        modbus_close(m_modbus);
        ui->connButton->setText("open com");
    }else{
        m_modbus = modbus_new_rtu( "COM3",
                19200,
                'N',
                8,
                1);
        if(m_modbus==NULL){
            QMessageBox::critical( this, tr( "modbus_new_rtu failed" ),
                                   tr( "Unable to create the libmodbus context!" ) );
        }

        if( modbus_connect( m_modbus ) == -1 ){
            QMessageBox::critical( this, tr( "Connection failed" ),
                tr( "Could not connect serial port!" ) );
    //    }else{
    //        QMessageBox::critical( this, tr( "No serial port found" ),
    //                tr( "Could not find any serial port "
    //                        "on this computer!" ) );
        }
        ui->connButton->setText("close com");

        sensor.show();
        sensor.setupRealtimeDataDemo();
    }
}

void mb_gui::on_commButton_clicked()
{
    if(ui->connButton->text()=="close com"){
        //com opened
        if(ui->commButton->text()=="stop com"){
                ui->commButton->setText("start com");
                mb_com_timer.stop();
                ui->connButton->setEnabled(true);

        }else{
            ui->commButton->setText("stop com");

            // setup a timer that repeatedly calls mb_com_slot:
            connect(&mb_com_timer, SIGNAL(timeout()), this, SLOT(mb_com_slot()));
            mb_com_timer.start(100); // Interval 0 means to refresh as fast as possible

            ui->connButton->setDisabled(true);
        }
    }else{
        //com not opened
        QMessageBox::critical( this, tr( "comm failed" ),
            tr( "Open COM Port before Comm!" ) );
        return;
    }
}

void mb_gui::mb_com_slot(void)
{
    int ret = -1;
    const int slave_addr = 1;
    uint16_t input_buf[3];
    memset( input_buf, 0, 6 );

    // 执行任务
    modbus_set_slave( m_modbus, slave_addr );
    ret = modbus_read_input_registers( m_modbus, 0, 3, input_buf );

    QString str;
    str.setNum(input_buf[0]);
    ui->label_1->setText(str);
    str.setNum(input_buf[1]);
    ui->label_2->setText(str);
    str.setNum(input_buf[2]);
    ui->label_3->setText(str);

    sensor.realtimeDataSlot();
}
