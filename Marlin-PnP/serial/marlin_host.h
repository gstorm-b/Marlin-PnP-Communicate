#ifndef MARLIN_HOST_H
#define MARLIN_HOST_H

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QSerialPort>
#include <QTimer>

#include <QDebug>

#include "serialsettingdialog.h"

#define LINE_FEED_CHAR (char)0x0A
#define MUTEX_SERIAL_LOCK_TIMEOUT 500
#define VALVE_ENABLE "M106 S255"
#define VALVE_DISBALE "M106 S0"
#define BUMP_ENABLE "M104 S255"
#define BUMP_DISBALE "M104 S0"

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
    double X = 0;
    double Y = 0;
    double Z = 0;
    double R = 0;

    Position() {}

    Position(double x, double y, double z, double r) {
      X = x;
      Y = y;
      Z = z;
      R = r;
    }
  };

  explicit Marlin_Host(QThread::Priority priority = QThread::NormalPriority,
                        QObject *parent = nullptr);
  ~Marlin_Host();

  ///*** Marlin host <=> MH ***///

  void MH_Connect(SerialSetting setting);
  void MH_Disconnect();
  bool MH_IsConnected();
  void MH_WriteCommand(QString command);
  void MH_WritePriorCommand(QString command);
  void MH_Home();
  void MH_DisableStepper();
  void MH_ManualJog(Axis axis, double distance);
  void MH_PnP(Position pick, double pick_jump,
              Position place, double place_jump,
              double speed = 22000, double lift_speed = 10000);
  void MH_EnableValve();
  void MH_DisableValve();
  bool MH_IsValveEnable();
  void MH_EnableBump();
  void MH_DisableBump();
  bool MH_IsBumpEnable();
  Position MH_CurrentTargetPosition();
  QString MH_LastCommand();
  static QString MH_StrMoveL(Position coor, double speed = 0, bool use_r = false);

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
  void MH_Signal_TargetPoisionChanged(const Position new_target);

private:
  QThread::Priority thread_priority_;
  bool thread_running_;
  QMutex mutex_;
  QSerialPort *serial_host_ = nullptr;
  SerialSetting serial_setting_;
  bool marlin_host_connected_;
  bool wait_disconnect_;
  QList<QString> command_queue_;
  QList<QString> prior_command_queue_;
  QString last_command_;
  Position target_position_;
  bool is_bump_enable_;
  bool is_valve_enable_;
};

#endif // MARLIN_HOST_H
