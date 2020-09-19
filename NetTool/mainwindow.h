#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpclient.h"
#include "tcpserver.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getHostIpAddress();

private slots:
    void on_tcpServStartBtn_clicked();

    void on_tcpClientStartBtn_clicked();

    void on_tcpServSendTxtBtn_clicked();

    void on_tcpServSendFileBtn_clicked();

    void on_tcpClientSendTxtBtn_clicked();

    void on_tcpClientSendFileBtn_clicked();

    void onTcpServerRecvData(QByteArray qb);
    void onTcpClientRecvData(QByteArray qb);

private:
    Ui::MainWindow *ui;
    QString ip;
    TcpServer *mTcpServer;
    TcpClient *mTcpClient;
};
#endif // MAINWINDOW_H
