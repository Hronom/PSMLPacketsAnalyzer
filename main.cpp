#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));//установка нужной кодировки
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));//установка нужной кодировки
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));//установка нужной кодировки для Tr

    QApplication xApp(argc, argv);
    MainWindow xMainWindow;
    xMainWindow.show();

    return xApp.exec();
}
