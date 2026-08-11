#include "widget.h"

#include <QApplication>

#include <QFile>
#include <QLocalSocket>
#include <QLocalServer>

const QString SERVER_NAME = "dashao-work-clock";
constexpr int BLOCK_WAIT_MS = 100;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLocalSocket socket;
    socket.connectToServer(SERVER_NAME);
    if (socket.waitForConnected(BLOCK_WAIT_MS)) {
        socket.write("show");
        socket.flush();
        socket.waitForBytesWritten();
        socket.disconnectFromServer();
        return 0;
    }
    QLocalServer server;
    QLocalServer::removeServer(SERVER_NAME);
    if (!server.listen(SERVER_NAME)) {
        qInfo() << "服务器监听失败";
        return 1;
    }

    QFile styleFile(":/style.qss");

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    Widget w;
    w.show();
    
    return a.exec();
}
