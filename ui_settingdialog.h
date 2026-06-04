/********************************************************************************
** Form generated from reading UI file 'settingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGDIALOG_H
#define UI_SETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *btn_addTarget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *SettingDialog)
    {
        if (SettingDialog->objectName().isEmpty())
            SettingDialog->setObjectName("SettingDialog");
        SettingDialog->resize(320, 300);
        gridLayout = new QGridLayout(SettingDialog);
        gridLayout->setObjectName("gridLayout");
        btn_addTarget = new QPushButton(SettingDialog);
        btn_addTarget->setObjectName("btn_addTarget");
        btn_addTarget->setMinimumSize(QSize(90, 0));
        btn_addTarget->setMaximumSize(QSize(90, 16777215));

        gridLayout->addWidget(btn_addTarget, 0, 1, 1, 1);

        widget_2 = new QWidget(SettingDialog);
        widget_2->setObjectName("widget_2");
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, 12, -1, -1);
        listWidget = new QListWidget(widget_2);
        listWidget->setObjectName("listWidget");

        horizontalLayout_2->addWidget(listWidget);


        gridLayout->addWidget(widget_2, 1, 0, 1, 2);

        widget = new QWidget(SettingDialog);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(widget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        doubleSpinBox = new QDoubleSpinBox(widget);
        doubleSpinBox->setObjectName("doubleSpinBox");
        doubleSpinBox->setMinimumSize(QSize(80, 25));
        doubleSpinBox->setMaximumSize(QSize(80, 16777215));
        doubleSpinBox->setDecimals(1);
        doubleSpinBox->setMinimum(0.100000000000000);
        doubleSpinBox->setMaximum(24.000000000000000);
        doubleSpinBox->setValue(2.000000000000000);

        horizontalLayout->addWidget(doubleSpinBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(SettingDialog);

        QMetaObject::connectSlotsByName(SettingDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingDialog)
    {
        SettingDialog->setWindowTitle(QCoreApplication::translate("SettingDialog", "Dialog", nullptr));
        btn_addTarget->setText(QCoreApplication::translate("SettingDialog", "\346\267\273\345\212\240\346\257\217\346\227\245\347\233\256\346\240\207", nullptr));
        label->setText(QCoreApplication::translate("SettingDialog", "\346\257\217\346\227\245\347\233\256\346\240\207\346\227\266\351\225\277\357\274\232", nullptr));
        doubleSpinBox->setSuffix(QCoreApplication::translate("SettingDialog", "\345\260\217\346\227\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingDialog: public Ui_SettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGDIALOG_H
