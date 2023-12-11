#include <SimpleButton.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SimpleList.h>
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
uint8_t HoldingInterval = 120;
//=====================================================================================//
//||                                   Menu Items                                    ||//
//=====================================================================================//
enum pageType {ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, SCAN_MENU, TEST_MENU1, TEST_MENU2, MY_MENU1, MY_MENU2, MY_MENU3, MY_MENU4, MY_MENU5, MY_MENU6, MY_MENU7, MY_MENU8, MY_MENU9, MY_MENU10, MY_MENU11};   //SETUP THE enum with all the menu page option
enum pageType currPage = ROOT_MENU;                       //holds which page is currently selected
//=====================================================================================//
//||                                Colour Setting                                   ||//
//=====================================================================================//
int StatusBarbg = 0x0410;
int StatusBarTX = TFT_BLACK;
int MenuBlock = TFT_BLACK;
int Cursor = TFT_WHITE;
int MenuItemTX = TFT_WHITE;
int SelectedMenuTX = TFT_BLACK;
int SelectedMenuBG = TFT_WHITE;
//=====================================================================================//
//||                                CUSTOM & FUNCTION                                ||//
//=====================================================================================//
//scanNetworks();    THIS IS USED TO SCAN NEARBY WIFI NETWORK AND STORE DATA ON A CLASS
//StatusBar(CHAR);   THIS IS USED TO PRINT A STATUS BAR AT TOP OF THE DISPLAY WITH PAGE NAME



//=====================================================================================//
//||                        ScanNetwork & Add Data to List                           ||//
//=====================================================================================//
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
//========================================================================================//
//||                                        VoidSetUP                                   ||//
//========================================================================================//
void setup() {
  Serial.begin(115200);                              //SERIAL SETUP
  UP = new ButtonPullup(27);                         //Creat a button named UP and its connected to P32
  DOWN = new ButtonPullup(14);                       //Creat a button named DOWN and its connected to P33
  RIGHT = new ButtonPullup(33);                      //Creat a button named RIGHT and its connected to P14
  LEFT = new ButtonPullup(32);                       //Creat a button named LEFT and its connected to P27
  ACCEPT = new ButtonPullup(25);                     //Creat a button named OK and its connected to P25
  tft.begin();                                       //Initialize TFT
  tft.setRotation(3);                                //Rotation of tft
  tft.fillScreen(TFT_BLACK);                         //Background Of tft
  tft.setTextSize(1);                                //Text Size
  tft.startWrite();                                  // Begin manual display update
}
//==========================================================================================//
//||                                       VoidLoop                                       ||//
//==========================================================================================//
void loop(){
  switch (currPage){
    case ROOT_MENU:    page_RootMenu(); break;
    case SUB_MENU1:    page_SubMenu1(); break;
    case SUB_MENU2:    page_SubMenu2(); break;
    case SUB_MENU3:    page_SubMenu3(); break;
    case SCAN_MENU:    page_ScanMenu(); break;
    case TEST_MENU1:   page_TestMenu1(); break;
    case TEST_MENU2:   page_TestMenu2(); break;
    case MY_MENU1:     page_MyMenu1(); break;
    case MY_MENU2:     page_MyMenu2(); break;
    case MY_MENU3:     page_MyMenu3(); break;
    case MY_MENU4:     page_MyMenu4(); break;
    case MY_MENU5:     page_MyMenu5(); break;
    case MY_MENU6:     page_MyMenu6(); break;
    case MY_MENU7:     page_MyMenu7(); break;
    case MY_MENU8:     page_MyMenu8(); break;
    case MY_MENU9:     page_MyMenu9(); break;
    case MY_MENU10:    page_MyMenu10(); break;
    case MY_MENU11:    page_MyMenu11(); break;
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
  tft.setCursor(0, 30);
  if (root_pos == 1) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 29, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);  
    tft.println("SCAN");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     SCAN");
    }
  tft.setCursor(0, 42);
  if (root_pos == 2) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 41, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("SNIFFER");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);  
      tft.println("     SNIFFER");
      }
  tft.setCursor(0, 54);
  if (root_pos == 3) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 53, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("ATTACKs");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     ATTACKs");
      }
  tft.setCursor(0, 66);
  if (root_pos == 4) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 65, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("HOTSPOT");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     HOTSPOT");
      }
  tft.setCursor(0, 78);
  if (root_pos == 5) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 77, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("WAR DRIVE");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     WAR DRIVE");
      }
  tft.setCursor(0, 90);
  if (root_pos == 6) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 89, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("SIGNAL STRENTGH");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     SIGNAL STRENTGH");
      }
  tft.setCursor(0, 102);
  if (root_pos == 7) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 101, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("PACKET MONITOR");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     PACKET MONITOR");
      }
  tft.setCursor(0, 114);
  if (root_pos == 8) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 113, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);  
    tft.println("REBOOT");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     REBOOT");
      }
      updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
//=========================UP-CLICKED button handling===========================//
    if (UP->clicked()) {
      root_pos--;
      if (root_pos < 1) {
        root_pos =8;
      }
      updateDisplay = true;
        }
