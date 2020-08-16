#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

/**
 * 生成密码
 * @param num 是否包含数字
 * @param lower 是否包含小写字母
 * @param upper 是否包含大写字母
 * @param special 包含的特殊字符 不需要可以为空字符串
 * @param custom 包含的自定义字符 不需要可以为空字符串
 * @param exclude 不要在密码里存在的字符 排除的字符串
 * @param minLen 生成的密码最小长度
 * @param maxLen 生成的密码最大长度 如果需要固定长度的密码 把最小和最大长度设置相同即可
 * @param generateSize 生成多少个密码
 * @param savePath 生成的密码文件保存到哪个文件夹里 如果是空字符串 就不保存
 **/
void MainWindow::generatePwd(bool num, bool lower, bool upper, QString special, QString custom, QString exclude, int minLen, int maxLen, int generateSize, bool noRepeat, bool disorder, QString savePath)
{
    QLOG_INFO() << "==================开始生成密码==========================";
    QLOG_INFO() << "是否包含数字:" << num;
    QLOG_INFO() << "是否包含小写字母:" << lower;
    QLOG_INFO() << "是否包含大写字母:" << upper;
    QLOG_INFO() << "包含的特殊字符:" << special;
    QLOG_INFO() << "包含的自定义字符:" << custom;
    QLOG_INFO() << "要排除的字符:" << exclude;
    QLOG_INFO() << "密码最小长度:" << minLen;
    QLOG_INFO() << "密码最大长度:" << maxLen;
    QLOG_INFO() << "要生成的密码个数:" << generateSize;
    QLOG_INFO() << "禁止重复:" << noRepeat;
    QLOG_INFO() << "禁止有序:" << disorder;
    QLOG_INFO() << "密码保存文件位置:" << savePath;

    QFile file;
    bool saveFile = false;
    if(!savePath.isEmpty())
    {
        file.setFileName(savePath);
        saveFile = file.open(QIODevice::Append);
    }
    QLOG_INFO() << "密码保存到文件:" << saveFile;

    //数字
    QString numStr = QString(PWD_NUMBER);
    //小写字母
    QString lowerStr = QString(PWD_LOWER);
    //大写字母
    QString upperStr = QString(PWD_UPPER);
    //特殊字符
    QString specialStr = QString(special.trimmed());
    //自定义字符
    QString customStr = QString(custom.trimmed());
    //排除字符
    QString excludeStr = QString(exclude.trimmed());

    //在备选字符串中去除排除字符串
    if(!excludeStr.isEmpty())
    {
        numStr = delStr(numStr, excludeStr);
        lowerStr = delStr(lowerStr, excludeStr);
        upperStr = delStr(upperStr, excludeStr);
        specialStr = delStr(specialStr,excludeStr);
        customStr = delStr(customStr, excludeStr);
    }

    //所有密码字符
    QString pwdCandidates = "";

    //密码用到的字符类型列表
    QStringList pwdStrList;
    if(num)
    {
        pwdStrList.append(numStr);
        pwdCandidates += numStr;
    }
    if(lower)
    {
        pwdStrList.append(lowerStr);
        pwdCandidates += lowerStr;
    }
    if(upper)
    {
        pwdStrList.append(upperStr);
        pwdCandidates += upperStr;
    }
    if(!specialStr.isEmpty())
    {
        pwdStrList.append(specialStr);
        pwdCandidates += specialStr;
    }
    if(!customStr.isEmpty())
    {
        pwdStrList.append(customStr);
        pwdCandidates += customStr;
    }

    //原始字符串
    QString pwdCandidatesAll = QString(pwdCandidates);

    //去除重复
    if(noRepeat)
    {
        QLOG_INFO() << "去除重复项";
        pwdCandidates = delRepeatStr(pwdCandidates);
        for(int i = 0; i < pwdStrList.size(); i ++)
        {
            QString item = pwdStrList.at(i);
            pwdStrList.removeOne(item);
            item = delRepeatStr(item);
            pwdStrList.insert(i, item);
        }
    }


    for(int i = 0; i < generateSize; i ++)
    {
        QString pwd = "";
        int pwdLen = minLen;
        if(minLen < maxLen)
        {
            // 生成最长到最短差值的数字 生成的数字不包括传入的最大值 所以给传入的最大值 + 1
            pwdLen += QRandomGenerator::global()->bounded(maxLen - minLen + 1);
        }

        //每一个新密码使用的类型列表 都从完整密码类型列表里copy一份
        QStringList tmpType(pwdStrList);
        //密码类型是否都存在了  密码类型都存在之后就不用管类型是否必须存在了
        bool typeOk = false;
        //最后一个字符
        QChar lastChar = ' ';
        //根据密码长度 随机出需要的密码
        for(int n = 0; n < pwdLen; n ++)
        {
            //密码备选字符串
            QString pwdItem = "";
            //如果需要的类型还不全 就每个类型挑一个
            if(!typeOk)
            {
                //随机密码类型
                int type = QRandomGenerator::global()->bounded(tmpType.size());
                pwdItem = tmpType.at(type);
                tmpType.removeOne(pwdItem);
                if(tmpType.isEmpty())
                {
                    tmpType.append(pwdStrList);
                    typeOk = true;
                }
            }
            else
            {
                pwdItem = pwdCandidates;
            }

            //禁止重复
            if(noRepeat)
            {
                //如果禁止重复 就从备选列表里删除已经存在于密码字符串中的字符
                pwdItem = delStr(pwdItem, pwd);
            }

            //禁止有序
            QChar ch = ' ';
            int chIndex = -1;
            int preChIndex = -1;
            do
            {
                int charIndex = QRandomGenerator::global()->bounded(pwdItem.size());
                ch = pwdItem.at(charIndex);
                chIndex = pwdCandidatesAll.indexOf(ch);
                preChIndex = pwdCandidatesAll.indexOf(lastChar);
            } while (chIndex > 0 && preChIndex > 0 && qAbs(chIndex - preChIndex) == 1);

            //密码字符
            pwd += ch;
            lastChar = ch;
        }
        QLOG_INFO() << "本次生成密码:" << pwd << "  长度:" << pwdLen;
        ui->resultEdt->appendPlainText(pwd);
        if(saveFile)
        {
            file.write(pwd.toStdString().data());
            file.write("\n");
        }
    }

    file.close();

    QLOG_INFO() << "==================密码生成完毕==========================";
}


