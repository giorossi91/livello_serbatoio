EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Monitor Serbatoio"
Date "2020-12-27"
Rev "v1.0_Rev.A"
Comp ""
Comment1 "Versione Espandibile"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Crystal Y1
U 1 1 5F0B9E0C
P 7700 2800
F 0 "Y1" V 7950 2850 50  0000 R CNN
F 1 "16MHz" V 7450 2950 50  0000 R CNN
F 2 "simboli_custom:Crystal_TH_AS16_5mm" H 7700 2800 50  0001 C CNN
F 3 "~" H 7700 2800 50  0001 C CNN
	1    7700 2800
	0    -1   -1   0   
$EndComp
Text GLabel 10200 1250 0    50   Input ~ 0
TRIG
Text GLabel 10200 1350 0    50   Output ~ 0
ECHO
$Comp
L power:GND #PWR019
U 1 1 5F0DCA54
P 10150 1500
F 0 "#PWR019" H 10150 1250 50  0001 C CNN
F 1 "GND" H 10155 1327 50  0000 C CNN
F 2 "" H 10150 1500 50  0001 C CNN
F 3 "" H 10150 1500 50  0001 C CNN
	1    10150 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 1500 10150 1450
$Comp
L power:GND #PWR011
U 1 1 5F0F92C9
P 8050 1600
F 0 "#PWR011" H 8050 1350 50  0001 C CNN
F 1 "GND" H 8055 1427 50  0000 C CNN
F 2 "" H 8050 1600 50  0001 C CNN
F 3 "" H 8050 1600 50  0001 C CNN
	1    8050 1600
	1    0    0    -1  
$EndComp
Text GLabel 9350 1250 0    50   Output ~ 0
D12
Text GLabel 9350 1400 0    50   Input ~ 0
D11
Text GLabel 9550 1400 2    50   Output ~ 0
TRIG
Wire Wire Line
	9350 1400 9550 1400
Text GLabel 9550 1250 2    50   Input ~ 0
ECHO
Wire Wire Line
	9550 1250 9350 1250
Text GLabel 6800 2450 2    50   Output ~ 0
D11
Text GLabel 6800 2550 2    50   Input ~ 0
D12
Text GLabel 6800 4150 2    50   Output ~ 0
D3
Text GLabel 6800 4250 2    50   Output ~ 0
D4
Text GLabel 6800 4350 2    50   Output ~ 0
D5
Text GLabel 6800 4450 2    50   Output ~ 0
D6
Text GLabel 6800 4550 2    50   Output ~ 0
D7
Wire Wire Line
	6650 4150 6800 4150
Wire Wire Line
	6650 4250 6800 4250
Wire Wire Line
	6650 4350 6800 4350
Wire Wire Line
	6650 4450 6800 4450
Wire Wire Line
	6650 4550 6800 4550
Text GLabel 6800 2150 2    50   Output ~ 0
D8
Wire Wire Line
	6650 2150 6800 2150
Text GLabel 6800 2250 2    50   Output ~ 0
D9
Wire Wire Line
	6650 2250 6800 2250
$Comp
L power:GND #PWR06
U 1 1 5F185C92
P 6050 5000
F 0 "#PWR06" H 6050 4750 50  0001 C CNN
F 1 "GND" H 6055 4827 50  0000 C CNN
F 2 "" H 6050 5000 50  0001 C CNN
F 3 "" H 6050 5000 50  0001 C CNN
	1    6050 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 4850 6050 5000
$Comp
L Device:C C8
U 1 1 5F1B005F
P 7950 2950
F 0 "C8" V 8000 3050 50  0000 C CNN
F 1 "22pF" V 8100 2950 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 7988 2800 50  0001 C CNN
F 3 "~" H 7950 2950 50  0001 C CNN
	1    7950 2950
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5F1B03E9
P 8100 2800
F 0 "#PWR010" H 8100 2550 50  0001 C CNN
F 1 "GND" V 8105 2672 50  0000 R CNN
F 2 "" H 8100 2800 50  0001 C CNN
F 3 "" H 8100 2800 50  0001 C CNN
	1    8100 2800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 5F1B0B14
P 7450 2800
F 0 "R7" H 7400 2550 50  0000 L CNN
F 1 "1M" V 7450 2750 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 7380 2800 50  0001 C CNN
F 3 "~" H 7450 2800 50  0001 C CNN
	1    7450 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5F1D54A8
P 6700 1700
F 0 "#PWR08" H 6700 1450 50  0001 C CNN
F 1 "GND" V 6705 1572 50  0000 R CNN
F 2 "" H 6700 1700 50  0001 C CNN
F 3 "" H 6700 1700 50  0001 C CNN
	1    6700 1700
	0    -1   -1   0   
$EndComp
Text GLabel 6800 3650 2    50   Input ~ 0
RESET
Wire Wire Line
	6650 3650 6800 3650
Text GLabel 4550 5700 2    50   Output ~ 0
RESET
$Comp
L Device:R R6
U 1 1 5F204DE3
P 4000 5550
F 0 "R6" H 4050 5550 50  0000 L CNN
F 1 "10k" V 4000 5500 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 3930 5550 50  0001 C CNN
F 3 "~" H 4000 5550 50  0001 C CNN
	1    4000 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:D D3
U 1 1 5F205ACA
P 4350 5550
F 0 "D3" V 4304 5630 50  0000 L CNN
F 1 "1N4700" V 4395 5630 50  0000 L CNN
F 2 "Diode_THT:D_DO-41_SOD81_P7.62mm_Horizontal" H 4350 5550 50  0001 C CNN
F 3 "~" H 4350 5550 50  0001 C CNN
	1    4350 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	4350 5400 4000 5400
Wire Wire Line
	4000 5400 4000 5250
Connection ~ 4000 5400
Wire Wire Line
	4000 5700 4350 5700
