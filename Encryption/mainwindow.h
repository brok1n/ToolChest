#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //修改样式
    void setStyle(QString styleName);

    QByteArray getFileMd5(QString filePath);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private slots:
    void on_md5SelectFileBtn_clicked();

    void on_md5ExecBtn_clicked();

    void on_fileSelectFileBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
