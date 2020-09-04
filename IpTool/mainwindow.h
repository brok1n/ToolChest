#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getNetInterfaceTypeNameByType(int type);

public slots:
    void requestFinished(QNetworkReply *reply);
private:
    Ui::MainWindow *ui;
    QString ipv6;
};
#endif // MAINWINDOW_H
