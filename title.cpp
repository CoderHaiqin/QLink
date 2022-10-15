#include "title.h"
#include "ui_title.h"

Title::Title(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Title)
{
    ui->setupUi(this);
    QObject::connect(&gameWidget, &GameWidget::backToTitle, this, &Title::backToTitle);
}

Title::~Title()
{
    delete ui;
}

/**
 * @brief 开始单人模式
 */
void Title::on_singalPlayer_clicked()
{
    this->hide();
    gameWidget.init(SINGAL_MODE);
    gameWidget.show();
}

/**
 * @brief 开始多人模式
 */
void Title::on_doublePlayer_clicked()
{
    this->hide();
    gameWidget.init(DOUBLE_MODE);
    gameWidget.show();
}

/**
 * @brief 读取存档
 */
void Title::on_loadData_clicked()
{
    this->hide();
    gameWidget.init(LOAD_DATA);
    gameWidget.show();
}

/**
 * @brief 返回标题
 * 这是一个槽函数，但信号是由gameWidget发送的
 */
void Title::backToTitle()
{
    gameWidget.hide();
    this->show();
}

/**
 * @brief 退出游戏
 */
void Title::on_quit_clicked()
{
    QApplication::quit();
}

