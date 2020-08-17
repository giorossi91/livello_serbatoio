EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "schema_scheda"
Date "2020-07-12"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
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
$Comp
L Device:Crystal Y1
U 1 1 5F0B9E0C
P 7500 2800
F 0 "Y1" V 7546 2669 50  0000 R CNN
F 1 "16MHz" V 7455 2669 50  0000 R CNN
F 2 "simboli_custom:Crystal_TH_AS16_5mm" H 7500 2800 50  0001 C CNN
F 3 "~" H 7500 2800 50  0001 C CNN
	1    7500 2800
	0    -1   -1   0   
$EndComp
$Comp
L Connector:USB_B J1
U 1 1 5F0BA65A
P 950 2950
F 0 "J1" H 1007 3417 50  0000 C CNN
F 1 "USB_B" H 1007 3326 50  0000 C CNN
F 2 "Connector_USB:USB_B_OST_USB-B1HSxx_Horizontal" H 1100 2900 50  0001 C CNN
F 3 " ~" H 1100 2900 50  0001 C CNN
	1    950  2950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J2
U 1 1 5F0D6DF0
P 10500 1350
F 0 "J2" H 10500 1050 50  0000 C CNN
F 1 "CONN_US" H 10750 1300 50  0000 C CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x04_P1.00mm_Vertical" H 10500 1350 50  0001 C CNN
F 3 "~" H 10500 1350 50  0001 C CNN
	1    10500 1350
	1    0    0    1   
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
	10150 1150 10300 1150
Wire Wire Line
	10150 1500 10150 1450
Wire Wire Line
	10150 1450 10300 1450
Wire Wire Line
	10300 1250 10200 1250
Wire Wire Line
	10200 1350 10300 1350
$Comp
L power:GND #PWR011
U 1 1 5F0F92C9
P 7850 1850
F 0 "#PWR011" H 7850 1600 50  0001 C CNN
F 1 "GND" H 7855 1677 50  0000 C CNN
F 2 "" H 7850 1850 50  0001 C CNN
F 3 "" H 7850 1850 50  0001 C CNN
	1    7850 1850
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
Wire Wire Line
	6650 2450 6800 2450
Wire Wire Line
	6650 2550 6800 2550
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
Text GLabel 6800 2650 2    50   Output ~ 0
D13
Wire Wire Line
	6650 2650 6800 2650
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
NoConn ~ 6650 2350
NoConn ~ 6650 3050
NoConn ~ 6650 3150
NoConn ~ 6650 3250
NoConn ~ 6650 3350
NoConn ~ 6650 3450
NoConn ~ 6650 3550
$Comp
L Device:C C7
U 1 1 5F1AF8C5
P 7800 2600
F 0 "C7" V 7750 2700 50  0000 C CNN
F 1 "22pF" V 7639 2600 50  0000 C CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 7838 2450 50  0001 C CNN
F 3 "~" H 7800 2600 50  0001 C CNN
	1    7800 2600
	0    1    1    0   
$EndComp
$Comp
L Device:C C8
U 1 1 5F1B005F
P 7800 3050
F 0 "C8" V 7850 3150 50  0000 C CNN
F 1 "22pF" V 7950 3050 50  0000 C CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 7838 2900 50  0001 C CNN
F 3 "~" H 7800 3050 50  0001 C CNN
	1    7800 3050
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
P 7150 2800
F 0 "R7" H 7220 2846 50  0000 L CNN
F 1 "1M" V 7150 2750 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7080 2800 50  0001 C CNN
F 3 "~" H 7150 2800 50  0001 C CNN
	1    7150 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 2750 7050 2750
Wire Wire Line
	7050 2750 7050 2650
Wire Wire Line
	7050 2650 7150 2650
Wire Wire Line
	7150 2650 7500 2650
Connection ~ 7150 2650
Wire Wire Line
	6650 2850 7050 2850
Wire Wire Line
	7050 2850 7050 2950
Wire Wire Line
	7050 2950 7150 2950
Wire Wire Line
	7150 2950 7500 2950
Connection ~ 7150 2950
Wire Wire Line
	7500 2950 7500 3050
Wire Wire Line
	7500 3050 7650 3050
Connection ~ 7500 2950
Wire Wire Line
	7950 3050 8100 3050
