#include "tcpclient.h"
#include "QsLog/QsLog.h"
#include <QHostAddress>

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}

TcpClient::~TcpClient()
{

}

bool TcpClient::start(QString ip, int port)
{
    this->ip = ip;
    this->port = port;

    mSocket = new QTcpSocket(this);

    connect(mSocket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);

    mSocket->connectToHost(this->ip, this->port);
    mSocket->waitForConnected(3000);
    running = true;
    return running;
}

bool TcpClient::stop()
{
    running = false;
    mSocket->close();
    mSocket->waitForDisconnected(3000);
    return true;
}

bool TcpClient::isRunning()
{
    return running;
}

void TcpClient::sendData(QByteArray qb)
{
    mSocket->write(qb);
    mSocket->flush();
}

void TcpClient::onReadyRead()
{
    QByteArray byteArr = mSocket->readAll();
    emit onRecvDataSignal(byteArr);
}
