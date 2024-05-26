#ifndef MARLIN_HOST_H
#define MARLIN_HOST_H

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QSerialPort>

#include <QDebug>

#include "serialsettingdialog.h"

#define LINE_FEED_CHAR (char)0x0A
#define MUTEX_SERIAL_LOCK_TIMEOUT 500

class Marlin_Host : public QThread
{
  Q_OBJECT
public:
  enum Axis {
    X = 0,
    Y = 1,
    Z = 2,
    R = 3
  };

  struct Position {
    double X;
    double Y;
    double Z;
  };

  explicit Marlin_Host(QThread::Priority priority = QThread::NormalPriority,
                        QObject *parent = nullptr);
  ~Marlin_Host();

  ///*** Marlin host <=> MH ***///

  void MH_Connect(SerialSetting setting);
  void MH_Disconnect();
  bool MH_IsConnected();
  void MH_WriteCommand(QString command);
  void MH_Home();
  void MH_DisableStepper();
  void MH_ManualJog(Axis axis, double distance);
  void MH_EnableVaccum();
  void MH_DisableVaccum();
  void MH_EnableBump();
  void MH_DisableBump();
  QString MH_LastCommand();

private:
  void run() override;
  bool MH_Serial_Initialize();
  void MH_Serial_WriteData(const QByteArray &data);

signals:
  void MH_Signal_Connected();
  void MH_Signal_Disconnected();
  void MH_Signal_ConnectFailed(QString msg);
  void MH_Signal_ErrorOccurred(QString msg);
  void MH_Signal_ReadBytesToShow(const QByteArray &bytes);
  void MH_Signal_WrittenBytesToShow(const QByteArray &bytes);
  void MH_Signal_ReadBytesAvailable(const QByteArray &data);

private:
  QThread::Priority thread_priority_;
  bool thread_running_;
  QMutex mutex_;
  QSerialPort *serial_host_ = nullptr;
  SerialSetting serial_setting_;
  bool marlin_host_connected_;
  bool wait_disconnect_;
  QList<QString> command_queue_;
  QString last_command_;
};

#endif // MARLIN_HOST_H
