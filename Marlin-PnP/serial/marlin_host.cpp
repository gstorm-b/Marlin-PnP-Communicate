#include "marlin_host.h"

Marlin_Host::Marlin_Host(QThread::Priority priority, QObject *parent)
    : QThread{parent} {
  thread_priority_ = priority;
  thread_running_ = false;
  marlin_host_connected_ = false;
  wait_disconnect_ = false;

  MH_TcpServerInitialize();
}

Marlin_Host::~Marlin_Host() {
  tcp_server_->deleteLater();
  client_connection_->deleteLater();
}

void Marlin_Host::MH_Connect(SerialSetting setting) {
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  if(!thread_running_) {
    serial_setting_ = setting;
    thread_running_ = true;
    start(thread_priority_);
  }
  mutex_.unlock();
}

void Marlin_Host::MH_Disconnect() {
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  if (thread_running_) {
    wait_disconnect_ = true;
  }
  mutex_.unlock();
}

bool Marlin_Host::MH_IsConnected() {
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  bool is_connected = marlin_host_connected_;
  mutex_.unlock();
  return is_connected;
}

void Marlin_Host::MH_WriteCommand(QString command) {
  if (!MH_IsConnected()) {
    return;
  }

  if (command.isEmpty()) {
    return;
  }
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  command_queue_.push_back(command);
  mutex_.unlock();
}

void Marlin_Host::MH_WriteCommandAtFront(QString command) {
  if (!MH_IsConnected()) {
    return;
  }

  if (command.isEmpty()) {
    return;
  }
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  command_queue_.push_front(command);
  mutex_.unlock();
}

void Marlin_Host::MH_Home() {
  MH_WriteCommand("G28");
}

void Marlin_Host::MH_DisableStepper() {
  MH_WriteCommand("M18");
}

void Marlin_Host::MH_ManualJog(Axis axis, double distance) {
  QString jog_command = "G0";

  switch (axis) {
    case Axis::X:
      jog_command += " X" + QString::number(distance) + " F3000";
      break;
    case Axis::Y:
      jog_command += " Y" + QString::number(distance) + " F3000";
      break;
    case Axis::Z:
      jog_command += " Z" + QString::number(distance) + " F3000";
      break;
    case Axis::R:
      jog_command += " E" + QString::number(distance) + " F3000";
      break;
    default:
      return;
  }

  MH_WriteCommand("G91");
  MH_WriteCommand(jog_command);
  MH_WriteCommand("G90");
  MH_WriteCommand("M114");
}

void Marlin_Host::MH_PnP(Position pick, double pick_jump,
            Position place, double place_jump,
            double speed, double lift_speed, int delay) {

  Position pick_high = pick;
  pick_high.Z += pick_jump;
  Position place_high = place;
  place_high.Z += place_jump;

//  int feedrate = speed * 60;
  int lift_feedrate = lift_speed * 60;

  MH_WriteCommand(MH_StrMoveL(pick_high, speed, true));
  MH_WriteCommand("M114");
  MH_EnableValve();
  MH_WaitFinishMove();
  MH_WriteCommand(QString::asprintf("G0 Z%.3f F%d", pick.Z + 1, lift_feedrate));
  MH_WriteCommand("M114");
  MH_WaitFinishMove();
  MH_DisableValve();
  MH_Dwell(delay);
  MH_WriteCommand(QString::asprintf("G0 Z%.3f", pick_high.Z));
  MH_WriteCommand("M114");
  MH_WriteCommand(MH_StrMoveL(place_high, speed, true));
  MH_WriteCommand("M114");
  MH_WriteCommand(QString::asprintf("G0 Z%.3f F%d", place.Z, lift_feedrate));
  MH_WriteCommand("M114");
  MH_WaitFinishMove();
  MH_EnableValve();
  MH_Dwell(delay);
  MH_WriteCommand(QString::asprintf("G0 Z%.3f", place_high.Z));
  MH_WriteCommand("M114");
  MH_WaitDonePnp();
  MH_DisableValve();

}

void Marlin_Host::MH_EnableValve() {
  MH_WriteCommand(VALVE_ENABLE);
}

