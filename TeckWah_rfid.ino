/* Author: Benjamin Low
 *
 * Description: Code adapted from http://bildr.org/2011/02/rfid-arduino/
 * Uses the Sparkfun ID-20 RFID reader connected to an Arduino. See above
 * URL link for the wiring connections. Blinks some LEDS if any of the
 * pre-registered RFID tags are read. One RFID reader per Arduino.
 *
 * Last updated: 23 Jan 2016
 */

const long LED_DELAY = 250;
const long RESET_DELAY = 150;

long curr_led2_time, curr_led3_time, curr_led4_time, curr_led5_time;
boolean is_led2_on, is_led3_on, is_led4_on, is_led5_on;

int RFIDResetPin = 13;
int tag_number, prev_tag_number;

//Register your RFID tags here
char tag1[13] = "50009EC3676A";
char tag2[13] = "4B0082C35359";
char tag3[13] = "4D006A6E4D04";
char tag4[13] = "50009EEA91B5";

void setup() {
  Serial.begin(9600);

  pinMode(RFIDResetPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);

  //LEDs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {

  char tagString[13];
  int index = 0;
  boolean reading = false;

  while (Serial.available()) {

    int readByte = Serial.read(); //read next available byte

    if (readByte == 2) reading = true; //begining of tag
    if (readByte == 3) reading = false; //end of tag

    if (reading && readByte != 2 && readByte != 10 && readByte != 13) {
      //store the tag
      tagString[index] = readByte;
      index ++;
    }
  }

  checkTag(tagString); //Check if it is a match and updates the current tag number

  if (tag_number != prev_tag_number) { //only sends when a tag has changed
 
          Serial.print("tag");
          Serial.println(tag_number);
          prev_tag_number = tag_number;  
  }

  clearTag(tagString); //Clear the char of all value

  resetReader(); //reset both RFID readers
}

void checkTag(char tag[]) {
  ///////////////////////////////////
  //Check the read tag against known tags
  ///////////////////////////////////

  if (strlen(tag) == 0) {
    turn_off_leds();
    return; //empty, no need to contunue
  }

  if (compareTag(tag, tag1)) { // if matched tag1, do this
    lightLED(2);
    tag_number = 1;

  } else if (compareTag(tag, tag2)) { 
    lightLED(3);
    tag_number = 2;

  } else if (compareTag(tag, tag3)) {
    lightLED(4);
    tag_number = 3;

  } else if (compareTag(tag, tag4)) {
    lightLED(5);
    tag_number = 4;
  }
  else {
    //unknown tag
      Serial.print("unknown tag: ");
      Serial.println(tag); //read out any unknown tag
  }
}

void lightLED(int pin) {
  ///////////////////////////////////
  //Turn on LED on pin "pin" for 250ms
  ///////////////////////////////////
  if (pin == 2) {
    if (is_led2_on == false) {
      digitalWrite(2, HIGH);
      curr_led2_time = millis();
      is_led2_on = true;
    }
    else if (millis() - curr_led2_time > LED_DELAY) {
      digitalWrite(2, LOW);
      is_led2_on = false;
    }
  }

  if (pin == 3) {
    if (is_led3_on == false) {
      digitalWrite(3, HIGH);
      curr_led3_time = millis();
      is_led3_on = true;
    }
    else if (millis() - curr_led3_time > LED_DELAY) {
      digitalWrite(3, LOW);
      is_led3_on = false;
    }
  }

  if (pin == 4) {
    if (is_led4_on == false) {
      digitalWrite(4, HIGH);
      curr_led4_time = millis();
      is_led4_on = true;
    }
    else if (millis() - curr_led4_time > LED_DELAY) {
      digitalWrite(4, LOW);
      is_led4_on = false;
    }
  }

  if (pin == 5) {
    if (is_led5_on == false) {
      digitalWrite(5, HIGH);
      curr_led5_time = millis();
      is_led5_on = true;
    }
    else if (millis() - curr_led5_time > LED_DELAY) {
      digitalWrite(5, LOW);
      is_led5_on = false;
    }
  }
}

void turn_off_leds() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  is_led2_on = false;
  is_led3_on = false;
  is_led4_on = false;
  is_led5_on = false;
}

void resetReader() {
  ///////////////////////////////////
  //Reset the RFID reader to read again.
  ///////////////////////////////////
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(RESET_DELAY);
}

void clearTag(char one[]) {
  ///////////////////////////////////
  //clear the char array by filling with null - ASCII 0
  //Will think same tag has been read otherwise
  ///////////////////////////////////
  for (int i = 0; i < strlen(one); i++) {
    one[i] = 0;
  }
}

boolean compareTag(char one[], char two[]) {
  ///////////////////////////////////
  //compare two value to see if same,
  //strcmp not working 100% so we do this
  ///////////////////////////////////

  if (strlen(one) == 0) return false; //empty

  for (int i = 0; i < 12; i++) {
    if (one[i] != two[i]) return false;
  }

  return true; //no mismatches
}
