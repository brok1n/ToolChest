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

private slots:
    void onTimeout();

private:
    Ui::MainWindow *ui;
    QMap<QChar, QPixmap*> m_ImgMap;
    QTimer *m_Timer;
    QString m_LastTime;
};
#endif // MAINWINDOW_H