//=========================UP-HOLDING button handling===========================//
    if (UP->holding(HoldingInterval)) {
      root_pos--;
      if (root_pos < 1) {
        root_pos =8;
      }
      updateDisplay = true;
        }
//======================DOWN button handling===========================//
    if (DOWN->clicked()) {
      root_pos++;
      if (root_pos > 8) {
        root_pos = 1;
      }
      updateDisplay = true;  
        }
//======================DOWN-HOLDING button handling===========================//
    if (DOWN->holding(HoldingInterval)) {
      root_pos++;
      if (root_pos > 8) {
        root_pos = 1;
      }
      updateDisplay = true;  
        }
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {

        }
//======================LEFT button handling============================//
    if (LEFT->clicked()) {

        }
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
        case 8: ESP.restart();          break;
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
  tft.setTextColor(Cursor, MenuBlock);
  tft.setCursor(0, 30);
  if (sub_pos == 1) {
    tft.print("|>> ");
    tft.fillRoundRect(22, 29, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("BACK");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     BACK");
      }
  tft.setCursor(0, 42);
  if (sub_pos == 2) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 41, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("SCAN FOR APs");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     SCAN FOR APs");
      }
  tft.setCursor(0, 54);
  if (sub_pos == 3) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 53, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("SCAN STATIONs");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     SCAN STATIONs");
      }
  updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
//========================UP button handling========================//
    if (UP->clicked()) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =3;
      }
      updateDisplay = true;
        }
//=========================UP-HOLDING button handling===========================//
    if (UP->holding(HoldingInterval)) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =3;
      }
      updateDisplay = true;
        }
//======================DOWN button handling========================//
    if (DOWN->clicked()) {
      sub_pos++;
      if (sub_pos > 3) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================DOWN-HOLDING button handling===========================//
    if (DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 3) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {

        }
//======================LEFT button handling============================//
    if (LEFT->clicked()) {

        }   
//=====================ACCEPT BUTTON HANDLING=====================//
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
  tft.setCursor(0, 30);
  if (sub_pos == 1) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 29, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("BACK");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     BACK");
      }
  tft.setCursor(0, 42);
  if (sub_pos == 2) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 41, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("BEACON SNIFF");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     BEACON SNIFF");
      }
  tft.setCursor(0, 54);
  if (sub_pos == 3) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 53, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("PROBE SNIFF");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     PROBE SNIFF");
      }
  tft.setCursor(0, 66);
  if (sub_pos ==4 ) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 65, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("PMKID");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     PMKID");
      }
  tft.setCursor(0, 78);
  if (sub_pos ==5 ) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 77, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("RAW SNIFF");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     RAW SNIFF");
      }
  updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
//========================UP button handling========================//
    if (UP->clicked()) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =5;
      }
      updateDisplay = true;
        }
//=========================UP-HOLDING button handling===========================//
    if (UP->holding(HoldingInterval)) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =5;
      }
      updateDisplay = true;
        }
//======================DOWN button handling========================//
    if (DOWN->clicked()) {
      sub_pos++;
      if (sub_pos > 5) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================DOWN-HOLDING button handling===========================//
    if (DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 5) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {

        }
//======================LEFT button handling============================//
    if (LEFT->clicked()) {

        } 
//=====================ACCEPT BUTTON HANDLING====================//
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
  tft.setCursor(0, 30);
  if (sub_pos == 1) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 29, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("BACK");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     BACK");
      }
  tft.setCursor(0, 42);
  if (sub_pos == 2) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 41, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("DEAUTH");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     DEAUTH");
      }
  tft.setCursor(0, 54);
  if (sub_pos == 3) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 53, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("PHISHING");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     PHISHING");
      }
  tft.setCursor(0, 66);
  if (sub_pos ==4 ) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 65, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("MITM");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     MITM");
      }
  tft.setCursor(0, 78);
  if (sub_pos ==5 ) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 77, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("BEACON FLOOD");
    } else {
      tft.setTextColor(MenuItemTX, MenuBlock);
      tft.println("     BEACON FLOOD");
      }
  tft.setCursor(0, 90);
  if (sub_pos ==6 ) {
    tft.setTextColor(Cursor, MenuBlock);
    tft.print("|>> ");
    tft.fillRoundRect(22, 89, 120, 10, 2, SelectedMenuBG);
    tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
    tft.println("PROBE FLOOD");
  } else {
    tft.setTextColor(MenuItemTX, MenuBlock);
    tft.println("     PROBE FLOOD");
    }
  updateDisplay = false;
    }
    tft.startWrite();
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
//==========================UP button handling========================//
    if (UP->clicked()) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =6;
      }
      updateDisplay = true;
        }
//=========================UP-HOLDING button handling===========================//
    if (UP->holding(HoldingInterval)) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =6;
      }
      updateDisplay = true;
        }
