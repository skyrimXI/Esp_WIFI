#include <SimpleButton.h>

using namespace simplebutton;

/*
   One button connected to gpio 12 (D6 on the NodeMCU)
   It's the standard wiring as descriped here: https://www.arduino.cc/en/Tutorial/Button
 */

Button* b = NULL;

void setup() {
    Serial.begin(115200);
    Serial.println();

    b = new Button(12);

    b->setOnDoubleClicked([]() {
        Serial.println("doubleclicked");
    });

    b->setOnClicked([]() {
        Serial.println("clicked");
    });

    b->setOnHolding([]() {
        Serial.println("holding");
    });

    Serial.println("Started");
}

void loop() {
    b->update();
}