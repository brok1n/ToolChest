#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showDateTimeLcd(QString str);
    //修改样式
    void setStyle(QString styleName);

private slots:
    void onTimeout();

    void on_stopBtn_clicked();

    void on_startBtn_clicked();

    void on_refreshBtn_clicked();


    void on_unixCbox_currentIndexChanged(int index);

    void on_unixToDateTimeBtn_clicked();

    void on_dateTimeToUnixBtn_clicked();

    void on_dateTimeUnixCbox_currentIndexChanged(int index);

    void on_ymdToUnixBtn_clicked();

    void on_ymdUnixCbox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QMap<QChar, QPixmap*> m_ImgMap;
    QTimer *m_Timer;
    QString m_LastTime;
};
#endif // MAINWINDOW_H
