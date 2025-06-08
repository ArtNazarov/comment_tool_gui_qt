#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString initialDir;
    if (argc > 1) {
        initialDir = QString::fromLocal8Bit(argv[1]);
    }

    MainWindow w(initialDir);
    w.show();
    return a.exec();
}
