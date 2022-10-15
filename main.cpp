#include "title.h"

#include <QApplication>

#if TEST

#else

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Title w;
    w.show();
    return a.exec();
}

#endif