//=========================DOWN button handling========================//
    if (DOWN->clicked()) {
      sub_pos++;
      if (sub_pos > 6) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================DOWN-HOLDING button handling===========================//
    if (DOWN->holding(HoldingInterval)) {
      sub_pos++;
      if (sub_pos > 6) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {

        }
//======================LEFT button handling============================//
    if (LEFT->clicked()) {

        }  
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
void page_ScanMenu(void){

}
//====================================================================================//
void page_TestMenu1(void){

}
//====================================================================================//
void page_TestMenu2(void){

}
//====================================================================================//
void page_MyMenu1(void){

}
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
      tft.setCursor(0, 30);
      if (sub_posA == 0) {
        tft.setTextColor(Cursor, MenuBlock);
        tft.print("|>> ");
        tft.fillRoundRect(22, 29, 130, 10, 2, SelectedMenuBG);
        tft.setTextColor(SelectedMenuTX, SelectedMenuBG);
        tft.println("BACK");
      } else {
        tft.setTextColor(MenuItemTX, MenuBlock);
        tft.print("     BACK");
      }
      if (scannedNetworks.size() == 0) {
        tft.setCursor(5, 60);
        tft.setTextColor(0xE800, MenuBlock);
        tft.println("No Network Found");
      } else {
        for (int i = 0; i < scannedNetworks.size(); i++) {
          tft.setCursor(0, 42 + i * 12);
          if (sub_posA == i + 1) {
            tft.setTextColor(Cursor, MenuBlock);
            tft.print("|>> ");
            tft.fillRoundRect(22, 41 + i * 12, 130, 10, 2, SelectedMenuBG);
            tft.setTextColor(SelectedMenuTX  , SelectedMenuBG);
            tft.println(  scannedNetworks.get(i).ssid);
          } else {
            tft.setTextColor(MenuItemTX, MenuBlock);
            tft.print("     ");
            tft.println(  scannedNetworks.get(i).ssid);
          }
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
//===============================Up button handing==========================//
  if (UP->clicked()) {
    sub_posA--;
    if (sub_posA < 0) {
    sub_posA = scannedNetworks.size();  
    }
   updateDisplay = true; 
  }
//=========================UP-HOLDING button handling===========================//
  if (UP->holding(HoldingInterval)) {
    sub_posA--;
    if (sub_posA < 0) {
    sub_posA = scannedNetworks.size();  
    }
   updateDisplay = true; 
  }
//============================DOWN button handling===========================//
if (DOWN->clicked()) {
  sub_posA++;
  if (sub_posA > scannedNetworks.size()) {
    sub_posA = 0;
  }
  updateDisplay = true;
}
//======================DOWN-HOLDING button handling===========================//
if (DOWN->holding(HoldingInterval)) {
  sub_posA++;
  if (sub_posA > scannedNetworks.size()) {
    sub_posA = 0;
  }
  updateDisplay = true;
}
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {

        }
//======================LEFT button handling============================//
    if (LEFT->clicked()) {

        }
//==========================ACCEPT BUTTON HANDLING========================//
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
      tft.setCursor(5, 30);
      tft.println("SSID: " + scannedNetworks.get(sub_posA - 1).ssid);
      tft.setCursor(5, 40);
      tft.println("MAC: " + scannedNetworks.get(sub_posA - 1).mac);
      tft.setCursor(5, 50);
      tft.println("Encryption Type: " + String(scannedNetworks.get(sub_posA - 1).encryptionType));
      tft.setCursor(5, 60);
      tft.println("Channel: " + String(scannedNetworks.get(sub_posA - 1).channel));
      tft.setCursor(5, 70);
      tft.println("RSSI: " + String(scannedNetworks.get(sub_posA - 1).rssi));
      updateDisplay = false; 
     }
    UP->update();
    DOWN->update();
    ACCEPT->update();
    RIGHT->update();
    LEFT->update();
//=======================UP BUTTON HANDLING=====================//
        if (UP->clicked()) {
          currPage = MY_MENU2; //Back to 2nd Menu
        }
//=======================DOWN BUTTON HANDING====================//
        if (DOWN->clicked()) {
          currPage = MY_MENU2; //Back to 2nd Menu
        }
//======================RIGHT button handling===========================//
     if (RIGHT->clicked()) {

        }
//======================LEFT button handling============================//
    if (LEFT->clicked()) {

        }
//==========================ACCEPT BUTTON HANDLING==============//
        if (ACCEPT->clicked()) {
          updateDisplay = true; //update just Display
      return;
        }
    delay(10);
    }
}
//====================================================================================//
void page_MyMenu4(void){

}
//====================================================================================//
void page_MyMenu5(void){

}
//====================================================================================//
void page_MyMenu6(void){

}
//====================================================================================//
void page_MyMenu7(void){

}
//====================================================================================//
void page_MyMenu8(void){

}
//====================================================================================//
void page_MyMenu9(void){

}
//====================================================================================//
void page_MyMenu10(void){

}
//====================================================================================//
void page_MyMenu11(void){

}

//=========================================================================================================//
//||                                    DISPLAY PRINT TOOLS                                              ||// 
//=========================================================================================================//

void StatusBar (const char* Status){
    tft.fillScreen(TFT_BLACK);
    tft.fillRoundRect(5, 5, 150, 20, 2, StatusBarbg);
    tft.setCursor(10, 10);
    tft.setTextColor(StatusBarTX, StatusBarbg);
    tft.print(Status);
}
