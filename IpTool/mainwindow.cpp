#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostInfo>
#include <QHostAddress>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkAddressEntry>
#include <QNetworkRequest>

#include <QJsonObject>
#include "QsLog/QsLog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ipv6("")
{
    ui->setupUi(this);

    QLOG_INFO() << "开始获取网络信息...";
    QString localHostName = QHostInfo::localHostName();

    QString hostStr = QString("      主机名:%1").arg(localHostName);
    ui->textBrowser->append(hostStr);

    QLOG_INFO() << hostStr;

    QHostInfo info = QHostInfo::fromName(localHostName);

    ui->textBrowser->append(info.localDomainName());
    ui->textBrowser->append("正在获取外网IP");

    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();

    ui->textBrowser->append("");
    int index = 0;
    for (int i = 0; i < interfaceList.size(); i ++ )
    {
        QNetworkInterface interface = interfaceList[i];
        if(!interface.isValid() && interface.flags() == QNetworkInterface::IsRunning)
        {
            QLOG_INFO() << "无效网卡 ... 忽略";
            continue;
        }


        QString interfaceStr = QString("%1  %2\n          MAC:%3").arg(interface.humanReadableName()).arg(getNetInterfaceTypeNameByType(interface.type())).arg(interface.hardwareAddress());
        ui->textBrowser->append(interfaceStr);
        QLOG_INFO() << interfaceStr;

        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        for (int n = 0; n < entryList.size(); n ++ )
        {
            QNetworkAddressEntry entry = entryList[n];

            QString ip = entry.ip().toString().trimmed();
            QString mask = entry.netmask().toString().trimmed();
            QString broadcast = entry.broadcast().toString().trimmed();

            if(ip.contains("%"))
            {
                QLOG_INFO() << "无效地址信息 ... 忽略";
                continue;
            }


            if(ip.contains(":") && !ip.contains("::") && !ip.startsWith("fe80"))
            {
                if(index < 2)
                {
                    ipv6 = ip;
                }
                index ++;
            }

            QString ipStr = QString("           IP:%1").arg(ip);
            ui->textBrowser->append(ipStr);
            QLOG_INFO() << ipStr;

            if(mask.length() > 2 && !mask.contains("ff"))
            {
                QString maskStr = QString("     子网掩码:%1").arg(mask);
                ui->textBrowser->append(maskStr);
                QLOG_INFO() << maskStr;
            }

            if(broadcast.length() > 2)
            {
                QString broadcastStr = QString("     广播地址:%1").arg(broadcast);
                ui->textBrowser->append(broadcastStr);
                QLOG_INFO() << broadcastStr;
            }

        }
        ui->textBrowser->append("");
    }


    QLOG_INFO() << "开始获取外网信息...";
    //请求外网IP
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));

    request.setUrl(QUrl("http://ip.taobao.com/outGetIpInfo?&ip=myip&accessKey=alibaba-inc"));
    QNetworkReply* reply = naManager->get(request);
}

void MainWindow::requestFinished(QNetworkReply* reply)
{
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.toInt() != 200)
    {
        QString msg = ui->textBrowser->toPlainText();
        msg = msg.replace("正在获取外网IP", "外网IP获取失败！");
        ui->textBrowser->setText(msg);
        QLOG_WARN() << "网络请求失败！";
        return;
    }

    QLOG_WARN() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
    {
        QLOG_WARN() << "reason=" << reason.toString();
    }


    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        QString msg = ui->textBrowser->toPlainText();
        msg = msg.replace("正在获取外网IP", "外网IP获取失败！");
        ui->textBrowser->setText(msg);
        QLOG_WARN() << "网络请求失败！";
    }
    else {
        // 获取返回内容
        QByteArray result = reply->readAll();

        QJsonParseError e;
        QJsonDocument dom = QJsonDocument::fromJson(result, &e);
        if(e.error == QJsonParseError::NoError && !dom.isNull())
        {
            QJsonObject obj = dom.object();
            if(!obj.contains("data"))
            {
                QLOG_WARN() << "数据格式错误!";
                return;
            }

            QJsonObject dataObj = obj.value("data").toObject();

            QString country = "";
            if(dataObj.contains("country"))
            {
                country = dataObj.value("country").toString();
            }

            QString region = "";
            if(dataObj.contains("region"))
            {
                region = dataObj.value("region").toString();
            }

            QString city = "";
            if(dataObj.contains("city"))
            {
                city = dataObj.value("city").toString();
            }

            QString ip = "";
            if(dataObj.contains("ip"))
            {
                ip = dataObj.value("ip").toString();
            }

            QString isp = "";
            if(dataObj.contains("isp"))
            {
                isp = dataObj.value("isp").toString();
            }

            QString extIpStr = QString("        国家:%1\n        区域:%2\n        城市:%3\n      运营商:%4\n外网IPv4地址:%5\n外网IPv6地址:%6").arg(country).arg(region).arg(city).arg(isp).arg(ip).arg(ipv6);

            QString msg = ui->textBrowser->toPlainText();
            msg = msg.replace("正在获取外网IP", extIpStr);
            ui->textBrowser->setText(msg);

            QLOG_INFO() << extIpStr;

        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

/**
        Loopback = 1,
        Virtual,  2
        Ethernet, 3
        Slip,     4
        CanBus,   5
        Ppp,      6
        Fddi,     7
        Wifi,     8
        Ieee80211 = Wifi,   // alias
        Phonet,   9
        Ieee802154, 10
        SixLoWPAN,  11 // 6LoWPAN, but we can't start with a digit
        Ieee80216,  12
        Ieee1394,   13
        Unknown = 0
*/
QString MainWindow::getNetInterfaceTypeNameByType(int type)
{
    QString result = "Unknown";
    switch (type) {
    case 0:
        break;
    case 1:
        result = "Loopback";
        break;
    case 2:
        result = "Virtual";
        break;
    case 3:
        result = "Ethernet";
        break;
    case 4:
        result = "Slip";
        break;
    case 5:
        result = "CanBus";
        break;
    case 6:
        result = "PPP";
        break;
    case 7:
        result = "Fddi";
        break;
    case 8:
        result = "Wifi";
        break;
    case 9:
        result = "Phonet";
        break;
    case 10:
        result = "Ieee802154";
        break;
    case 11:
        result = "SixLoWPAN";
        break;
    case 12:
        result = "Ieee80216";
        break;
    case 13:
        result = "Ieee1394";
        break;
    }
    return result;
}

