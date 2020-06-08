#include <Arduino.h>
#include <MemoryFree.h>
#include <Keypad.h>
#include "Keyboard.h"
//#include <cstring>
//#include <cstdlib>

#define BUILTIN_LED 13

const unsigned int cmdSize = 64;
const unsigned int numChars = cmdSize+10;

char receivedChars[numChars];   // an array to store the received data

char currentCmd[cmdSize];

boolean newData = false;

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    } else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

/*
  Matrix keypad mapping
*/
const byte n_rows = 4;
const byte n_cols = 4;

const char matrix[n_rows][n_cols] = {
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
char cmd1[cmdSize], cmd2[cmdSize], cmd3[cmdSize], cmd4[cmdSize], cmd5[cmdSize], cmd6[cmdSize], cmd7[cmdSize], cmd8[cmdSize], cmd9[cmdSize], cmd0[cmdSize];
char cmdStar[cmdSize], cmdHash[cmdSize], cmdA[cmdSize], cmdB[cmdSize], cmdC[cmdSize], cmdD[cmdSize];

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

void parseCommand(char cmdOrig[cmdSize]) {
  Serial.print("Parsing command ");
  Serial.println(cmdOrig);
  snprintf(currentCmd, cmdSize, "%s", cmdOrig);

  Serial.print("Parsing command current ");
  Serial.println(currentCmd);

  char *pch;
  int defaultDelay = 0;
  pch = strtok (currentCmd, "|");
  while (pch != NULL) {
    bool hold = false;
    bool release = false;

    char cmd[cmdSize];

    if (pch[0] == '#') {
      if (pch[1] == '+' && strlen(pch) > 2) {
        hold = true;
        snprintf(cmd, cmdSize, "%s", pch+2);
      } else if (pch[1] == '-' && strlen(pch) > 2) {
        release = true;
        snprintf(cmd, cmdSize, "%s", pch+2);
      } else {
        snprintf(cmd, cmdSize, "%s", pch+1);
      }

      if (strcmp(cmd, "SPC") == 0) { Keyboard.print(' ');
      } else if (strcmp(cmd, "HASH") == 0) { Keyboard.print('#');
      } else if (strcmp(cmd, "PIPE") == 0) { Keyboard.print('|');
      } else if (strcmp(cmd, "EQ") == 0) { Keyboard.print('=');
      } else if (strcmp(cmd, "PCT") == 0) { Keyboard.print('%');

      } else if (strcmp(cmd, "S_QUOTE") == 0) { Keyboard.print('\'');
      } else if (strcmp(cmd, "D_QUOTE") == 0) { Keyboard.print('"');

      } else if (strcmp(cmd, "LT") == 0) { Keyboard.print('<');
      } else if (strcmp(cmd, "GT") == 0) { Keyboard.print('>');

      } else if (strcmp(cmd, "L_CTRL") == 0) { pressKey(KEY_LEFT_CTRL, hold, release);
      } else if (strcmp(cmd, "R_CTRL") == 0) { pressKey(KEY_RIGHT_CTRL, hold, release);
      } else if (strcmp(cmd, "L_ALT") == 0) { pressKey(KEY_LEFT_ALT, hold, release);
      } else if (strcmp(cmd, "R_ALT") == 0) { pressKey(KEY_RIGHT_ALT, hold, release);
      } else if (strcmp(cmd, "L_SHIFT") == 0) { pressKey(KEY_LEFT_SHIFT, hold, release);
      } else if (strcmp(cmd, "R_SHIFT") == 0) { pressKey(KEY_RIGHT_SHIFT, hold, release);
      } else if (strcmp(cmd, "L_WIN") == 0) { pressKey(KEY_LEFT_GUI, hold, release);
      } else if (strcmp(cmd, "R_WIN") == 0) { pressKey(KEY_RIGHT_GUI, hold, release);

      } else if (strcmp(cmd, "L_ARROW") == 0) { pressKey(KEY_LEFT_ARROW, hold, release);
      } else if (strcmp(cmd, "R_ARROW") == 0) { pressKey(KEY_RIGHT_ARROW, hold, release);
      } else if (strcmp(cmd, "U_ARROW") == 0) { pressKey(KEY_UP_ARROW, hold, release);
      } else if (strcmp(cmd, "D_ARROW") == 0) { pressKey(KEY_DOWN_ARROW, hold, release);

      } else if (strcmp(cmd, "DEL") == 0) { pressKey(KEY_DELETE, hold, release);
      } else if (strcmp(cmd, "RET") == 0) { pressKey(KEY_RETURN, hold, release);
      } else if (strcmp(cmd, "ESC") == 0) { pressKey(KEY_ESC, hold, release);
      } else if (strcmp(cmd, "BACK") == 0) { pressKey(KEY_BACKSPACE, hold, release);
      } else if (strcmp(cmd, "TAB") == 0) { pressKey(KEY_TAB, hold, release);
      } else if (strcmp(cmd, "INS") == 0) { pressKey(KEY_INSERT, hold, release);

      } else if (strcmp(cmd, "PG_UP") == 0) { pressKey(KEY_PAGE_UP, hold, release);
      } else if (strcmp(cmd, "PG_DOWN") == 0) { pressKey(KEY_PAGE_DOWN, hold, release);

      } else if (strcmp(cmd, "HOME") == 0) { pressKey(KEY_HOME, hold, release);
      } else if (strcmp(cmd, "END") == 0) { pressKey(KEY_END, hold, release);
      } else if (strcmp(cmd, "CAPS") == 0) { pressKey(KEY_CAPS_LOCK, hold, release);

      } else if (strcmp(cmd, "F1") == 0) { pressKey(KEY_F1, hold, release);
      } else if (strcmp(cmd, "F2") == 0) { pressKey(KEY_F2, hold, release);
      } else if (strcmp(cmd, "F3") == 0) { pressKey(KEY_F3, hold, release);
      } else if (strcmp(cmd, "F4") == 0) { pressKey(KEY_F4, hold, release);
      } else if (strcmp(cmd, "F5") == 0) { pressKey(KEY_F5, hold, release);
      } else if (strcmp(cmd, "F6") == 0) { pressKey(KEY_F6, hold, release);
      } else if (strcmp(cmd, "F7") == 0) { pressKey(KEY_F7, hold, release);
      } else if (strcmp(cmd, "F8") == 0) { pressKey(KEY_F8, hold, release);
      } else if (strcmp(cmd, "F9") == 0) { pressKey(KEY_F9, hold, release);
      } else if (strcmp(cmd, "F10") == 0) { pressKey(KEY_F10, hold, release);
      } else if (strcmp(cmd, "F11") == 0) { pressKey(KEY_F11, hold, release);
      } else if (strcmp(cmd, "F12") == 0) { pressKey(KEY_F12, hold, release);

      } else if (strcmp(cmd, "CMD_1") == 0) { parseCommand(cmd1);
      } else if (strcmp(cmd, "CMD_2") == 0) { parseCommand(cmd2);
      } else if (strcmp(cmd, "CMD_3") == 0) { parseCommand(cmd3);
      } else if (strcmp(cmd, "CMD_4") == 0) { parseCommand(cmd4);
      } else if (strcmp(cmd, "CMD_5") == 0) { parseCommand(cmd5);
      } else if (strcmp(cmd, "CMD_6") == 0) { parseCommand(cmd6);
      } else if (strcmp(cmd, "CMD_7") == 0) { parseCommand(cmd7);
      } else if (strcmp(cmd, "CMD_8") == 0) { parseCommand(cmd8);
      } else if (strcmp(cmd, "CMD_9") == 0) { parseCommand(cmd9);
      } else if (strcmp(cmd, "CMD_0") == 0) { parseCommand(cmd0);
      } else if (strcmp(cmd, "CMD_A") == 0) { parseCommand(cmdA);
      } else if (strcmp(cmd, "CMD_B") == 0) { parseCommand(cmdB);
      } else if (strcmp(cmd, "CMD_C") == 0) { parseCommand(cmdC);
      } else if (strcmp(cmd, "CMD_D") == 0) { parseCommand(cmdD);
      } else if (strcmp(cmd, "CMD_STAR") == 0) { parseCommand(cmdStar);
      } else if (strcmp(cmd, "CMD_HASH") == 0) { parseCommand(cmdHash);

      } else if (strcmp(cmd, "REL") == 0) { Keyboard.releaseAll();

      } else if (cmd[0] == 'T' && cmd[1] == '_') {
        char nCmd[cmdSize];
        strncpy(nCmd, cmd+2, sizeof(nCmd));

        int dl = atoi(nCmd);
        if (dl > 0) {
          delay(dl);
        } else {
          Keyboard.print(cmd);
        }
      } else if (cmd[0] == 'D' && cmd[1] == 'T' && cmd[2] == '_') {
        char nCmd[cmdSize];
        strncpy(nCmd, cmd+3, sizeof(nCmd));

        int dl = atoi(nCmd);
        if (dl > 0) {
          defaultDelay = dl;
        } else {
          Keyboard.print(cmd);
        }
      } else {
        Keyboard.print(cmd);
      }

    } else {
      Keyboard.print(pch);
    }

    pch = strtok(NULL, "|");

    delay(defaultDelay);
  }

  currentCmd[0] = '\0';

  Keyboard.releaseAll();
}


/*
  Key Handling
*/
void handleKeyPress(char key) {
  Serial1.print("KEY%");
  Serial1.print(key);
  Serial1.println("|");



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

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;

/*
  Loop
*/
void loop()
{
  char pressedKey = keypad.getKey();

  if (pressedKey) {
    Serial.print("P:");
    Serial.println(pressedKey);
  }

  digitalWrite(BUILTIN_LED, HIGH);

  if (newData) {
    Serial.print("Received: ");
    Serial.println(receivedChars);

    if (receivedChars[0] == 'C' && receivedChars[1] == 'M' && receivedChars[2] == 'D') {
      // Example: CMD%0%0%SPC m e b|

      char rawRow = receivedChars[4];
      char rawCol = receivedChars[6];

      char rawCmd[cmdSize];

      snprintf(rawCmd, cmdSize, "%s", receivedChars+8);

      int row = rawRow - '0';
      int col = rawCol - '0';

      // Ugly, I know
      switch (row) {
      case 0:
        switch (col) {
        case 0: snprintf(cmd1, cmdSize, "%s", rawCmd); break;
        case 1: snprintf(cmd2, cmdSize, "%s", rawCmd); break;
        case 2: snprintf(cmd3, cmdSize, "%s", rawCmd); break;
        case 3: snprintf(cmdA, cmdSize, "%s", rawCmd); break;
        }
        break;
      case 1:
        switch (col) {
        case 0: snprintf(cmd4, cmdSize, "%s", rawCmd); break;
        case 1: snprintf(cmd5, cmdSize, "%s", rawCmd); break;
        case 2: snprintf(cmd6, cmdSize, "%s", rawCmd); break;
        case 3: snprintf(cmdB, cmdSize, "%s", rawCmd); break;
        }
        break;
      case 2:
        switch (col) {
        case 0: snprintf(cmd7, cmdSize, "%s", rawCmd); break;
        case 1: snprintf(cmd8, cmdSize, "%s", rawCmd); break;
        case 2: snprintf(cmd9, cmdSize, "%s", rawCmd); break;
        case 3: snprintf(cmdC, cmdSize, "%s", rawCmd); break;
        }
        break;
      case 3:
        switch (col) {
        case 0: snprintf(cmdStar, cmdSize, "%s", rawCmd); break;
        case 1: snprintf(cmd0,    cmdSize, "%s", rawCmd); break;
        case 2: snprintf(cmdHash, cmdSize, "%s", rawCmd); break;
        case 3: snprintf(cmdD,    cmdSize, "%s", rawCmd); break;
        }
        break;
      }

      Serial.println();
      Serial.print("Understood CMD%");
      Serial.print(row);
      Serial.print("%");
      Serial.print(col);
      Serial.print("%");
      Serial.println(rawCmd);
    } else if (receivedChars[0] == 'K' && receivedChars[1] == 'E' && receivedChars[2] == 'Y') { // A key has been pressed remotely on Blynk
      // Example: KEY%1|
      char key = receivedChars[4];

      Serial.println();
      Serial.print("RP:");
      Serial.println(key);

      handleKeyPress(key);
    }

    newData = false;
    receivedChars[0] = '\0';
  } else {
    recvWithStartEndMarkers();
  }
  digitalWrite(BUILTIN_LED, LOW);

  currentMillis = millis();
  if (currentMillis - startMillis >= period)  {
    Serial.print("FM:");
    Serial.println(freeMemory());
    startMillis = currentMillis;
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

  // Empty buffer
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial1.println("BOOT|");

  startMillis = millis(); 
}
