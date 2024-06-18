#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  event_initialize();

  marlin_host = new Marlin_Host(QThread::HighPriority, this);
  marlin_host_initial_event();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  QMessageBox::StandardButton resBtn = QMessageBox::question(
                                        this, this->windowTitle(),
                                        tr("Are you sure?\n"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
  if (resBtn != QMessageBox::Yes) {
    event->ignore();
    return;
  }

  if (marlin_host->MH_IsConnected()) {
    marlin_host->MH_Disconnect();
  }

  event->accept();
}

void MainWindow::event_initialize() {
  connect(ui->btn_serial_connect, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_serial_connect);
  connect(ui->btn_serial_send, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_serial_send);
  connect(ui->line_edit_serial_command, &QLineEdit::returnPressed,
          this, &MainWindow::event_return_pressed_line_edit_serial_command);
//  connect(ui->line_edit_serial_command

  connect(ui->btn_home, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_home);
  connect(ui->btn_disable_motor, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_disable_motor);
  connect(ui->btn_enable_bump, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_enable_bump);
  connect(ui->btn_enable_valve, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_enable_valve);

  /// MANUAL JOG X
  connect(ui->btn_move_x_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_plus_1);
  connect(ui->btn_move_x_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_plus_10);
  connect(ui->btn_move_x_plus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_plus_100);

  connect(ui->btn_move_x_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_minus_1);
  connect(ui->btn_move_x_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_minus_10);
  connect(ui->btn_move_x_minus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_minus_100);

  connect(ui->btn_move_x, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x);

  /// MANUAL JOG Y
  connect(ui->btn_move_y_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_plus_1);
  connect(ui->btn_move_y_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_plus_10);
  connect(ui->btn_move_y_plus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_plus_100);

  connect(ui->btn_move_y_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_minus_1);
  connect(ui->btn_move_y_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_minus_10);
  connect(ui->btn_move_y_minus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_minus_100);

  connect(ui->btn_move_y, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y);

  /// MANUAL JOG Z
  connect(ui->btn_move_z_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_plus_1);
  connect(ui->btn_move_z_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_plus_10);
  connect(ui->btn_move_z_plus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_plus_100);

  connect(ui->btn_move_z_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_minus_1);
  connect(ui->btn_move_z_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_minus_10);
  connect(ui->btn_move_z_minus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_minus_100);

  connect(ui->btn_move_z, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z);

  /// MANUAL JOG R
  connect(ui->btn_move_r_plus_01, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_plus_01);
  connect(ui->btn_move_r_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_plus_1);
  connect(ui->btn_move_r_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_plus_10);

  connect(ui->btn_move_r_minus_01, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_minus_01);
  connect(ui->btn_move_r_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_minus_1);
  connect(ui->btn_move_r_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_minus_10);

  connect(ui->btn_move_r, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r);
}

void MainWindow::event_clicked_btn_serial_connect() {
  if(!marlin_host->MH_IsConnected()) {
    SerialSettingDialog *serial_setting = new SerialSettingDialog(this);
    connect(serial_setting, &SerialSettingDialog::UserAcceptSerialSetting,
            this, [this] (SerialSetting setting) {
              if (setting.name.isEmpty()) {
                return;
              }
              QString port_name = "Port Name: " + setting.name;
              QString baud_rate = "Baudrate: " + setting.string_baudrate;
              ui->lb_serial_connected_name->setText(port_name);
              ui->lb_serial_connected_baud->setText(baud_rate);
              marlin_host->MH_Connect(setting);
            });
    serial_setting->ShowDialog();
  } else {
    marlin_host->MH_Disconnect();
  }
}

void MainWindow::event_clicked_btn_serial_send() {
//  send_marlin_command();

  Marlin_Host::Position pick(80, 90, 9.5, 0);
  Marlin_Host::Position place(160, 180, 9.5, 50);
  Marlin_Host::Position pick1(300, 100, 9.5, 0);
  Marlin_Host::Position place1(300, 180, 9.5, 50);
  if (!test) {
    marlin_host->MH_PnP(pick, 30, place, 30, 20000, 10000);
    marlin_host->MH_PnP(pick1, 30, place1, 30, 20000, 10000);
  } else {
    marlin_host->MH_PnP(place, 30, pick, 30, 20000, 10000);
    marlin_host->MH_PnP(place1, 30, pick1, 30, 20000, 10000);
  }
  test = !test;
}

//void MainWindow::event_clicked_btn_serial_send_prior() {
//  if (marlin_host->MH_IsConnected()) {
//////    marlin_host->MH_WritePriorCommand(ui->line_edit_serial_command_prior->text());
////    marlin_host->MH_WriteCommand("G0 X100 Y100 Z30 F22000");
////    // start
////    marlin_host->MH_WriteCommand("G0 X100 Y100 Z10 F10000");
////    marlin_host->MH_DisableValve();
////    marlin_host->MH_WriteCommand("G0 X100 Y100 Z20 F10000");
////    marlin_host->MH_WriteCommand("G0 X300 Y100 Z30 F22000");
////    marlin_host->MH_WriteCommand("G0 X300 Y100 Z10 F10000");
////    marlin_host->MH_EnableValve();
////    marlin_host->MH_WriteCommand("G4 P100");
////    marlin_host->MH_WriteCommand("G0 X300 Y100 Z30 F10000");
////    marlin_host->MH_DisableValve();
////    Marlin_Host::Position pick(100.5, 27.8, 50.815, 297.32);
////    qDebug() << Marlin_Host::MH_StrMoveL(pick);
////    qDebug() << Marlin_Host::MH_StrMoveL(pick, 50.7);
////    qDebug() << Marlin_Host::MH_StrMoveL(pick, 0, true);
////    qDebug() << Marlin_Host::MH_StrMoveL(pick, 50.7, true);

//    Marlin_Host::Position pick(100, 100, 1, 0);
//    Marlin_Host::Position place(160, 220, 1, 50);
//    Marlin_Host::Position pick1(130, 100, 1, 0);
//    Marlin_Host::Position place1(100, 220, 1, 50);
//    if (!test) {
//      marlin_host->MH_PnP(pick, 30, place, 30, 20000, 10000);
//      marlin_host->MH_PnP(pick1, 30, place1, 30, 20000, 10000);
//    } else {
//      marlin_host->MH_PnP(place, 30, pick, 30, 20000, 10000);
//      marlin_host->MH_PnP(place1, 30, pick1, 30, 20000, 10000);
//    }
//    test = !test;
//  }
//}

//void MainWindow::event_clicked_btn_serial_test() {
//  if (marlin_host->MH_IsConnected()) {
//    for(int i=0;i<50;i++) {
//      marlin_host->MH_WriteCommand("G0 X100 Y100 F20000");
//      marlin_host->MH_WriteCommand("G1 E10 F10000");
//      marlin_host->MH_WriteCommand("G0 Z40 F15000");
//      marlin_host->MH_WriteCommand("G0 Z70 F15000");
//      marlin_host->MH_WriteCommand("G0 X150 Y200 F20000");
//      marlin_host->MH_WriteCommand("G1 E0 F10000");
//      marlin_host->MH_WriteCommand("G0 Z40 F15000");
//      marlin_host->MH_WriteCommand("G0 Z70 F15000");
//    }
//  }
//if (marlin_host->MH_IsConnected()) {
//  for(int i=0;i<30;i++) {
//    marlin_host->MH_WriteCommand("G1 E10 F10000");
//    marlin_host->MH_WriteCommand("G0 Z40 F10000");
//    marlin_host->MH_WriteCommand("G0 Z70 F10000");
//    marlin_host->MH_WriteCommand("G1 E0 F10000");
//  }
//}
//}

void MainWindow::event_return_pressed_line_edit_serial_command() {
  send_marlin_command();
}

void MainWindow::event_clicked_btn_home() {
  marlin_host->MH_Home();
}

void MainWindow::event_clicked_btn_disable_motor() {
  marlin_host->MH_DisableStepper();
}

void MainWindow::event_clicked_btn_enable_bump() {
  if (!marlin_host->MH_IsBumpEnable()) {
    marlin_host->MH_EnableBump();
    ui->btn_enable_bump->setText("Bump (On)");
  } else {
    marlin_host->MH_DisableBump();
    ui->btn_enable_bump->setText("Bump (Off)");
  }
}

void MainWindow::event_clicked_btn_enable_valve() {
  if (!marlin_host->MH_IsValveEnable()) {
    marlin_host->MH_EnableValve();
    ui->btn_enable_valve->setText("Valve (On)");
  } else {
    marlin_host->MH_DisableValve();
    ui->btn_enable_valve->setText("Valve (Off)");
  }
}


/// MANUAL JOG X
void MainWindow::event_clicked_btn_move_x_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, 1.0);
}