void Marlin_Host::MH_DisableValve() {
  MH_WriteCommand(VALVE_DISBALE);
}

bool Marlin_Host::MH_IsValveEnable() {
  bool state;
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  state = is_valve_enable_;
  mutex_.unlock();
  return state;
}


void Marlin_Host::MH_EnableBump() {
  MH_WriteCommand(BUMP_ENABLE);
}

void Marlin_Host::MH_DisableBump() {
  MH_WriteCommand(BUMP_DISBALE);
}

bool Marlin_Host::MH_IsBumpEnable() {
  bool state;
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  state = is_bump_enable_;
  mutex_.unlock();
  return state;
}

void Marlin_Host::MH_Dwell(int time) {
  MH_WriteCommand(QString::asprintf("G4 P%d", time));
}

void Marlin_Host::MH_WaitFinishMove() {
  MH_WriteCommand("M400");
}

void Marlin_Host::MH_WaitDonePnp() {
  MH_WriteCommand("M409");
}


Marlin_Host::Position Marlin_Host::MH_CurrentTargetPosition() {
  Marlin_Host::Position target;
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  target = target_position_;
  mutex_.unlock();
  return target;
}

QString Marlin_Host::MH_LastCommand(){
  QString command = "";
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  command = last_command_;
  mutex_.unlock();
  return command;
}

const QString Marlin_Host::MH_StrMoveL(Position coor, double speed, bool use_r){
  QString command;
  if (use_r) {
    command = QString::asprintf("G1 X%.3f Y%.3f Z%.3f E%.3f", coor.X, coor.Y, coor.Z, coor.R);
  } else {
    command = QString::asprintf("G0 X%.3f Y%.3f Z%.3f", coor.X, coor.Y, coor.Z);
  }

  if (speed > 0) {
    speed *= 60.0;
    command.append(QString::asprintf(" F%d", (int)speed));
  }

  return command;
}

void Marlin_Host::run() {

  if (!MH_SerialInitialize()) {
    thread_running_ = false;
    qDebug() << "Marlin host initialze failed!";
    return;
  }

  qDebug() << "Marlin host running...";
  QByteArray read_bytes;
  bool wait_command_respond = false;
  this->msleep(1000);
  MH_WriteCommandAtFront("G21");
  MH_WriteCommandAtFront("M503");
//  MH_WriteCommand("M154 S1");

  while (thread_running_) {
    // thread handle

    ///// input bytes handle
    QList<QByteArray> lines;
    // read input
    if (serial_host_->waitForReadyRead(10)) {
      read_bytes.push_back(serial_host_->readAll());
      lines = read_bytes.split(0x0a);
      read_bytes = lines.back();

      if (lines.size() > 1) {
        for (int i=0;i<(lines.size()-1);i++) {
          // command respond ok case
          if ((lines[i] == "ok") && (wait_command_respond)) {
            wait_command_respond =  false;
            lines[i].push_back(" (");
            lines[i].push_back(last_command_.toUtf8());
            lines[i].push_back(")");
            emit MH_Signal_ReadBytesToShow(lines[i]);

            if (last_command_ == VALVE_ENABLE) {
              is_valve_enable_ = true;
            } else if (last_command_ == VALVE_DISBALE) {
              is_valve_enable_ = false;
            } else if (last_command_ == BUMP_ENABLE) {
              is_bump_enable_ = true;
            } else if (last_command_ == BUMP_DISBALE) {
              is_bump_enable_ = false;
            }
            continue;
          } else if (lines[i] == "rep:PnP Done") {

            emit MH_Signal_PnpDone();
            emit MH_Signal_ReadBytesToShow(lines[i]);
            continue;
          }

          // get position
          if ((lines[i]).contains("X:") &&
              (lines[i]).contains("Y:") &&
              (lines[i]).contains("Z:") &&
              (lines[i]).contains("E:")) {

            QList<QByteArray> axes = lines[i].split(' ');
            axes[0].remove(0, 2);
            axes[1].remove(0, 2);
            axes[2].remove(0, 2);
            axes[3].remove(0, 2);
            target_position_.X = axes[0].toDouble();
            target_position_.Y = axes[1].toDouble();
            target_position_.Z = axes[2].toDouble();
            target_position_.R = axes[3].toDouble();
            emit MH_Signal_TargetPoisionChanged(target_position_);
          }

          emit MH_Signal_ReadBytesToShow(lines[i]);
        }
      }
    }

    ///// output bytes handle
    if ((command_queue_.size() > 0) && (!wait_command_respond)) {
      QByteArray command_bytes = command_queue_.front().toUtf8();
      last_command_ = command_queue_.front();
      command_bytes.push_back(LINE_FEED_CHAR);
      MH_Serial_WriteData(command_bytes);
      wait_command_respond = true;
      command_queue_.pop_front();
      emit MH_Signal_WrittenBytesToShow(last_command_.toUtf8());
    }

    if (wait_disconnect_) {
      thread_running_ = false;
      wait_disconnect_ = false;
    }
  }

  if (serial_host_->isOpen()) {
    serial_host_->close();
  }

  emit MH_Signal_Disconnected();
  marlin_host_connected_ = false;

  qDebug() << "Marlin host stopped.";
}

