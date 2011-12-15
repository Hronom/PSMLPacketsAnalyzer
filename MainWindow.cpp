#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QFileDialog"
#include "QDomDocument"
#include "QDomNodeList"
#include "QDebug"
#include "QScrollBar"
#include "qmath.h"
#include "QList"
#include "QMessageBox"
#include "QtCore"

MainWindow::MainWindow(QWidget *xParent) : QMainWindow(xParent), mUI(new Ui::MainWindow)
{
    mUI->setupUi(this);
    mUI->ListOfPackets->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    mScene = new QGraphicsScene();
    mScene->setBspTreeDepth(1);

    mUI->graphicsView->setInteractive(true);
    mUI->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    mUI->graphicsView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    mUI->graphicsView->setScene(mScene);
}

MainWindow::~MainWindow()
{
    delete mUI;
}

void MainWindow::fillTable()
{
    mUI->ListOfPackets->clearContents();
    mUI->ListOfPackets->setRowCount(0);

    mMinPacketSize = mListOfValues.at(0).toInt();
    mMaxPacketSize = mMinPacketSize;

    mUI->ListOfPackets->setSortingEnabled(false);

    for(int i=0; i < mListOfValues.count(); i++)
    {
        bool xNeedAddItem = true;

        for(int j=0; j < mUI->ListOfPackets->rowCount(); j++)
        {
            int xCurrentCellValue = mUI->ListOfPackets->item(j,0)->data(Qt::DisplayRole).toInt();
            int xMin = xCurrentCellValue - ((xCurrentCellValue*10)/100);
            int xMax = xCurrentCellValue + ((xCurrentCellValue*10)/100);

            if( xMin < mListOfValues.at(i).toInt() && xMax > mListOfValues.at(i).toInt() )
            {
                mUI->ListOfPackets->item(j,1)->setData(Qt::DisplayRole, mUI->ListOfPackets->item(j,1)->data(Qt::DisplayRole).toInt()+1);
                xNeedAddItem = false;
            }
        }

        if(xNeedAddItem == true)
        {
            QTableWidgetItem *xPacketSizeItem = new QTableWidgetItem();
            xPacketSizeItem->setData(Qt::DisplayRole, mListOfValues.at(i).toInt());
            QTableWidgetItem *xPacketCountItem = new QTableWidgetItem();
            xPacketCountItem->setData(Qt::DisplayRole, mListOfValues.count(xPacketSizeItem->text()));

            mUI->ListOfPackets->insertRow(0);
            mUI->ListOfPackets->setItem(0, 0, xPacketSizeItem);
            mUI->ListOfPackets->setItem(0, 1, xPacketCountItem);
        }

        if(mMinPacketSize > mListOfValues.at(i).toInt())
            mMinPacketSize = mListOfValues.at(i).toInt();
        else
            if(mMaxPacketSize < mListOfValues.at(i).toInt())
                mMaxPacketSize = mListOfValues.at(i).toInt();
    }

    mUI->ListOfPackets->setSortingEnabled(true);

    mUI->MinSpinBox->setValue(mMinPacketSize);
    mUI->MaxSpinBox->setValue(mMaxPacketSize);

    mUI->ElementsCount->setText(tr("Количество отображённых элементов: ") + QString::number(mUI->ListOfPackets->rowCount()));
}

