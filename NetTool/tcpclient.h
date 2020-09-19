#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();
    bool start(QString ip, int port);
    bool stop();

    bool isRunning();

    void sendData(QByteArray qb);

signals:
    void onRecvDataSignal(QByteArray qb);

public slots:
    void onReadyRead();

private:
    QString ip;
    int port;

    bool running;

    QTcpSocket *mSocket;
};

#endif // TCPCLIENT_H
