#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExpValidator>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QRandomGenerator>
#include "QsLog/QsLog.h"

#define PWD_NUMBER "1234567890"
#define PWD_LOWER "abcdefghijklmnopqrstuvwxyz"
#define PWD_UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#define PWD_TYPE_NUM 0xF
#define PWD_TYPE_LOWER 0xF0
#define PWD_TYPE_UPPER 0xF00
#define PWD_TYPE_SPECIAL 0xF000
#define PWD_TYPE_CUSTOM 0xF0000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void generatePwd(bool num, bool lower, bool upper, QString special, QString custom, QString exclude, int minLen, int maxLen, int generateSize, bool noRepeat, bool disorder,QString savePath);
    QString delStr(QString str, QString exclude);
    QString delRepeatStr(QString str);

private slots:
    void on_generateBtn_clicked();

    void on_specialCbox_stateChanged(int arg1);

    void on_customCbox_stateChanged(int arg1);

    void on_excludeCbox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
//    QsLogging::Logger *log;
};
#endif // MAINWINDOW_H
