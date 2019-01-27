#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QColor>
#include <QTime>
#include <stdlib.h> //rand
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
    if(_spinninggoing)
    {
        return;
    }
    else if(_money >= _currentBet)
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
    _waitmsecs = 1;
    _spinninggoing = false;
    QString pic = R"(:/kuvat/diamond.png)";
    for(int x = 0; x < 5; x++)
    {
        std::vector<QGraphicsPixmapItem*> innerVec;
        for(int y = 0; y < 4; y++)
        {
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pic);
            item->setScale(0.22);
            item->setPos(10 + 135*x,10 + 115 * y);
            innerVec.push_back(item);
            _scene->addItem(item);
            if(afterBoard.find(x) == afterBoard.end())
            {
                afterBoard[x] = std::vector<int>();
            }
            afterBoard[x].push_back(-1);
        }
        picVec.push_back(innerVec);
        _lockList.push_back(false);
    }
}

void MainWindow::wait()
{
    QTime waitTime = QTime::currentTime().addMSecs(_waitmsecs);
    while (QTime::currentTime() < waitTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::spinAnimation()
{
    _spinninggoing = true;
    // Pics are 512x512 (*0.25)
    int roundIndex = 0;
    int roundTime = 345;
    for(int movement = 0; movement != 6*roundTime; movement++)
    {
        if(movement == (roundIndex+1)*roundTime)
        {
            _lockList[roundIndex] = true;
            roundIndex++;
        }
        for(int i = (int)picVec.size() - 1; i >= 0; i--)
        {
            if(_lockList[i] != true)
            {
                // For one column
                for(int j = (int)picVec[i].size() - 1; j >= 0; j--)
                {
                    bool forstore = false;
                    if(j == roundIndex)
                    {
                        forstore = true;
                    }
                    picVec[i][j]->moveBy(0,1);
                    if(picVec[i][j]->pos().y() > 355)
                    {
                        picVec[i][j]->setPixmap(getPic(forstore,j));
                        picVec[i][j]->moveBy(0,-460);
                    }
                }
            }
        }
        wait();
    }
    for(int i = 0; i !=  (int) _lockList.size(); i++)
    {
        _lockList[i] = false;
    }
    _waitmsecs = 1;
    _spinninggoing = false;
}

QString MainWindow::getPicPath(int index)
{
    if(index == 0){return R"(:/kuvat/strawberry.png)";}
    else if(index == 1){return R"(:/kuvat/cherry.png)";}
    else if(index == 2){return R"(:/kuvat/cherries.png)";}
    else if(index == 3){return R"(:/kuvat/diamond.png)";}
    else if(index == 4){return R"(:/kuvat/bigwin.png)";}
    else if(index == 5){return R"(:/kuvat/jackpot.png)";}
    else {return R"(:/kuvat/slot-machine.png)";}
}

QString MainWindow::getPic(bool lockingsituation, int y)
{
    int randomGen = rand() % 100 + 1;
    int randomIndex;
    if(randomGen < 20){randomIndex = 0;}
    else if(randomGen < 40){randomIndex = 1;}
    else if(randomGen < 60){randomIndex = 2;}
    else if(randomGen < 80){randomIndex = 3;}
    else if(randomGen < 90){randomIndex = 4;}
    else if(randomGen < 95){randomIndex = 5;}
    else {randomIndex = 6;}

    if(lockingsituation)
    {
        storeToBoard(randomIndex, y);
    }
    return getPicPath(randomIndex);
}

void MainWindow::storeToBoard(int index, int y)
{
    for(int x = 0; x < 5; x++)
    {
        if(afterBoard[x][y] == -1)
        {
            afterBoard[x][y] = index;
            return;
        }

    }
}
