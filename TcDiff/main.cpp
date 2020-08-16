#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <iostream>

#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogDestConsole.h"
#include "QsLog/QsLogDestFile.h"
#include "QsLog/QsLogDestFunctor.h"
#include "QsLog/QsLogLevel.h"

void logFunction(const QString &message, QsLogging::Level level)
{
//    std::cout << qPrintable(message) << " ::: " << static_cast<int>(level) << std::endl;
    std::cout << qPrintable(message) << std::endl;
}

void initLog(QApplication &a, QsLogging::Logger &log)
{
    using namespace QsLogging;

    log.setLoggingLevel(QsLogging::TraceLevel);

    QDir logDir(a.applicationDirPath() + QDir::separator() + "log");
    if(!logDir.exists())
    {
        logDir.mkdir(a.applicationDirPath() + QDir::separator() + "log");
    }

//    const QString sLogPath(QDir(a.applicationDirPath()).filePath("log.txt"));
    const QString sLogPath(logDir.filePath("log.txt"));

    // 2. add two destinations
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
     sLogPath, EnableLogRotation, MaxSizeBytes(512), MaxOldLogCount(2)));
//    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));
//    log.addDestination(debugDestination);
    log.addDestination(fileDestination);
    log.addDestination(functorDestination);

//    qDebug() << "logger init success";
//    QLOG_INFO() << "logger init success";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QsLogging::Logger &log = QsLogging::Logger::instance();
    initLog(a, log);

    MainWindow w;
    w.show();
    int code = a.exec();
    QsLogging::Logger::destroyInstance();
    return code;
}
