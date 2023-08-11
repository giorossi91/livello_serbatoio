# Tank level monitor

## Scope
This project realizes a simple device to monitor the water level in a tank.

## Architecture
The monitor is realized with an ATMEGA328P microcontroller to run the SW produced with Arduino IDE environment, an ultrasonic sensor to measure the level of the water and an LCD 16x2 characters to show the liters amount associated to the percentage of filling.

## Sofware
The SW has been developed with Arduino IDE and tested with CppUnit and a Qt GUI used to simulate the external environment.

## Phases of the project
### Phase 1 (Prototype)
The project was realized on a prototype board using the schematic in `schemi\schema_scheda.pdf`. The microcontroller was hosted in an Arduino UNO rev3 board.
The schematic was realized with Fritzing. The enclosure was hand-made and adapted from an existing one.

### Phase 2 (Finished)
The entire schematic was re-designed with Kicad and a PCB has been designed and produced. The enclosure has been designed with Freecad and 3D printed.

## Project Website
Go to [project website](https://giorossi91.github.io/livello_serbatoio)
