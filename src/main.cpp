#include "widget.h"

#include <QApplication>

#include <QFile>
#include <QLocalSocket>
#include <QLocalServer>

const QString SERVER_NAME = "dashao-work-clock-1.3.3";
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

    QObject::connect(&server, &QLocalServer::newConnection, [&server, &w]() {
        QLocalSocket *clientSocket = server.nextPendingConnection();
        if (!clientSocket) return;

        QObject::connect(clientSocket, &QLocalSocket::readyRead, [clientSocket, &w]() {
            QByteArray data = clientSocket->readAll();
            qInfo() << "收到消息:" << data;

            if (data.trimmed() == "show") {
                w.raise();
                w.showNormal();
                w.activateWindow();
            }
        });

        QObject::connect(clientSocket, &QLocalSocket::disconnected, clientSocket, &QLocalSocket::deleteLater);
    });

    return a.exec();
}
