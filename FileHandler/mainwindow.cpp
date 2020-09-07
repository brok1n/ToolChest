#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QMimeData>
#include "QsLog/QsLog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //默认样式
    this->setStyle("style");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStyle(QString styleName) {
    QFile file(":/css/" + styleName + ".css");
    file.open(QFile::ReadOnly);
    QString style = QLatin1String(file.readAll());
    file.close();
    this->setStyleSheet(style);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    int x = ui->filePathInp->pos().x();
    int y = ui->filePathInp->pos().y();
    int w = ui->filePathInp->width();
    int h = ui->filePathInp->height();

    int ex = event->pos().x();
    int ey = event->pos().y();

    if(ex >= x && ey >= y && ex <= (x+w) && ey <= (y+h))
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{

    QString filePath = event->mimeData()->urls().first().toLocalFile();

    QLOG_INFO() << "拖拽文件:" << filePath;

    ui->filePathInp->setText(filePath);

}

void MainWindow::on_selectFileBtn_clicked()
{

}
