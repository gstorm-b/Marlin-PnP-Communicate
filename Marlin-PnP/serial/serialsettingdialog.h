#ifndef SERIALSETTINGDIALOG_H
#define SERIALSETTINGDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>

static const char BlankString[] = QT_TRANSLATE_NOOP("SerialSettingWidget", "N/A");

struct SerialSetting
{
  QString name;
  qint32 baudrate;
  QString string_baudrate;
  QSerialPort::DataBits data_bits;
  QString string_data_bits;
  QSerialPort::Parity parity;
  QString string_parity;
  QSerialPort::StopBits stop_bits;
  QString string_stop_bits;
  QSerialPort::FlowControl flow_control;
  QString string_flow_control;
  int write_timeout;
  QString string_write_timeout;
  //  bool local_echo_enabled;
};

namespace Ui {
class SerialSettingDialog;
}

class SerialSettingDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SerialSettingDialog(QWidget *parent = nullptr);
  ~SerialSettingDialog();

  void ShowDialog();
  SerialSetting GetSerialSetting() const;

private:
  void InitActions();
  void FillPortParameter();
  void FillAvailablePort();
  void DisplayPortInfo(int index);

signals:
  void UserAcceptSerialSetting(SerialSetting setting);

private:
  Ui::SerialSettingDialog *ui;
};

#endif // SERIALSETTINGDIALOG_H
