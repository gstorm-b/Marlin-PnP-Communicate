#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  event_initialize();

  marlin_host = new Marlin_Host(QThread::NormalPriority, this);
  marlin_host_initial_event();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::event_initialize() {
  connect(ui->btn_serial_connect, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_serial_connect);
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

void MainWindow::marlin_host_initial_event() {
  connect(marlin_host, &Marlin_Host::MH_Signal_Connected,
          this, &MainWindow::marlin_host_event_connected);
  connect(marlin_host, &Marlin_Host::MH_Signal_Disconnected,
          this, &MainWindow::marlin_host_event_disconnected);
  connect(marlin_host, &Marlin_Host::MH_Signal_ConnectFailed,
          this, &MainWindow::marlin_host_event_connect_failed);
  connect(marlin_host, &Marlin_Host::MH_Signal_ErrorOccurred,
          this, &MainWindow::marlin_host_event_error_occurred);
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