Wire Wire Line
	4350 5700 4550 5700
Connection ~ 4350 5700
Text GLabel 3450 5700 0    50   Input ~ 0
DTR
$Comp
L power:GND #PWR04
U 1 1 5F2407AF
P 4000 6100
F 0 "#PWR04" H 4000 5850 50  0001 C CNN
F 1 "GND" H 4005 5927 50  0000 C CNN
F 2 "" H 4000 6100 50  0001 C CNN
F 3 "" H 4000 6100 50  0001 C CNN
	1    4000 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 5700 3600 5700
Wire Wire Line
	3900 5700 4000 5700
Connection ~ 4000 5700
$Comp
L Device:C C3
U 1 1 5F287EEB
P 5350 2000
F 0 "C3" H 5300 1900 50  0000 C CNN
F 1 "100nF" V 5500 2000 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 5388 1850 50  0001 C CNN
F 3 "~" H 5350 2000 50  0001 C CNN
	1    5350 2000
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5F288484
P 5350 1850
F 0 "#PWR03" H 5350 1600 50  0001 C CNN
F 1 "GND" V 5355 1722 50  0000 R CNN
F 2 "" H 5350 1850 50  0001 C CNN
F 3 "" H 5350 1850 50  0001 C CNN
	1    5350 1850
	-1   0    0    1   
$EndComp
Wire Wire Line
	5450 2150 5350 2150
$Comp
L Device:C C6
U 1 1 5F0BF95B
P 6450 1700
F 0 "C6" V 6700 1700 50  0000 C CNN
F 1 "100nF" V 6600 1700 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 6488 1550 50  0001 C CNN
F 3 "~" H 6450 1700 50  0001 C CNN
	1    6450 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1250 2750 1300 2750
Connection ~ 1300 2750
Wire Wire Line
	1300 2350 1300 2750
$Comp
L Device:R R9
U 1 1 5F3D6AC6
P 5800 1000
F 0 "R9" V 5700 1000 50  0000 C CNN
F 1 "1k" V 5800 1000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 5730 1000 50  0001 C CNN
F 3 "~" H 5800 1000 50  0001 C CNN
	1    5800 1000
	0    1    1    0   
$EndComp
$Comp
L Device:LED D4
U 1 1 5F3D7411
P 6150 1000
F 0 "D4" H 6143 745 50  0000 C CNN
F 1 "LED_ON" H 6143 836 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 6150 1000 50  0001 C CNN
F 3 "~" H 6150 1000 50  0001 C CNN
	1    6150 1000
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5F3D7EE5
P 6500 1000
F 0 "#PWR012" H 6500 750 50  0001 C CNN
F 1 "GND" H 6505 827 50  0000 C CNN
F 2 "" H 6500 1000 50  0001 C CNN
F 3 "" H 6500 1000 50  0001 C CNN
	1    6500 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 1000 5650 1000
Wire Wire Line
	5950 1000 6000 1000
Wire Wire Line
	6300 1000 6500 1000
Text GLabel 6800 4050 2    50   Input ~ 0
D2
Wire Wire Line
	6800 4050 6650 4050
$Comp
L power:VCC #PWR09
U 1 1 5F561E2E
P 5600 1000
F 0 "#PWR09" H 5600 850 50  0001 C CNN
F 1 "VCC" H 5615 1173 50  0000 C CNN
F 2 "" H 5600 1000 50  0001 C CNN
F 3 "" H 5600 1000 50  0001 C CNN
	1    5600 1000
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR020
U 1 1 5F56D17A
P 10150 1150
F 0 "#PWR020" H 10150 1000 50  0001 C CNN
F 1 "VCC" H 10165 1323 50  0000 C CNN
F 2 "" H 10150 1150 50  0001 C CNN
F 3 "" H 10150 1150 50  0001 C CNN
	1    10150 1150
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR05
U 1 1 5F570FC7
P 4000 5250
F 0 "#PWR05" H 4000 5100 50  0001 C CNN
F 1 "VCC" H 4015 5423 50  0000 C CNN
F 2 "" H 4000 5250 50  0001 C CNN
F 3 "" H 4000 5250 50  0001 C CNN
	1    4000 5250
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0101
U 1 1 5F5AA697
P 6100 1500
F 0 "#PWR0101" H 6100 1350 50  0001 C CNN
F 1 "VCC" H 6115 1673 50  0000 C CNN
F 2 "" H 6100 1500 50  0001 C CNN
F 3 "" H 6100 1500 50  0001 C CNN
	1    6100 1500
	1    0    0    -1  
$EndComp
Text Notes 9750 700  0    50   ~ 0
Ultrasonic sensor
Text Notes 9500 2750 0    50   ~ 0
LCD header
Wire Notes Line
	8500 2600 10850 2600
Wire Notes Line
	10850 6300 8500 6300
Wire Notes Line
	5000 1800 5000 6850
Wire Notes Line
	5000 6850 650  6850
Wire Notes Line
	650  6850 650  1800
Text GLabel 6800 3850 2    50   Input ~ 0
TX
Text GLabel 6800 3950 2    50   Output ~ 0
RX
Wire Wire Line
	6650 3850 6800 3850
Wire Wire Line
	6650 3950 6800 3950
Wire Notes Line
	8500 6300 8500 2600
Text GLabel 9050 4050 2    50   Output ~ 0
BLA
Text GLabel 8800 4050 0    50   Input ~ 0
D9
$Comp
L power:VCC #PWR018
U 1 1 5F57014C
P 10050 5550
F 0 "#PWR018" H 10050 5400 50  0001 C CNN
F 1 "VCC" H 10065 5723 50  0000 C CNN
F 2 "" H 10050 5550 50  0001 C CNN
F 3 "" H 10050 5550 50  0001 C CNN
	1    10050 5550
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR015
U 1 1 5F56E79B
P 9050 3100
F 0 "#PWR015" H 9050 2950 50  0001 C CNN
F 1 "VCC" H 9065 3273 50  0000 C CNN
F 2 "" H 9050 3100 50  0001 C CNN
F 3 "" H 9050 3100 50  0001 C CNN
	1    9050 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 5550 10250 5550
