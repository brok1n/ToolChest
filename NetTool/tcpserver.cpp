#include "tcpserver.h"
#include "QsLog/QsLog.h"

TcpServer::TcpServer(QObject *parent)
    : QObject(parent)
    , ip("")
    , port(9988)
    , running(false)
{

}

TcpServer::~TcpServer()
{
//    readThread.quit();
//    writeThread.quit();

//    readThread.wait();
//    writeThread.wait();
}



bool TcpServer::start(QString ip, int port)
{
    this->ip = ip;
    this->port = port;

    mQtcpServer = new QTcpServer(this);
    mQtcpServer->setMaxPendingConnections(1);

    connect(mQtcpServer, &QTcpServer::newConnection, this, &TcpServer::onNewConnection);
    connect(mQtcpServer, &QTcpServer::acceptError, this, &TcpServer::onAcceptError);

    if(!mQtcpServer->listen(QHostAddress(this->ip), this->port))
    {
        this->running = false;
        return false;
    }
    this->running = true;
    return true;
}

bool TcpServer::stop()
{
    this->mQtcpServer->close();
    this->running = this->mQtcpServer->isListening();
    if (this->running)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool TcpServer::isRunning()
{
    return running;
}

void TcpServer::sendData(QByteArray qb)
{
    mSocket->write(qb);
    mSocket->flush();
}

void TcpServer::onReadyRead()
{
    QByteArray byteArr = mSocket->readAll();
    emit onRecvDataSignal(byteArr);
}

void TcpServer::onNewConnection()
{
    QLOG_INFO() << "on New connection";

    mSocket = mQtcpServer->nextPendingConnection();

    connect(mSocket, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
}

void TcpServer::onAcceptError(QAbstractSocket::SocketError socketError)
{
    QLOG_INFO() << "on accept error";
}
