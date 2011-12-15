QT       += core gui xml

TARGET = PSMLPacketsAnalyzer
TEMPLATE = app qt


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

RC_FILE = RTP.rc

CODEC = Windows-1251
CODECFORSRC = Windows-1251
CODECFORTR = Windows-1251

