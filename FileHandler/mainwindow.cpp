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

    ui->filePathInp->installEventFilter(this);
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

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->filePathInp && event->type() == QEvent::DragEnter)
    {
        QDragEnterEvent* dragEvent = static_cast<QDragEnterEvent*>(event);
        QString filePath = dragEvent->mimeData()->urls().first().toLocalFile();

        QLOG_INFO() << "拖拽文件:" << filePath;

        ui->filePathInp->setText(filePath);
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_selectFileBtn_clicked()
{

}
