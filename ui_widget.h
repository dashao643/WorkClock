/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout_4;
    QTabWidget *tabWidget;
    QWidget *tabTime;
    QVBoxLayout *verticalLayout;
    QLabel *label_date;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_clock;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_startStop;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btn_save;
    QSpacerItem *horizontalSpacer_3;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_3;
    QGridLayout *gridLayout;
    QLabel *label_today;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_change;
    QPushButton *btn_reset;
    QPushButton *btn_setting;
    QWidget *tabRecord;
    QGridLayout *gridLayout_3;
    QTableView *tableView;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QLabel *label_pastDay7;
    QLabel *label_pastDay30;
    QLabel *label_latest;
    QWidget *tabTool;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *lineEditToUpper;
    QPushButton *btnToUpper;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *lineEditRemove;
    QPushButton *btnRemove;
    QTextEdit *textEdit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(400, 480);
        horizontalLayout_4 = new QHBoxLayout(Widget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName("tabWidget");
        tabTime = new QWidget();
        tabTime->setObjectName("tabTime");
        verticalLayout = new QVBoxLayout(tabTime);
        verticalLayout->setObjectName("verticalLayout");
        label_date = new QLabel(tabTime);
        label_date->setObjectName("label_date");
        label_date->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label_date);

        widget = new QWidget(tabTime);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        label_clock = new QLabel(widget);
        label_clock->setObjectName("label_clock");
        label_clock->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label_clock);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(tabTime);
        widget_2->setObjectName("widget_2");
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btn_startStop = new QPushButton(widget_2);
        btn_startStop->setObjectName("btn_startStop");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_startStop->sizePolicy().hasHeightForWidth());
        btn_startStop->setSizePolicy(sizePolicy);
        btn_startStop->setMinimumSize(QSize(50, 50));
        btn_startStop->setMaximumSize(QSize(50, 50));

        horizontalLayout_2->addWidget(btn_startStop);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        btn_save = new QPushButton(widget_2);
        btn_save->setObjectName("btn_save");
        sizePolicy.setHeightForWidth(btn_save->sizePolicy().hasHeightForWidth());
        btn_save->setSizePolicy(sizePolicy);
        btn_save->setMinimumSize(QSize(50, 50));
        btn_save->setMaximumSize(QSize(50, 50));

        horizontalLayout_2->addWidget(btn_save);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(widget_2);

        widget_5 = new QWidget(tabTime);
        widget_5->setObjectName("widget_5");
        widget_5->setMinimumSize(QSize(0, 100));
        widget_5->setMaximumSize(QSize(16777215, 100));
        horizontalLayout_5 = new QHBoxLayout(widget_5);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        widget_3 = new QWidget(widget_5);
        widget_3->setObjectName("widget_3");
        gridLayout = new QGridLayout(widget_3);
        gridLayout->setObjectName("gridLayout");
        label_today = new QLabel(widget_3);
        label_today->setObjectName("label_today");

        gridLayout->addWidget(label_today, 0, 0, 1, 1);


        horizontalLayout_5->addWidget(widget_3);

        widget_4 = new QWidget(widget_5);
        widget_4->setObjectName("widget_4");
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        btn_change = new QPushButton(widget_4);
        btn_change->setObjectName("btn_change");
        sizePolicy.setHeightForWidth(btn_change->sizePolicy().hasHeightForWidth());
        btn_change->setSizePolicy(sizePolicy);
        btn_change->setMinimumSize(QSize(40, 40));
        btn_change->setMaximumSize(QSize(40, 40));

        horizontalLayout_3->addWidget(btn_change);

        btn_reset = new QPushButton(widget_4);
        btn_reset->setObjectName("btn_reset");
        sizePolicy.setHeightForWidth(btn_reset->sizePolicy().hasHeightForWidth());
        btn_reset->setSizePolicy(sizePolicy);
        btn_reset->setMinimumSize(QSize(40, 40));
        btn_reset->setMaximumSize(QSize(40, 40));

        horizontalLayout_3->addWidget(btn_reset);

        btn_setting = new QPushButton(widget_4);
        btn_setting->setObjectName("btn_setting");
        sizePolicy.setHeightForWidth(btn_setting->sizePolicy().hasHeightForWidth());
        btn_setting->setSizePolicy(sizePolicy);
        btn_setting->setMinimumSize(QSize(40, 40));
        btn_setting->setMaximumSize(QSize(40, 40));

        horizontalLayout_3->addWidget(btn_setting);


        horizontalLayout_5->addWidget(widget_4);


        verticalLayout->addWidget(widget_5);

        tabWidget->addTab(tabTime, QString());
        tabRecord = new QWidget();
        tabRecord->setObjectName("tabRecord");
        gridLayout_3 = new QGridLayout(tabRecord);
        gridLayout_3->setObjectName("gridLayout_3");
        tableView = new QTableView(tabRecord);
        tableView->setObjectName("tableView");

        gridLayout_3->addWidget(tableView, 0, 0, 1, 1);

        tabWidget->addTab(tabRecord, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        label_pastDay7 = new QLabel(tab);
        label_pastDay7->setObjectName("label_pastDay7");

        verticalLayout_2->addWidget(label_pastDay7);

        label_pastDay30 = new QLabel(tab);
        label_pastDay30->setObjectName("label_pastDay30");

        verticalLayout_2->addWidget(label_pastDay30);

        label_latest = new QLabel(tab);
        label_latest->setObjectName("label_latest");

        verticalLayout_2->addWidget(label_latest);

        tabWidget->addTab(tab, QString());
        tabTool = new QWidget();
        tabTool->setObjectName("tabTool");
        verticalLayout_3 = new QVBoxLayout(tabTool);
        verticalLayout_3->setObjectName("verticalLayout_3");
        groupBox = new QGroupBox(tabTool);
        groupBox->setObjectName("groupBox");
        horizontalLayout_6 = new QHBoxLayout(groupBox);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(3, 0, 3, 0);
        lineEditToUpper = new QLineEdit(groupBox);
        lineEditToUpper->setObjectName("lineEditToUpper");

        horizontalLayout_6->addWidget(lineEditToUpper);

        btnToUpper = new QPushButton(groupBox);
        btnToUpper->setObjectName("btnToUpper");

        horizontalLayout_6->addWidget(btnToUpper);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tabTool);
        groupBox_2->setObjectName("groupBox_2");
        horizontalLayout_7 = new QHBoxLayout(groupBox_2);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(3, 0, 3, 0);
        lineEditRemove = new QLineEdit(groupBox_2);
        lineEditRemove->setObjectName("lineEditRemove");

        horizontalLayout_7->addWidget(lineEditRemove);

        btnRemove = new QPushButton(groupBox_2);
        btnRemove->setObjectName("btnRemove");

        horizontalLayout_7->addWidget(btnRemove);


        verticalLayout_3->addWidget(groupBox_2);

        textEdit = new QTextEdit(tabTool);
        textEdit->setObjectName("textEdit");

        verticalLayout_3->addWidget(textEdit);

        tabWidget->addTab(tabTool, QString());

        horizontalLayout_4->addWidget(tabWidget);


        retranslateUi(Widget);
        QObject::connect(lineEditToUpper, &QLineEdit::returnPressed, btnToUpper, qOverload<>(&QPushButton::click));
        QObject::connect(lineEditRemove, &QLineEdit::returnPressed, btnRemove, qOverload<>(&QPushButton::click));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_date->setText(QCoreApplication::translate("Widget", "<html><head/><body><p><span style=\" font-size:16pt; font-weight:700;\">XXXX\345\271\264-XX\346\234\210-XX\346\227\245</span></p></body></html>", nullptr));
        label_clock->setText(QCoreApplication::translate("Widget", "<html><head/><body><p><span style=\" font-size:36pt;\">00:00:00</span></p></body></html>", nullptr));
        btn_startStop->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
        btn_save->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230", nullptr));
        label_today->setText(QCoreApplication::translate("Widget", "<html><head/><body><p><span style=\" font-size:12pt;\">\345\267\262\345\267\245\344\275\234\346\227\266\351\225\277\357\274\2320\345\260\217\346\227\2660\345\210\206\351\222\237</span></p></body></html>", nullptr));
        btn_change->setText(QCoreApplication::translate("Widget", "\350\260\203\346\225\264", nullptr));
        btn_reset->setText(QCoreApplication::translate("Widget", "\351\207\215\347\275\256", nullptr));
        btn_setting->setText(QCoreApplication::translate("Widget", "\350\256\276\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTime), QCoreApplication::translate("Widget", "\346\227\266\351\227\264\346\230\276\347\244\272", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRecord), QCoreApplication::translate("Widget", "\350\277\207\345\276\200\350\256\260\345\275\225", nullptr));
        label_pastDay7->setText(QCoreApplication::translate("Widget", "\350\277\207\345\216\273\344\270\203\345\244\251\345\271\263\345\235\207\357\274\232", nullptr));
        label_pastDay30->setText(QCoreApplication::translate("Widget", "\350\277\207\345\216\273\344\270\211\345\215\201\345\244\251\345\271\263\345\235\207\357\274\232", nullptr));
        label_latest->setText(QCoreApplication::translate("Widget", "\344\270\212\346\254\241\344\277\235\345\255\230\346\227\266\351\227\264\357\274\232", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Widget", "\345\233\276\350\241\250", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Widget", "\345\260\217\345\206\231\350\275\254\345\244\247\345\206\231", nullptr));
        btnToUpper->setText(QCoreApplication::translate("Widget", "\350\275\254\346\215\242", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Widget", "\345\216\273\351\231\244\347\251\272\346\240\274", nullptr));
        btnRemove->setText(QCoreApplication::translate("Widget", "\350\275\254\346\215\242", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTool), QCoreApplication::translate("Widget", "\345\267\245\345\205\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