void MainWindow::event_clicked_btn_move_x_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, 10.0);
}

void MainWindow::event_clicked_btn_move_x_plus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, 100.0);
}

void MainWindow::event_clicked_btn_move_x_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, -1.0);
}

void MainWindow::event_clicked_btn_move_x_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, -10.0);
}

void MainWindow::event_clicked_btn_move_x_minus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, -100.0);
}

void MainWindow::event_clicked_btn_move_x() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_x->value()) :
                        (ui->dSpinBox_move_x->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, distance);
}

/// MANUAL JOG Y
void MainWindow::event_clicked_btn_move_y_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, 1.0);
}

void MainWindow::event_clicked_btn_move_y_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, 10.0);
}

void MainWindow::event_clicked_btn_move_y_plus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, 100.0);
}

void MainWindow::event_clicked_btn_move_y_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, -1.0);
}

void MainWindow::event_clicked_btn_move_y_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, -10.0);
}

void MainWindow::event_clicked_btn_move_y_minus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, -100.0);
}

void MainWindow::event_clicked_btn_move_y() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_y->value()) :
                        (ui->dSpinBox_move_y->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, distance);
}

/// MANUAL JOG Z
void MainWindow::event_clicked_btn_move_z_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, 1.0);
}

void MainWindow::event_clicked_btn_move_z_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, 10.0);
}

