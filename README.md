# NB-IoT Hardware Board and Software

This is the git repository for the book titled "LTE Cellular Narrowband Internet of Things (NB-IoT): Practical Projects for the Cloud and Data Visualization" by CRC press. This git repository contains Arduino, Google maps, and data visualization projects that are explained in the book.

The NB board supports Windows 10, Linux, and Android.

# NB-IoT Hardware Board (NB Board)
The NB-IoT hardware board can be obtained from https://www.5ghub.us


# Software and Tools Included in this Repository
-Chapters: Contains all Arduino sketches for all chapters in the book.

-5G-NB-IoT_Arduino.zip: Contains the library (used with Arduino IDE).

-5G-NB-IoT.zip: Contains the NB board package (used with Arduino IDE).

-Document: Contains Quectel documentations for AT commands.

-Driver: Contains Windows driver for the BG96 chipset.

-Tools: Different tools inclduing Quectel QNavigator that can be used with the BG96 wireless modem.

# How to Use Arduino IDE with the Board

1.	Install Arduino IDE for Windows from the following web site:
https://www.arduino.cc/en/Main/Software

2.	Launch Arduino IDE and choose File->Preferences. In the Additional Boards Manager URLs, insert the following URL:
https://raw.githubusercontent.com/5ghub/NB-IoT/master/package_5G-NB-IoT_index.json

3.	In Arduino IDE, choose Tools->Board->Boards Manager and install “5G-NB-IoT SAMD Board”.

4.	Choose “5G NB-IoT (Native USB Port)”

5.  In the Arduino IDE, Choose Sketch->Include Library->Add .Zip Library and select the file 5G-NB-IoT_Arduino.zip 

6.	You are ready now to use the Arduino IDE and NB board and run the first sketch.

# Other Software Tools used in the Book 

1- Node.js: can be donwloaded from  https://nodejs.org/en/

2- Amazon Web Services Software Development Kit (SDK) for Node.js: can be downloaded from 
https://docs.aws.amazon.com/sdk-for-javascript/v2/developer-guide/installing-jssdk.html

3- Chart.js library: can be donwloaded from https://www.chartjs.org

4- Visual Studio Code: can be downloaded from https://code.visualstudio.com/



