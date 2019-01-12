#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QColor>
#include <QTime>
#include "dialogstart.hh"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _betIndex = 0;
    _currentBet = 0.2;
    _scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(_scene);

    setUpBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addMoney(double money)
{
    _money = money;
    ui->Cash->setText(QString::number(money));
}

void MainWindow::spinWheels()
{
    if(_money >= _currentBet)
    {
        _money -= _currentBet;
        ui->Cash->setText(QString::number(_money));
        //Spinnaa
        spinAnimation();

    }
    else
    {
        DialogStart dlog;
        if(dlog.exec() == QDialog::Rejected)
        {
            destroy();
        }
        else
        {
            addMoney(dlog.getMoney());
        }
    }
}

void MainWindow::changeBet()
{
    double bets [6] = {0.20,0.40,0.80,1.20,1.60,2.00};
    if(_betIndex != 5)
    {
        _betIndex++;
    }
    else
    {
        _betIndex = 0;
    }
    _currentBet = bets[_betIndex];

    ui->Bet->setText("Bet \n " + QString::number(bets[_betIndex]));
}

void MainWindow::setUpBoard()
{
    QString pic = R"(:/kuvat/diamond.png)";
    for(int x = 0; x < 5; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            coord key = {x,y};
            picMap[key] = new QGraphicsPixmapItem(pic);
            picMap[key]->setScale(0.25);
            picMap[key]->setPos(10 + 135*x,10 + 125 * y);
            _scene->addItem(picMap[key]);
        }
    }
}

void MainWindow::wait(int msecs)
{
    QTime dieTime = QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::spinAnimation()
{
    for(int a = 375*3; a != 0; a--)
    {
        for(auto i : picMap)
        {
            i.second->moveBy(0,1);
            if(i.second->pos().y() > 375)
            {
                i.second->moveBy(0,-375);
            }
        }
        wait(2);
    }

}
