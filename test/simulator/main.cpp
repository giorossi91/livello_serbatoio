#include "livelloserbatoiosim.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LivelloSerbatoioSim w;
    w.show();

    return a.exec();
}
