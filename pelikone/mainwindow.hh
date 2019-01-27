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
    void wait();
    void spinAnimation();
    /**
     * @brief getPicPath
     * @return
     */
    QString getPicPath(int index);
    QString getPic(bool lockingsituation, int y);
    void storeToBoard(int index, int y);

    double _money;
    double _currentBet;
    int _betIndex;

    std::vector<std::vector<QGraphicsPixmapItem*>> picVec;
    std::map<int,int> gameBoard;
    std::map<int,std::vector<int>> afterBoard;
    std::vector<bool> _lockList;
    int _waitmsecs; // For time between moving reels.
    bool _spinninggoing; // Flag for spinning animation.
};

#endif // MAINWINDOW_HH
