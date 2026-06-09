#include "widget.h"

#include <QApplication>

#include <QSharedMemory>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":/style.qss");

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    QSharedMemory sharedMem("WorkClock_20260609_Unique_ID");

    if (!sharedMem.create(1)) {
        QMessageBox::information(nullptr, "提示", "程序已在运行中！");
        return 0;
    }

    Widget w;
    w.show();
    return a.exec();
}