bool Marlin_Host::MH_SerialInitialize() {
  serial_host_ = new QSerialPort;
  connect(this, &Marlin_Host::finished,
          serial_host_, &QSerialPort::deleteLater);

  serial_host_->setPortName(serial_setting_.name);
  serial_host_->setBaudRate(serial_setting_.baudrate);
  serial_host_->setDataBits(serial_setting_.data_bits);
  serial_host_->setParity(serial_setting_.parity);
  serial_host_->setStopBits(serial_setting_.stop_bits);
  serial_host_->setFlowControl(serial_setting_.flow_control);

  command_queue_.clear();
  prior_command_queue_.clear();

  if (serial_host_->open(QIODevice::ReadWrite)) {
    marlin_host_connected_ = true;
    is_bump_enable_ = false;
    is_valve_enable_ = false;
    emit MH_Signal_Connected();
    return true;
  } else {
    emit MH_Signal_ConnectFailed(serial_host_->errorString());
    return false;
  }
}

void Marlin_Host::MH_Serial_WriteData(const QByteArray &data) {
  serial_host_->write(data);
  if (!serial_host_->waitForBytesWritten(serial_setting_.write_timeout)) {
    const QString error = tr("Failed to write all data to port %1.\n"
                             "Error: %2").arg(serial_host_->portName(),
                                   serial_host_->errorString());
    emit MH_Signal_ErrorOccurred(error);
    return;
  }
//  qDebug() << "Marlin host data written.";
}

void Marlin_Host::MH_TcpServerInitialize() {
  tcp_server_ = new QTcpServer(this);
  client_connection_ = new QTcpSocket(this);

  MH_TcpServerListen();

  connect(tcp_server_, &QTcpServer::newConnection, this, &Marlin_Host::MH_TcpInComming);
    connect(this, &Marlin_Host::MH_Signal_PnpDone, this, &Marlin_Host::MH_TcpPnpDone);
}

void Marlin_Host::MH_TcpServerListen() {
  // get TCP Sever address
  const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

  for (const QHostAddress &entry : ipAddressesList) {
    // use the first non-localhost IPv4 address
    if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
      tcp_address_ = entry.toString();
      qDebug() << QString(
                      "TCP Server is listening at address %1, port %2").
                  arg(tcp_address_).arg(tcp_port_);
      break;
    }
  }

  if (!tcp_server_->listen(QHostAddress::Any, tcp_port_)) {
    qDebug() << "Listen failed!";
    emit MH_Signal_TcpServerInitFailed(tcp_server_->errorString());
    return;
  }
}

void Marlin_Host::MH_TcpInComming() {
  if (client_connection_->state() != QTcpSocket::UnconnectedState) {
    return;
  }

  qDebug() << "Incomming TCP Client connect...";

  client_connection_ = tcp_server_->nextPendingConnection();
  connect(client_connection_, &QAbstractSocket::disconnected,
          this, &Marlin_Host::MH_TcpDisconnected);

  client_connection_->write("You're handling Marlin PNP control port...\n");
  connect(client_connection_, &QTcpSocket::readyRead, this, &Marlin_Host::MH_TcpReadFromClient);

  tcp_server_->close();
}

