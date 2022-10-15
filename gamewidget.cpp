#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    this->grabKeyboard();
    timer = new QTimer(this);
    timer->start(PERIOD);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    QObject::connect(&game, &Game::gameover, this, &GameWidget::gameover);

}

/**
 * @brief 接收输入，传给game类
 */
void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        game.getInput(event->key());
    }
}

/**
 * @brief 接收输入，这里接收的是按键被松开的信号
 */
void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        game.releaseInput(event->key());
    }
}

/**
 * @brief 绘制画面
 */
void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    game.paint(&painter);
}

/**
 * @brief 初始化，根据gameMode
 * @param gameMode
 */
void GameWidget::init(int gameMode)
{
    if(gameMode == LOAD_DATA){
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/save", tr("Text files (*.txt)"));
        game.init(fileName);
    }
    else{
        game.init(gameMode);
    }
}

/**
 * @brief 每次循环调用一次
 */
void GameWidget::update()
{
    game.update();
    repaint();
}

/**
 * @brief gameover时调用，显示信息并返回标题界面
 * @param gameoverMsg
 */
void GameWidget::gameover(const QString& gameoverMsg)
{
    QMessageBox::information(this, "game over", gameoverMsg);
    emit backToTitle();
}



GameWidget::~GameWidget()
{
    delete timer;
    delete ui;
}

/**
 * @brief 保存游戏
 */
void GameWidget::on_pushButton_clicked()
{
    game.pause();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/save", tr("Text files (*.txt)"));
    game.save(fileName);

    game.restart();
}

void GameWidget::on_pushButton_2_clicked()
{
    emit backToTitle();
}