Text GLabel 10250 5550 2    50   Output ~ 0
VDD
Wire Wire Line
	10150 5250 10350 5250
Wire Wire Line
	10150 5100 10350 5100
Text GLabel 10350 5250 2    50   Output ~ 0
E
Text GLabel 10350 5100 2    50   Output ~ 0
RS
Text GLabel 10150 5100 0    50   Input ~ 0
D3
Text GLabel 10150 5250 0    50   Input ~ 0
D4
Text GLabel 9050 4250 2    50   Output ~ 0
DB0
Text GLabel 9050 4550 2    50   Output ~ 0
DB1
Text GLabel 9050 4850 2    50   Output ~ 0
DB2
Text GLabel 9050 5150 2    50   Output ~ 0
DB3
Wire Wire Line
	8800 6050 9050 6050
Wire Wire Line
	8800 5900 9050 5900
Wire Wire Line
	8800 5750 9050 5750
Wire Wire Line
	8800 5600 9050 5600
Text GLabel 9050 6050 2    50   Output ~ 0
DB7
Text GLabel 9050 5900 2    50   Output ~ 0
DB6
Text GLabel 9050 5750 2    50   Output ~ 0
DB5
Text GLabel 8800 6050 0    50   Input ~ 0
D8
Text GLabel 8800 5900 0    50   Input ~ 0
D7
Text GLabel 8800 5750 0    50   Input ~ 0
D6
Text GLabel 9050 5600 2    50   Output ~ 0
DB4
Text GLabel 8800 5600 0    50   Input ~ 0
D5
Wire Wire Line
	9050 3200 9050 3100
Wire Wire Line
	8900 3350 8750 3350
Wire Wire Line
	9050 3600 9050 3500
Text GLabel 8750 3350 0    50   Output ~ 0
V0
Wire Wire Line
	10250 4550 10350 4550
Wire Wire Line
	10250 4450 10350 4450
Wire Wire Line
	10250 4350 10350 4350
Wire Wire Line
	10250 4250 10350 4250
Wire Wire Line
	10250 4150 10350 4150
Wire Wire Line
	10250 4050 10350 4050
Wire Wire Line
	10250 3950 10350 3950
Wire Wire Line
	10250 3850 10350 3850
Wire Wire Line
	10250 3750 10350 3750
Wire Wire Line
	10250 3650 10350 3650
Wire Wire Line
	10250 3550 10350 3550
Wire Wire Line
	10250 3450 10350 3450
Wire Wire Line
	10250 3350 10350 3350
Wire Wire Line
	10250 3250 10350 3250
Wire Wire Line
	10250 3150 10350 3150
Text GLabel 10250 3250 0    50   Input ~ 0
BLA
Text GLabel 10250 3350 0    50   Input ~ 0
DB7
Text GLabel 10250 3450 0    50   Input ~ 0
DB6
Text GLabel 10250 3550 0    50   Input ~ 0
DB5
Text GLabel 10250 3650 0    50   Input ~ 0
DB4
Text GLabel 10250 3750 0    50   Input ~ 0
DB3
Text GLabel 10250 3850 0    50   Input ~ 0
DB2
Text GLabel 10250 3950 0    50   Input ~ 0
DB1
Text GLabel 10250 4050 0    50   Input ~ 0
DB0
Text GLabel 10250 4150 0    50   Input ~ 0
E
Text GLabel 10250 4350 0    50   Input ~ 0
RS
Text GLabel 10250 4450 0    50   Input ~ 0
V0
Text GLabel 10250 4550 0    50   Input ~ 0
VDD
Wire Wire Line
	10250 4650 10350 4650
$Comp
L Connector:Conn_01x16_Female J3
U 1 1 5F0B738E
P 10550 3950
F 0 "J3" H 10442 2925 50  0000 C CNN
F 1 "CONN_LCD" H 10442 3016 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x16_P2.54mm_Vertical" H 10550 3950 50  0001 C CNN
F 3 "~" H 10550 3950 50  0001 C CNN
	1    10550 3950
	1    0    0    1   
$EndComp
$Comp
L Device:R_POT_TRIM RV1
U 1 1 5F0B6050
P 9050 3350
F 0 "RV1" H 8980 3396 50  0000 R CNN
F 1 "POT_LCDC" H 8980 3305 50  0000 R CNN
F 2 "simboli_custom:Trimmer_ACP_470" H 9050 3350 50  0001 C CNN
F 3 "~" H 9050 3350 50  0001 C CNN
	1    9050 3350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5F0B54BC
P 9050 3600
F 0 "#PWR016" H 9050 3350 50  0001 C CNN
F 1 "GND" H 9055 3427 50  0000 C CNN
F 2 "" H 9050 3600 50  0001 C CNN
F 3 "" H 9050 3600 50  0001 C CNN
	1    9050 3600
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR013
U 1 1 5F56C6EF
P 7750 1500
F 0 "#PWR013" H 7750 1350 50  0001 C CNN
F 1 "VCC" V 7750 1700 50  0000 C CNN
F 2 "" H 7750 1500 50  0001 C CNN
F 3 "" H 7750 1500 50  0001 C CNN
	1    7750 1500
	0    -1   -1   0   
