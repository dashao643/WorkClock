#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSharedMemory sharedMem("WorkClock_20260215_Unique_ID");

    if (!sharedMem.create(1)) {
        QMessageBox::information(nullptr, "提示", "程序已在运行中！");

        return 0;
    }

    Widget w;
    w.show();

    return app.exec();
}
