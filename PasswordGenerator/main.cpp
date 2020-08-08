#include "mainwindow.h"

#include <QApplication>
#include "QsLog/QsLog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QsLogging::Logger &log = QsLogging::Logger::instance();
    MainWindow w(log);
    w.show();
    return a.exec();
}
