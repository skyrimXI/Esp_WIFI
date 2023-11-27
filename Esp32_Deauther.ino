#include <SimpleButton.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SimpleList.h>
//======================================Button & TFT ==============================================//
using namespace simplebutton;
Button* UP = NULL;
Button* DOWN = NULL;
Button* RIGHT = NULL;
Button* LEFT = NULL;
Button* ACCEPT = NULL;
TFT_eSPI tft;
//=======================================Global Variables==========================================//
int test;                                    //This is for SUB_MENU2 
uint8_t root_pos = 1;
class NetworkData { // Class to store network data
public:
  String ssid;
  String mac;
  int rssi;
  int channel;
  int encryptionType;
};
SimpleList<NetworkData> scannedNetworks; 

//====================================MENUs-Items=================================================//
enum pageType {ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, SCAN_MENU, TEST_MENU1, TEST_MENU2, MY_MENU1, MY_MENU2, MY_MENU3, MY_MENU4, MY_MENU5, MY_MENU6, MY_MENU7, MY_MENU8, MY_MENU9, MY_MENU10, MY_MENU11};   //SETUP THE enum with all the menu page option
enum pageType currPage = ROOT_MENU;                       //holds which page is currently selected
//=================================================================================//
//||                      ScanNetwork & Add Data to List                         ||//
//================================================================================//
void scanNetworks() {
  scannedNetworks.clear();                                // Clear the existing list of scanned networks
  int networksFound = WiFi.scanNetworks();                // Scan for Wi-Fi networks
  if (networksFound == -1) {                              // Check if scanning was successful
    Serial.println("Failed to scan networks.");
    return;
  }
  for (int i = 0; i < networksFound; ++i) {               // Retrieve and add network data to the list
    NetworkData data;
    data.ssid = WiFi.SSID(i);
    data.mac = WiFi.BSSIDstr(i);
    data.rssi = WiFi.RSSI(i);
    data.channel = WiFi.channel(i);
    data.encryptionType = WiFi.encryptionType(i);
    scannedNetworks.add(data);
  }
}
//=================================================================================================//
//||                                        VoidSetUP                                            ||//
//=================================================================================================//
void setup() {
  Serial.begin(115200);                     //SERIAL SETUP
  UP = new ButtonPullup(27);                //Creat a button named UP and its connected to P32
  DOWN = new ButtonPullup(14);              //Creat a button named DOWN and its connected to P33
  RIGHT = new ButtonPullup(33);             //Creat a button named RIGHT and its connected to P14
  LEFT = new ButtonPullup(32);              //Creat a button named LEFT and its connected to P27
  ACCEPT = new ButtonPullup(25);            //Creat a button named OK and its connected to P25
  tft.begin();                              //Initialize TFT
  tft.setRotation(3);                       //Rotation of tft
  tft.fillScreen(TFT_BLACK);                //Background Of tft
  tft.setTextSize(1);                       //Text Size
  tft.startWrite();                         // Begin manual display update
}
//===================================================================================================//
//||                                             VoidLoop                                          ||//
//==================================================================================================//
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
//====================================================================================//
//||                                    ROOT_MENU = MAIN_MENU                       ||// 
//====================================================================================//             
void page_RootMenu(void){
  boolean updateDisplay = true;
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  boolean isUpButtonPressed = false;
  boolean wasUpButtonPressed = false;
  boolean isDownButtonPressed = false;
  boolean wasDownButtonPressed = false;
  boolean isAcceptButtonPressed = false;
  boolean wasAcceptButtonPressed = false;
    while (currPage == ROOT_MENU) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    tft.fillScreen(TFT_BLACK);
    tft.fillRoundRect(5, 5, 118, 20, 2, 0x0410);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_BLACK, 0x0410);
    tft.print("MENUs");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 30, 128, 138, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 30);
  if (root_pos == 1) {  tft.print("|>> ");  tft.println("SCAN");} else {  tft.println("     SCAN");}
  tft.setCursor(0, 40);
  if (root_pos == 2) {  tft.print("|>> "); tft.println("SNIFFER");} else {  tft.println("     SNIFFER");}
  tft.setCursor(0, 50);
  if (root_pos == 3) {  tft.print("|>> ");  tft.println("ATTACKs");} else { tft.println("     ATTACKs");}
  tft.setCursor(0, 60);
  if (root_pos == 4) {  tft.print("|>> ");  tft.println("HOTSPOT");} else {  tft.println("     HOTSPOT");}
  tft.setCursor(0, 70);
  if (root_pos == 5) {  tft.print("|>> ");  tft.println("WAR DRIVE");} else {  tft.println("     WAR DRIVE");}
  tft.setCursor(0, 80);
  if (root_pos == 6) {  tft.print("|>> ");  tft.println("SIGNAL STRENTGH");} else {  tft.println("     SIGNAL STRENTGH");}
  tft.setCursor(0, 90);
  if (root_pos == 7) {  tft.print("|>> ");  tft.println("PACKET MONITOR");} else {  tft.println("     PACKET MONITOR");}
  tft.setCursor(0, 100);
  if (root_pos == 8) {  tft.print("|>> ");  tft.println("REBOOT");} else {  tft.println("     REBOOT");}
      updateDisplay = false;
    }
    tft.startWrite();
