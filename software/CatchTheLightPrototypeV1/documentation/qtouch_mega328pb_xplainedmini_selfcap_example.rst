INTRODUCTION
============
This is the documentation for the QTouch Mega328PB Xplained Mini Selfcap Example.

Example description
===================
This example demonstates the basic touch application where the on-board touch sensors are measured and the touch status is indicated using LED. The touch library parameters are also displayed in the datavisualizer software when the hardware kit is connected through edbg/medbg vritual com port.

RELATED DOCUMENTS / APPLICATION NOTES
=====================================
Refer the microchip developer page link `"Generate Atmel Board User Project" <http://microchipdeveloper.com/touch:generate-atmel-board-touch-project>`_ and `"Atmega328pb Xplainedmini kit user guide" <http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42469-ATmega328PB-Xplained-Mini_User-Guide.pdf>`_ for more details. 

SUPPORTED EVALUATION KIT
========================
	* `Atmega328pb Xplainedmini kit <http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATMEGA328PB-XMINI>`_

INTERFACE SETTINGS
==================
	* Timer
		* Mode 2 CTC
		* Instance TC0
		* Input clock 8Mhz with prescaler 1024
		* Compare A match interrupt enabled
	* CPUINIT
		* Global Interrupt Enabled
	* Digital Output Pin
		* PB05 as LED0 pin
	* Touch Buttons
		* Y6 (PE2) as button0, Y7 (PE3) as button1
	* Datavisualizer port pins
		* PD0 as TXD, PD1 as RXD

RUNNING THE DEMO
================
1. Select "EXPORT PROJECT" tab and click "Download Pack" to save the .atzip file.
2. Import .atzip file into Atmel Studio 7, File->Import->Atmel Start Project.
3. Build Solution and make sure no compiler errors occur.
4. Press "Start without debugging" or use "CTRL+ALT+F5" hotkeys to run the application.
5. The LED0 turns ON when touch made either of the two touch buttons and turns OFF when touch is removed. 
6. Open Atmel Data Visualizer software and set the config path to the folder that contains the datastreamer scripts. 
7. Open serial port connection and connect to the target. Verify the connection is made successfully and the touch status is displayed on the dashboard. 