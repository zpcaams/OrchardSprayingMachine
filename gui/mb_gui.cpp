
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
        m_modbus = modbus_new_rtu( "COM5",
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
    }
}

void mb_gui::on_commButton_clicked()
{
    if(ui->connButton->text()=="close com"){
        //com opened
        if(ui->commButton->text()=="stop com"){
                ui->commButton->setText("start com");

                //wait until thread done and close it
                _com_thread->quit();
                _com_thread->wait();
                delete _com_thread;

                ui->connButton->setEnabled(true);

        }else{
            ui->commButton->setText("stop com");

            _com_thread = new com_thread;
             QObject::connect(_com_thread, SIGNAL(update_gui()),
                              this, SLOT(update_gui()), Qt::QueuedConnection);
            _com_thread->start();

            ui->connButton->setDisabled(true);
        }
    }else{
        //com not opened
        QMessageBox::critical( this, tr( "comm failed" ),
            tr( "Open COM Port before Comm!" ) );
        return;
    }
}

void mb_gui::update_gui(void)
{
    int ret = -1;
    const int slave_addr = 1;
    uint8_t input_buf[6];
    uint8_t hold_buf[2];
    int valve_buf;
    int flow_buf;
    uint16_t* input_buf16 = (uint16_t *) input_buf;
    uint16_t* hold_buf16 = (uint16_t *) hold_buf;
    memset( input_buf, 0, 6 );

    // 执行任务
    if( m_modbus == NULL )
    {
        QMessageBox::critical( this, tr( "Connection failed" ),
                                    tr( "Serial port not defined!" ) );
        return;
    }

    *hold_buf16 = ui->hSliderValve->value();
    modbus_set_slave( m_modbus, slave_addr );
    ret = modbus_read_input_registers( m_modbus, 0, 3, input_buf16 );
    ret = modbus_write_register( m_modbus, 0, *hold_buf16 );
    valve_buf = (input_buf[1]<<8) + (input_buf[0]);
    flow_buf = (input_buf[3]<<8) + (input_buf[2]);

    QString str;
    str.setNum(*hold_buf16);
    ui->labelValve->setText(str);

    ui->pBarValve->setValue(valve_buf);

    ui->pBarFlow->setValue(flow_buf);
}