//=============================Update buttons=======================//
    UP->update();
    isUpButtonPressed = UP->clicked();
    DOWN->update();
    isDownButtonPressed = DOWN->clicked();
    RIGHT->update();
    LEFT->update();
    ACCEPT->update();
    isAcceptButtonPressed = ACCEPT->clicked();
//========================UP button handling========================//
    if (isUpButtonPressed && !wasUpButtonPressed) {
      root_pos--;
      if (root_pos < 1) {
        root_pos =8;
      }
      updateDisplay = true;
        }
//======================DOWN button handling========================//
    if (isDownButtonPressed && !wasDownButtonPressed) {
      root_pos++;
      if (root_pos > 8) {
        root_pos = 1;
      }
      updateDisplay = true;  
        }  
//==========================ACCEPT BUTTON HANDLING==============//
        if (isAcceptButtonPressed && !wasAcceptButtonPressed) {
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
    wasUpButtonPressed = isUpButtonPressed;
    wasDownButtonPressed = isDownButtonPressed;
    wasAcceptButtonPressed = isAcceptButtonPressed;
    delay(10);  
  }
}

//====================================================================================//
//||                                    Submenu1 = Scan option                       ||// 
//====================================================================================//
void page_SubMenu1(void){
  boolean updateDisplay = true;
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  int sub_pos = 1;
  boolean isUpButtonPressed = false;
  boolean wasUpButtonPressed = false;
  boolean isDownButtonPressed = false;
  boolean wasDownButtonPressed = false;
  boolean isAcceptButtonPressed = false;
  boolean wasAcceptButtonPressed = false;
    while (currPage == SUB_MENU1) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    tft.fillScreen(TFT_BLACK);
    tft.fillRoundRect(5, 5, 118, 20, 2, 0x0410);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_BLACK, 0x0410);
    tft.print("SCAN");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 30, 128, 100, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 30);
  if (sub_pos == 1) {  tft.print("|>> ");  tft.println("BACK");} else {  tft.println("     BACK");}
  tft.setCursor(0, 40);
  if (sub_pos == 2) {  tft.print("|>> "); tft.println("SCAN FOR APs");} else {  tft.println("     SCAN FOR APs");}
  tft.setCursor(0, 50);
  if (sub_pos == 3) {  tft.print("|>> ");  tft.println("SCAN STATIONs");} else { tft.println("     SCAN STATIONs");}
  updateDisplay = false;
    }
    tft.startWrite();
//=============================Update buttons=======================//
    UP->update();
    isUpButtonPressed = UP->clicked();
    DOWN->update();
    isDownButtonPressed = DOWN->clicked();
    RIGHT->update();
    LEFT->update();
    ACCEPT->update();
    isAcceptButtonPressed = ACCEPT->clicked();
//========================UP button handling========================//
    if (isUpButtonPressed && !wasUpButtonPressed) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =3;
      }
      updateDisplay = true;
        }
//======================DOWN button handling========================//
    if (isDownButtonPressed && !wasDownButtonPressed) {
      sub_pos++;
      if (sub_pos > 3) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }  
