/*
  Github - https://github.com/skyrimXI/Esp_WIFI
  Colour Picker - https://barth-dev.de/online/rgb565-color-picker/
               - RGB565
  Default Board - LoLIN D32
*/
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
//=======================================================================================//
//||                                   UI Setting                                      ||//
//=======================================================================================//
enum pageType {ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, SCAN_MENU, TEST_MENU1, TEST_MENU2, MY_MENU1, MY_MENU2, MY_MENU3, MY_MENU4, MY_MENU5, MY_MENU6, MY_MENU7, MY_MENU8, MY_MENU9, MY_MENU10, MY_MENU11};//SETUP THE enum with all the menu page option
enum pageType currPage = ROOT_MENU;               //holds which page is currently selected
int StatusBarbg = 0x10D1;                            //Back Ground Of Status Bar
int StatusBarTX = 0xFFFF;                            //Status Bar Text Color
int MenuBlock = TFT_BLACK;                           //Back Ground Colour of Menu's Block 
int Cursor = TFT_WHITE;                              //Cursor Color
int MenuItemTX = TFT_WHITE;                          //Menu's Text Color
int SelectedMenuTX = TFT_BLACK;                      //Menu's Text When Selected 
int SelectedMenuBG = 0xFFE4;                         //Back Ground When Menu is selected
uint8_t HoldingInterval = 120;                       //Interval in Between Long Press Action
int BackLight = 12;                                  //PWM/BackLight PIN of Display 
int PWM = 0;                                         //Default Value Of Brightness
//========================================================================================//
//||                                        VoidSetUP                                   ||//
//========================================================================================//
void setup() {
  Serial.begin(115200);                              //SERIAL SETUP
  pinMode(BackLight, OUTPUT);                        //Initialization of BackLight PWM Pin
  Brightness = map(PWM, -3, 3, 3, 255);              //Set Brightness Initially
  analogWrite(BackLight, Brightness);                //Turn ON display BackLight
  tft.begin();                                       //Initialize TFT
  EEPROM.begin(512);                                 //Initialization Of EPPROM
  rotation = EEPROM.read(0);                         //Read Value stoted From "0" this Address of EEPROM
  configureButtons(rotation);                        //This is a function For Button SetUp
  tft.setRotation(rotation);                         //Rotation of tft (0/1/2/3)
  tft.fillScreen(MenuBlock);                         //Clear tft by fill Color Black
  tft.setTextSize(1);                                //Text Size (1/2/3)
  tft.setTextWrap(false);                            //Text Wrapping (true/false)
  tftHight = tft.height();                           //Set Tft Hight Dynamically according to Oriantation
  tftWidth = tft.width();                            //Set tft width Dynamicall according to oriantation
  textHight = tft.fontHeight();                      //Set Font hight in pixel
  tft.startWrite();                                  // Begin manual display update
  }
//==========================================================================================//
//||                                       VoidLoop                                       ||//
//==========================================================================================//
void loop(){
  switch (currPage){
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
void page_RootMenu(void){
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
    while (currPage == ROOT_MENU) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    StatusBar ("MENUs");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) {
  tft.fillRect(0, 28, 150, 98, MenuBlock);
  MenuItems ("SCAN", 30, 1, root_pos);
  MenuItems ("SNIFFER", 42, 2, root_pos);
  MenuItems ("ATTACKs", 54, 3, root_pos);
  MenuItems ("HOTSPOT", 66, 4, root_pos);
  MenuItems ("WAR DRIVE", 78, 5, root_pos);
  MenuItems ("PACKET MONITOR", 90, 6, root_pos);
  MenuItems ("SIGNAL STRENTGH", 102, 7, root_pos);
  MenuItems ("SETTING", 114, 8, root_pos);
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
        root_pos =8;
      }
      updateDisplay = true;
        }
//=========================DOWN button handling==========================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      root_pos++;
      if (root_pos > 8) {
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
        case 2: currPage = SUB_MENU2;   break;
        case 3: currPage = SUB_MENU3;   break;
        case 4: currPage = SCAN_MENU;   break;
        case 5: currPage = TEST_MENU1;  break;
        case 6: currPage = TEST_MENU2;  break;
        case 7: currPage = MY_MENU1;    break;
        case 8: currPage = MY_MENU11;    break;
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
void page_SubMenu1(void){
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
    while (currPage == SUB_MENU1) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    StatusBar ("SCAN");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 28, 150, 98, MenuBlock);
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
        sub_pos =3;
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
          currPage = MY_MENU3;                    //CODE TO SCAN FOR STATION
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
//==================================================================================================//
//||                                    Submenu2 = SNIFF OPTION                                   ||// 
//==================================================================================================//
void page_SubMenu2(void){
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
    while (currPage == SUB_MENU2) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    StatusBar ("SNIFFER");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 28, 150, 98, MenuBlock);
  MenuItems ("BACK", 30, 1, sub_pos);
  MenuItems ("BEACON SNIFF", 42, 2, sub_pos);
  MenuItems ("PROBE SNIFF", 54, 3, sub_pos);
  MenuItems ("PMKID SNIFF", 66, 4, sub_pos);
  MenuItems ("RAW SNIFF", 78, 5, sub_pos);
  updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
//==========================UP button handling============================//
    if (UP->clicked() || UP->holding(HoldingInterval)) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =5;
      }
      updateDisplay = true;
        }