void Marlin_Host::MH_TcpReadFromClient() {
  if (!MH_IsConnected()) {
    return;
  }

  QString read_bytes = QString::fromUtf8(client_connection_->readAll());
//  qDebug() << QString::fromUtf8(read_bytes);
//  qDebug() << read_bytes;

  QList<QString> commands = read_bytes.split(TCP_CHAR_END);

  for (const QString &cmd : commands) {
    if (cmd.isEmpty()) {
      continue;
    }

    MH_TcpCommandHandle(cmd);
  }
}

void Marlin_Host::MH_TcpCommandHandle(const QString &command) {
  if (command.isEmpty()) {
    return;
  }

  QList<QString> element = command.split(TCP_CHAR_SEPARATOR);

  if ((element.at(0) == TCP_CMD_HOME) && (element.size() == 1)){

    MH_Home();

  } else if ((element.at(0) == TCP_CMD_MOVEL) && (element.size() == 3)) {

    Position pos(element.at(1));
    if (pos.gotFromStr) {
      MH_WriteCommand(MH_StrMoveL(pos, element.at(2).toDouble(), true));
    } else {
      client_connection_->write("Command wrong format.\n");
    }

  } else if ((element.at(0) == TCP_CMD_SET_BUMP) && (element.size() == 2)) {

    if (element.at(1) == "1") MH_EnableBump();
    else if (element.at(1) == "0") MH_DisableBump();

  } else if ((element.at(0) == TCP_CMD_SET_VALVE) && (element.size() == 2)) {

    if (element.at(1) == "1") MH_EnableValve();
    else if (element.at(1) == "0") MH_DisableValve();

  } else if ((element.at(0) == TCP_CMD_PNP) && (element.size() >= 5)) {

    Position pick_point(element.at(1));
    double pick_jump = element.at(2).toDouble();
    Position place_point(element.at(3));
    double place_jump = element.at(4).toDouble();

    if (!pick_point.gotFromStr || !place_point.gotFromStr) {
      client_connection_->write("Command wrong format.\n");
      return;
    }

    double speed;
    double lift_speed;
    double delay;

//    qDebug() << "Element size:" << element.size();
//    qDebug() << "Pick point:" << pick_point.toQString();
//    qDebug() << "Pick jump:" << pick_jump;
//    qDebug() << "Pick point:" << place_point.toQString();
//    qDebug() << "Place jump:" << place_jump;

    if (element.size() == 6) {
      speed = element.at(5).toDouble();
      MH_PnP(pick_point, pick_jump, place_point, place_jump, speed);

    } else if (element.size() == 7) {
      speed = element.at(5).toDouble();
      lift_speed = element.at(6).toDouble();
      MH_PnP(pick_point, pick_jump, place_point, place_jump,
             speed, lift_speed);

    } else if (element.size() == 8) {
      speed = element.at(5).toDouble();
      lift_speed = element.at(6).toDouble();
      delay = element.at(7).toInt();
      MH_PnP(pick_point, pick_jump, place_point, place_jump,
             speed, lift_speed, delay);

    } else {
      MH_PnP(pick_point, pick_jump, place_point, place_jump);
    }

  } else if ((element.at(0) == TCP_CMD_GCODE) && (element.size() == 2)) {

    MH_WriteCommand(element.at(1));

  } else {
    client_connection_->write(
        "Command doesn't exist or wrong format.\n");
  }
}

void Marlin_Host::MH_TcpDisconnected() {
  qDebug() << "Client disconnected!";
  MH_TcpServerListen();
}

void Marlin_Host::MH_TcpPnpDone() {
  client_connection_->write("PnP Done.\n");
  qDebug() << "PnP Done.\n";
}

double mmFeedrate(double mmpm) {
  return mmpm / 60.0;
}

// PnP,(200 80 9 0),30,(20 150 9 1),30;
// PnP,(200 80 9 0),30,(20 150 9 1),30,10000,5000;
// PnP,(200 80 9 0),30,(20 150 9 1),30,10000,5000,2000;
// MoveL,position,speed