//==========================ACCEPT BUTTON HANDLING==============//
        if (isAcceptButtonPressed && !wasAcceptButtonPressed) {
        switch (sub_pos) {
        case 1: currPage = ROOT_MENU;   break;    //RETURN TO BACK MENU
        case 2:
          scanNetworks();
          currPage = MY_MENU2;                    //CODE TO SCAN FOR APs
          break;    
        case 3: currPage = MY_MENU3;              //CODE TO SCAN FOR STATION
          break;    
      }
      updateDisplay = true;
    }
      while (millis() - loopStartMs < 25) {
      delay(20);
    }
    tft.endWrite();
    wasUpButtonPressed = isUpButtonPressed;
    wasDownButtonPressed = isDownButtonPressed;
    wasAcceptButtonPressed = isAcceptButtonPressed;
    delay(10);  
  }
}
//====================================================================================//
//||                                    Submenu2 = SNIFF OPTION                       ||// 
//====================================================================================//
void page_SubMenu2(void){
  boolean updateDisplay = true;
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  int sub_pos = 1;
  boolean isUpButtonPressed = false;
  boolean wasUpButtonPressed = false;
  boolean isDownButtonPressed = false;
  boolean wasDownButtonPressed = false;
  boolean isAcceptButtonPressed = false;
  boolean wasAcceptButtonPressed = false;
    while (currPage == SUB_MENU2) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    tft.fillScreen(TFT_BLACK);
    tft.fillRoundRect(5, 5, 118, 20, 2, 0x0410);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_BLACK, 0x0410);
    tft.print("SNIFFER");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 30, 128, 138, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 30);
  if (sub_pos == 1) {  tft.print("|>> ");  tft.println("BACK");} else {  tft.println("     BACK");}
  tft.setCursor(0, 40);
  if (sub_pos == 2) {  tft.print("|>> "); tft.println("BEACON SNIFF");} else {  tft.println("     BEACON SNIFF");}
  tft.setCursor(0, 50);
  if (sub_pos == 3) {  tft.print("|>> ");  tft.println("PROBE SNIFF");} else { tft.println("     PROBE SNIFF");}
  tft.setCursor(0, 60);
  if (sub_pos ==4 ) {  tft.print("|>> ");  tft.println("PMKID");} else { tft.println("     PMKID");}
  tft.setCursor(0, 70);
  if (sub_pos ==5 ) {  tft.print("|>> ");  tft.println("RAW SNIFF");} else { tft.println("     RAW SNIFF");}
  updateDisplay = false;
    }
    tft.startWrite();
//=============================Update buttons=======================//
    UP->update();
    isUpButtonPressed = UP->clicked();
    DOWN->update();
    isDownButtonPressed = DOWN->clicked();
    RIGHT->update();
    LEFT->update();
    ACCEPT->update();
    isAcceptButtonPressed = ACCEPT->clicked();
//========================UP button handling========================//
    if (isUpButtonPressed && !wasUpButtonPressed) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =5;
      }
      updateDisplay = true;
        }
//======================DOWN button handling========================//
    if (isDownButtonPressed && !wasDownButtonPressed) {
      sub_pos++;
      if (sub_pos > 5) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }  