/**
 * 从str字符串中删除exclude中存在的字符
 * */
QString MainWindow::delStr(QString str, QString exclude)
{
    if(str.isEmpty()) return str;
    for(int i = 0; i < exclude.length(); i ++)
    {
        QChar ch = exclude.at(i);
        str.replace(ch, "");
    }
    return str;
}

QString MainWindow::delRepeatStr(QString str)
{
    if(str.length() < 2)
        return str;
    QString s = "";
    for(int i = 0; i < str.length(); i ++)
    {
        QChar ch = str.at(i);
        if(!s.contains(ch))
        {
            s += ch;
        }
    }
    return s;
}

QString MainWindow::getSavePath()
{
    return this->savePath;
}

void MainWindow::setSavePath(QString path)
{
    this->savePath = path;
}

void MainWindow::on_generateBtn_clicked()
{
    //要生成多少个密码
    int generateCount = this->ui->generateSizeEdt->text().toInt();
    //如果个数输入框是空 就设置成1  最少生成1个密码
    generateCount = qMax(1, generateCount);
    this->ui->generateSizeEdt->setText(QString("%1").arg(generateCount));
    QLOG_INFO() << "生成密码个数:" << generateCount;

    bool saveToDesktop = this->ui->saveToDesktopCbox->isChecked();
    bool resetResult = this->ui->resetResultCbox->isChecked();
    QLOG_INFO() << "保存到桌面:" << saveToDesktop;
    QLOG_INFO() << "重置输出框:" << resetResult;

    //密码最短最长长度
    int pwdLenMin = this->ui->pwdLenMinEdt->text().toInt();
    pwdLenMin = qMax(1, pwdLenMin);
    int pwdLenMax = this->ui->pwdLenMaxEdt->text().toInt();
    pwdLenMax = qMax(1, pwdLenMax);
    QLOG_INFO() << "密码长度 最短:" << pwdLenMin << " 最长:" << pwdLenMax;

    if(pwdLenMax < pwdLenMin)
    {
        pwdLenMax = pwdLenMin;
        ui->pwdLenMaxEdt->setText(QString(pwdLenMin));
    }

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

    //是否清空结果
    if(resetResult)
    {
        this->ui->resultEdt->clear();
        QLOG_INFO() << "清空结果列表";
    }

    //是否存储到桌面
    QString path = "";
    if(saveToDesktop)
    {
        path = savePath;
    }

    //生成密码
    generatePwd(ui->numCbox->isChecked(),
                ui->lowerCbox->isChecked(),
                ui->upperCbox->isChecked(),
                specialStr,
                customStr,
                excludeStr,
                pwdLenMin,
                pwdLenMax,
                generateCount,
                ui->noRepeatCbox->isChecked(),
                ui->disOrderCbox->isChecked(),
                savePath);

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