$EndComp
Text GLabel 7750 1400 0    50   Input ~ 0
D13
Text GLabel 7750 1300 0    50   Output ~ 0
D2
$Comp
L Device:R R11
U 1 1 5F0B2704
P 7850 1150
F 0 "R11" H 7950 1150 50  0000 L CNN
F 1 "10k" V 7850 1100 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 7780 1150 50  0001 C CNN
F 3 "~" H 7850 1150 50  0001 C CNN
	1    7850 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 4050 9050 4050
Wire Notes Line
	5400 5350 8050 5350
Wire Notes Line
	8050 5350 8050 6450
Wire Notes Line
	8050 6450 5400 6450
Wire Notes Line
	5400 6450 5400 5350
Text Notes 6350 5450 0    50   ~ 0
ICSP - Bootloader
Text GLabel 6350 5850 0    50   Input ~ 0
ICSP_MISO
Text GLabel 7300 5950 2    50   Input ~ 0
ICSP_MOSI
$Comp
L power:VCC #PWR017
U 1 1 5F4B97E7
P 7300 5850
F 0 "#PWR017" H 7300 5700 50  0001 C CNN
F 1 "VCC" V 7315 5977 50  0000 L CNN
F 2 "" H 7300 5850 50  0001 C CNN
F 3 "" H 7300 5850 50  0001 C CNN
	1    7300 5850
	1    0    0    -1  
$EndComp
Text GLabel 6350 5950 0    50   Input ~ 0
ICSP_SCK
Text GLabel 6350 6050 0    50   Input ~ 0
ICSP_RST
$Comp
L power:GND #PWR07
U 1 1 5F4BAD5C
P 7300 6050
F 0 "#PWR07" H 7300 5800 50  0001 C CNN
F 1 "GND" V 7305 5922 50  0000 R CNN
F 2 "" H 7300 6050 50  0001 C CNN
F 3 "" H 7300 6050 50  0001 C CNN
	1    7300 6050
	1    0    0    -1  
$EndComp
Text GLabel 7650 3600 2    50   Input ~ 0
ICSP_MISO
Text GLabel 7650 3500 2    50   Output ~ 0
ICSP_MOSI
Text GLabel 4350 5850 3    50   Output ~ 0
ICSP_RST
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5F5C5141
P 2500 1000
F 0 "H1" H 2600 1049 50  0000 L CNN
F 1 "HOLE1" H 2350 1200 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 2500 1000 50  0001 C CNN
F 3 "~" H 2500 1000 50  0001 C CNN
	1    2500 1000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5F5C614B
P 2850 1000
F 0 "H2" H 2950 1049 50  0000 L CNN
F 1 "HOLE2" H 2700 1200 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 2850 1000 50  0001 C CNN
F 3 "~" H 2850 1000 50  0001 C CNN
	1    2850 1000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5F5C6841
P 3200 1000
F 0 "H3" H 3300 1049 50  0000 L CNN
F 1 "HOLE3" H 3100 1200 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 3200 1000 50  0001 C CNN
F 3 "~" H 3200 1000 50  0001 C CNN
	1    3200 1000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5F5C77FA
P 3550 1000
F 0 "H4" H 3650 1049 50  0000 L CNN
F 1 "HOLE4" H 3450 1200 50  0000 L CNN
F 2 "MountingHole:MountingHole_3mm_Pad_Via" H 3550 1000 50  0001 C CNN
F 3 "~" H 3550 1000 50  0001 C CNN
	1    3550 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5F5C8BD6
P 3050 1150
F 0 "#PWR022" H 3050 900 50  0001 C CNN
F 1 "GND" H 3055 977 50  0000 C CNN
F 2 "" H 3050 1150 50  0001 C CNN
F 3 "" H 3050 1150 50  0001 C CNN
	1    3050 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1100 2850 1100
Wire Wire Line
	2850 1100 3050 1100
Connection ~ 2850 1100
Wire Wire Line
	3200 1100 3550 1100
Connection ~ 3200 1100
Wire Wire Line
	6100 1500 6100 1700
Wire Wire Line
	6100 1700 6050 1700
Wire Wire Line
	6050 1700 6050 1850
Wire Wire Line
	6100 1700 6150 1700
Wire Wire Line
	6150 1700 6150 1850
Connection ~ 6100 1700
Connection ~ 6150 1700
Wire Wire Line
	6150 1700 6300 1700
Wire Wire Line
	6700 1700 6600 1700
Text Label 7300 2650 0    50   ~ 0
X1
Text Label 7300 2950 0    50   ~ 0
X2
Wire Wire Line
	3050 1150 3050 1100
Connection ~ 3050 1100
Wire Wire Line
	3050 1100 3200 1100
Wire Notes Line
	650  1800 5000 1800
$Comp
L livello_serbatoio_definitiva-rescue:USB_B-Connector J1
U 1 1 5F0BA65A
P 950 2950
F 0 "J1" H 1007 3417 50  0000 C CNN
F 1 "USB_B" H 1007 3326 50  0000 C CNN
F 2 "simboli_custom:USB_B_OST_USB-B1HSxx_Horizontal" H 1100 2900 50  0001 C CNN
F 3 " ~" H 1100 2900 50  0001 C CNN
	1    950  2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  3350 950  3350
Connection ~ 950  3350
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J5
U 1 1 5F8DF7DB
P 6750 5950
F 0 "J5" H 6800 6267 50  0000 C CNN
F 1 "ICSP" H 6800 6176 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 6750 5950 50  0001 C CNN
F 3 "~" H 6750 5950 50  0001 C CNN
	1    6750 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 5850 6550 5850
Wire Wire Line
	6350 5950 6550 5950
Wire Wire Line
	6350 6050 6550 6050
Wire Wire Line
	7050 5850 7300 5850
Wire Wire Line
	7050 5950 7300 5950
Wire Wire Line
	7050 6050 7300 6050