Wire Wire Line
	8100 3050 8100 2800
Wire Wire Line
	8100 2600 8100 2800
Wire Wire Line
	7950 2600 8100 2600
Connection ~ 8100 2800
Wire Wire Line
	7650 2600 7500 2600
Wire Wire Line
	7500 2600 7500 2650
Connection ~ 7500 2650
$Comp
L Device:C C5
U 1 1 5F1D4DDB
P 6550 1350
F 0 "C5" V 6298 1350 50  0000 C CNN
F 1 "100nF" V 6389 1350 50  0000 C CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 6588 1200 50  0001 C CNN
F 3 "~" H 6550 1350 50  0001 C CNN
	1    6550 1350
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5F1D54A8
P 6900 1650
F 0 "#PWR08" H 6900 1400 50  0001 C CNN
F 1 "GND" V 6905 1522 50  0000 R CNN
F 2 "" H 6900 1650 50  0001 C CNN
F 3 "" H 6900 1650 50  0001 C CNN
	1    6900 1650
	0    -1   -1   0   
$EndComp
Text GLabel 6800 3650 2    50   Input ~ 0
RESET
Wire Wire Line
	6650 3650 6800 3650
Text GLabel 3800 5700 2    50   Output ~ 0
RESET
$Comp
L Device:R R6
U 1 1 5F204DE3
P 3250 5550
F 0 "R6" V 3350 5500 50  0000 L CNN
F 1 "10k" V 3250 5500 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3180 5550 50  0001 C CNN
F 3 "~" H 3250 5550 50  0001 C CNN
	1    3250 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:D D3
U 1 1 5F205ACA
P 3600 5550
F 0 "D3" V 3554 5630 50  0000 L CNN
F 1 "1N4148TR" V 3645 5630 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 3600 5550 50  0001 C CNN
F 3 "~" H 3600 5550 50  0001 C CNN
	1    3600 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 5400 3250 5400
Wire Wire Line
	3250 5400 3250 5250
Connection ~ 3250 5400
Wire Wire Line
	3250 5700 3600 5700
Wire Wire Line
	3600 5700 3800 5700
Connection ~ 3600 5700
Text GLabel 2700 5700 0    50   Input ~ 0
DTR
$Comp
L Device:C C4
U 1 1 5F23FB6A
P 3000 5700
F 0 "C4" V 2748 5700 50  0000 C CNN
F 1 "100nF" V 2839 5700 50  0000 C CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 3038 5550 50  0001 C CNN
F 3 "~" H 3000 5700 50  0001 C CNN
	1    3000 5700
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 5F24028B
P 2850 5900
F 0 "R5" H 2950 5900 50  0000 L CNN
F 1 "1k" V 2850 5850 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2780 5900 50  0001 C CNN
F 3 "~" H 2850 5900 50  0001 C CNN
	1    2850 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5F2407AF
P 2850 6150
F 0 "#PWR04" H 2850 5900 50  0001 C CNN
F 1 "GND" H 2855 5977 50  0000 C CNN
F 2 "" H 2850 6150 50  0001 C CNN
F 3 "" H 2850 6150 50  0001 C CNN
	1    2850 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5700 2850 5700
Wire Wire Line
	3150 5700 3250 5700
Connection ~ 3250 5700
Wire Wire Line
	2850 5750 2850 5700
Connection ~ 2850 5700
Wire Wire Line
	2850 6050 2850 6150
$Comp
L Switch:SW_Push SW1
U 1 1 5F27E84B
P 2850 5350
F 0 "SW1" V 2850 5350 50  0000 R CNN
F 1 "SW_RST" V 2850 5750 50  0000 R CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 2850 5550 50  0001 C CNN
F 3 "~" H 2850 5550 50  0001 C CNN
	1    2850 5350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2850 5550 2850 5700
Wire Wire Line
	2850 5150 2450 5150
Wire Wire Line
	2450 5150 2450 6050
Wire Wire Line
	2450 6050 2850 6050
Connection ~ 2850 6050
$Comp
L Device:C C3
U 1 1 5F287EEB
P 5350 2000
F 0 "C3" H 5300 1900 50  0000 C CNN
F 1 "100nF" V 5500 2000 50  0000 C CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 5388 1850 50  0001 C CNN
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
P 6550 1750
F 0 "C6" V 6298 1750 50  0000 C CNN
F 1 "100nF" V 6389 1750 50  0000 C CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 6588 1600 50  0001 C CNN
F 3 "~" H 6550 1750 50  0001 C CNN
	1    6550 1750
	0    1    1    0   
