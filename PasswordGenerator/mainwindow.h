#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExpValidator>
#include <QDebug>
#include "QsLog/QsLog.h"

#define PWD_NUMBER "1234567890"
#define PWD_LOWER "abcdefghijklmnopqrstuvwxyz"
#define PWD_UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QsLogging::Logger *log,QWidget *parent = nullptr);
    ~MainWindow();
    void generatePwd(bool num, bool lower, bool upper, QString special, QString custom, QString exclude, int minLen, int maxLen, int generateSize, QString savePath);
    void excludeStr(QString str, QString exclude);

private slots:
    void on_generateBtn_clicked();

    void on_specialCbox_stateChanged(int arg1);

    void on_customCbox_stateChanged(int arg1);

    void on_excludeCbox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QsLogging::Logger &log;
};
#endif // MAINWINDOW_H
