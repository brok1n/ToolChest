#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCryptographicHash>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
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

void MainWindow::on_md5SelectFileBtn_clicked()
{
    //文件夹路径
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择要计算MD5的文件"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    if (!filePath.isEmpty())
    {
        ui->md5FileInp->setText(filePath);
        QLOG_INFO() << "选择了文件:" << filePath;
    }

}

void MainWindow::on_md5ExecBtn_clicked()
{
    QString filePath = ui->md5FileInp->text().trimmed();
    QLOG_INFO() << "将要计算MD5的文件:" << filePath;

    QString sourceStr = ui->md5SourceTxtEdit->toPlainText();
    QLOG_INFO() << "将要计算MD5的字符串:" << sourceStr;

    QLOG_INFO() << "开始计算MD5";

    if(!filePath.isEmpty() && QFile::exists(filePath) || !sourceStr.isEmpty())
    {
        ui->md5OutBw->clear();
    }

    if(!filePath.isEmpty() && QFile::exists(filePath))
    {
        QString fileMd5 = QString(getFileMd5(filePath).toHex());
        QLOG_INFO() << "文件MD5小写32位:" << fileMd5;

        QString fileMd5Upper = fileMd5.toUpper();
        QLOG_INFO() << "文件MD5大写32位:" << fileMd5Upper;

        QString fileMd5Lower16 = fileMd5.mid(8, 16);
        QLOG_INFO() << "文件MD5小写16位:" << fileMd5Lower16;

        QString fileMd5Upper16 = fileMd5Upper.mid(8, 16);
        QLOG_INFO() << "文件MD5大写16位:" << fileMd5Upper16;

        ui->md5OutBw->append(QString("文件MD5小写32位: %1").arg(fileMd5));
        ui->md5OutBw->append(QString("文件MD5大写32位: %1").arg(fileMd5Upper));
        ui->md5OutBw->append(QString("文件MD5小写16位: %1").arg(fileMd5Lower16));
        ui->md5OutBw->append(QString("文件MD5大写16位: %1").arg(fileMd5Upper16));

        ui->md5OutBw->append("");
    }



    if(!sourceStr.isEmpty())
    {
        QCryptographicHash md(QCryptographicHash::Md5);

        md.addData(sourceStr.toUtf8());

        QString strMd5 = QString(md.result().toHex());
        QLOG_INFO() << "字符串MD5小写32位:" << strMd5;

        QString strMd5Upper = strMd5.toUpper();
        QLOG_INFO() << "字符串MD5大写32位:" << strMd5Upper;

        QString strMd5Lower16 = strMd5.mid(8, 16);
        QLOG_INFO() << "字符串MD5小写16位:" << strMd5Lower16;

        QString strMd5Upper16 = strMd5Upper.mid(8, 16);
        QLOG_INFO() << "字符串MD5大写16位:" << strMd5Upper16;


        ui->md5OutBw->append(QString("字符串MD5小写32位: %1").arg(strMd5));
        ui->md5OutBw->append(QString("字符串MD5大写32位: %1").arg(strMd5Upper));
        ui->md5OutBw->append(QString("字符串MD5小写16位: %1").arg(strMd5Lower16));
        ui->md5OutBw->append(QString("字符串MD5大写16位: %1").arg(strMd5Upper16));

        ui->md5OutBw->append("");

    }
}

/*
*   获取文件md5值
*/
QByteArray MainWindow::getFileMd5(QString filePath)
{
    QFile localFile(filePath);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    localFile.close();
    QByteArray md5 = ch.result();

    return md5;
}

void MainWindow::on_fileSelectFileBtn_clicked()
{
    //文件夹路径
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择要处理的文件"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    if (!filePath.isEmpty())
    {
        ui->fileInp->setText(filePath);
        QLOG_INFO() << "选择了文件:" << filePath;
    }

}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    int x = ui->md5FileInp->pos().x();
    int y = ui->md5FileInp->pos().y() + 40;
    int w = ui->md5FileInp->width();
    int h = ui->md5FileInp->height();

    int ex = event->pos().x();
    int ey = event->pos().y();

    QLOG_INFO() << "x:" << x << " y:" << y << " w:" << w << " h:" << h << " ex:" << ex << " ey:" << ey;

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

    switch (ui->tabWidget->currentIndex()) {
    case 0:
        ui->md5FileInp->setText(filePath);
        break;
    case 1:
        ui->fileInp->setText(filePath);
        break;
    }
}