$EndComp
$Comp
L Device:L L1
U 1 1 5F0F5110
P 6150 1600
F 0 "L1" H 6203 1646 50  0000 L CNN
F 1 "10uH" H 6203 1555 50  0000 L CNN
F 2 "Inductor_THT:L_Axial_L6.6mm_D2.7mm_P10.16mm_Horizontal_Vishay_IM-2" H 6150 1600 50  0001 C CNN
F 3 "~" H 6150 1600 50  0001 C CNN
	1    6150 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1350 6050 1350
Wire Wire Line
	6150 1450 6150 950 
Wire Wire Line
	6050 1350 6050 950 
Wire Wire Line
	6150 1750 6400 1750
Wire Wire Line
	6700 1350 6900 1350
Wire Wire Line
	6900 1350 6900 1650
Wire Wire Line
	6900 1650 6900 1750
Wire Wire Line
	6900 1750 6700 1750
Connection ~ 6900 1650
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
Wire Wire Line
	2850 4350 3050 4350
Wire Wire Line
	3150 4350 3250 4350
Wire Wire Line
	3150 4350 3050 4350
Connection ~ 3150 4350
Connection ~ 3050 4350
Wire Wire Line
	2250 4050 2250 4350
Wire Wire Line
	2250 4350 2850 4350
Connection ~ 2850 4350
Wire Wire Line
	3050 4350 3050 4550
Wire Wire Line
	850  3350 850  3400
Wire Wire Line
	850  3400 950  3400
Connection ~ 950  3400
Connection ~ 2250 4350
Wire Wire Line
	950  4350 2250 4350
$Comp
L Device:C C1
U 1 1 5F17191B
P 1300 3500
F 0 "C1" H 1415 3546 50  0000 L CNN
F 1 "100nF" H 1415 3455 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 1338 3350 50  0001 C CNN
F 3 "~" H 1300 3500 50  0001 C CNN
	1    1300 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 2750 1300 2750
Wire Wire Line
	1300 3650 950  3650
Connection ~ 950  3650
Wire Wire Line
	950  3650 950  4350
$Comp
L Device:C C2
U 1 1 5F19380D
P 1750 3500
F 0 "C2" H 1865 3546 50  0000 L CNN
F 1 "100nF" H 1865 3455 50  0000 L CNN
F 2 "Capacitor_THT:C_Axial_L3.8mm_D2.6mm_P7.50mm_Horizontal" H 1788 3350 50  0001 C CNN
F 3 "~" H 1750 3500 50  0001 C CNN
	1    1750 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 2650 1750 2650
Wire Wire Line
	1750 2650 1750 3350
Wire Wire Line
	1750 3650 1300 3650
Connection ~ 1300 3650
Wire Wire Line
	1300 2350 1350 2350
Wire Wire Line
	2250 3350 2150 3350
Wire Wire Line
	2150 3350 2150 2350
Connection ~ 2150 2350
NoConn ~ 2250 3550
NoConn ~ 2250 3750
Wire Wire Line
	2150 2150 2150 2350
$Comp
L Device:Fuse F1
U 1 1 5F1C7FD2
P 1500 2350
F 0 "F1" V 1303 2350 50  0000 C CNN
F 1 "500mA" V 1394 2350 50  0000 C CNN
F 2 "Fuse:Fuse_Littelfuse-LVR100" V 1430 2350 50  0001 C CNN
F 3 "~" H 1500 2350 50  0001 C CNN
	1    1500 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 2350 2150 2350
Text GLabel 3950 3050 2    50   Output ~ 0
DTR
Wire Wire Line
	3850 3050 3950 3050
NoConn ~ 3850 3350
NoConn ~ 3850 3250
NoConn ~ 3850 3150
NoConn ~ 3850 2950
NoConn ~ 3850 2850
$Comp
L Device:LED D2
U 1 1 5F3348BF
P 4150 3750
F 0 "D2" H 4250 3700 50  0000 C CNN
F 1 "LED_RX" H 4150 3600 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 4150 3750 50  0001 C CNN
F 3 "~" H 4150 3750 50  0001 C CNN
	1    4150 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5F335CF4
