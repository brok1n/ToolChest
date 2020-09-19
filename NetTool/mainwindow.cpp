#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QsLog/QsLog.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ip("")
{
    ui->setupUi(this);

    ip = getHostIpAddress();

    ui->tcpServIpCbox->addItem(ip);

    mTcpServer = new TcpServer();
    mTcpClient = new TcpClient();

    connect(mTcpServer, &TcpServer::onRecvDataSignal, this,&MainWindow::onTcpServerRecvData);
    connect(mTcpClient, &TcpClient::onRecvDataSignal, this,&MainWindow::onTcpClientRecvData);

    ui->tcpServStatusLbl->setText("TCP服务器未运行");
    ui->tcpServStatusLbl->setStyleSheet("color:black;");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tcpServStartBtn_clicked()
{
    if(!mTcpServer->isRunning())
    {
        QLOG_INFO() << "当前TCP服务器没有运行,开始启动TCP服务器!";
        QString tcpServerIp = ui->tcpServIpCbox->currentText();
        int port = ui->tcpServPortEdit->text().toInt();

        QLOG_INFO() << " tcp server ip:" << tcpServerIp << " port:" << port;

        bool status = mTcpServer->start(tcpServerIp, port);
        if(status)
        {
            ui->tcpServStatusLbl->setText(QString("TCP服务器 %1:%2 正在运行").arg(tcpServerIp).arg(port));
            ui->tcpServStatusLbl->setStyleSheet("color:green;");
            ui->tcpServStartBtn->setText("停止");
            QLOG_INFO() << "TCP服务器启动成功！";
        }
        else
        {
            ui->tcpServStatusLbl->setText(QString("TCP服务器 %1:%2 启动失败！").arg(tcpServerIp).arg(port));
            ui->tcpServStatusLbl->setStyleSheet("color:red;");
            QLOG_INFO() << "TCP服务器启动失败！";
        }
    }
    else
    {
        QLOG_INFO() << "当前TCP服务器正在运行，开始停止TCP服务器!";
        bool status = mTcpServer->stop();
        if(status)
        {
            ui->tcpServStatusLbl->setText("TCP服务器已停止");
            ui->tcpServStatusLbl->setStyleSheet("color:black;");
            ui->tcpServStartBtn->setText("启动");
            QLOG_INFO() << "TCP服务器停止成功!";
        }
        else
        {
            QLOG_INFO() << "TCP服务器停止失败！";
        }
        ui->tcpServRecvTxtEdit->clear();
    }
}

void MainWindow::on_tcpClientStartBtn_clicked()
{
    if(!mTcpClient->isRunning())
    {
        QLOG_INFO() << "当前客户端没有连接,开始连接TCP服务器!";
        QString tcpServerIp = ui->tcpClientIpEdit->text();
        int port = ui->tcpClientPortEdit->text().toInt();

        QLOG_INFO() << " tcp socket ip:" << tcpServerIp << " port:" << port;

        bool status = mTcpClient->start(tcpServerIp, port);
        if(status)
        {
            ui->tcpClientStatusLbl->setText(QString("连接到 %1:%2 TCP服务器").arg(tcpServerIp).arg(port));
            ui->tcpClientStatusLbl->setStyleSheet("color:green;");
            ui->tcpClientStartBtn->setText("断开连接");
            QLOG_INFO() << "TCP服务器连接成功！";
        }
        else
        {
            ui->tcpClientStatusLbl->setText(QString("连接到 %1:%2 TCP服务器失败！").arg(tcpServerIp).arg(port));
            ui->tcpClientStatusLbl->setStyleSheet("color:red;");
            QLOG_INFO() << "TCP服务器连接失败！";
        }
    }
    else
    {
        QLOG_INFO() << "当前已连接到TCP服务器，开始断开连接!";
        bool status = mTcpClient->stop();
        if(status)
        {
            ui->tcpClientStatusLbl->setText("TCP服务器已断开连接!");
            ui->tcpClientStatusLbl->setStyleSheet("color:black;");
            ui->tcpClientStartBtn->setText("连接");
            QLOG_INFO() << "TCP服务器断开连接成功!";
        }
        else
        {
            QLOG_INFO() << "TCP服务器断开连接失败！";
        }
        ui->tcpClientRecvTxtEdit->clear();
    }
}

void MainWindow::on_tcpServSendTxtBtn_clicked()
{
    QString str = ui->tcpServSendTxtEdit->toPlainText();
    mTcpServer->sendData(str.toUtf8());
}

void MainWindow::on_tcpServSendFileBtn_clicked()
{

}

void MainWindow::on_tcpClientSendTxtBtn_clicked()
{
    QString str = ui->tcpClientSendTxtEdit->toPlainText();
    mTcpClient->sendData(str.toUtf8());
}

void MainWindow::on_tcpClientSendFileBtn_clicked()
{

}

void MainWindow::onTcpServerRecvData(QByteArray qb)
{
    ui->tcpServRecvTxtEdit->insertPlainText(QString().prepend(qb));
}

void MainWindow::onTcpClientRecvData(QByteArray qb)
{
    ui->tcpClientRecvTxtEdit->insertPlainText(QString().prepend(qb));
}

QString MainWindow::getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return strIpAddress;
}
