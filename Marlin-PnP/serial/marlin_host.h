#ifndef MARLIN_HOST_H
#define MARLIN_HOST_H

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QSerialPort>
#include <QTimer>

#include <QDebug>

#include "serialsettingdialog.h"

#define LINE_FEED_CHAR              (char)0x0A
#define MUTEX_SERIAL_LOCK_TIMEOUT   500
#define VALVE_ENABLE                "M104 T1 S255"
#define VALVE_DISBALE               "M104 T1 S0"
#define BUMP_ENABLE                 "M104 T0 S255"
#define BUMP_DISBALE                "M104 T0 S0"

#define TCP_CHAR_SEPARATOR          ','
#define TCP_CHAR_END                ';'

#define TCP_HOST_PORT               27800

#define TCP_CMD_HOME                "Home"
//#define TCP_CMD_JOG                 "Jog"
#define TCP_CMD_MOVEL               "MoveL"
#define TCP_CMD_SET_VALVE           "SetValve"
#define TCP_CMD_SET_BUMP            "SetBump"
#define TCP_CMD_PNP                 "PnP"
#define TCP_CMD_GCODE               "Gcode"

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
    bool gotFromStr = false;

    Position() {}

    Position(double x, double y, double z, double r) {
      X = x;
      Y = y;
      Z = z;
      R = r;
    }

    Position(QString str_pos) {
      if (     (str_pos.front() != '(') || (str_pos.back() != ')') ) {
        gotFromStr =false;
        return;
      }

      str_pos.removeFirst();
      str_pos.removeLast();

      QList<QString> axis_list = str_pos.split(' ');
      if (axis_list.size() != 4) {
        gotFromStr =false;
        return;
      }

      X = axis_list.at(0).toDouble();
      Y = axis_list.at(1).toDouble();
      Z = axis_list.at(2).toDouble();
      R = axis_list.at(3).toDouble();
      gotFromStr = true;
    }

    const QString& toQString() {
      return QString::asprintf("%.3f %.3f %.3f %.3f", X, Y, Z, R);
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
  void MH_WriteCommandAtFront(QString command);
  void MH_Home();
  void MH_DisableStepper();
  void MH_ManualJog(Axis axis, double distance);
  void MH_PnP(Position pick, double pick_jump,
              Position place, double place_jump,
              double speed = 20000, double lift_speed = 10000,
              int delay = 200);
  void MH_EnableValve();
  void MH_DisableValve();
  bool MH_IsValveEnable();
  void MH_EnableBump();
  void MH_DisableBump();
  bool MH_IsBumpEnable();
  void MH_Dwell(int time);
  void MH_WaitFinishMove();
  void MH_WaitDonePnp();
  Position MH_CurrentTargetPosition();
  QString MH_LastCommand();
  static const QString MH_StrMoveL(Position coor, double speed = 0, bool use_r = false);

private:
  void run() override;
  bool MH_SerialInitialize();
  void MH_Serial_WriteData(const QByteArray &data);
  void MH_TcpServerInitialize();
  void MH_TcpServerListen();
  void MH_TcpInComming();
  void MH_TcpReadFromClient();
  void MH_TcpCommandHandle(const QString &command);
  void MH_TcpDisconnected();
  void MH_TcpPnpDone();

  double mmFeedrate(double mmpm);

signals:
  void MH_Signal_Connected();
  void MH_Signal_Disconnected();
  void MH_Signal_ConnectFailed(QString msg);
  void MH_Signal_ErrorOccurred(QString msg);
  void MH_Signal_ReadBytesToShow(const QByteArray &bytes);
  void MH_Signal_WrittenBytesToShow(const QByteArray &bytes);
  void MH_Signal_ReadBytesAvailable(const QByteArray &data);
  void MH_Signal_TargetPoisionChanged(const Position new_target);
  void MH_Signal_TcpServerInitFailed(QString msg);
  void MH_Signal_PnpDone();

private:
  QThread::Priority thread_priority_;
  bool thread_running_;
  QMutex mutex_;
  QTcpServer *tcp_server_ = nullptr;
  QTcpSocket *client_connection_ = nullptr;
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
  QString tcp_address_;
  const int tcp_port_ = TCP_HOST_PORT;
};

#endif // MARLIN_HOST_H
