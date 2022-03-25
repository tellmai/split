#include "DataSplit.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataSplit w;
    w.show();
    return a.exec();
}