void MainWindow::drawGraph()
{
    mPacketRectList.clear();
    mScene->clear();

    int xPacketsHorizSize = 10;
    int xSpaceBetweenPackets = 5;
    int xEndLineXValue = xSpaceBetweenPackets + (xPacketsHorizSize + xSpaceBetweenPackets) * mListOfValues.count();
    int xEndLineYValue = -(mMaxPacketSize + xSpaceBetweenPackets);

    QPen xLinePen;
    xLinePen.setWidth(3);
    mScene->addLine(-10,0,xEndLineXValue,0,xLinePen);
    mScene->addLine(0,10,0,xEndLineYValue,xLinePen);

    QPen xArrowPen;
    xArrowPen.setWidth(3);
    xArrowPen.setMiterLimit(3);
    xArrowPen.setJoinStyle(Qt::RoundJoin);
    xArrowPen.setCapStyle(Qt::RoundCap);
    QBrush xArrowBrush;
    xArrowBrush.setColor(Qt::black);
    xArrowBrush.setStyle(Qt::SolidPattern);

    QPolygonF xArrowX;
    xArrowX << QPointF(xEndLineXValue, -10)
            << QPointF(xEndLineXValue + 10, 0)
            << QPointF(xEndLineXValue, 10);
    mScene->addPolygon(xArrowX, xArrowPen, xArrowBrush);

    QPolygonF xArrowY;
    xArrowY << QPointF(-10, xEndLineYValue)
            << QPointF(0, xEndLineYValue - 10)
            << QPointF(10, xEndLineYValue);
    mScene->addPolygon(xArrowY, xArrowPen, xArrowBrush);

    QFont xTextFont;
    xTextFont.setPixelSize(22);
    QGraphicsTextItem *xTextX;
    xTextX = mScene->addText(tr("Последовательность пакетов"), xTextFont);
    xTextX->setPos(0,0);
    QGraphicsTextItem *xTextY;
    xTextY = mScene->addText(tr("Размер пакета"),xTextFont);
    xTextY->setPos(-33,-3);
    xTextY->rotate(-90);

    QPen xRectPen;
    xRectPen.setWidth(1);
    xRectPen.setColor(Qt::gray);
    QBrush xRectBrush;
    xRectBrush.setColor(Qt::gray);
    xRectBrush.setStyle(Qt::SolidPattern);

    for(int i = 0; i<mListOfValues.count(); i++)
    {
        QGraphicsRectItem *xPacketRect;
        xPacketRect = mScene->addRect(xSpaceBetweenPackets+i*xPacketsHorizSize + i*xSpaceBetweenPackets,-5,xPacketsHorizSize,-mListOfValues.value(i).toInt(),xRectPen,xRectBrush);
        xPacketRect->setCursor(Qt::PointingHandCursor);
        xPacketRect->setToolTip(tr("Размер пакета: ") + mListOfValues.value(i) + tr(" байт"));
        xPacketRect->setData(1, QVariant(mListOfValues.value(i).toInt()));

        mPacketRectList.push_back(xPacketRect);
    }

    mUI->label_5->setText(tr("Количество отображённых пакетов: ") + QString::number(mPacketRectList.count()));
    mUI->label_6->setText(tr("Количество выделеных пакетов: 0"));

    mScene->update();
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString xOpenFilePath = QFileDialog::getOpenFileName(this,
                                                         tr("Открыть Файл"),
                                                         qApp->applicationDirPath(),
                                                         tr("PSML файлы (*.psml);;Все файлы (*.*)"));

    if(xOpenFilePath != "")
    {
        QDomDocument xDomDocument("xml");

        QFile xFile(xOpenFilePath);
        xFile.open(QIODevice::ReadOnly);
        xDomDocument.setContent(&xFile);
        xFile.close();

        QDomElement xRootElement = xDomDocument.documentElement();

        mListOfValues.clear();

        QDomElement xStructureElement = xRootElement.firstChildElement("structure");
        QDomNodeList xDomNodeList = xStructureElement.childNodes();
        int xIndexOfLength = -1;
        for(int i = 0; i<xDomNodeList.size(); i++)
            if(xDomNodeList.at(i).lastChild().nodeValue() == "Length")
                xIndexOfLength = i;

        if(xIndexOfLength == -1)
        {
            QMessageBox::warning(this, tr("Ошбика"), tr("В файла не найдено описание параметра Length для пакетов"));
            return;
        }

        for(QDomElement xProgramElement = xRootElement.firstChildElement("packet"); xProgramElement.isNull() == false; xProgramElement = xProgramElement.nextSiblingElement("packet"))
        {
            mListOfValues.push_back(xProgramElement.childNodes().at(xIndexOfLength).lastChild().nodeValue());
        }

        fillTable();
        drawGraph();
    }
}

void MainWindow::on_pushButton_clicked()
{
    for(int i=0; i<mUI->ListOfPackets->rowCount(); i++)
    {
        if(mUI->MinSpinBox->text().toInt() <= mUI->ListOfPackets->item(i,0)->data(Qt::DisplayRole).toInt() &&
                mUI->ListOfPackets->item(i,0)->data(Qt::DisplayRole).toInt() <= mUI->MaxSpinBox->text().toInt())
            mUI->ListOfPackets->showRow(i);
        else
            mUI->ListOfPackets->hideRow(i);

        int xVisibleRowCount = 0;
        for(int i=0; i<mUI->ListOfPackets->rowCount(); i++)
            if(mUI->ListOfPackets->isRowHidden(i) == false) xVisibleRowCount++;
        mUI->ElementsCount->setText(tr("Количество отображённых элементов: ") + QString::number(mUI->ListOfPackets->rowCount()));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    mUI->MinSpinBox->setValue(mMinPacketSize);
    mUI->MaxSpinBox->setValue(mMaxPacketSize);
    on_pushButton_clicked();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_pushButton_3_clicked()
{
    int xSelectedPacketsCount = 0;
    int xMinSize = mUI->MinSpinBox2->text().toInt();
    int xMaxSize = mUI->MaxSpinBox2->text().toInt();

    QBrush xRectBrush;
    xRectBrush.setStyle(Qt::SolidPattern);

    for(int i=0; i<mPacketRectList.count(); i++)
    {
        int xCurrentPacketSize = mPacketRectList.at(i)->data(1).toInt();

        if(xCurrentPacketSize >= xMinSize && xCurrentPacketSize <= xMaxSize)
        {
            xRectBrush.setColor(Qt::darkGreen);
            mPacketRectList.at(i)->setBrush(xRectBrush);
            xSelectedPacketsCount++;
        }
        else
        {
            xRectBrush.setColor(Qt::gray);
            mPacketRectList.at(i)->setBrush(xRectBrush);
        }
    }

    mUI->label_6->setText(tr("Количество выделеных пакетов: ") + QString::number(xSelectedPacketsCount));
    mScene->update();
}

void MainWindow::on_pushButton_4_clicked()
{
    mUI->MinSpinBox2->setValue(0);
    mUI->MaxSpinBox2->setValue(0);

    QBrush xRectBrush;
    xRectBrush.setStyle(Qt::SolidPattern);
    xRectBrush.setColor(Qt::gray);

    for(int i=0; i<mPacketRectList.count(); i++)
        mPacketRectList.at(i)->setBrush(xRectBrush);

    mScene->update();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    qreal xScale = qPow(qreal(2), (value - 250) / qreal(50));
    QMatrix matrix;
    matrix.scale(xScale, xScale);
    mUI->graphicsView->setMatrix(matrix);
}

void MainWindow::on_action_triggered()
{
    QMessageBox::about(this, tr("О программе"), tr("Программа <b>RTPPacketsAnalyzer</b> создана для анализа отчётов созданных программой <a href='http://www.wireshark.org'>WireShark</a><hr>Автор: Hronom<br>email: <a href='mailto:hronom@gmail.com'>hronom@gmail.com</a>"));
}
