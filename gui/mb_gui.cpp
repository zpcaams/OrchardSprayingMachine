
#include <QtWidgets/QMessageBox>
#include "mb_gui.h"
#include "ui_mb_gui.h"

mb_gui::mb_gui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mb_gui)
{
    ui->setupUi(this);

    /*
     *Setup modbus pointer
     */
    m_modbus = modbus_new_rtu( "COM3",
            19200,
            'N',
            8,
            1);
    if(m_modbus==NULL){
        QMessageBox::critical( this, tr( "modbus_new_rtu failed" ),
                               tr( "Unable to create the libmodbus context!" ) );
        return;
    }

    /*
     *Setup sensor display window
     */
    sensor.show();
    sensor.setupRealtimeDataDemo();
}

mb_gui::~mb_gui()
{
    delete ui;
}

void mb_gui::on_commButton_clicked()
{
    //com opened
    if(ui->commButton->text()=="stop"){
        ui->commButton->setText("start");
        mb_com_timer.stop();
        modbus_close(m_modbus);

    }else if(ui->commButton->text()=="start"){
        ui->commButton->setText("stop");

        if( modbus_connect( m_modbus ) == -1 ){
            QMessageBox::critical( this, tr( "Connection failed" ),
                tr( "Could not connect serial port!" ) );
            return;
        }

        // setup a timer that repeatedly calls mb_com_slot:
        connect(&mb_com_timer, SIGNAL(timeout()), this, SLOT(mb_com_slot()));
        mb_com_timer.start(95); // Interval 0 means to refresh as fast as possible

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

    sensor_left_low.append(input_buf[0]);
    sensor_left_mid.append(input_buf[1]);
    sensor_left_high.append(input_buf[2]);

    QString str;
    str.setNum(input_buf[0]);
    ui->label_1->setText(str);
    str.setNum(input_buf[1]);
    ui->label_2->setText(str);
    str.setNum(input_buf[2]);
    ui->label_3->setText(str);

    sensor.add_new_data(input_buf[0], input_buf[1], input_buf[2]);
}

void mb_gui::on_saveButton_clicked()
{
    /*
     *Reset the whole UI
     */
    ui->commButton->setText("start");
    mb_com_timer.stop();
    modbus_close(m_modbus);
    sensor.clear_data();

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
    }

    QFile file(fileNames[0]);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    int i;
    for(i=0;i<(sensor_left_low.size());i++){
        out << sensor_left_low[i] << "; " <<
               sensor_left_mid[i] << "; " <<
               sensor_left_high[i] << "; " << "\n";
    }

}
