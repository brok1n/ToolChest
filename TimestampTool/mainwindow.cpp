#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QsLog/QsLog.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_LastTime("")
{
    ui->setupUi(this);

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
   ui->ymdEdit->setText(current_date);

   //unix时间戳
   QString unix = QString::number(current_date_time.toTime_t());
   ui->currentUnixTb->setText(unix);
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
    ui->currentUnixTb->setText(unix);
}

