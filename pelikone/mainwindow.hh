#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <map>
#include <QGraphicsPixmapItem>

using coord = std::pair<int,int>;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addMoney(double money);

private slots:
    void spinWheels();
    void changeBet();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *_scene;

    void setUpBoard();
    void wait(int msecs);
    void spinAnimation();

    double _money;
    double _currentBet;
    int _betIndex;

    std::map<coord,QGraphicsPixmapItem*> picMap;
    std::map<int,int> gameBoard;
};

#endif // MAINWINDOW_HH