Text GLabel 7650 3700 2    50   Output ~ 0
ICSP_SCK
$Comp
L Connector:Screw_Terminal_01x04 J2
U 1 1 5FA7826E
P 8250 1400
F 0 "J2" H 8350 1350 50  0000 L CNN
F 1 "BTN_LED_GROUP" H 8200 1100 50  0000 L CNN
F 2 "simboli_custom:TerminalBlock_RND_205-00047_1x04_P5.00mm_Horizontal" H 8250 1400 50  0001 C CNN
F 3 "~" H 8250 1400 50  0001 C CNN
	1    8250 1400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x04 J4
U 1 1 5FA7956C
P 10600 1250
F 0 "J4" H 10680 1242 50  0000 L CNN
F 1 "CONN_US" H 10550 950 50  0000 L CNN
F 2 "simboli_custom:TerminalBlock_RND_205-00047_1x04_P5.00mm_Horizontal" H 10600 1250 50  0001 C CNN
F 3 "~" H 10600 1250 50  0001 C CNN
	1    10600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1300 8050 1300
Wire Wire Line
	10150 1150 10400 1150
Wire Wire Line
	10200 1250 10400 1250
Wire Wire Line
	10200 1350 10400 1350
Wire Wire Line
	10150 1450 10400 1450
$Comp
L power:GND #PWR014
U 1 1 5FAE0A2B
P 10250 3150
F 0 "#PWR014" H 10250 2900 50  0001 C CNN
F 1 "GND" V 10255 3022 50  0000 R CNN
F 2 "" H 10250 3150 50  0001 C CNN
F 3 "" H 10250 3150 50  0001 C CNN
	1    10250 3150
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5FAE8CB8
P 10250 4650
F 0 "#PWR025" H 10250 4400 50  0001 C CNN
F 1 "GND" H 10255 4477 50  0000 C CNN
F 2 "" H 10250 4650 50  0001 C CNN
F 3 "" H 10250 4650 50  0001 C CNN
	1    10250 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5FAEB343
P 10250 4250
F 0 "#PWR024" H 10250 4000 50  0001 C CNN
F 1 "GND" V 10255 4122 50  0000 R CNN
F 2 "" H 10250 4250 50  0001 C CNN
F 3 "" H 10250 4250 50  0001 C CNN
	1    10250 4250
	0    1    1    0   
$EndComp
Text Notes 9850 6000 0    50   ~ 0
BLK connected to GND\nRW connected to GND\nVSS connected to GND
Wire Notes Line
	10850 2600 10850 6300
Text Notes 10200 3150 0    50   ~ 0
BLK
Text Notes 10250 4250 0    50   ~ 0
RW
Text Notes 10250 4650 0    50   ~ 0
VSS
$Comp
L Power_Protection:USBLC6-2SC6 U3
U 1 1 5FB09FDB
P 1650 5800
F 0 "U3" H 1400 6150 50  0000 C CNN
F 1 "USBLC6-2SC6" H 2000 6150 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 1650 5300 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/usblc6-2.pdf" H 1850 6150 50  0001 C CNN
	1    1650 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5F8EF1F8
P 1950 2650
F 0 "#PWR023" H 1950 2400 50  0001 C CNN
F 1 "GND" H 1955 2477 50  0000 C CNN
F 2 "" H 1950 2650 50  0001 C CNN
F 3 "" H 1950 2650 50  0001 C CNN
	1    1950 2650
	1    0    0    -1  
$EndComp
Text Label 1700 4350 0    50   ~ 0
GND
Text Label 3650 2350 0    50   ~ 0
+5V
Wire Wire Line
	4250 2750 4350 2750
Wire Wire Line
	4250 2650 4350 2650
Wire Wire Line
	3850 2750 3950 2750
Wire Wire Line
	3850 2650 3950 2650
$Comp
L Device:R R2
U 1 1 5F436A47
P 4100 2750
F 0 "R2" V 4050 2900 50  0000 C CNN
F 1 "1k" V 4100 2750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 4030 2750 50  0001 C CNN
F 3 "~" H 4100 2750 50  0001 C CNN
	1    4100 2750
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5F435E22
P 4100 2650
F 0 "R1" V 4050 2800 50  0000 C CNN
F 1 "1k" V 4100 2650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 4030 2650 50  0001 C CNN
F 3 "~" H 4100 2650 50  0001 C CNN
	1    4100 2650
	0    1    1    0   
$EndComp
Text GLabel 4350 2750 2    50   Input ~ 0
RX
Text GLabel 4350 2650 2    50   Output ~ 0
TX
Text Notes 2550 1950 0    50   ~ 0
USB Programmer
Connection ~ 2150 2150
$Comp
L power:VCC #PWR021
U 1 1 5F57695F
P 2150 2150
F 0 "#PWR021" H 2150 2000 50  0001 C CNN
F 1 "VCC" H 2150 2350 50  0000 C CNN
F 2 "" H 2150 2150 50  0001 C CNN
F 3 "" H 2150 2150 50  0001 C CNN
	1    2150 2150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #PWR01
U 1 1 5F55DD66
P 2150 2150
F 0 "#PWR01" H 2150 2000 50  0001 C CNN
F 1 "PWR_FLAG" V 2150 2277 50  0000 L CNN
F 2 "" H 2150 2150 50  0001 C CNN
F 3 "" H 2150 2150 50  0001 C CNN
	1    2150 2150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2150 2350 2950 2350
Connection ~ 2950 2350
Connection ~ 3150 2350
Wire Wire Line
	3150 2350 3050 2350
NoConn ~ 3850 4050
NoConn ~ 3850 3950
NoConn ~ 3850 3850
Wire Wire Line
	4650 3650 4650 3750
Connection ~ 4650 3650
Wire Wire Line
	4650 2350 4650 3650
Wire Wire Line
	3150 2350 4650 2350
Wire Wire Line
	4000 3750 3850 3750
Wire Wire Line
	4000 3650 3850 3650
