#include "livelloserbatoiosim.h"
#include <QApplication>

#include "arduino_stubs.h"
#include "arduinoboard.h"

int main(int argc, char *argv[])
{
    // setup arduino board to use for simulator
    board = new ArduinoBoard();

    QApplication a(argc, argv);
    LivelloSerbatoioSim w;
    w.show();

    return a.exec();
}
