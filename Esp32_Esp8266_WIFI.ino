#include <SimpleButton.h>
#include <TFT_eSPI.h>

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
//====================================MENUs-Items=================================================//
enum pageType {ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, SCAN_MENU, TEST_MENU1, TEST_MENU2, MY_MENU1, MY_MENU2, MY_MENU3};   //SETUP THE enum with all the menu page option
enum pageType currPage = ROOT_MENU;         //holds which page is currently selected

//=================================================================================================//
//||                                        VoidSetUP                                            ||//
//=================================================================================================//
void setup() {
  Serial.begin(115200);                     //SERIAL SETUP
  UP = new ButtonPullup(32);                //Creat a button named UP and its connected to P32
  DOWN = new ButtonPullup(33);              //Creat a button named DOWN and its connected to P33
  RIGHT = new ButtonPullup(14);             //Creat a button named RIGHT and its connected to P14
  LEFT = new ButtonPullup(27);              //Creat a button named LEFT and its connected to P27
  ACCEPT = new ButtonPullup(25);               //Creat a button named OK and its connected to P25
  tft.begin();                              //Initialize TFT
  tft.setRotation(4);                       //Rotation of tft
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
  if (root_pos == 3) {  tft.print("|>> ");  tft.println("ATTACK");} else { tft.println("     ATTACK");}
  tft.setCursor(0, 60);
  if (root_pos == 4) {  tft.print("|>> ");  tft.println("WAR DRIVE");} else {  tft.println("     WAR DRIVER");}
  tft.setCursor(0, 70);
  if (root_pos == 5) {  tft.print("|>> ");  tft.println("PACKET MONITOR");} else {  tft.println("     PACKET MONITOR");}
  tft.setCursor(0, 80);
  if (root_pos == 6) {  tft.print("|>> ");  tft.println("SIGNAL STRENTGH");} else {  tft.println("     SIGNAL STRENTGH");}
  tft.setCursor(0, 90);
  if (root_pos == 7) {  tft.print("|>> ");  tft.println("PMKID CAPTURE");} else {  tft.println("     PMKID CAPTURE");}
  tft.setCursor(0, 100);
  if (root_pos == 8) {  tft.print("|>> ");  tft.println("HOTSPOT");} else {  tft.println("     HOTSPOT");}
  tft.setCursor(0, 110);
  if (root_pos == 9) {  tft.print("|>> ");  tft.println("REBOOT");} else {  tft.println("     REBOOT");}
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
        root_pos =9;
      }
      updateDisplay = true;
        }
//======================DOWN button handling========================//
    if (isDownButtonPressed && !wasDownButtonPressed) {
      root_pos++;
      if (root_pos > 9) {
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
        case 8: currPage = MY_MENU2;    break;
        case 9: ESP.restart();          break;
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
  tft.fillRect(0, 30, 128, 138, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 30);
  if (sub_pos == 1) {  tft.print("|>> ");  tft.println("BACK");} else {  tft.println("     BACK");}
  tft.setCursor(0, 40);
  if (sub_pos == 2) {  tft.print("|>> "); tft.println("SCAN FOR APs");} else {  tft.println("     SCAN FOR APs");}
  tft.setCursor(0, 50);
  if (sub_pos == 3) {  tft.print("|>> ");  tft.println("SCAN STATIONs");} else { tft.println("     SCAN STATIONs");}
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
        case 2: currPage = SUB_MENU2;   break;    //CODE TO SCAN FOR APs
        case 3: currPage = SUB_MENU3;   break;    //CODE TO SCAN FOR STATION
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
void page_SubMenu2(void){

}
//====================================================================================//
void page_SubMenu3(void){

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

}
//====================================================================================//
void page_MyMenu3(void){

}
