/********************************************************************************
** Form generated from reading UI file 'serialsettingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALSETTINGDIALOG_H
#define UI_SERIALSETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SerialSettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupbox_port_select;
    QGridLayout *gridLayout;
    QLabel *label_manufacturer;
    QLabel *label_description;
    QLabel *label_vendor_id;
    QLabel *label_product_id;
    QLabel *label_location;
    QLabel *label_serial_number;
    QHBoxLayout *horizontalLayout;
    QComboBox *combobox_serial_port_info;
    QPushButton *button_refresh;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupbox_parameters;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_3;
    QComboBox *combobox_data_bits;
    QComboBox *combobox_stop_bits;
    QLabel *label_parity;
    QLabel *label_baudrate;
    QComboBox *combobox_parity;
    QLabel *label_data_bits;
    QComboBox *combobox_flow_control;
    QLabel *label_flow_control;
    QLabel *label_stop_bits;
    QComboBox *combobox_baudrate;
    QGridLayout *gridLayout_4;
    QSpinBox *spinBox_response_time;
    QLabel *label_response_time;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SerialSettingDialog)
    {
        if (SerialSettingDialog->objectName().isEmpty())
            SerialSettingDialog->setObjectName("SerialSettingDialog");
        SerialSettingDialog->resize(433, 272);
        verticalLayout = new QVBoxLayout(SerialSettingDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        groupbox_port_select = new QGroupBox(SerialSettingDialog);
        groupbox_port_select->setObjectName("groupbox_port_select");
        gridLayout = new QGridLayout(groupbox_port_select);
        gridLayout->setSpacing(8);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(9, 9, 9, 9);
        label_manufacturer = new QLabel(groupbox_port_select);
        label_manufacturer->setObjectName("label_manufacturer");

        gridLayout->addWidget(label_manufacturer, 3, 0, 1, 1);

        label_description = new QLabel(groupbox_port_select);
        label_description->setObjectName("label_description");
        label_description->setMinimumSize(QSize(200, 0));

        gridLayout->addWidget(label_description, 2, 0, 1, 1);

        label_vendor_id = new QLabel(groupbox_port_select);
        label_vendor_id->setObjectName("label_vendor_id");

        gridLayout->addWidget(label_vendor_id, 6, 0, 1, 1);

        label_product_id = new QLabel(groupbox_port_select);
        label_product_id->setObjectName("label_product_id");

        gridLayout->addWidget(label_product_id, 7, 0, 1, 1);

        label_location = new QLabel(groupbox_port_select);
        label_location->setObjectName("label_location");

        gridLayout->addWidget(label_location, 5, 0, 1, 1);

        label_serial_number = new QLabel(groupbox_port_select);
        label_serial_number->setObjectName("label_serial_number");

        gridLayout->addWidget(label_serial_number, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName("horizontalLayout");
        combobox_serial_port_info = new QComboBox(groupbox_port_select);
        combobox_serial_port_info->setObjectName("combobox_serial_port_info");
        combobox_serial_port_info->setMinimumSize(QSize(120, 0));
        combobox_serial_port_info->setMaximumSize(QSize(120, 16777215));

        horizontalLayout->addWidget(combobox_serial_port_info);

        button_refresh = new QPushButton(groupbox_port_select);
        button_refresh->setObjectName("button_refresh");
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(button_refresh->sizePolicy().hasHeightForWidth());
        button_refresh->setSizePolicy(sizePolicy);
        button_refresh->setMaximumSize(QSize(60, 24));

        horizontalLayout->addWidget(button_refresh);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        horizontalLayout_2->addWidget(groupbox_port_select);

        groupbox_parameters = new QGroupBox(SerialSettingDialog);
        groupbox_parameters->setObjectName("groupbox_parameters");
        verticalLayout_2 = new QVBoxLayout(groupbox_parameters);
        verticalLayout_2->setObjectName("verticalLayout_2");
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        combobox_data_bits = new QComboBox(groupbox_parameters);
        combobox_data_bits->setObjectName("combobox_data_bits");

        gridLayout_3->addWidget(combobox_data_bits, 1, 1, 1, 1);

        combobox_stop_bits = new QComboBox(groupbox_parameters);
        combobox_stop_bits->setObjectName("combobox_stop_bits");

        gridLayout_3->addWidget(combobox_stop_bits, 3, 1, 1, 1);

        label_parity = new QLabel(groupbox_parameters);
        label_parity->setObjectName("label_parity");

        gridLayout_3->addWidget(label_parity, 2, 0, 1, 1);

        label_baudrate = new QLabel(groupbox_parameters);
        label_baudrate->setObjectName("label_baudrate");

        gridLayout_3->addWidget(label_baudrate, 0, 0, 1, 1);

        combobox_parity = new QComboBox(groupbox_parameters);
        combobox_parity->setObjectName("combobox_parity");

        gridLayout_3->addWidget(combobox_parity, 2, 1, 1, 1);

        label_data_bits = new QLabel(groupbox_parameters);
        label_data_bits->setObjectName("label_data_bits");

        gridLayout_3->addWidget(label_data_bits, 1, 0, 1, 1);

        combobox_flow_control = new QComboBox(groupbox_parameters);
        combobox_flow_control->setObjectName("combobox_flow_control");

        gridLayout_3->addWidget(combobox_flow_control, 4, 1, 1, 1);

        label_flow_control = new QLabel(groupbox_parameters);
        label_flow_control->setObjectName("label_flow_control");

        gridLayout_3->addWidget(label_flow_control, 4, 0, 1, 1);

        label_stop_bits = new QLabel(groupbox_parameters);
        label_stop_bits->setObjectName("label_stop_bits");

        gridLayout_3->addWidget(label_stop_bits, 3, 0, 1, 1);

        combobox_baudrate = new QComboBox(groupbox_parameters);
        combobox_baudrate->setObjectName("combobox_baudrate");

        gridLayout_3->addWidget(combobox_baudrate, 0, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_3);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        spinBox_response_time = new QSpinBox(groupbox_parameters);
        spinBox_response_time->setObjectName("spinBox_response_time");
        spinBox_response_time->setMinimum(100);
        spinBox_response_time->setMaximum(3000);
        spinBox_response_time->setValue(500);

        gridLayout_4->addWidget(spinBox_response_time, 0, 1, 1, 1);

        label_response_time = new QLabel(groupbox_parameters);
        label_response_time->setObjectName("label_response_time");

        gridLayout_4->addWidget(label_response_time, 0, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_4);


        horizontalLayout_2->addWidget(groupbox_parameters);


        verticalLayout->addLayout(horizontalLayout_2);

        buttonBox = new QDialogButtonBox(SerialSettingDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SerialSettingDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SerialSettingDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SerialSettingDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SerialSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *SerialSettingDialog)
    {
        SerialSettingDialog->setWindowTitle(QCoreApplication::translate("SerialSettingDialog", "Dialog", nullptr));
        groupbox_port_select->setTitle(QCoreApplication::translate("SerialSettingDialog", "Select Serial Port", nullptr));
        label_manufacturer->setText(QCoreApplication::translate("SerialSettingDialog", "Manufacturer:", nullptr));
        label_description->setText(QCoreApplication::translate("SerialSettingDialog", "Description:", nullptr));
        label_vendor_id->setText(QCoreApplication::translate("SerialSettingDialog", "Vendor ID:", nullptr));
        label_product_id->setText(QCoreApplication::translate("SerialSettingDialog", "Product ID:", nullptr));
        label_location->setText(QCoreApplication::translate("SerialSettingDialog", "Location:", nullptr));
        label_serial_number->setText(QCoreApplication::translate("SerialSettingDialog", "Serial number:", nullptr));
        button_refresh->setText(QCoreApplication::translate("SerialSettingDialog", "Refresh", nullptr));
        groupbox_parameters->setTitle(QCoreApplication::translate("SerialSettingDialog", "Serial Parameters", nullptr));
        label_parity->setText(QCoreApplication::translate("SerialSettingDialog", "Parity:", nullptr));
        label_baudrate->setText(QCoreApplication::translate("SerialSettingDialog", "BaudRate:", nullptr));
        label_data_bits->setText(QCoreApplication::translate("SerialSettingDialog", "Data bits:", nullptr));
        label_flow_control->setText(QCoreApplication::translate("SerialSettingDialog", "Flow control:", nullptr));
        label_stop_bits->setText(QCoreApplication::translate("SerialSettingDialog", "Stop bits:", nullptr));
        label_response_time->setText(QCoreApplication::translate("SerialSettingDialog", "Response timeout:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SerialSettingDialog: public Ui_SerialSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALSETTINGDIALOG_H
