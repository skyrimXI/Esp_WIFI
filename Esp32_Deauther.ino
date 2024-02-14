#include <SimpleButton.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SimpleList.h>
#include <EEPROM.h>
//=====================================================================================//
//||                                Buttons & TFT                                    ||//
//=====================================================================================//
using namespace simplebutton;
Button* UP = NULL;
Button* DOWN = NULL;
Button* RIGHT = NULL;
Button* LEFT = NULL;
Button* ACCEPT = NULL;
TFT_eSPI tft;
//=====================================================================================//
//||                                Global Variables                                 ||//
//=====================================================================================//
class NetworkData {
  public:
    String ssid;
    String mac;
    int rssi;
    int channel;
    int encryptionType;
};
SimpleList<NetworkData> scannedNetworks;
uint8_t root_pos = 1;
uint8_t sub_pos = 1;
int sub_posA = 0;
boolean updateDisplay = true;
uint8_t tftHight;
uint8_t tftWidth;
uint8_t textHight;
int rotation;
int Brightness;
int Themes;
//=======================================================================================//
//||                                   UI Setting                                      ||//
//=======================================================================================//
enum pageType {ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, SCAN_MENU, TEST_MENU1, TEST_MENU2, MY_MENU1, MY_MENU2, MY_MENU3, MY_MENU4, MY_MENU5, MY_MENU6, MY_MENU7, MY_MENU8, MY_MENU9, MY_MENU10, MY_MENU11};//SETUP THE enum with all the menu page option
enum pageType currPage = ROOT_MENU;              //holds which page is currently selected
uint16_t colors[][5] = {
  // column---column---column---column--------------------ROW----------------------------------------------------------
  //   0        1       2         3       4
  // DARK     LIGHT   Light1    DARK1
  {0x10D1, 0x0000,  0xFFFF,  0xFC00,  0x0000},       //  0    StatusBarbg = Back Ground Of Status Bar
  {0xFFFF, 0xFFFF,  0x0000,  0xFFFF,  0xFFFF},       //  1    StatusBarTX = Status Bar Text Color
  {0x0000, 0xFFFF,  0x0000,  0x4008,  0xFFE4},       //  2    BackGround = Back Ground Colour
  {0xFFFF, 0x0000,  0xFFFF,  0xFFFF,  0x0210},       //  3    Cursor = Cursor Color
  {0xFFFF, 0x0000,  0xFFFF,  0xFFFF,  0x0000},       //  4    MenuItemTX = Menu's Text Color
  {0x0000, 0xFFFF,  0x0000,  0x0000,  0xFFFF},       //  5    SelectedMenuTX = Menu's Text When Selected
  {0xFFE4, 0x0000,  0xFFFF,  0xFFFF,  0x801F}        //  6    SelectedMenuBG = Back Ground When Menu is selected
};
//---------------------------------------------------------------------------------------------------------------------
uint8_t HoldingInterval = 120;                       //Interval in Between Long Press Action
int BackLight = 12;                                  //PWM/BackLight PIN of Display
int PWM = 0;                                         //Default Value Of Brightness
int MaxTheme = sizeof(colors[0]) / sizeof(colors[0][0]) - 1;
//========================================================================================//
//||                                        VoidSetUP                                   ||//
//========================================================================================//
void setup() {
  tft.init();                                        //Initialize TFT
  Brightness = map(PWM, -3, 3, 3, 255);              //Set Brightness According to Brightness Value
  analogWrite(BackLight, Brightness);                //initilize BackLight according to Brightness Value
  EEPROM.begin(512);                                 //Initialization Of EPPROM
  rotation = EEPROM.read(0);                         //Read Value stoted From "0" this Address of EEPROM
  Themes = EEPROM.read(8);                           //Read Value stoted From "8" this Address of EEPROM
  configureButtons(rotation);                        //Initialization of Buttons
  tft.setRotation(rotation);                         //Rotation of tft (0/1/2/3)
  tft.setTextSize(1);                                //Text Size (1/2/3)
  tft.setTextWrap(false);                            //Text Wrapping (true/false)
  tftHight = tft.height();                           //Set Tft Hight Dynamically according to Oriantation
  tftWidth = tft.width();                            //Set tft width Dynamicall according to oriantation
  textHight = tft.fontHeight();                      //Set Font hight in pixel
  tft.startWrite();                                  // Begin manual display update
  SplashScreen();                                    //Splash Screen Function
}
//==========================================================================================//
//||                                       VoidLoop                                       ||//
//==========================================================================================//
void loop() {
  switch (currPage) {
    case ROOT_MENU:       page_RootMenu(); break;       //Main/Root Menu
    case SUB_MENU1:       page_SubMenu1(); break;       //Menus For Scan Option
    case SUB_MENU2:       page_SubMenu2(); break;       //Menus Sniffer Option
    case SUB_MENU3:       page_SubMenu3(); break;       //Menus Attack Option
    case SCAN_MENU:       page_ScanMenu(); break;       //
    case TEST_MENU1:      page_TestMenu1(); break;      //
    case TEST_MENU2:      page_TestMenu2(); break;      //
    case MY_MENU1:        page_MyMenu1(); break;        //
    case MY_MENU2:        page_MyMenu2(); break;        //List Of SSID
    case MY_MENU3:        page_MyMenu3(); break;        //Information Page
    case MY_MENU4:        page_MyMenu4(); break;        //
    case MY_MENU5:        page_MyMenu5(); break;        //
    case MY_MENU6:        page_MyMenu6(); break;        //
    case MY_MENU7:        page_MyMenu7(); break;        //
    case MY_MENU8:        page_MyMenu8(); break;        //
    case MY_MENU9:        page_MyMenu9(); break;        //
    case MY_MENU10:       page_MyMenu10(); break;       //
    case MY_MENU11:       page_MyMenu11(); break;       //Setting Page
  }
}