Wire Wire Line
	4350 3750 4300 3750
Wire Wire Line
	4350 3650 4300 3650
$Comp
L Device:R R3
U 1 1 5F33668B
P 4500 3650
F 0 "R3" V 4400 3750 50  0000 C CNN
F 1 "1k" V 4500 3650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 4430 3650 50  0001 C CNN
F 3 "~" H 4500 3650 50  0001 C CNN
	1    4500 3650
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5F336185
P 4500 3750
F 0 "R4" V 4600 3850 50  0000 C CNN
F 1 "1k" V 4500 3750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 4430 3750 50  0001 C CNN
F 3 "~" H 4500 3750 50  0001 C CNN
	1    4500 3750
	0    1    1    0   
$EndComp
$Comp
L Device:LED D1
U 1 1 5F335CF4
P 4150 3650
F 0 "D1" H 4250 3700 50  0000 C CNN
F 1 "LED_TX" H 4150 3800 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 4150 3650 50  0001 C CNN
F 3 "~" H 4150 3650 50  0001 C CNN
	1    4150 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5F3348BF
P 4150 3750
F 0 "D2" H 4250 3700 50  0000 C CNN
F 1 "LED_RX" H 4150 3600 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 4150 3750 50  0001 C CNN
F 3 "~" H 4150 3750 50  0001 C CNN
	1    4150 3750
	1    0    0    -1  
$EndComp
NoConn ~ 3850 3150
NoConn ~ 3850 3250
NoConn ~ 3850 3350
Wire Wire Line
	3850 3050 3950 3050
Text GLabel 3950 3050 2    50   Output ~ 0
DTR
Wire Wire Line
	1650 2350 2150 2350
Wire Wire Line
	2150 2150 2150 2350
NoConn ~ 2250 3750
NoConn ~ 2250 3550
Connection ~ 2150 2350
Wire Wire Line
	2150 3350 2150 2350
Wire Wire Line
	2250 3350 2150 3350
Wire Wire Line
	1300 2350 1350 2350
$Comp
L Device:C C2
U 1 1 5F19380D
P 2100 2650
F 0 "C2" V 2250 2600 50  0000 L CNN
F 1 "100nF" V 1950 2550 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 2138 2500 50  0001 C CNN
F 3 "~" H 2100 2650 50  0001 C CNN
	1    2100 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	950  4350 2250 4350
Connection ~ 2250 4350
Wire Wire Line
	3050 4350 3050 4550
Connection ~ 2850 4350
Wire Wire Line
	2250 4350 2850 4350
Wire Wire Line
	2250 4050 2250 4350
Connection ~ 3050 4350
Connection ~ 3150 4350
Wire Wire Line
	3150 4350 3050 4350
Wire Wire Line
	3150 4350 3250 4350
Wire Wire Line
	2850 4350 3050 4350
$Comp
L power:GND #PWR02
U 1 1 5F11910C
P 3050 4550
F 0 "#PWR02" H 3050 4300 50  0001 C CNN
F 1 "GND" H 3055 4377 50  0000 C CNN
F 2 "" H 3050 4550 50  0001 C CNN
F 3 "" H 3050 4550 50  0001 C CNN
	1    3050 4550
	1    0    0    -1  
$EndComp
$Comp
L Interface_USB:FT232RL U1
U 1 1 5F0B8F73
P 3050 3350
F 0 "U1" H 2450 4250 50  0000 C CNN
F 1 "FT232RL" H 3050 3350 50  0000 C CNN
F 2 "Package_SO:SSOP-28_5.3x10.2mm_P0.65mm" H 4150 2450 50  0001 C CNN
F 3 "https://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT232R.pdf" H 3050 3350 50  0001 C CNN
	1    3050 3350
	1    0    0    -1  
$EndComp
Text GLabel 1350 2950 2    50   BiDi ~ 0
D_IN_+
Text GLabel 1350 3050 2    50   BiDi ~ 0
D_IN_-
Text GLabel 1400 2750 2    50   BiDi ~ 0
VBUS
Text GLabel 2100 2950 0    50   BiDi ~ 0
D_OUT_+
Text GLabel 2100 3050 0    50   BiDi ~ 0
D_OUT_-
Text GLabel 1650 5400 1    50   BiDi ~ 0
VBUS
$Comp
L power:GND #PWR0102
U 1 1 5FBB60A6
P 1650 6200
F 0 "#PWR0102" H 1650 5950 50  0001 C CNN
F 1 "GND" H 1655 6027 50  0000 C CNN
F 2 "" H 1650 6200 50  0001 C CNN
F 3 "" H 1650 6200 50  0001 C CNN
	1    1650 6200
	1    0    0    -1  
$EndComp
Text GLabel 1250 5900 0    50   BiDi ~ 0
D_IN_+
Text GLabel 1250 5700 0    50   BiDi ~ 0
D_OUT_+
Text GLabel 2050 5900 2    50   BiDi ~ 0
D_IN_-
Text GLabel 2050 5700 2    50   BiDi ~ 0
D_OUT_-
Wire Wire Line
	1300 2750 1400 2750
Wire Wire Line
	1250 2950 1350 2950
Wire Wire Line
	1250 3050 1350 3050
Wire Wire Line
	2250 2950 2100 2950
Wire Wire Line
	2100 3050 2250 3050
$Comp
L Device:C C5
U 1 1 5FC1FA1B
P 2750 5600
F 0 "C5" H 2950 5600 50  0000 C CNN
F 1 "100nF" H 2500 5600 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 2788 5450 50  0001 C CNN
F 3 "~" H 2750 5600 50  0001 C CNN
	1    2750 5600
	-1   0    0    1   
