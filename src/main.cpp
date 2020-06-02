#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include "Keyboard.h"

//#include <cstdlib>

#define BUILTIN_LED 13


/*
  Matrix keypad mapping
*/
const byte n_rows = 4;
const byte n_cols = 4;

char matrix[n_rows][n_cols] = {
                               {'1','2','3','A'},
                               {'4','5','6','B'},
                               {'7','8','9','C'},
                               {'*','0','#','D'}
};

// Define keypad pins
byte colPins[n_cols] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad
byte rowPins[n_rows] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(matrix), rowPins, colPins, n_rows, n_cols); 

/*
  Initialize commands
*/
String cmd1, cmd2, cmd3, cmd4, cmd5, cmd6, cmd7, cmd8, cmd9, cmd0;
String cmdStar, cmdHash, cmdA, cmdB, cmdC, cmdD;

/*
  Keyboard utils
*/
void pressKey(char key, bool hold, bool release) {
  if (!hold && !release) {
    Keyboard.press(key);
    delay(10);
    Keyboard.release(key);
  } else {
    if (hold) {
      Keyboard.press(key);
    } else if (release) {
      Keyboard.release(key);
    }
  }
}

/*
  Command parsing
*/
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void parseCommand(String cmd) {
  char *str = cmd.c_str();
  char *pch;
  int defaultDelay = 0;
  pch = strtok (str," ");
  while (pch != NULL)
    {
      String str = pch;

      bool hold = false;
      bool release = false;

      if (str.startsWith("+") && str.length() > 1) {
        hold = true;
        str = str.substring(1);
      } else if (str.startsWith("-") && str.length() > 1) {
        release = true;
        str = str.substring(1);
      }

      if (str == "SPC") { Keyboard.print(" ");
      } else if (str == "L_CTRL") { pressKey(KEY_LEFT_CTRL, hold, release);
      } else if (str == "L_ALT") { pressKey(KEY_LEFT_ALT, hold, release);
      } else if (str == "L_SHIFT") { pressKey(KEY_LEFT_SHIFT, hold, release);
      } else if (str == "R_CTRL") { pressKey(KEY_RIGHT_CTRL, hold, release);
      } else if (str == "R_ALT") { pressKey(KEY_RIGHT_ALT, hold, release);
      } else if (str == "R_SHIFT") { pressKey(KEY_RIGHT_SHIFT, hold, release);

      } else if (str == "L_WIN") { pressKey(KEY_LEFT_GUI, hold, release);
      } else if (str == "R_WIN") { pressKey(KEY_RIGHT_GUI, hold, release);

      } else if (str == "L_ARROW") { pressKey(KEY_LEFT_ARROW, hold, release);
      } else if (str == "R_ARROW") { pressKey(KEY_RIGHT_ARROW, hold, release);
      } else if (str == "U_ARROW") { pressKey(KEY_UP_ARROW, hold, release);
      } else if (str == "D_ARROW") { pressKey(KEY_DOWN_ARROW, hold, release);

      } else if (str == "DEL") { pressKey(KEY_DELETE, hold, release);
      } else if (str == "ENTER") { pressKey(KEY_RETURN, hold, release);
      } else if (str == "RET") { pressKey(KEY_RETURN, hold, release);
      } else if (str == "ESC") { pressKey(KEY_ESC, hold, release);
      } else if (str == "BACKSPC") { pressKey(KEY_BACKSPACE, hold, release);
      } else if (str == "BACK") { pressKey(KEY_BACKSPACE, hold, release);
      } else if (str == "TAB") { pressKey(KEY_TAB, hold, release);
      } else if (str == "INS") { pressKey(KEY_INSERT, hold, release);
      } else if (str == "PG_UP") { pressKey(KEY_PAGE_UP, hold, release);
      } else if (str == "PG_DOWN") { pressKey(KEY_PAGE_DOWN, hold, release);
      } else if (str == "HOME") { pressKey(KEY_HOME, hold, release);
      } else if (str == "END") { pressKey(KEY_END, hold, release);
      } else if (str == "CAPS") { pressKey(KEY_CAPS_LOCK, hold, release);
      } else if (str == "CAPSLOCK") { pressKey(KEY_CAPS_LOCK, hold, release);

      } else if (str == "F1") { pressKey(KEY_F1, hold, release);
      } else if (str == "F2") { pressKey(KEY_F2, hold, release);
      } else if (str == "F3") { pressKey(KEY_F3, hold, release);
      } else if (str == "F4") { pressKey(KEY_F4, hold, release);
      } else if (str == "F5") { pressKey(KEY_F5, hold, release);
      } else if (str == "F6") { pressKey(KEY_F6, hold, release);
      } else if (str == "F7") { pressKey(KEY_F7, hold, release);
      } else if (str == "F8") { pressKey(KEY_F8, hold, release);
      } else if (str == "F9") { pressKey(KEY_F9, hold, release);
      } else if (str == "F10") { pressKey(KEY_F10, hold, release);
      } else if (str == "F11") { pressKey(KEY_F11, hold, release);
      } else if (str == "F12") { pressKey(KEY_F12, hold, release);

      } else if (str == "BRK") { Keyboard.releaseAll();
      } else if (str == "REL") { Keyboard.releaseAll();

      } else if (str.startsWith("T_")) {
        String tmp = str.substring(2);
        Serial.println(tmp);
        int dl = tmp.toInt();
        if (dl > 0) {
          delay(dl);
        } else {
          Keyboard.print(pch);
        }
      } else if (str.startsWith("DT_")) {
        String tmp = str.substring(3);
        Serial.println(tmp);
        int dl = tmp.toInt();
        if (dl > 0) {
          defaultDelay = dl;
        } else {
          Keyboard.print(pch);
        }

      } else { Keyboard.print(pch); };

      pch = strtok(NULL, " ");

      delay(defaultDelay);
    }

  Keyboard.releaseAll();
}


