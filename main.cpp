#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));//��������� ������ ���������
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));//��������� ������ ���������
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));//��������� ������ ��������� ��� Tr

    QApplication xApp(argc, argv);
    MainWindow xMainWindow;
    xMainWindow.show();

    return xApp.exec();
}