$EndComp
Text GLabel 2750 5450 1    50   BiDi ~ 0
VBUS
$Comp
L power:GND #PWR0103
U 1 1 5FC24330
P 2750 5750
F 0 "#PWR0103" H 2750 5500 50  0001 C CNN
F 1 "GND" H 2755 5577 50  0000 C CNN
F 2 "" H 2750 5750 50  0001 C CNN
F 3 "" H 2750 5750 50  0001 C CNN
	1    2750 5750
	1    0    0    -1  
$EndComp
Text Notes 1600 5000 0    50   ~ 0
ESD Protection
Wire Wire Line
	950  3350 950  4350
$Comp
L Device:C C1
U 1 1 5F17191B
P 3050 2150
F 0 "C1" H 3165 2196 50  0000 L CNN
F 1 "100nF" H 3165 2105 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 3088 2000 50  0001 C CNN
F 3 "~" H 3050 2150 50  0001 C CNN
	1    3050 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2300 3050 2350
Connection ~ 3050 2350
Wire Wire Line
	3050 2350 2950 2350
$Comp
L power:GND #PWR0104
U 1 1 5FD135C3
P 3050 2000
F 0 "#PWR0104" H 3050 1750 50  0001 C CNN
F 1 "GND" V 3055 1872 50  0000 R CNN
F 2 "" H 3050 2000 50  0001 C CNN
F 3 "" H 3050 2000 50  0001 C CNN
	1    3050 2000
	0    -1   -1   0   
$EndComp
$Comp
L Device:Polyfuse F1
U 1 1 5FB200DA
P 1500 2350
F 0 "F1" V 1400 2350 50  0000 C CNN
F 1 "MF-MSMF050-2" V 1600 2500 50  0000 C CNN
F 2 "simboli_custom:MF-MSMF_1812_4532Metric_Pad1.30x3.40mm_HandSolder" H 1550 2150 50  0001 L CNN
F 3 "~" H 1500 2350 50  0001 C CNN
	1    1500 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	6650 2750 7250 2750
Wire Wire Line
	7250 2750 7250 2650
Wire Wire Line
	7250 2650 7450 2650
$Comp
L Device:C C7
U 1 1 5F1AF8C5
P 7950 2650
F 0 "C7" V 7900 2750 50  0000 C CNN
F 1 "22pF" V 7789 2650 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 7988 2500 50  0001 C CNN
F 3 "~" H 7950 2650 50  0001 C CNN
	1    7950 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	7450 2650 7700 2650
Connection ~ 7450 2650
Wire Wire Line
	7700 2650 7800 2650
Connection ~ 7700 2650
Wire Wire Line
	8100 2650 8100 2800
Wire Wire Line
	8100 2800 8100 2950
Connection ~ 8100 2800
Wire Wire Line
	7800 2950 7700 2950
Wire Wire Line
	7700 2950 7450 2950
Connection ~ 7700 2950
Wire Wire Line
	6650 2850 7250 2850
Wire Wire Line
	7250 2850 7250 2950
Wire Wire Line
	7250 2950 7450 2950
Connection ~ 7450 2950
Text GLabel 7650 3400 2    50   Output ~ 0
SPI_SS
Wire Wire Line
	6650 2650 6800 2650
Text GLabel 6800 2650 2    50   Output ~ 0
D13
Wire Wire Line
	7550 3700 7650 3700
Wire Wire Line
	7650 3600 7550 3600
Wire Wire Line
	6650 2550 6800 2550
Wire Wire Line
	6650 2450 6800 2450
Wire Wire Line
	7650 3500 7550 3500
Text GLabel 6800 2350 2    50   BiDi ~ 0
D10
Wire Wire Line
	6800 2350 6650 2350
Text GLabel 7550 3400 0    50   BiDi ~ 0
D10
Text GLabel 7550 3700 0    50   Output ~ 0
D13
Text GLabel 7550 3600 0    50   Input ~ 0
D12
Text GLabel 7550 3500 0    50   Output ~ 0
D11
Wire Wire Line
	7650 3400 7550 3400
Text GLabel 6800 3050 2    50   Input ~ 0
A0
Text GLabel 6800 3150 2    50   Input ~ 0
A1
Text GLabel 6800 3250 2    50   Input ~ 0
A2
Text GLabel 6800 3350 2    50   Input ~ 0
A3
Text GLabel 6800 3450 2    50   Input ~ 0
A4
Text GLabel 6800 3550 2    50   Input ~ 0
A5
$Comp
L MCU_Microchip_ATmega:ATmega328P-PU U2
U 1 1 5F0B08F0
P 6050 3350
F 0 "U2" H 5650 4800 50  0000 R CNN
F 1 "ATmega328P-PU" H 6250 3600 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 6050 3350 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 6050 3350 50  0001 C CNN
	1    6050 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 3050 6800 3050
Wire Wire Line
	6650 3150 6800 3150
Wire Wire Line
	6650 3250 6800 3250
Wire Wire Line
	6650 3350 6800 3350
Wire Wire Line
	6650 3450 6800 3450
Wire Wire Line
	6650 3550 6800 3550
Text GLabel 8750 4850 0    50   Output ~ 0
A1
Text GLabel 8750 4550 0    50   Output ~ 0
A2
Text GLabel 8750 4250 0    50   Output ~ 0
A3
Text GLabel 7550 4350 0    50   Output ~ 0
A4
Text GLabel 7550 4450 0    50   Output ~ 0
A5
$Comp
L Connector:Conn_01x05_Female J7
U 1 1 5FD28169
P 7850 4250
F 0 "J7" H 7878 4276 50  0000 L CNN
F 1 "Exp_Analog" H 7878 4185 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical" H 7850 4250 50  0001 C CNN
F 3 "~" H 7850 4250 50  0001 C CNN
	1    7850 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5FD3FBF6