/*
  Key Handling
*/
void handleKeyPress(char key) {
  String str = "KEY key=";
  str.concat(key);
  str.concat("|");
  Serial1.println(str);
  str.remove(0);

  switch (key){
  case '1': parseCommand(cmd1); break;
  case '2': parseCommand(cmd2); break;
  case '3': parseCommand(cmd3); break;
  case '4': parseCommand(cmd4); break;
  case '5': parseCommand(cmd5); break;
  case '6': parseCommand(cmd6); break;
  case '7': parseCommand(cmd7); break;
  case '8': parseCommand(cmd8); break;
  case '9': parseCommand(cmd9); break;
  case '0': parseCommand(cmd0); break;
  case '*': parseCommand(cmdStar); break;
  case '#': parseCommand(cmdHash); break;
  case 'A': parseCommand(cmdA); break;
  case 'B': parseCommand(cmdB); break;
  case 'C': parseCommand(cmdC); break;
  case 'D': parseCommand(cmdD); break;
  }
}

void handleKeyHold(char key) {
  switch (key){
  case '*':
    Serial1.println("LOVE|");
    break;
  }
}

void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
  case PRESSED:
    handleKeyPress(key);
    break;
  case HOLD:
    handleKeyHold(key);
    break;
  }
}

/*
  Loop
*/
void loop()
{
  char pressedKey = keypad.getKey();

  if (pressedKey) {
    Serial.print("Key pressed: ");
    Serial.println(pressedKey);
  }

  while (Serial1.available()) {
    if (Serial1.available() > 0) {
      digitalWrite(BUILTIN_LED, HIGH);
      String str = Serial1.readStringUntil('|');
      str.trim();

      if (str.startsWith("CMD")) { // A command is being binded to key
        // Example: CMD row=0 col=0 cmd=SPC m e b|
        String rawRow = getValue(str, '=', 1);
        String rawCol = getValue(str, '=', 2);
        String rawCmd = getValue(str, '=', 3);

        int row = rawRow.toInt();
        int col = rawCol.toInt();

        // Ugly, I know
        switch (row) {
        case 0:
          switch (col) {
          case 0: cmd1 = rawCmd; break;
          case 1: cmd2 = rawCmd; break;
          case 2: cmd3 = rawCmd; break;
          case 3: cmdA = rawCmd; break;
          }
          break;
        case 1:
          switch (col) {
          case 0: cmd4 = rawCmd; break;
          case 1: cmd5 = rawCmd; break;
          case 2: cmd6 = rawCmd; break;
          case 3: cmdB = rawCmd; break;
          }
          break;
        case 2:
          switch (col) {
          case 0: cmd7 = rawCmd; break;
          case 1: cmd8 = rawCmd; break;
          case 2: cmd9 = rawCmd; break;
          case 3: cmdC = rawCmd; break;
          }
          break;
        case 3:
          switch (col) {
          case 0: cmdStar = rawCmd; break;
          case 1: cmd0 = rawCmd; break;
          case 2: cmdHash = rawCmd; break;
          case 3: cmdD = rawCmd; break;
          }
          break;
        }

        Serial.println();
        Serial.print("Got new command: row=");
        Serial.print(row);
        Serial.print(" col=");
        Serial.print(col);
        Serial.print(" cmd=");
        Serial.println(rawCmd);
      } else if (str.startsWith("KEY")) { // A key has been pressed remotely on Blynk
        // Example: KEY key=1|
        String key = getValue(str, '=', 1);

        Serial.println();
        Serial.print("Key pressed remotely: ");
        Serial.println(key);

        handleKeyPress(key.charAt(0));
      }

      Serial1.println("ACK|");
      digitalWrite(BUILTIN_LED, LOW);
    }
  }
}

/*
  Setup
*/
void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);

  //while (!Serial) {};
  Serial.println("Command Deck");
  Keyboard.begin();

  keypad.addEventListener(keypadEvent);
  keypad.setHoldTime(5000);

  while (!Serial1) {};
  delay(2000);
  Serial1.println("BOOTUP|");
}
