#ifndef MARLIN_HOST_H
#define MARLIN_HOST_H

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QSerialPort>

#include <QDebug>

#include "serialsettingdialog.h"

#define MUTEX_SERIAL_LOCK_TIMEOUT 500

class Marlin_Host : public QThread
{
  Q_OBJECT
public:
  explicit Marlin_Host(QThread::Priority priority = QThread::NormalPriority,
                        QObject *parent = nullptr);
  ~Marlin_Host();

  ///*** Marlin host <=> MH ***///

  void MH_Connect(SerialSetting setting);
  void MH_Disconnect();
  bool MH_IsConnected();

private:
  void run() override;
  void MH_Serial_Initialize();
  void MH_Serial_ClearData();

signals:
  void MH_Signal_Connected();
  void MH_Signal_Disconnected();
  void MH_Signal_ConnectFailed(QString msg);
  void MH_Signal_ErrorOccurred(QString msg);

private:
  QThread::Priority thread_priority_;
  bool thread_running_;
  QMutex mutex_;
  QSerialPort *serial_host_ = nullptr;
  SerialSetting serial_setting_;
  bool marlin_host_connected_;
  bool wait_disconnect_;
};

#endif // MARLIN_HOST_H
