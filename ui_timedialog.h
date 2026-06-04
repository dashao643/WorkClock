/********************************************************************************
** Form generated from reading UI file 'timedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMEDIALOG_H
#define UI_TIMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TimeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QSpinBox *spinBox;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_OK;
    QPushButton *btn_cancel;

    void setupUi(QDialog *TimeDialog)
    {
        if (TimeDialog->objectName().isEmpty())
            TimeDialog->setObjectName("TimeDialog");
        TimeDialog->resize(208, 135);
        verticalLayout = new QVBoxLayout(TimeDialog);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(TimeDialog);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        widget = new QWidget(TimeDialog);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        spinBox = new QSpinBox(widget);
        spinBox->setObjectName("spinBox");
        spinBox->setMinimumSize(QSize(100, 25));
        spinBox->setMaximumSize(QSize(100, 16777215));
        spinBox->setAlignment(Qt::AlignmentFlag::AlignCenter);
        spinBox->setMinimum(-60);
        spinBox->setMaximum(60);

        horizontalLayout->addWidget(spinBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(TimeDialog);
        widget_2->setObjectName("widget_2");
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        btn_OK = new QPushButton(widget_2);
        btn_OK->setObjectName("btn_OK");

        horizontalLayout_2->addWidget(btn_OK);

        btn_cancel = new QPushButton(widget_2);
        btn_cancel->setObjectName("btn_cancel");

        horizontalLayout_2->addWidget(btn_cancel);


        verticalLayout->addWidget(widget_2);


        retranslateUi(TimeDialog);

        btn_OK->setDefault(true);


        QMetaObject::connectSlotsByName(TimeDialog);
    } // setupUi

    void retranslateUi(QDialog *TimeDialog)
    {
        TimeDialog->setWindowTitle(QCoreApplication::translate("TimeDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("TimeDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">\350\260\203\346\225\264\344\273\212\346\227\245\350\256\260\345\275\225\357\274\232</span></p></body></html>", nullptr));
        spinBox->setSuffix(QCoreApplication::translate("TimeDialog", "\345\210\206\351\222\237", nullptr));
        btn_OK->setText(QCoreApplication::translate("TimeDialog", "\347\241\256\345\256\232", nullptr));
        btn_cancel->setText(QCoreApplication::translate("TimeDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TimeDialog: public Ui_TimeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMEDIALOG_H
