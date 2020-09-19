#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>

//class TcpServer;

//class TcpServerReadWorker : public QObject
//{
//    Q_OBJECT
//public:
//    TcpServerReadWorker(QTcpServer *server, QObject *parent = nullptr);
//    ~TcpServerReadWorker();
//    //停止
//    void stop();

//    //启动读取线程
//    void start(TcpServer *master);

//public slots:
//    //线程主循环
//    void run(QTcpSocket *socket);

//signals:
//    void onRecvData(QByteArray qb);
//private:
//    QThread readThread;
//    QTcpServer *server;
//    QTcpSocket *mSocket;

//    bool running;
//};


class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
    bool start(QString ip, int port);
    bool stop();

    bool isRunning();

    void sendData(QByteArray qb);

signals:
    void startTcpServerReader(QTcpSocket *socket);

    void onRecvDataSignal(QByteArray qb);

public slots:
    void onReadyRead();

    void onNewConnection();
    void onAcceptError(QAbstractSocket::SocketError socketError);

private:
    QString ip;
    int port;
    bool running;
    QTcpServer *mQtcpServer;

    QTcpSocket *mSocket;
};

#endif // TCPSERVER_H
