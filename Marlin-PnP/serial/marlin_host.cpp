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
  mutex_.tryLock(MUTEX_SERIAL_LOCK_TIMEOUT);
  command_queue_.push_back(command);
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

//  qDebug() << jog_command;
  MH_WriteCommand("G91");
  MH_WriteCommand(jog_command);
  MH_WriteCommand("G90");
  MH_WriteCommand("M114");
}

void Marlin_Host::run() {
  if (!MH_Serial_Initialize()) {
    thread_running_ = false;
    qDebug() << "Marlin host initialze failed!";
    return;
  }

  qDebug() << "Marlin host running...";
  QByteArray read_bytes;
  bool wait_command_confirm = false;

  while (thread_running_) {
    // thread handle
    QList<QByteArray> lines = read_bytes.split(0x0a);

    if (serial_host_->waitForReadyRead(10)) {
//      qDebug() << "Available bytes: " << serial_host_->bytesAvailable();
      read_bytes.push_back(serial_host_->readAll());
      lines = read_bytes.split(0x0a);
      read_bytes = lines.back();
      qDebug() << "Read line: " << read_bytes;

      if (lines.size() > 1) {
        for (int i=0;i<(lines.size()-1);i++) {

          if ((lines[i] == "ok") && (wait_command_confirm)) {
            wait_command_confirm =  false;
            command_queue_.pop_front();
          }
          emit MH_Signal_ReadBytesAvailable(lines[i]);
        }
      }
    }

    if ((command_queue_.size() > 0) && (!wait_command_confirm)) {
      QByteArray command_bytes = command_queue_.front().toUtf8();
      command_bytes.push_back(LINE_FEED_CHAR);
      MH_Serial_WriteData(command_bytes);
      wait_command_confirm = true;
//      command_queue_.pop_front();
      emit MH_Signal_ReadBytesAvailable(command_bytes);
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

  if (serial_host_->open(QIODevice::ReadWrite)) {
    marlin_host_connected_ = true;
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
