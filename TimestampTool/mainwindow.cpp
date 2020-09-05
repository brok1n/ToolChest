#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QsLog/QsLog.h"

#include <QDateTime>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_LastTime("")
{
    ui->setupUi(this);

    //默认样式
    this->setStyle("style");

    //所有图片加载到内存 放到map里 '0' => 图片0  '1' => 图片1 ...
    for (int i = 0; i < 10; i ++ )
    {
        m_ImgMap.insert(QString::number(i).at(0), new QPixmap(QString(":/img/%1.png").arg(i)));
    }
    m_ImgMap.insert(':', new QPixmap(":/img/=.png"));  // :
    m_ImgMap.insert('-', new QPixmap(":/img/_.png"));  // -

   QLOG_INFO() << "图片个数:" << m_ImgMap.size();

   //当前时间
   QDateTime current_date_time =QDateTime::currentDateTime();
   QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
//   showDateTimeLcd("2020-09-05 14:33:56");
//   showDateTimeLcd("----:--:-- --:--:--");
   showDateTimeLcd(current_date);
   ui->dateTimeEdit->setText(current_date);

   //unix时间戳
   QString unix = QString::number(current_date_time.toTime_t());
   ui->currentUnixEdit->setText(unix);
    ui->unixEdit->setText(unix);

   //创建定时器 绑定事件到onTimeout函数
   m_Timer = new QTimer(this);
   connect(m_Timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

   //启动定时器 每1秒执行一次 onTimeout
   m_Timer->start(1000);
}

MainWindow::~MainWindow()
{
    m_Timer->stop();
    delete ui;
}


void MainWindow::showDateTimeLcd(QString str)
{
    if(str.length() < 19)
    {
        return;
    }

    //遍历字符串 取每个字符 根据字符 取出图片 设置到Label里
    for (int i = 0; i < 19; i ++ )
    {
        QChar ch = str.at(i);
        QPixmap *ch_img = m_ImgMap[ch];
        if(ch_img != nullptr)
        {
            ((QLabel*)ui->lcdLayout->itemAt(i)->widget())->setPixmap(*ch_img);
        }
        else
        {
            ((QLabel*)ui->lcdLayout->itemAt(i)->widget())->clear();
        }
    }
}

void MainWindow::onTimeout()
{
    //当前时间日期字符串
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    //显示当前时间日期LCD样式
    showDateTimeLcd(current_date);

    //当前unix时间戳字符串
    QString unix = QString::number(current_date_time.toTime_t());
    ui->currentUnixEdit->setText(unix);
}


void MainWindow::setStyle(QString styleName) {
    QFile file(":/css/" + styleName + ".css");
    file.open(QFile::ReadOnly);
    QString style = QLatin1String(file.readAll());
    file.close();
    this->setStyleSheet(style);
}

void MainWindow::on_stopBtn_clicked()
{
    if(m_Timer->isActive())
    {
        m_Timer->stop();
    }
}

void MainWindow::on_startBtn_clicked()
{
    if(!m_Timer->isActive())
    {
        m_Timer->start(1000);
    }
}


void MainWindow::on_refreshBtn_clicked()
{
    onTimeout();
}


void MainWindow::on_unixCbox_currentIndexChanged(int index)
{
    qint64 unix = ui->unixEdit->text().trimmed().toLongLong();
    if(index == 1)
    {
        unix /= 1000;
    }
    else if(index == 0 && unix > 9999999999)
    {
        unix /= 1000;
        ui->unixEdit->setText(QString::number(unix));
    }


    QDateTime dateTime = QDateTime::fromTime_t(unix);
    QString current_date = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    ui->unixDateTimeEdit->setText(current_date);

}

void MainWindow::on_unixToDateTimeBtn_clicked()
{
    on_unixCbox_currentIndexChanged(ui->unixCbox->currentIndex());
}

void MainWindow::on_dateTimeToUnixBtn_clicked()
{
    on_dateTimeUnixCbox_currentIndexChanged(ui->dateTimeUnixCbox->currentIndex());
}

void MainWindow::on_dateTimeUnixCbox_currentIndexChanged(int index)
{
    QString dateTimeStr = ui->dateTimeEdit->text().trimmed();
    QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm:ss");
    QString unixStr = QString::number(dateTime.toTime_t());
    if(index == 1)
    {
        unixStr.append("000");
    }
    ui->dateTimeUnixEdit->setText(unixStr);
}

void MainWindow::on_ymdToUnixBtn_clicked()
{
    on_ymdUnixCbox_currentIndexChanged(ui->ymdUnixCbox->currentIndex());
}

void MainWindow::on_ymdUnixCbox_currentIndexChanged(int index)
{
    QString year = ui->yearEdit->text().trimmed();
    QString month = ui->monthEdit->text().trimmed();
    QString day = ui->dayEdit->text().trimmed();

    QString hour = ui->hourEdit->text().trimmed();
    QString minute = ui->minuteEdit->text().trimmed();
    QString seconds = ui->secondsEdit->text().trimmed();

    if(month == "")
    {
        month = "01";
    }
    else if(month.length() == 1)
    {
        month = month.insert(0, '0');
    }

    if(day == "")
    {
        day = "01";
    }
    else if(day.length() == 1)
    {
        day = day.insert(0, '0');
    }

    if(hour == "")
    {
        hour = "00";
    }
    else if(hour.length() == 1)
    {
        hour = hour.insert(0, '0');
    }

    if(minute == "")
    {
        minute = "00";
    }
    else if(minute.length() == 1)
    {
        minute = minute.insert(0, '0');
    }

    if(seconds == "")
    {
        seconds = "00";
    }
    else if(seconds.length() == 1)
    {
        seconds = seconds.insert(0, '0');
    }

    ui->monthEdit->setText(month);
    ui->dayEdit->setText(day);
    ui->hourEdit->setText(hour);
    ui->minuteEdit->setText(minute);
    ui->secondsEdit->setText(seconds);

    QString dateTimeStr = year.append("-").append(month).append("-").append(day).append(" ").append(hour).append(":").append(minute).append(":").append(seconds);
    QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm:ss");
    QString unixStr = QString::number(dateTime.toTime_t());
    if(index == 1)
    {
        unixStr.append("000");
    }
    ui->ymdUnixEdit->setText(unixStr);
}
