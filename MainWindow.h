#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *mUI;

    QGraphicsScene *mScene;

    int mMinPacketSize;
    int mMaxPacketSize;

    QStringList mListOfValues;
    QVector<QGraphicsRectItem*> mPacketRectList;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void fillTable();
    void drawGraph();

private slots:
    void on_actionOpenFile_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_actionAbout_Qt_triggered();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_verticalSlider_valueChanged(int value);
    void on_action_triggered();
};

#endif // MAINWINDOW_H