//======================DOWN button handling=============================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 5) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {}
//======================LEFT button handling============================//
    if (LEFT->clicked()) {}
//=====================ACCEPT BUTTON HANDLING==========================//
        if (ACCEPT->clicked()) {
        switch (sub_pos) {
        case 1: currPage = ROOT_MENU;   break;    //RETURN TO BACK MENU
        case 2: currPage = SUB_MENU2;   break;    //CODE TO SNIFF BEACON FRAME
        case 3: currPage = SUB_MENU3;   break;    //CODE TO SNIFF PROBE FRAME
        case 4: currPage = SUB_MENU3;   break;    //CODE TO SNIFF PMKID
        case 5: currPage = SUB_MENU3;   break;    //CODE TO SNIFF RAW FILEs (PCAP)
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
//||                                    Submenu3 = ATTACKs OPTION                                        ||// 
//=========================================================================================================//
void page_SubMenu3(void){
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
    while (currPage == SUB_MENU3) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    StatusBar ("ATTACKs");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 28, 150, 98, MenuBlock);
  MenuItems ("BACK", 30, 1, sub_pos);
  MenuItems ("DEAUTH", 42, 2, sub_pos);
  MenuItems ("PHISHING", 54, 3, sub_pos);
  MenuItems ("MITM", 66, 4, sub_pos);
  MenuItems ("BEACON FLOOD", 78, 5, sub_pos);
  MenuItems ("PROBE FLOOD", 90, 6, sub_pos);
  updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
//==========================UP button handling============================//
    if (UP->clicked() || UP->holding(HoldingInterval)) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =6;
      }
      updateDisplay = true;
        }
//============================DOWN button handling========================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 6) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {}
//======================LEFT button handling============================//
    if (LEFT->clicked()) {}
