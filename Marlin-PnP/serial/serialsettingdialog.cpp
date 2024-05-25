#include "serialsettingdialog.h"
#include "ui_serialsettingdialog.h"

SerialSettingDialog::SerialSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSettingDialog)
{
  ui->setupUi(this);

  InitActions();
}

SerialSettingDialog::~SerialSettingDialog()
{
  delete ui;
}

void SerialSettingDialog::ShowDialog() {
  FillPortParameter();
  FillAvailablePort();
  setModal(true);
  show();
}

SerialSetting SerialSettingDialog::GetSerialSetting() const {
  SerialSetting setting;
  // NAME
  setting.name = ui->combobox_serial_port_info->currentText();
  // BAUDRATE
  const auto baudrate = ui->combobox_baudrate->currentData();
  setting.baudrate = baudrate.value<QSerialPort::BaudRate>();
  setting.string_baudrate = QString::number(setting.baudrate);
  // DATE BITS
  const auto data_bits = ui->combobox_data_bits->currentData();
  setting.data_bits = data_bits.value<QSerialPort::DataBits>();
  setting.string_data_bits = ui->combobox_data_bits->currentText();
  // PARITY BIT
  const auto parity = ui->combobox_parity->currentData();
  setting.parity = parity.value<QSerialPort::Parity>();
  setting.string_parity = ui->combobox_parity->currentText();
  // STOP BITS
  const auto stop_bits = ui->combobox_stop_bits->currentData();
  setting.stop_bits = stop_bits.value<QSerialPort::StopBits>();
  setting.string_stop_bits = ui->combobox_stop_bits->currentText();
  // FLOW CONTROL
  const auto flow_control = ui->combobox_flow_control->currentData();
  setting.flow_control = flow_control.value<QSerialPort::FlowControl>();
  setting.string_flow_control = ui->combobox_flow_control->currentText();
  // RESPONSE TIME OUT
  setting.write_timeout = ui->spinBox_response_time->value();
  setting.string_write_timeout = QString::number(setting.write_timeout, 10);
  // LOCAL ECHO
  //  setting.string_flow_control = ui->combobox_local_echo->isChecked();

  return setting;
}

void SerialSettingDialog::InitActions() {
  connect(ui->button_refresh, &QPushButton::clicked,
          this, &SerialSettingDialog::FillAvailablePort);
  connect(ui->combobox_serial_port_info, &QComboBox::currentIndexChanged,
          this, &SerialSettingDialog::DisplayPortInfo);
  connect(this, &SerialSettingDialog::accepted, this, [this] () {
    emit UserAcceptSerialSetting(GetSerialSetting());
  });
  connect(this, &SerialSettingDialog::finished,
          this, &SerialSettingDialog::deleteLater);
}

void SerialSettingDialog::FillPortParameter() {
  ui->combobox_baudrate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
  ui->combobox_baudrate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
  ui->combobox_baudrate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
  ui->combobox_baudrate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
  ui->combobox_baudrate->addItem(QStringLiteral("250000"), 250000);
  ui->combobox_baudrate->setCurrentIndex(4);

  ui->combobox_data_bits->addItem(QStringLiteral("5"), QSerialPort::Data5);
  ui->combobox_data_bits->addItem(QStringLiteral("6"), QSerialPort::Data6);
  ui->combobox_data_bits->addItem(QStringLiteral("7"), QSerialPort::Data7);
  ui->combobox_data_bits->addItem(QStringLiteral("8"), QSerialPort::Data8);
  ui->combobox_data_bits->setCurrentIndex(3);

  ui->combobox_parity->addItem(tr("None"), QSerialPort::NoParity);
  ui->combobox_parity->addItem(tr("Even"), QSerialPort::EvenParity);
  ui->combobox_parity->addItem(tr("Odd"), QSerialPort::OddParity);
  ui->combobox_parity->addItem(tr("Mark"), QSerialPort::MarkParity);
  ui->combobox_parity->addItem(tr("Space"), QSerialPort::SpaceParity);
  ui->combobox_parity->setCurrentIndex(0);

  ui->combobox_stop_bits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
  ui->combobox_stop_bits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
  ui->combobox_stop_bits->setCurrentIndex(0);

  ui->combobox_flow_control->addItem(tr("None"), QSerialPort::NoFlowControl);
  ui->combobox_flow_control->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
  ui->combobox_flow_control->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
  ui->combobox_flow_control->setCurrentIndex(2);
}

void SerialSettingDialog::FillAvailablePort() {
  ui->combobox_serial_port_info->clear();
  const QString blank_string = tr(::BlankString);
  const auto infos = QSerialPortInfo::availablePorts();

  for (const QSerialPortInfo &info : infos) {
    QStringList list;
    const QString description = info.description();
    const QString manufacturer = info.manufacturer();
    const QString serial_number = info.serialNumber();
    const auto vendor_id = info.vendorIdentifier();
    const auto product_id = info.productIdentifier();
    list << info.portName()
         << (!description.isEmpty() ? description : blank_string)
         << (!manufacturer.isEmpty() ? manufacturer : blank_string)
         << (!serial_number.isEmpty() ? serial_number : blank_string)
         << info.systemLocation()
         << (vendor_id ? QString::number(vendor_id, 16) : blank_string)
         << (product_id ? QString::number(product_id, 16) : blank_string);

    ui->combobox_serial_port_info->addItem(list.constFirst(), list);
  }
}

void SerialSettingDialog::DisplayPortInfo(int index) {
  if (index <= -1)
    return;

  const QString blank_string = tr(::BlankString);

  const QStringList list = ui->combobox_serial_port_info->itemData(index).toStringList();
  ui->label_description->setText(tr("Description: %1").arg(list.value(1, blank_string)));
  ui->label_manufacturer->setText(tr("Manufacturer: %1").arg(list.value(2, blank_string)));
  ui->label_serial_number->setText(tr("Serial number: %1").arg(list.value(3, blank_string)));
  ui->label_location->setText(tr("Location: %1").arg(list.value(4, blank_string)));
  ui->label_vendor_id->setText(tr("Vendor Identifier: %1").arg(list.value(5, blank_string)));
  ui->label_product_id->setText(tr("Product Identifier: %1").arg(list.value(6, blank_string)));
}
