## About this Project 🧤
Yet Another handheld Device with WIFI Information Gathering and Some Tricks.
Main purpose of this Project is just to Get Better At Programming in general and Get familier with Esp/Arduino Micro-Controller.
At this stage pls consider mySelf as NOOB and please correct my mistake and make this Project all together and consider changes frequently
Lets keep in touch if anyone want to help me or have some other cool idea or anyone wanna teach me i would love to learn thing related to this projects
## Inspiration / Credits 👏
- [esp8266_deauther](https://github.com/SpacehuhnTech/esp8266_deauther)
- [ESP32Marauder](https://github.com/justcallmekoko/ESP32Marauder)
- [ForOurGood](https://www.youtube.com/@ForOurGood/featured)
## Getting Started 🤞
#### Hardware - at this stage
- Esp32 Wroome Dev-board
- ST7735 160 x 128 1.8 TFT Module
- Tactile Push Module
- External Anttena for Esp32
#### Software - at this stage
- Arduino IDE (1.8.19)
- Esp32 Core (2.0.10)
- Libraries 
  - [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
  - [SimpleList](https://github.com/spacehuhn/SimpleList)
  - [SimpleButton](https://github.com/spacehuhn/SimpleButton)
## Features :
- UI 💻
    - Status Bar to show on which page is currently active
    - Brightness Controll 💡
    - Multiple Themes 🎨
    - Dynamic Rotations 📲
UI Features missing or needs to Implemented (Needs help)
    - Verticle Menus Scrolling
    - Horizontal Scrolling
    - Sroll Bar
    - Bettery Level Monitor 🔋
      - Their is StatusBar UI element on a color and that StatusBar is used to show page name and also ther is a Element to show free heap memory available
        and as status is in a spefic color i want and this statusBar is A rountRect element on TFT and i want to change or Update color of this statusBar according
        to bettery level.
  (Issue am facing as of now how to get Bettery Level and am just aware of Voltage Divider but but i think voltage Divider Circuit Consume Power just for this reason
  i discarded this approach and if anyone has any bettery way to measure voltage please let me know) 
- Application
    - Scan 📡
      - Scan For Aps
      - Scan For Stations 
    - Sniffer
      - Beacon
      - Probe
      - Pmkid
      - RAW 
    - Attack 💀
      - Deauth
      - Phishing
      - Beacon Flood 
    - Monitoring
      - Packet Monitor
      - Channel Monitor 
    - Hotspot 
    - IR Remote 🖱 (still dont have Plans for Implemented)
      - IR Clone
      - IR BruteForce 
    - Temp/Clock ⏰ (still dont have Plans for Implemented)
      - Desk Clock With RealTime Temp, Humidity, Air Pressure 
    - Setting 🛠
      - Rotation Setting (Need Reboot to Effect) 📲
      - Brightness Setting 💡
      - Theme Setting 🎨
      - Save Setting
      - Reboot 📴