void MainWindow::event_clicked_btn_move_z_plus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, 100.0);
}

void MainWindow::event_clicked_btn_move_z_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, -1.0);
}

void MainWindow::event_clicked_btn_move_z_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, -10.0);
}

void MainWindow::event_clicked_btn_move_z_minus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, -100.0);
}

void MainWindow::event_clicked_btn_move_z() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_z->value()) :
                        (ui->dSpinBox_move_z->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, distance);
}

/// MANUAL JOG R
void MainWindow::event_clicked_btn_move_r_plus_01() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, 0.1);
}

void MainWindow::event_clicked_btn_move_r_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, 1.0);
}

void MainWindow::event_clicked_btn_move_r_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, 10.0);
}

void MainWindow::event_clicked_btn_move_r_minus_01() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, -0.1);
}

void MainWindow::event_clicked_btn_move_r_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, -1.0);
}

void MainWindow::event_clicked_btn_move_r_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, -10.0);
}

void MainWindow::event_clicked_btn_move_r() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_r->value()) :
                        (ui->dSpinBox_move_r->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, distance);
}

void MainWindow::marlin_host_initial_event() {
  connect(marlin_host, &Marlin_Host::MH_Signal_Connected,
          this, &MainWindow::marlin_host_event_connected);
  connect(marlin_host, &Marlin_Host::MH_Signal_Disconnected,
          this, &MainWindow::marlin_host_event_disconnected);
  connect(marlin_host, &Marlin_Host::MH_Signal_ConnectFailed,
          this, &MainWindow::marlin_host_event_connect_failed);
  connect(marlin_host, &Marlin_Host::MH_Signal_ErrorOccurred,
          this, &MainWindow::marlin_host_event_error_occurred);
  connect(marlin_host, &Marlin_Host::MH_Signal_ReadBytesToShow,
          this, &MainWindow::marlin_host_event_show_read_bytes);
  connect(marlin_host, &Marlin_Host::MH_Signal_WrittenBytesToShow,
          this, &MainWindow::marlin_host_event_show_written_bytes);
  connect(marlin_host, &Marlin_Host::MH_Signal_TargetPoisionChanged,
          this, &MainWindow::marlin_host_event_target_position_changed);
}

void MainWindow::marlin_host_event_connected() {
  ui->btn_serial_connect->setText("Disconnect");
  ui->statusbar->showMessage("Marlin connected.", 5000);
}
void MainWindow::marlin_host_event_disconnected() {
  QString port_name = "Port Name: ###";
  QString baud_rate = "Baudrate: ###";
  ui->lb_serial_connected_name->setText(port_name);
  ui->lb_serial_connected_baud->setText(baud_rate);
  ui->btn_serial_connect->setText("Connect");
  ui->statusbar->showMessage("Marlin disconnected.", 5000);
}
void MainWindow::marlin_host_event_connect_failed(QString msg) {
  QMessageBox::critical(this, tr("Error"), msg);
  ui->statusbar->showMessage("Marlin connect failed.", 5000);
}
void MainWindow::marlin_host_event_error_occurred(QString msg) {
  QMessageBox::critical(this, tr("Critical Error"), msg);
  ui->statusbar->showMessage("Critical Error, close port.", 5000);
}

void MainWindow::marlin_host_event_show_read_bytes
    (const QByteArray &data_bytes) {
  QString msg = "[INPUT]   " + QString::fromUtf8(data_bytes);
  ui->text_browser_serial_read->setTextBackgroundColor(QColor(189, 253, 255));
  ui->text_browser_serial_read->append(msg + "  ");
}

void MainWindow::marlin_host_event_show_written_bytes
    (const QByteArray &data_bytes) {
  QString msg = "[OUTPUT]   " + QString::fromUtf8(data_bytes);
  ui->text_browser_serial_read->setTextBackgroundColor(QColor(255, 189, 210));
  ui->text_browser_serial_read->append(msg + "  ");
}

void MainWindow::marlin_host_event_target_position_changed(
    const Marlin_Host::Position target) {
  ui->lb_position_x->setText("X: " + QString::number(target.X));
  ui->lb_position_y->setText("Y: " + QString::number(target.Y));
  ui->lb_position_z->setText("Z: " + QString::number(target.Z));
  ui->lb_position_r->setText("R: " + QString::number(target.R));
}

void MainWindow::send_marlin_command(){
  if (marlin_host->MH_IsConnected()) {
    QString command = ui->line_edit_serial_command->text();
    marlin_host->MH_WriteCommand(command);
    ui->line_edit_serial_command->saveText();
  }
}
