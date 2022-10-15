#ifndef TITLE_H
#define TITLE_H

#include <QWidget>
#include "gamewidget.h"
#include "QCoreApplication"

namespace Ui {
class Title;
}

class Title : public QWidget
{
    Q_OBJECT

public:
    Title(QWidget *parent = nullptr);
    ~Title();

private:
    Ui::Title *ui;
    GameWidget gameWidget;

private slots:
    void on_singalPlayer_clicked();
    void on_doublePlayer_clicked();
    void on_loadData_clicked();
    void on_quit_clicked();

public slots:
    void backToTitle();
};



#endif // TITLE_H
