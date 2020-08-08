#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardPaths>



MainWindow::MainWindow(QsLogging::Logger *log, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->log = log;

    //密码个数输入框只允许输入数字并且长度最大为5
    QRegExp sizeRe("[0-9]{1,5}");
    ui->generateSizeEdt->setValidator(new QRegExpValidator(sizeRe, this));

    //密码最长最短
    ui->pwdLenMaxEdt->setValidator(new QRegExpValidator(sizeRe, this));
    ui->pwdLenMinEdt->setValidator(new QRegExpValidator(sizeRe, this));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generatePwd(bool num, bool lower, bool upper, QString special, QString custom, QString exclude, int minLen, int maxLen, int generateSize, QString savePath)
{

}



void MainWindow::excludeStr(QString str, QString exclude)
{
    qDebug() << "exclude before:" << str;
    for(int i = 0; i < exclude.length(); i ++)
    {
        QChar ch = exclude.at(i);
        str.replace(ch, "");
    }
    qDebug() << "exclude after:" << str;
}

void MainWindow::on_generateBtn_clicked()
{
    //要生成多少个密码
    int generateCount = this->ui->generateSizeEdt->text().toInt();
    //如果个数输入框是空 就设置成1  最少生成1个密码
    generateCount = qMax(1, generateCount);
    this->ui->generateSizeEdt->setText(QString("%1").arg(generateCount));
    qDebug() << "size:" << generateCount;

    bool saveToDesktop = this->ui->saveToDesktopCbox->isChecked();
    bool resetResult = this->ui->resetResultCbox->isChecked();
    qDebug() << "保存到桌面:" << saveToDesktop;
    qDebug() << "重置输出框:" << resetResult;

    //密码最短最长长度
    int pwdLenMin = this->ui->pwdLenMinEdt->text().toInt();
    pwdLenMin = qMax(1, pwdLenMin);
    int pwdLenMax = this->ui->pwdLenMaxEdt->text().toInt();
    pwdLenMax = qMax(1, pwdLenMax);
    qDebug() << "密码长度 最短:" << pwdLenMin << " 最长:" << pwdLenMax;

    //排除字符串
    QString excludeStr = "";
    if(this->ui->excludeCbox->isChecked())
    {
        excludeStr = this->ui->excludeEdt->text().trimmed();
    }

    //自定义字符
    QString customStr = "";
    if(this->ui->customCbox->isChecked())
    {
        customStr = this->ui->customEdt->text().trimmed();
    }

    //特殊字符
    QString specialStr = "";
    if(this->ui->specialCbox->isChecked())
    {
        specialStr = this->ui->specialEdt->text().trimmed();
    }

    //大写字母
    QString upperStr = "";
    if(this->ui->upperCbox->isChecked())
    {
        upperStr = PWD_UPPER;
    }

    //小写字母
    QString lowerStr = "";
    if(this->ui->lowerCbox->isChecked())
    {
        lowerStr = PWD_LOWER;
    }

    //数字
    QString numberStr = "";
    if(this->ui->numCbox->isChecked())
    {
        numberStr = PWD_NUMBER;
    }


    //是否清空结果
    if(resetResult)
    {
        this->ui->resultEdt->clear();
    }

    //是否存储到桌面
    QString savePath = "";
    if(saveToDesktop)
    {
        savePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    //密码字符串
    QString pwdCandidates = numberStr + lowerStr + upperStr + specialStr + customStr;

    //生成密码
//    generatePwd(pwdCandidates, excludeStr, pwdLenMin, pwdLenMax, generateCount, savePath);

}

void MainWindow::on_specialCbox_stateChanged(int checked)
{
    if(checked)
    {
        this->ui->specialEdt->setEnabled(true);
    }
    else
    {
        this->ui->specialEdt->setEnabled(false);
    }
}

void MainWindow::on_customCbox_stateChanged(int checked)
{
    if(checked)
    {
        this->ui->customEdt->setEnabled(true);
    }
    else
    {
        this->ui->customEdt->setEnabled(false);
    }
}

void MainWindow::on_excludeCbox_stateChanged(int checked)
{
    if(checked)
    {
        this->ui->excludeEdt->setEnabled(true);
    }
    else
    {
        this->ui->excludeEdt->setEnabled(false);
    }
}