//==========================ACCEPT BUTTON HANDLING==============//
        if (isAcceptButtonPressed && !wasAcceptButtonPressed) {
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
    wasUpButtonPressed = isUpButtonPressed;
    wasDownButtonPressed = isDownButtonPressed;
    wasAcceptButtonPressed = isAcceptButtonPressed;
    delay(10);  
  }
}
//=========================================================================================================//
//||                                    Submenu3 = ATTACKs OPTION                                        ||// 
//=========================================================================================================//
void page_SubMenu3(void){
  boolean updateDisplay = true;
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  int sub_pos = 1;
  boolean isUpButtonPressed = false;
  boolean wasUpButtonPressed = false;
  boolean isDownButtonPressed = false;
  boolean wasDownButtonPressed = false;
  boolean isAcceptButtonPressed = false;
  boolean wasAcceptButtonPressed = false;
    while (currPage == SUB_MENU3) {
    loopStartMs = millis();
    if (!staticElementsDrawn) {
    tft.fillScreen(TFT_BLACK);
    tft.fillRoundRect(5, 5, 118, 20, 2, 0x0410);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_BLACK, 0x0410);
    tft.print("ATTACKs");
    staticElementsDrawn = true; 
  }
  if (updateDisplay) { 
  tft.fillRect(0, 30, 128, 138, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 30);
  if (sub_pos == 1) {  tft.print("|>> ");  tft.println("BACK");} else {  tft.println("     BACK");}
  tft.setCursor(0, 40);
  if (sub_pos == 2) {  tft.print("|>> "); tft.println("DEAUTH");} else {  tft.println("     DEAUTH");}
  tft.setCursor(0, 50);
  if (sub_pos == 3) {  tft.print("|>> ");  tft.println("PHISHING");} else { tft.println("     PHISHING");}
  tft.setCursor(0, 60);
  if (sub_pos ==4 ) {  tft.print("|>> ");  tft.println("MITM");} else { tft.println("     MITM");}
  tft.setCursor(0, 70);
  if (sub_pos ==5 ) {  tft.print("|>> ");  tft.println("BEACON FLOOD");} else { tft.println("     BEACON FLOOD");}
  tft.setCursor(0, 80);
  if (sub_pos ==6 ) {  tft.print("|>> ");  tft.println("PROBE FLOOD");} else { tft.println("     PROBE FLOOD");}
  updateDisplay = false;
    }
    tft.startWrite();
//=============================Update buttons=======================//
    UP->update();
    isUpButtonPressed = UP->clicked();
    DOWN->update();
    isDownButtonPressed = DOWN->clicked();
    RIGHT->update();
    LEFT->update();
    ACCEPT->update();
    isAcceptButtonPressed = ACCEPT->clicked();
//========================UP button handling========================//
    if (isUpButtonPressed && !wasUpButtonPressed) {
      sub_pos--;
      if (sub_pos < 1) {
        sub_pos =6;
      }
      updateDisplay = true;
        }
//======================DOWN button handling========================//
    if (isDownButtonPressed && !wasDownButtonPressed) {
      sub_pos++;
      if (sub_pos > 6) {
        sub_pos = 1;
      }
      updateDisplay = true;  
        }  
//==========================ACCEPT BUTTON HANDLING==============//
        if (isAcceptButtonPressed && !wasAcceptButtonPressed) {
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
    wasUpButtonPressed = isUpButtonPressed;
    wasDownButtonPressed = isDownButtonPressed;
    wasAcceptButtonPressed = isAcceptButtonPressed;
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
//====================================================================================//
void page_MyMenu2(void){
   boolean updateDisplay = true;
  boolean staticElementsDrawn = false;
  uint32_t loopStartMs;
  int sub_pos = 0;
  boolean isUpButtonPressed = false;
  boolean wasUpButtonPressed = false;
  boolean isDownButtonPressed = false;
  boolean wasDownButtonPressed = false;
  boolean isAcceptButtonPressed = false;
  boolean wasAcceptButtonPressed = false;
  while (currPage == MY_MENU2) {
    loopStartMs = millis();
    delay(25);
    if (!staticElementsDrawn) {
    tft.fillScreen(TFT_BLACK);
    tft.fillRoundRect(5, 5, 118, 20, 2, 0x0410);
    tft.setCursor(8, 8);
    tft.setTextColor(TFT_BLACK, 0x0410);
    tft.print("SSIDs");
    staticElementsDrawn = true; // Set the flag to indicate that static elements have been drawn
  }
    if (updateDisplay) {
      tft.fillRect(0, 30, 160, 98, TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setCursor(5, 30);
      if (sub_pos == 0) {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.print("|>> ");
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.println("BACK");
      } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.print("     BACK");
      }

      if (scannedNetworks.size() == 0) {
        tft.setCursor(5, 40);
        tft.setTextColor(0xE800, TFT_BLACK);
        tft.println("No Network Found");
      } else {
        for (int i = 0; i < scannedNetworks.size(); i++) {
          tft.setCursor(5, 40 + i * 10);
          if (sub_pos == i + 1) {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.print("|>> ");
            tft.setTextColor(TFT_YELLOW  , TFT_BLACK);
            tft.println(  scannedNetworks.get(i).ssid);
          } else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
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
//===============================Up button handing============================//
    isUpButtonPressed = UP->clicked();
    if (isUpButtonPressed && !wasUpButtonPressed) {
      wasUpButtonPressed = true;
    }
    if (!isUpButtonPressed && wasUpButtonPressed) {
      wasUpButtonPressed = false;
      sub_pos--;
      if (sub_pos < 0) {
        sub_pos = scannedNetworks.size();
      }
      updateDisplay = true;
    }
//==============================Down Button handling===========================//
    isDownButtonPressed = DOWN->clicked();
    if (isDownButtonPressed && !wasDownButtonPressed) {
      wasDownButtonPressed = true;
    }
    if (!isDownButtonPressed && wasDownButtonPressed) {
      wasDownButtonPressed = false;
      sub_pos++;
      if (sub_pos > scannedNetworks.size()) {
        sub_pos = 0;
      }
      updateDisplay = true;
    }
test = sub_pos;
//================================Accept Button Handling============================//
isAcceptButtonPressed = ACCEPT->clicked();
if (isAcceptButtonPressed && !wasAcceptButtonPressed) {
  wasAcceptButtonPressed = true;
}
if (!isAcceptButtonPressed && wasAcceptButtonPressed) {
  wasAcceptButtonPressed = false;
  if (sub_pos == 0){
    currPage = SUB_MENU1;
    WiFi.scanDelete();
  } else {
    if (sub_pos > 0 && sub_pos <= scannedNetworks.size()) {
    currPage = MY_MENU3;
  }
  }
  updateDisplay = true;
}
     while (millis() - loopStartMs < 25) {
      delay(10);
    }
  }
} 

//====================================================================================//
void page_MyMenu3(void){
  boolean updateDisplay = true;
  boolean staticElementsDrawn = false;
  boolean isUpButtonPressed = false;
  boolean wasUpButtonPressed = false;
  boolean isDownButtonPressed = false;
  boolean wasDownButtonPressed = false;
  boolean isAcceptButtonPressed = false;
  boolean wasAcceptButtonPressed = false;
while (currPage == MY_MENU3){
    if (!staticElementsDrawn) {
    tft.fillScreen(TFT_BLACK);
    tft.fillRoundRect(5, 5, 118, 20, 2, 0x0410);
    tft.setCursor(8, 8);
    tft.setTextColor(TFT_BLACK, 0x0410);
    tft.print("INFORMATION");
    staticElementsDrawn = true; // Set the flag to indicate that static elements have been drawn
  }
    if (updateDisplay){
      tft.fillRect(0, 30, 160, 98, TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setCursor(10, 30);
      tft.println("SSID: " + scannedNetworks.get(test - 1).ssid);
      tft.setCursor(10, 40);
      tft.println("MAC: " + scannedNetworks.get(test - 1).mac);
      tft.setCursor(10, 50);
      tft.println("Encryption Type: " + String(scannedNetworks.get(test - 1).encryptionType));
      tft.setCursor(10, 60);
      tft.println("Channel: " + String(scannedNetworks.get(test - 1).channel));
      tft.setCursor(10, 70);
      tft.println("RSSI: " + String(scannedNetworks.get(test - 1).rssi));
      updateDisplay = false; 
     }
    UP->update();
    isUpButtonPressed = UP->clicked();
    DOWN->update();
    isDownButtonPressed = DOWN->clicked();
    ACCEPT->update();
    isAcceptButtonPressed = ACCEPT->clicked();
//=======================UP BUTTON HANDLING=====================//
        if (isUpButtonPressed && !wasUpButtonPressed) {
          currPage = MY_MENU2; //Back to 2nd Menu
        }
//=======================DOWN BUTTON HANDING====================//
        if (isDownButtonPressed && !wasDownButtonPressed) {
          currPage = MY_MENU2; //Back to 2nd Menu
        }
//==========================ACCEPT BUTTON HANDLING==============//
        if (isAcceptButtonPressed && !wasAcceptButtonPressed) {
          updateDisplay = true; //update just Display
      return;
        }
    wasUpButtonPressed = isUpButtonPressed;
    wasDownButtonPressed = isDownButtonPressed;
    wasAcceptButtonPressed = isAcceptButtonPressed;
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