P 4150 3650
F 0 "D1" H 4250 3700 50  0000 C CNN
F 1 "LED_TX" H 4150 3800 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 4150 3650 50  0001 C CNN
F 3 "~" H 4150 3650 50  0001 C CNN
	1    4150 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5F336185
P 4500 3750
F 0 "R4" V 4600 3850 50  0000 C CNN
F 1 "1k" V 4500 3750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4430 3750 50  0001 C CNN
F 3 "~" H 4500 3750 50  0001 C CNN
	1    4500 3750
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5F33668B
P 4500 3650
F 0 "R3" V 4400 3750 50  0000 C CNN
F 1 "1k" V 4500 3650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4430 3650 50  0001 C CNN
F 3 "~" H 4500 3650 50  0001 C CNN
	1    4500 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	4350 3650 4300 3650
Wire Wire Line
	4350 3750 4300 3750
Connection ~ 850  3400
Wire Wire Line
	850  3400 850  3450
Connection ~ 1300 2750
Wire Wire Line
	1300 2750 1300 3350
Wire Wire Line
	950  3400 950  3350
Wire Wire Line
	950  3400 950  3650
Wire Wire Line
	1300 2350 1300 2750
$Comp
L Device:R R9
U 1 1 5F3D6AC6
P 7650 900
F 0 "R9" V 7550 900 50  0000 C CNN
F 1 "1k" V 7650 900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7580 900 50  0001 C CNN
F 3 "~" H 7650 900 50  0001 C CNN
	1    7650 900 
	0    1    1    0   
$EndComp
$Comp
L Device:LED D4
U 1 1 5F3D7411
P 8000 900
F 0 "D4" H 7993 645 50  0000 C CNN
F 1 "LED_ON" H 7993 736 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 8000 900 50  0001 C CNN
F 3 "~" H 8000 900 50  0001 C CNN
	1    8000 900 
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5F3D7EE5
P 8350 900
F 0 "#PWR012" H 8350 650 50  0001 C CNN
F 1 "GND" H 8355 727 50  0000 C CNN
F 2 "" H 8350 900 50  0001 C CNN
F 3 "" H 8350 900 50  0001 C CNN
	1    8350 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 900  7500 900 
Wire Wire Line
	7800 900  7850 900 
Wire Wire Line
	8150 900  8350 900 
Wire Wire Line
	4000 3650 3850 3650
Wire Wire Line
	4000 3750 3850 3750
Wire Wire Line
	3150 2350 4650 2350
Wire Wire Line
	4650 2350 4650 3650
Connection ~ 4650 3650
Wire Wire Line
	4650 3650 4650 3750
NoConn ~ 3850 3850
NoConn ~ 3850 3950
NoConn ~ 3850 4050
Wire Wire Line
	6050 1350 6050 1850
Connection ~ 6050 1350
Text GLabel 6800 4050 2    50   Input ~ 0
D2
Wire Wire Line
	6800 4050 6650 4050
Wire Wire Line
	3150 2350 2950 2350
Connection ~ 3150 2350
Connection ~ 2950 2350
Wire Wire Line
	6150 1850 6150 1750
Connection ~ 6150 1750
Wire Wire Line
	2150 2350 2950 2350
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
$Comp
L power:VCC #PWR09
U 1 1 5F561E2E
P 7450 900
F 0 "#PWR09" H 7450 750 50  0001 C CNN
F 1 "VCC" H 7465 1073 50  0000 C CNN
F 2 "" H 7450 900 50  0001 C CNN
F 3 "" H 7450 900 50  0001 C CNN
	1    7450 900 
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
P 3250 5250
F 0 "#PWR05" H 3250 5100 50  0001 C CNN
F 1 "VCC" H 3265 5423 50  0000 C CNN
F 2 "" H 3250 5250 50  0001 C CNN
F 3 "" H 3250 5250 50  0001 C CNN
	1    3250 5250
	1    0    0    -1  
