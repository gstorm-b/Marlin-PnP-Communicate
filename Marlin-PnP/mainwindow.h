#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>

#include "serial/serialsettingdialog.h"
#include "serial/marlin_host.h"

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
  void event_initialize();
  void event_clicked_btn_serial_connect();

  void marlin_host_initial_event();
  void marlin_host_event_connected();
  void marlin_host_event_disconnected();
  void marlin_host_event_connect_failed(QString msg);
  void marlin_host_event_error_occurred(QString msg);
private:
  Ui::MainWindow *ui;
  Marlin_Host *marlin_host = nullptr;
};
#endif // MAINWINDOW_H
