#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>

#include "serial/serialsettingdialog.h"
#include "serial/marlin_host.h"
#include "blineedit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void closeEvent(QCloseEvent *event) override;

  void event_initialize();
  void event_clicked_btn_serial_connect();
  void event_clicked_btn_serial_send();
  void event_clicked_btn_serial_clear();
  void event_return_pressed_line_edit_serial_command();
  void event_key_pressed_line_edit_serial_command(QKeyEvent *event);

  void event_clicked_btn_home();
  void event_clicked_btn_disable_motor();
  void event_clicked_btn_enable_bump();
  void event_clicked_btn_enable_valve();

  void event_clicked_btn_move_x_plus_1();
  void event_clicked_btn_move_x_plus_10();
  void event_clicked_btn_move_x_plus_100();
  void event_clicked_btn_move_x_minus_1();
  void event_clicked_btn_move_x_minus_10();
  void event_clicked_btn_move_x_minus_100();
  void event_clicked_btn_move_x();

  void event_clicked_btn_move_y_plus_1();
  void event_clicked_btn_move_y_plus_10();
  void event_clicked_btn_move_y_plus_100();
  void event_clicked_btn_move_y_minus_1();
  void event_clicked_btn_move_y_minus_10();
  void event_clicked_btn_move_y_minus_100();
  void event_clicked_btn_move_y();

  void event_clicked_btn_move_z_plus_1();
  void event_clicked_btn_move_z_plus_10();
  void event_clicked_btn_move_z_plus_100();
  void event_clicked_btn_move_z_minus_1();
  void event_clicked_btn_move_z_minus_10();
  void event_clicked_btn_move_z_minus_100();
  void event_clicked_btn_move_z();

  void event_clicked_btn_move_r_plus_01();
  void event_clicked_btn_move_r_plus_1();
  void event_clicked_btn_move_r_plus_10();
  void event_clicked_btn_move_r_minus_01();
  void event_clicked_btn_move_r_minus_1();
  void event_clicked_btn_move_r_minus_10();
  void event_clicked_btn_move_r();

  void marlin_host_initial_event();
  void marlin_host_event_connected();
  void marlin_host_event_disconnected();
  void marlin_host_event_connect_failed(QString msg);
  void marlin_host_event_error_occurred(QString msg);
  void marlin_host_event_show_read_bytes(const QByteArray &data_bytes);
  void marlin_host_event_show_written_bytes(const QByteArray &data_bytes);
  void marlin_host_event_target_position_changed(
      const Marlin_Host::Position target);

  void send_marlin_command();
private:
  Ui::MainWindow *ui;
  Marlin_Host *marlin_host = nullptr;

  bool test = false;
};
#endif // MAINWINDOW_H