//=========================================================================================//
//||                                    ROOT_MENU = MAIN_MENU                            ||//
//=========================================================================================//
void page_RootMenu(void) {
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  while (currPage == ROOT_MENU) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
      StatusBar ("MENUs");
      staticElementsDrawn = true;
    }
    if (updateDisplay) {
      tft.fillRect(0, 28, 150, 98, colors[2][Themes]);
      MenuItems ("SCAN", 30, 1, root_pos);
      MenuItems ("SNIFFER", 42, 2, root_pos);
      MenuItems ("ATTACKs", 54, 3, root_pos);
      MenuItems ("MONITORING", 66, 4, root_pos);
      MenuItems ("HOTSPOT", 78, 5, root_pos);
      MenuItems ("TEMP/CLOCK", 90, 6, root_pos);
      MenuItems ("SETTING", 102, 7, root_pos);
      updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
    //=======================UP button handling=============================//
    if (UP->clicked() || UP->holding(HoldingInterval)) {
      root_pos--;
      if (root_pos < 1) {
        root_pos = 7;
      }
      updateDisplay = true;
    }
    //=========================DOWN button handling==========================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      root_pos++;
      if (root_pos > 7) {
        root_pos = 1;
      }
      updateDisplay = true;
    }
    //======================RIGHT button handling===========================//
    if (RIGHT->clicked()) {}
    //======================LEFT button handling============================//
    if (LEFT->clicked()) {}
    //======================ACCEPT BUTTON HANDLING==========================//
    if (ACCEPT->clicked()) {
      switch (root_pos) {
        case 1: currPage = SUB_MENU1;   break;
        case 2: currPage = MY_MENU11;   break;
        case 3: currPage = MY_MENU11;   break;
        case 4: currPage = MY_MENU11;   break;
        case 5: currPage = MY_MENU11;  break;
        case 6: currPage = MY_MENU11;  break;
        case 7: currPage = MY_MENU11;    break;
      }
      updateDisplay = true;
    }
    while (millis() - loopStartMs < 25) {
      delay(20);
    }
    tft.endWrite();

    delay(10);
  }
}
//==============================================================================================//
//||                                    Submenu1 = Scan option                                ||//
//==============================================================================================//
void page_SubMenu1(void) {
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  while (currPage == SUB_MENU1) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
      StatusBar ("SCAN");
      staticElementsDrawn = true;
    }
    if (updateDisplay) {
      tft.fillRect(0, 28, 150, 98, colors[2][Themes]);
      MenuItems ("BACK", 30, 1, sub_pos);
      MenuItems ("SCAN FOR APs", 42, 2, sub_pos);
      MenuItems ("SCAN STATIONs", 54, 3, sub_pos);
      updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
    //========================UP button handling================================//
    if (UP->clicked() || UP->holding(HoldingInterval)) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos = 3;
      }
      updateDisplay = true;
    }
    //=========================DOWN button handling===========================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 3) {
        sub_pos = 1;
      }
      updateDisplay = true;
    }
    //======================RIGHT button handling===========================//
    if (RIGHT->clicked()) {}
    //======================LEFT button handling============================//
    if (LEFT->clicked()) {}
    //=====================ACCEPT BUTTON HANDLING===========================//
    if (ACCEPT->clicked()) {
      switch (sub_pos) {
        case 1:
          currPage = ROOT_MENU;
          break;                                  //RETURN TO BACK MENU
        case 2:
          scanNetworks();
          currPage = MY_MENU2;                    //CODE TO SCAN FOR APs
          break;
        case 3:
          currPage = MY_MENU11;                   //CODE TO SCAN FOR STATION
          break;
      }
      updateDisplay = true;
    }
    while (millis() - loopStartMs < 25) {
      delay(20);
    }
    tft.endWrite();
    delay(10);
  }
}
//====================================================================================//
void page_SubMenu2(void) {}
//====================================================================================//
void page_SubMenu3(void) {}
//====================================================================================//
void page_ScanMenu(void) {}
//====================================================================================//
void page_TestMenu1(void) {}
//====================================================================================//
void page_TestMenu2(void) {}
//====================================================================================//
void page_MyMenu1(void) {}
//=========================================================================================================//
//||                                         MyMenu2 = SSIDs                                             ||//
//=========================================================================================================//
void page_MyMenu2(void) {
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  while (currPage == MY_MENU2) {
    loopStartMs = millis();
    delay(25);
    if (!staticElementsDrawn) {
      StatusBar ("SSIDs");
      staticElementsDrawn = true;
    }
    if (updateDisplay) {
      tft.fillRect(0, 28, 160, 98, colors[2][Themes]);
      MenuItems("BACK", 30, 0, sub_posA);
      if (scannedNetworks.size() == 0) {
        tft.setCursor(5, 60);
        tft.setTextColor(0xE800, colors[2][Themes]);
        tft.println("No Network Found");
      } else {
        for (int i = 0; i < scannedNetworks.size(); i++) {
          MenuItems(scannedNetworks.get(i).ssid, 42 + i * 12, i + 1, sub_posA);
        }
      }
      updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
    //=============================Up button handing==========================//
    if (UP->clicked() || UP->holding(HoldingInterval)) {
      sub_posA--;
      if (sub_posA < 0) {
        sub_posA = scannedNetworks.size();
      }
      updateDisplay = true;
    }
    //========================DOWN button handling===========================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      sub_posA++;
      if (sub_posA > scannedNetworks.size()) {
        sub_posA = 0;
      }
      updateDisplay = true;
    }
    //======================RIGHT button handling===========================//
    if (RIGHT->clicked()) {}
    //======================LEFT button handling============================//
    if (LEFT->clicked()) {}
    //========================ACCEPT BUTTON HANDLING========================//
    if (ACCEPT->clicked()) {
      if (sub_posA == 0) {
        currPage = SUB_MENU1;
        WiFi.scanDelete();
      } else {
        if (sub_posA > 0 && sub_posA <= scannedNetworks.size()) {
          currPage = MY_MENU3;
        }
      }
      updateDisplay = true;
    }
    while (millis() - loopStartMs < 25) {
      delay(20);
    }
  }
}
//=========================================================================================================//
//||                                    MyMenu3 = Informations                                           ||//
//=========================================================================================================//
void page_MyMenu3(void) {
  boolean staticElementsDrawn = false;
  while (currPage == MY_MENU3) {
    if (!staticElementsDrawn) {
      StatusBar ("INFORMATION");
      staticElementsDrawn = true;
    }
    if (updateDisplay) {
      tft.fillRect(0, 30, 160, 98, colors[2][Themes]);
      tft.setTextColor(colors[4][Themes], colors[2][Themes]);
      tft.setCursor(8, 30);
      tft.println("SSID: " + scannedNetworks.get(sub_posA - 1).ssid);
      tft.setCursor(8, 42);
      tft.println("MAC: " + scannedNetworks.get(sub_posA - 1).mac);
      tft.setCursor(8, 54);
      tft.println("Encryption Type: " + String(scannedNetworks.get(sub_posA - 1).encryptionType));
      tft.setCursor(8, 66);
      tft.println("Channel: " + String(scannedNetworks.get(sub_posA - 1).channel));
      tft.setCursor(8, 78);
      tft.println("RSSI: " + String(scannedNetworks.get(sub_posA - 1).rssi));
      updateDisplay = false;
    }
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
    //=======================UP BUTTON HANDLING=============================//
    if (UP->clicked()) {
      currPage = MY_MENU2; //Back to 2nd Menu
      updateDisplay = true;
    }
    //=======================DOWN BUTTON HANDING============================//
    if (DOWN->clicked()) {
      currPage = MY_MENU2; //Back to 2nd Menu
      updateDisplay = true;
    }
    //======================RIGHT button handling===========================//
    if (RIGHT->clicked()) {}
    //======================LEFT button handling============================//
    if (LEFT->clicked()) {}
    //==========================ACCEPT BUTTON HANDLING======================//
    if (ACCEPT->clicked()) {
      updateDisplay = true; //update just Display
      return;
    }
    delay(10);
  }
}
//====================================================================================//
void page_MyMenu4(void) {}
//====================================================================================//
void page_MyMenu5(void) {}
//====================================================================================//
void page_MyMenu6(void) {}
//====================================================================================//
void page_MyMenu7(void) {}
//====================================================================================//
void page_MyMenu8(void) {}
//====================================================================================//
void page_MyMenu9(void) {}
//====================================================================================//
void page_MyMenu10(void) {}
//=========================================================================================================//
//||                                      MyMenu11 = Settings                                            ||//
//=========================================================================================================//
void page_MyMenu11(void) {
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  while (currPage == MY_MENU11) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
      StatusBar ("SETTINGs");
      staticElementsDrawn = true;
    }
    if (updateDisplay) {
      tft.fillRect(0, 28, 150, 98, colors[2][Themes]);
      MenuItems ("BACK", 30, 1, sub_pos);
      MenuItems ("ROTATION: ", 42, 2, sub_pos);
      tft.setCursor(90, 42);
      if (sub_pos == 2) {
        tft.setTextColor(colors[5][Themes], colors[6][Themes]);
        tft.print(rotation);
      } else {
        tft.setTextColor(colors[4][Themes], colors[2][Themes]);
        tft.print(rotation);
      }
      MenuItems ("BRIGHTNESS: ", 54, 3, sub_pos);
      tft.setCursor(98, 54);
      if (sub_pos == 3) {
        tft.setTextColor(colors[5][Themes], colors[6][Themes]);
        tft.print(PWM);
      } else {
        tft.setTextColor(colors[4][Themes], colors[2][Themes]);
        tft.print(PWM);
      }
      MenuItems ("THEMES: ", 66, 4, sub_pos);
      tft.setCursor(76, 66);
      if (sub_pos == 4) {
        tft.setTextColor(colors[5][Themes], colors[6][Themes]);
        tft.print(Themes);
      } else {
        tft.setTextColor(colors[4][Themes], colors[2][Themes]);
        tft.print(Themes);
      }
      MenuItems ("SAVE SETTING", 102, 5, sub_pos);
      MenuItems ("REBOOT", 114, 6, sub_pos);
      updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
    //========================UP button handling================================//
    if (UP->clicked() || UP->holding(HoldingInterval)) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos = 6;
      }
      updateDisplay = true;
    }
    //=========================DOWN button handling===========================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 6) {
        sub_pos = 1;
      }
      updateDisplay = true;
    }
    //======================RIGHT button handling===========================//
    if (RIGHT->clicked()) {
      switch (sub_pos) {
        case 1:          break;
        case 2:
          rotation++;
          if (rotation > 3) {
            rotation = 0;
          }
          updateDisplay = true;
          break;
        case 3:
          if (PWM < 3) {
            PWM ++;
            Brightness = map(PWM, -3, 3, 3, 255);
            analogWrite(BackLight, Brightness);
            updateDisplay = true;
          }
          break;
        case 4:
          if (Themes < MaxTheme) {
            Themes ++;
            staticElementsDrawn = false;
            updateDisplay = true;
          }
          break;
        case 5:          break;
        case 6:          break;
      }
    }
    //======================LEFT button handling============================//
    if (LEFT->clicked()) {
      switch (sub_pos) {
        case 1:          break;
        case 2:
          rotation--;
          if (rotation < 0) {
            rotation = 3;
          }
          updateDisplay = true;
          break;
        case 3:
          if (PWM > -3) {
            PWM --;
            Brightness = map(PWM, -3, 3, 3, 255);
            analogWrite(BackLight, Brightness);
            updateDisplay = true;
          }
          break;
        case 4:
          if (Themes > 0) {
            Themes --;
            staticElementsDrawn = false;
            updateDisplay = true;
          }
          break;
        case 5:          break;
        case 6:          break;
      }
    }
    //=====================ACCEPT BUTTON HANDLING===========================//
    if (ACCEPT->clicked()) {
      switch (sub_pos) {
        case 1: currPage = ROOT_MENU;                       break;
        case 2:                                             break;
        case 3:                                             break;
        case 4:                                             break;
        case 5:
          EEPROM.write(0, rotation);
          EEPROM.write(8, Themes);
          EEPROM.commit();
          break;
        case 6:
          ESP.restart();
          break;
      }
      updateDisplay = true;
    }
    while (millis() - loopStartMs < 25) {
      delay(20);
    }
    tft.endWrite();
    delay(10);
  }
}
//=========================================================================================================//
//||                                        CUSTOM FUNCTION                                             ||//
//=========================================================================================================//
void StatusBar(const char* Status) {
  tft.fillScreen(colors[2][Themes]);
  tft.fillRoundRect(5, 5, tftWidth - 10, 20, 2, colors[0][Themes]);
  tft.setCursor(10, 10);
  tft.setTextColor(colors[1][Themes], colors[0][Themes]);
  tft.print(Status);
  int vLineX = (rotation == 0 || rotation == 2) ? 88 : 119;
  tft.drawFastVLine(vLineX, 8, 13, colors[1][Themes]);
  int cursorX = (rotation == 0 || rotation == 2) ? 92 : 123;
  tft.setCursor(cursorX, 10);
  tft.printf("%.1f%%", (static_cast<float>(ESP.getFreeHeap()) / ESP.getHeapSize()) * 100);
}
//=========================================================================================================//
void MenuItems(const String& Item, uint8_t p1, uint8_t p2, uint8_t p3) {
  tft.setCursor(0, p1);
  tft.setTextColor((p2 == p3) ? colors[3][Themes] : colors[4][Themes], colors[2][Themes]);
  tft.print((p2 == p3) ? " >> " : "     ");
  tft.fillRoundRect(22, p1 - 2, tftWidth - 40, 11, 2, (p2 == p3) ? colors[6][Themes] : colors[2][Themes]);
  tft.setTextColor((p2 == p3) ? colors[5][Themes] : colors[4][Themes], (p2 == p3) ? colors[6][Themes] : colors[2][Themes]);
  tft.println(Item);
}
//=========================================================================================================//
void scanNetworks() {
  scannedNetworks.clear();
  WiFi.disconnect();
  delay(100);
  WiFi.scanNetworks(true);
  while (WiFi.scanComplete() == WIFI_SCAN_RUNNING) {
    tft.setCursor(10, 114);
    tft.print("SCANNING");
    for (int x = 65; x < tftWidth - 20; x += 3) {
      tft.drawPixel(x, 120, colors[4][Themes]);
      delay(10);
    }
  }
  if (WiFi.scanComplete() > 0) {
    for (int i = 0; i < WiFi.scanComplete(); ++i) {
      NetworkData data;
      data.ssid = WiFi.SSID(i);
      data.mac = WiFi.BSSIDstr(i);
      data.rssi = WiFi.RSSI(i);
      data.channel = WiFi.channel(i);
      data.encryptionType = WiFi.encryptionType(i);
      scannedNetworks.add(data);
    }
  }
}
//=========================================================================================================//
void configureButtons(int rotation) {
  int buttonPins[][4] = {
    {32, 33, 14, 27},
    {14, 27, 33, 32},
    {33, 32, 27, 14},
    {27, 14, 32, 33}
  };
  UP = new ButtonPullup(buttonPins[rotation][0]);
  DOWN = new ButtonPullup(buttonPins[rotation][1]);
  RIGHT = new ButtonPullup(buttonPins[rotation][2]);
  LEFT = new ButtonPullup(buttonPins[rotation][3]);
  ACCEPT = new ButtonPullup(25);
}
//=========================================================================================================//
void SplashScreen() {
  tft.fillScreen(TFT_BLACK);
  int textX, textY;
  if (rotation == 0 || rotation == 2) {
    textX = 50;
    textY = 70;
  } else {
    textX = 65;
    textY = 50;
  }
  tft.setCursor(textX, textY);
  tft.print("ESP32");
  if (rotation == 0 || rotation == 2) {
    textX = 18;
    textY = 80;
  } else {
    textX = 35;
    textY = 60;
  }
  tft.setCursor(textX, textY);
  tft.print("Made By SkyRimXI");
  delay(random(1000, 4500));
  tft.fillScreen(colors[2][Themes]);
}