//========================ACCEPT BUTTON HANDLING=======================//
        if (ACCEPT->clicked()) {
        switch (sub_pos) {
        case 1: currPage = ROOT_MENU;   break;    //RETURN TO BACK MENU
        case 2: currPage = SUB_MENU2;   break;    //CODE TO SNIFF BEACON FRAME
        case 3: currPage = SUB_MENU3;   break;    //CODE TO SNIFF PROBE FRAME
        case 4: currPage = SUB_MENU3;   break;    //CODE TO SNIFF PMKID
        case 5: currPage = SUB_MENU3;   break;    //CODE TO SNIFF RAW FILEs (PCAP)
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
void page_ScanMenu(void){}
//====================================================================================//
void page_TestMenu1(void){}
//====================================================================================//
void page_TestMenu2(void){}
//====================================================================================//
void page_MyMenu1(void){}
//=========================================================================================================//
//||                                         MyMenu2 = SSIDs                                             ||// 
//=========================================================================================================//
void page_MyMenu2(void){
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
      tft.fillRect(0, 28, 160, 98, MenuBlock);
      MenuItems("BACK", 30, 0, sub_posA);
  if (scannedNetworks.size() == 0) {
    tft.setCursor(5, 60);
    tft.setTextColor(0xE800, MenuBlock);
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
  if (sub_posA == 0){
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
void page_MyMenu3(void){
  boolean staticElementsDrawn = false;
  while (currPage == MY_MENU3){
    if (!staticElementsDrawn) {
    StatusBar ("INFORMATION");
    staticElementsDrawn = true;
  }
    if (updateDisplay){
      tft.fillRect(0, 30, 160, 98, MenuBlock);
      tft.setTextColor(MenuItemTX, MenuBlock);
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
void page_MyMenu4(void){}
//====================================================================================//
void page_MyMenu5(void){}
//====================================================================================//
void page_MyMenu6(void){}
//====================================================================================//
void page_MyMenu7(void){}
//====================================================================================//
void page_MyMenu8(void){}
//====================================================================================//
void page_MyMenu9(void){}
//====================================================================================//
void page_MyMenu10(void){}
//====================================================================================//
void page_MyMenu11(void){
    boolean staticElementsDrawn = false;
    uint32_t loopStartMs;
    while (currPage == MY_MENU11) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    StatusBar ("SETTINGs");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 28, 150, 98, MenuBlock);
  MenuItems ("BACK", 30, 1, sub_pos);
  MenuItems ("ROTATION: ", 42, 2, sub_pos);
  tft.setCursor(90,42);
  if (sub_pos == 2){
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.print(rotation);
  } else {
    tft.setTextColor(MenuItemTX, MenuBlock);
    tft.print(rotation);
  }
  MenuItems ("BRIGHTNESS: ", 54, 3, sub_pos);
  tft.setCursor(98,54);
  if (sub_pos == 3){
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.print(PWM);
  } else {
    tft.setTextColor(MenuItemTX, MenuBlock);
    tft.print(PWM);
  }
  MenuItems ("SAVE SETTING", 102, 4, sub_pos);
  MenuItems ("REBOOT", 114, 5, sub_pos);
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
        sub_pos =5;
      }
      updateDisplay = true;
        }
//=========================DOWN button handling===========================//
    if (DOWN->clicked() || DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 5) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================RIGHT button handling===========================//
    if (RIGHT->clicked()) {
        switch (sub_pos) {
        case 1:
          //   
          break;
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
            }
            Brightness = map(PWM, -3, 3, 3, 255);
            analogWrite(BackLight, Brightness);
            updateDisplay = true;
          break;
        case 4:
          //
          break; 
        case 5:
          //
          break;   
      }
    }
//======================LEFT button handling============================//
    if (LEFT->clicked()) {
        switch (sub_pos) {
        case 1:
          //   
          break;
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
            }
            Brightness = map(PWM, -3, 3, 3, 255);
            analogWrite(BackLight, Brightness);
            updateDisplay = true;
          break;
        case 4:
          //
          break; 
        case 5:
          //
          break;   
      }
    }
//=====================ACCEPT BUTTON HANDLING===========================//
        if (ACCEPT->clicked()) {
        switch (sub_pos) {
        case 1:
          currPage = ROOT_MENU;   
          break;
        case 2:
          //SETTINGS NEED TO CHANGE                             
          break;
        case 3:
          //SETTINGS NEED TO CHANGE                             
          break;
        case 4:
          EEPROM.write(0, rotation);
          EEPROM.commit();
          break; 
        case 5:
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
  tft.fillScreen(MenuBlock);
  tft.fillRoundRect(5, 5, tftWidth - 10, 20, 2, StatusBarbg);
  tft.setCursor(10, 10);
  tft.setTextColor(StatusBarTX, StatusBarbg);
  tft.print(Status);
  int vLineX = (rotation == 0 || rotation == 2) ? 88 : 119;
  tft.drawFastVLine(vLineX, 8, 13, StatusBarTX);
  int cursorX = (rotation == 0 || rotation == 2) ? 92 : 123;
  tft.setCursor(cursorX, 10);
  tft.printf("%.1f%%", (static_cast<float>(ESP.getFreeHeap()) / ESP.getHeapSize()) * 100);
}
//=========================================================================================================//
void MenuItems(const String& Item, uint8_t p1, uint8_t p2, uint8_t p3) {
  tft.setCursor(0, p1);
  tft.setTextColor((p2 == p3) ? Cursor : MenuItemTX, MenuBlock);
  tft.print((p2 == p3) ? " >> " : "     ");
  tft.fillRoundRect(22, p1 - 2, tftWidth - 40, 11, 2, (p2 == p3) ? SelectedMenuBG : MenuBlock);
  tft.setTextColor((p2 == p3) ? SelectedMenuTX : MenuItemTX, (p2 == p3) ? SelectedMenuBG : MenuBlock);
  tft.println(Item);
}
//=========================================================================================================//
void scanNetworks() {
  scannedNetworks.clear();                          // Clear the existing list of scanned networks
  int networksFound = WiFi.scanNetworks();          // Scan for Wi-Fi networks
  if (networksFound == -1) {                        // Check if scanning was successful
    Serial.println("Failed to scan networks.");
    return;
  }
  for (int i = 0; i < networksFound; ++i) {        // Retrieve and add network data to the list
    NetworkData data;
    data.ssid = WiFi.SSID(i);
    data.mac = WiFi.BSSIDstr(i);
    data.rssi = WiFi.RSSI(i);
    data.channel = WiFi.channel(i);
    data.encryptionType = WiFi.encryptionType(i);
    scannedNetworks.add(data);
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
    ACCEPT = new ButtonPullup(25); // Assuming ACCEPT has a fixed pin, in this case, 25
}