$EndComp
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
Connection ~ 2150 2150
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5F582FA7
P 6250 1850
F 0 "#FLG01" H 6250 1925 50  0001 C CNN
F 1 "PWR_FLAG" V 6250 1978 50  0000 L CNN
F 2 "" H 6250 1850 50  0001 C CNN
F 3 "~" H 6250 1850 50  0001 C CNN
	1    6250 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	6150 950  6100 950 
$Comp
L power:VCC #PWR0101
U 1 1 5F5AA697
P 6100 950
F 0 "#PWR0101" H 6100 800 50  0001 C CNN
F 1 "VCC" H 6115 1123 50  0000 C CNN
F 2 "" H 6100 950 50  0001 C CNN
F 3 "" H 6100 950 50  0001 C CNN
	1    6100 950 
	1    0    0    -1  
$EndComp
Connection ~ 6100 950 
Wire Wire Line
	6100 950  6050 950 
Wire Wire Line
	6250 1850 6150 1850
Connection ~ 6150 1850
Text Notes 9800 900  0    50   ~ 0
Ultrasonic sensor
Text Notes 9700 2750 0    50   ~ 0
LCD header
Wire Notes Line
	8500 2600 10850 2600
Wire Notes Line
	10850 6300 8500 6300
Wire Notes Line
	9000 800  11000 800 
Wire Notes Line
	11000 800  11000 1800
Wire Notes Line
	750  1800 5000 1800
Wire Notes Line
	5000 1800 5000 6850
Wire Notes Line
	5000 6850 650  6850
Wire Notes Line
	650  6850 650  1800
Wire Notes Line
	650  1800 700  1800
Text Notes 2550 1950 0    50   ~ 0
USB Programmer
Text GLabel 4350 2650 2    50   Output ~ 0
TX
Text GLabel 4350 2750 2    50   Input ~ 0
RX
Text GLabel 6800 3850 2    50   Input ~ 0
TX
Text GLabel 6800 3950 2    50   Output ~ 0
RX
Wire Wire Line
	6650 3850 6800 3850
Wire Wire Line
	6650 3950 6800 3950
Wire Wire Line
	1250 2950 2250 2950
Wire Wire Line
	1250 3050 2250 3050
$Comp
L Device:R R1
U 1 1 5F435E22
P 4100 2650
F 0 "R1" V 4000 2800 50  0000 C CNN
F 1 "1k" V 4100 2650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4030 2650 50  0001 C CNN
F 3 "~" H 4100 2650 50  0001 C CNN
	1    4100 2650
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5F436A47
P 4100 2750
F 0 "R2" V 4200 2900 50  0000 C CNN
F 1 "1k" V 4100 2750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4030 2750 50  0001 C CNN
F 3 "~" H 4100 2750 50  0001 C CNN
	1    4100 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	3850 2650 3950 2650
Wire Wire Line
	3850 2750 3950 2750
Wire Wire Line
	4250 2650 4350 2650
Wire Wire Line
	4250 2750 4350 2750
Wire Wire Line
	8100 1850 7850 1850
Wire Notes Line
	8500 6300 8500 2600
Wire Notes Line
	10850 2600 10850 6300
Text GLabel 9500 4400 2    50   Output ~ 0
BLA
$Comp
L Device:R R10
U 1 1 5F131632
P 9150 4400
F 0 "R10" V 9050 4400 50  0000 C CNN
F 1 "390" V 9150 4400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 9080 4400 50  0001 C CNN
F 3 "~" H 9150 4400 50  0001 C CNN
	1    9150 4400
	0    1    1    0   
$EndComp
Text GLabel 8800 4400 0    50   Input ~ 0
D9
Wire Wire Line
	8800 4400 9000 4400
Wire Wire Line
	9300 4400 9500 4400
$Comp
L power:VCC #PWR018
U 1 1 5F57014C
P 9600 5750
F 0 "#PWR018" H 9600 5600 50  0001 C CNN
F 1 "VCC" H 9615 5923 50  0000 C CNN
F 2 "" H 9600 5750 50  0001 C CNN
F 3 "" H 9600 5750 50  0001 C CNN
	1    9600 5750
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR015
U 1 1 5F56E79B
P 9150 3400
F 0 "#PWR015" H 9150 3250 50  0001 C CNN
F 1 "VCC" H 9165 3573 50  0000 C CNN
F 2 "" H 9150 3400 50  0001 C CNN
F 3 "" H 9150 3400 50  0001 C CNN
	1    9150 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 5750 9800 5750