P 7550 4150
F 0 "#PWR0105" H 7550 3900 50  0001 C CNN
F 1 "GND" V 7555 4022 50  0000 R CNN
F 2 "" H 7550 4150 50  0001 C CNN
F 3 "" H 7550 4150 50  0001 C CNN
	1    7550 4150
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR0106
U 1 1 5FD4360A
P 7550 4050
F 0 "#PWR0106" H 7550 3900 50  0001 C CNN
F 1 "VCC" V 7550 4250 50  0000 C CNN
F 2 "" H 7550 4050 50  0001 C CNN
F 3 "" H 7550 4050 50  0001 C CNN
	1    7550 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7550 4050 7650 4050
Wire Wire Line
	7550 4150 7650 4150
Wire Wire Line
	7550 4250 7650 4250
Wire Wire Line
	7550 4350 7650 4350
Wire Wire Line
	7550 4450 7650 4450
Text GLabel 7550 4250 0    50   Input ~ 0
SPI_SS
Text GLabel 8750 5150 0    50   Output ~ 0
A0
$Comp
L Jumper:SolderJumper_2_Open JP1
U 1 1 5FDFA447
P 8900 4250
F 0 "JP1" H 8900 4150 50  0000 C CNN
F 1 "A0J" H 8900 4350 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 8900 4250 50  0001 C CNN
F 3 "~" H 8900 4250 50  0001 C CNN
	1    8900 4250
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP2
U 1 1 5FDFEFE6
P 8900 4550
F 0 "JP2" H 8900 4450 50  0000 C CNN
F 1 "A1J" H 8900 4650 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 8900 4550 50  0001 C CNN
F 3 "~" H 8900 4550 50  0001 C CNN
	1    8900 4550
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP3
U 1 1 5FDFF515
P 8900 4850
F 0 "JP3" H 8900 4750 50  0000 C CNN
F 1 "A2J" H 8900 4950 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 8900 4850 50  0001 C CNN
F 3 "~" H 8900 4850 50  0001 C CNN
	1    8900 4850
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP4
U 1 1 5FDFFAFB
P 8900 5150
F 0 "JP4" H 8900 5050 50  0000 C CNN
F 1 "A3J" H 8900 5250 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 8900 5150 50  0001 C CNN
F 3 "~" H 8900 5150 50  0001 C CNN
	1    8900 5150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 5F27E84B
P 4000 5900
F 0 "SW1" V 4000 5850 50  0000 R CNN
F 1 "SW_RST" V 4000 6300 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 4000 6100 50  0001 C CNN
F 3 "~" H 4000 6100 50  0001 C CNN
	1    4000 5900
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C4
U 1 1 5F23FB6A
P 3750 5700
F 0 "C4" V 3498 5700 50  0000 C CNN
F 1 "100nF" V 3589 5700 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 3788 5550 50  0001 C CNN
F 3 "~" H 3750 5700 50  0001 C CNN
	1    3750 5700
	0    1    1    0   
$EndComp
Wire Wire Line
	4350 5850 4350 5700
$Comp
L Connector:Conn_01x04_Female J6
U 1 1 5FCB4005
P 7750 4800
F 0 "J6" H 7800 4800 50  0000 L CNN
F 1 "Exp_Analog_2" H 7800 4700 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 7750 4800 50  0001 C CNN
F 3 "~" H 7750 4800 50  0001 C CNN
	1    7750 4800
	1    0    0    -1  
$EndComp
Text GLabel 7550 4700 0    50   Output ~ 0
A3
Text GLabel 7550 4800 0    50   Output ~ 0
A2
Text GLabel 7550 4900 0    50   Output ~ 0
A1
Text GLabel 7550 5000 0    50   Output ~ 0
A0
$Comp
L power:GND #PWR0107
U 1 1 5FCD810B
P 7850 1000
F 0 "#PWR0107" H 7850 750 50  0001 C CNN
F 1 "GND" H 7855 827 50  0000 C CNN
F 2 "" H 7850 1000 50  0001 C CNN
F 3 "" H 7850 1000 50  0001 C CNN
	1    7850 1000
	-1   0    0    1   
$EndComp
Wire Wire Line
	7750 1300 7850 1300
Connection ~ 7850 1300
$Comp
L Device:R R8
U 1 1 5F0B4EF4
P 7900 1400
F 0 "R8" V 7950 1250 50  0000 C CNN
F 1 "330" V 7900 1400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal" V 7830 1400 50  0001 C CNN
F 3 "~" H 7900 1400 50  0001 C CNN
	1    7900 1400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8050 1500 7750 1500
Wire Notes Line
	3150 4850 3150 6850
Wire Notes Line
	650  4850 5000 4850
Text Notes 3950 5000 0    50   ~ 0
Reset
Wire Notes Line
	7250 1900 7250 600 
Wire Notes Line
	9000 600  9000 1900
Wire Notes Line
	11000 600  11000 1900
Wire Notes Line
	7250 1900 11000 1900
Wire Notes Line
	7250 600  11000 600 
Text Notes 7800 700  0    50   ~ 0
Button + LED
$Comp
L Connector:TestPoint TP1
U 1 1 5FE956AB
P 3850 2850
F 0 "TP1" V 3800 2800 50  0000 L CNN
F 1 "RTS_pad" V 3850 3050 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 4050 2850 50  0001 C CNN
F 3 "~" H 4050 2850 50  0001 C CNN
	1    3850 2850
	0    1    1    0   
$EndComp
$Comp
L Connector:TestPoint TP2
U 1 1 5FE99EB0
P 3850 2950
F 0 "TP2" V 3800 2900 50  0000 L CNN
F 1 "CTS_pad" V 3850 3150 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 4050 2950 50  0001 C CNN
F 3 "~" H 4050 2950 50  0001 C CNN
	1    3850 2950
	0    1    1    0   
$EndComp
$EndSCHEMATC
