#include "marlin_host.h"

Marlin_Host::Marlin_Host(QThread::Priority priority, QObject *parent)
    : QThread{parent} {
  thread_priority_ = priority;
  thread_running_ = false;
  marlin_host_connected_ = false;
  wait_disconnect_ = false;
}

Marlin_Host::~Marlin_Host() {

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
  if (command.isEmpty()) {
    return;
  }
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  command_queue_.push_back(command);
  mutex_.unlock();
}

void Marlin_Host::MH_WritePriorCommand(QString command) {
  if (command.isEmpty()) {
    return;
  }
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  prior_command_queue_.push_back(command);
  mutex_.unlock();
}

void Marlin_Host::MH_Home() {
  if (!MH_IsConnected()) {
    return;
  }

  MH_WriteCommand("G28");
}

void Marlin_Host::MH_DisableStepper() {
  if (!MH_IsConnected()) {
    return;
  }
  MH_WriteCommand("M18");
}

void Marlin_Host::MH_ManualJog(Axis axis, double distance) {
  if (!MH_IsConnected()) {
    return;
  }

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
            double speed, double lift_speed) {

  Position pick_high = pick;
  pick_high.Z += pick_jump;
  Position place_high = place;
  place_high.Z += place_jump;

//  MH_WriteCommand("M204 P1000");

  MH_WriteCommand(MH_StrMoveL(pick_high, speed, true));
  MH_WriteCommand("G4 P200");
  MH_WriteCommand(QString::asprintf("G0 Z%.3f F%.3f", pick.Z, lift_speed));
  MH_WriteCommand(QString::asprintf("G0 Z%.3f", pick_high.Z));

  MH_WriteCommand(MH_StrMoveL(place_high, speed, true));

  MH_WriteCommand(QString::asprintf("G0 Z%.3f F%.3f", place.Z, lift_speed));
  MH_EnableValve();
  MH_WriteCommand("G4 P200");
  MH_WriteCommand(QString::asprintf("G0 Z%.3f", place_high.Z));
  MH_DisableValve();
}

void Marlin_Host::MH_EnableValve() {
  if (!MH_IsConnected()) {
    return;
  }

  MH_WriteCommand(VALVE_ENABLE);
}

void Marlin_Host::MH_DisableValve() {
  if (!MH_IsConnected()) {
    return;
  }

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
  if (!MH_IsConnected()) {
    return;
  }

  MH_WriteCommand(BUMP_ENABLE);
}

void Marlin_Host::MH_DisableBump() {
  if (!MH_IsConnected()) {
    return;
  }

  MH_WriteCommand(BUMP_DISBALE);
}

bool Marlin_Host::MH_IsBumpEnable() {
  bool state;
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  state = is_bump_enable_;
  mutex_.unlock();
  return state;
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

QString Marlin_Host::MH_StrMoveL(Position coor, double speed, bool use_r){
  QString command;
  if (use_r) {
    command = QString::asprintf("G1 X%.3f Y%.3f Z%.3f R%.3f", coor.X, coor.Y, coor.Z, coor.R);
  } else {
    command = QString::asprintf("G0 X%.3f Y%.3f Z%.3f", coor.X, coor.Y, coor.Z);
  }

  if (speed > 0) {
    command.append(QString::asprintf(" F%.3f", speed));
  }

  return command;
}

void Marlin_Host::run() {
  if (!MH_Serial_Initialize()) {
    thread_running_ = false;
    qDebug() << "Marlin host initialze failed!";
    return;
  }

  qDebug() << "Marlin host running...";
  QByteArray read_bytes;
  bool wait_command_respond = false;
  this->msleep(2000);
  MH_WriteCommand("G21");
  MH_WriteCommand("M154 S1");

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

//    if (!wait_command_respond) {
//      if (prior_command_queue_.size() > 0) {
//        QByteArray command_bytes = prior_command_queue_.front().toUtf8();
//        last_command_ = prior_command_queue_.front();
//        command_bytes.push_back(LINE_FEED_CHAR);
//        MH_Serial_WriteData(command_bytes);
//        wait_command_respond = true;
//        prior_command_queue_.pop_front();
//        emit MH_Signal_WrittenBytesToShow(last_command_.toUtf8());
//      } else if (command_queue_.size() > 0) {
//        QByteArray command_bytes = command_queue_.front().toUtf8();
//        last_command_ = command_queue_.front();
//        command_bytes.push_back(LINE_FEED_CHAR);
//        MH_Serial_WriteData(command_bytes);
//        wait_command_respond = true;
//        command_queue_.pop_front();
//        emit MH_Signal_WrittenBytesToShow(last_command_.toUtf8());
//      }
//    }

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

bool Marlin_Host::MH_Serial_Initialize() {
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
  qDebug() << "Marlin host data written.";
}