Text GLabel 9800 5750 2    50   Output ~ 0
VDD
Wire Wire Line
	10500 5650 10700 5650
Connection ~ 10500 5650
Wire Wire Line
	10700 5650 10700 5500
Text GLabel 10700 5500 1    50   Output ~ 0
RW
Wire Wire Line
	9700 5450 9900 5450
Wire Wire Line
	9700 5300 9900 5300
Text GLabel 9900 5450 2    50   Output ~ 0
E
Text GLabel 9900 5300 2    50   Output ~ 0
RS
Text GLabel 9700 5300 0    50   Input ~ 0
D3
Text GLabel 9700 5450 0    50   Input ~ 0
D4
Wire Wire Line
	8950 5150 9100 5150
Wire Wire Line
	8950 5000 9100 5000
Wire Wire Line
	8950 4850 9100 4850
Wire Wire Line
	8950 4700 9100 4700
NoConn ~ 8950 5150
NoConn ~ 8950 5000
NoConn ~ 8950 4850
NoConn ~ 8950 4700
Text GLabel 9100 4700 2    50   Output ~ 0
DB0
Text GLabel 9100 4850 2    50   Output ~ 0
DB1
Text GLabel 9100 5000 2    50   Output ~ 0
DB2
Text GLabel 9100 5150 2    50   Output ~ 0
DB3
Wire Wire Line
	8850 5750 9100 5750
Wire Wire Line
	8850 5600 9100 5600
Wire Wire Line
	8850 5450 9100 5450
Wire Wire Line
	8850 5300 9100 5300
Text GLabel 9100 5750 2    50   Output ~ 0
DB7
Text GLabel 9100 5600 2    50   Output ~ 0
DB6
Text GLabel 9100 5450 2    50   Output ~ 0
DB5
Text GLabel 8850 5750 0    50   Input ~ 0
D8
Text GLabel 8850 5600 0    50   Input ~ 0
D7
Text GLabel 8850 5450 0    50   Input ~ 0
D6
Text GLabel 9100 5300 2    50   Output ~ 0
DB4
Text GLabel 8850 5300 0    50   Input ~ 0
D5
Connection ~ 10400 5650
Wire Wire Line
	10500 5650 10500 5500
Wire Wire Line
	10400 5650 10500 5650
Wire Wire Line
	10300 5650 10400 5650
Wire Wire Line
	10300 5500 10300 5650
Text GLabel 10500 5500 1    50   Output ~ 0
BLK
Text GLabel 10300 5500 1    50   Output ~ 0
VSS
$Comp
L power:GND #PWR014
U 1 1 5F12D258
P 10400 5650
F 0 "#PWR014" H 10400 5400 50  0001 C CNN
F 1 "GND" H 10405 5477 50  0000 C CNN
F 2 "" H 10400 5650 50  0001 C CNN
F 3 "" H 10400 5650 50  0001 C CNN
	1    10400 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 3800 9150 3900
Wire Wire Line
	9300 3650 9450 3650
Wire Wire Line
	9150 3400 9150 3500
Text GLabel 9450 3650 2    50   Output ~ 0
V0
Wire Wire Line
	10300 4750 10400 4750
Wire Wire Line
	10300 4650 10400 4650
Wire Wire Line
	10300 4550 10400 4550
Wire Wire Line
	10300 4450 10400 4450
Wire Wire Line
	10300 4350 10400 4350
Wire Wire Line
	10300 4250 10400 4250
Wire Wire Line
	10300 4150 10400 4150
Wire Wire Line
	10300 4050 10400 4050
Wire Wire Line
	10300 3950 10400 3950
Wire Wire Line
	10300 3850 10400 3850
Wire Wire Line
	10300 3750 10400 3750
Wire Wire Line
	10300 3650 10400 3650
Wire Wire Line
	10300 3550 10400 3550
Wire Wire Line
	10300 3450 10400 3450
Wire Wire Line
	10300 3350 10400 3350
