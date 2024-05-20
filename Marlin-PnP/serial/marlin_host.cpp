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

void Marlin_Host::run() {
  MH_Serial_Initialize();

  qDebug() << "Thread running...";
  while (thread_running_) {
    // thread handle
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

  qDebug() << "Theard done!";
}

void Marlin_Host::MH_Serial_Initialize() {
  serial_host_ = new QSerialPort;
  connect(this, &Marlin_Host::finished, serial_host_, &QSerialPort::deleteLater);

  serial_host_->setPortName(serial_setting_.name);
  serial_host_->setBaudRate(serial_setting_.baudrate);
  serial_host_->setDataBits(serial_setting_.data_bits);
  serial_host_->setParity(serial_setting_.parity);
  serial_host_->setStopBits(serial_setting_.stop_bits);
  serial_host_->setFlowControl(serial_setting_.flow_control);

  if (serial_host_->open(QIODevice::ReadWrite)) {
    marlin_host_connected_ = true;
    emit MH_Signal_Connected();
  } else {
    emit MH_Signal_ConnectFailed(serial_host_->errorString());
  }
}

void Marlin_Host::MH_Serial_ClearData() {

}
