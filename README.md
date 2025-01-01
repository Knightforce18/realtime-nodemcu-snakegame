# realtime-nodemcu-snakegame

# Note: 
The game can be run directly by uploading the [`realtime-nodemcu-snakegame-sketch.ino`](https://github.com/Knightforce18/realtime-nodemcu-snakegame/blob/main/realmtime-nodemcu-snakegame-sketch.ino) to the NodeMCU. Then, visit the display page (http://192.168.4.1) on one device and the controller page (http://192.168.4.1/controller) on another device using a browser.

---

# Table of Contents
1. [Introduction]

2. [Features]

3. [Getting-Started]

4. [How-it-Works]

5. [Screenshots]

6. [License]
---
# Introduction 
The [NodeMCU](https://nodemcu.readthedocs.io/en/release/getting-started/) Snake Game is a simple, interactive game where you controle the classic snake using a web-based controller. The game runs on the [NodeMCU](https://nodemcu.readthedocs.io/en/release/getting-started/), which acts as both an access point and a server. Websocket is used for real-time communication between the game display and the controller.

**This project combines web development, networking,and hardware, showcasing the versatility of NodeMCU.**

---
# Features
* **Quick Setup :** Just upload the code to the NodeMCU,connect your devices to its wifi network and start playing.
  
* **No External Display needed :** The game runs directly on your device's browser, so there is no need for an external display or additonal setup.

* **Real time control:** The game uses WebSocket for real time communication between the controller and dispay, making sure the action is fast and responsive.

*  **Standalone Access Point(AP):** The NodeMCU creates its own Wi-Fi network, so you cna connect your devicees directly to it, without needing an internet connection.

*   **Low Cost Hardware:** The entire game logic runs on the NodeMCU, a low-cost microcontroller that offers multiple functionalities. In this project, it serves as both a server and an access point (AP), providing all the necessary resources for the game to run smoothly without the need for expensive hardware.
  
---
# Getting-Started

To ge the NodeMCU Snake Game working correctly, follow these steps :
## Requirements

### Hardware:
* NodeMCU(ESP8266)
* USB cable to connect the NodeMCU to your Computer
### Software:
* Arduino IDE
* Wi-Fi enabled device(smartphone,laptop,computer)
---
## Steps
### Install the Arduino IDE:
* Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
### Set Up NodeMCU in Arduino IDE:
* Open Arduino IDE and go to : **File > Preferences.**
* In the "Addtional Boards Manager URL's filed, add :  http://arduino.esp8266.com/stable/package_esp8266com_index.json
* Go to : **Tools > Board > Board Manager**, search for **ESP8266**, and install the package.
* **Note:** If you are using **NodeMCU lolin** ,Select **NodeMCU 1.0(ESP-12E Module)** board and if you are using **NodeMCU amica**,select **NodeMCU 0.9(ESP-12 Module)** board from **Tools > Board**
### Upload the Sketch:
* Download the [realtime-nodemcu-snakegame-sketch.ino](https://github.com/Knightforce18/realtime-nodemcu-snakegame/blob/main/realmtime-nodemcu-snakegame-sketch.ino) file from the [github repo](https://github.com/Knightforce18/realtime-nodemcu-snakegame).
* Connect your NodeMCU to your computer using a USB cable.
* Select the correct port from Tools > Port.
* 	Click the Upload button(right arrow like button) to **upload** the sketch to the NodeMCU.
### Connect to the NodeMCU Wi-Fi Network:
* Once the upload is complete, the NodeMCU will create a Wi-Fi network named : **MiniProject** having password : **911911911**
  
  Note : You can change the ssid and password according to you by chaning these lines in the sketch code:
  
  const char* ssid = "MiniProject"; <br>
  const char* password = "911911911";
* Connect your phone and a laptop to this network.

### Start Playing
* open the browser and visit  http://192.168.4.1 to see the game display.
* On another device,visit  http://192.168.4.1/controller to open the controller page.
* Start controlling the snake and beat your high score!!
  
  Note : for Display page A big display is preffered like laptop or computer.
---
  
  