Text GLabel 10300 3350 0    50   Input ~ 0
BLK
Text GLabel 10300 3450 0    50   Input ~ 0
BLA
Text GLabel 10300 3550 0    50   Input ~ 0
DB7
Text GLabel 10300 3650 0    50   Input ~ 0
DB6
Text GLabel 10300 3750 0    50   Input ~ 0
DB5
Text GLabel 10300 3850 0    50   Input ~ 0
DB4
Text GLabel 10300 3950 0    50   Input ~ 0
DB3
Text GLabel 10300 4050 0    50   Input ~ 0
DB2
Text GLabel 10300 4150 0    50   Input ~ 0
DB1
Text GLabel 10300 4250 0    50   Input ~ 0
DB0
Text GLabel 10300 4350 0    50   Input ~ 0
E
Text GLabel 10300 4450 0    50   Input ~ 0
RW
Text GLabel 10300 4550 0    50   Input ~ 0
RS
Text GLabel 10300 4650 0    50   Input ~ 0
V0
Text GLabel 10300 4750 0    50   Input ~ 0
VDD
Wire Wire Line
	10300 4850 10400 4850
$Comp
L Connector:Conn_01x16_Female J3
U 1 1 5F0B738E
P 10600 4150
F 0 "J3" H 10492 3125 50  0000 C CNN
F 1 "CONN_LCD" H 10492 3216 50  0000 C CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x16_P1.00mm_Vertical" H 10600 4150 50  0001 C CNN
F 3 "~" H 10600 4150 50  0001 C CNN
	1    10600 4150
	1    0    0    1   
$EndComp
Text GLabel 10300 4850 0    50   Input ~ 0
VSS
$Comp
L Device:R_POT_TRIM RV1
U 1 1 5F0B6050
P 9150 3650
F 0 "RV1" H 9080 3696 50  0000 R CNN
F 1 "POT_LCDC" H 9080 3605 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Vishay_T73YP_Vertical" H 9150 3650 50  0001 C CNN
F 3 "~" H 9150 3650 50  0001 C CNN
	1    9150 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5F0B54BC
P 9150 3900
F 0 "#PWR016" H 9150 3650 50  0001 C CNN
F 1 "GND" H 9155 3727 50  0000 C CNN
F 2 "" H 9150 3900 50  0001 C CNN
F 3 "" H 9150 3900 50  0001 C CNN
	1    9150 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 1400 7550 1400
Connection ~ 7850 1500
Wire Wire Line
	7550 1500 7850 1500
Wire Wire Line
	7850 1500 8100 1500
Wire Wire Line
	7950 1300 8100 1300
$Comp
L Connector:Conn_01x04_Male J4
U 1 1 5F4B9CB0
P 8300 1500
F 0 "J4" H 8272 1382 50  0000 R CNN
F 1 "BTN_LED_GROUP" H 8272 1473 50  0000 R CNN
F 2 "Connector_Molex:Molex_Micro-Latch_53254-0470_1x04_P2.00mm_Horizontal" H 8300 1500 50  0001 C CNN
F 3 "~" H 8300 1500 50  0001 C CNN
	1    8300 1500
	-1   0    0    1   
$EndComp
Wire Notes Line
	9000 1800 9000 800 
Wire Notes Line
	11000 1800 9000 1800
$Comp
L power:VCC #PWR013
U 1 1 5F56C6EF
P 7550 1400
F 0 "#PWR013" H 7550 1250 50  0001 C CNN
F 1 "VCC" H 7565 1573 50  0000 C CNN
F 2 "" H 7550 1400 50  0001 C CNN
F 3 "" H 7550 1400 50  0001 C CNN
	1    7550 1400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 1300 7650 1300
Text GLabel 7600 1300 0    50   Input ~ 0
D13
Text GLabel 7550 1500 0    50   Output ~ 0
D2
$Comp
L Device:R R8
U 1 1 5F0B4EF4
P 7800 1300
F 0 "R8" V 7900 1300 50  0000 C CNN
F 1 "390" V 7800 1300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7730 1300 50  0001 C CNN
F 3 "~" H 7800 1300 50  0001 C CNN
	1    7800 1300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 5F0B2704
P 7850 1650
F 0 "R11" H 7950 1650 50  0000 L CNN
F 1 "10k" V 7850 1600 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7780 1650 50  0001 C CNN
F 3 "~" H 7850 1650 50  0001 C CNN
	1    7850 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1800 7850 1850
Connection ~ 7850 1850
Wire Wire Line
	8100 1600 8100 1850
$EndSCHEMATC
