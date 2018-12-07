#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "Memory.h"
#include <RF24.h>
#include "avr/interrupt.h"
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
typedef enum {SETUP, MENU, ABOUT_ME, NEW_CONTACT, MESSAGES, MESSAGE_OPEN, CONTACTS, NEW_MESSAGE, INTERRUPT} SystemState;
SystemState state = SETUP;
SystemState prevstate;
Memory memory;
RF24 radio(A1, A2);
uint64_t MYUUID;
Message recievedMessage;
void configureInterrupt() {

  unsigned char* pEICRA = reinterpret_cast<unsigned char*> (0x69);
  unsigned char* pEIMSK = reinterpret_cast<unsigned char*> (0x3D);
  unsigned char* pSREG = reinterpret_cast<unsigned char*> (0x5F);
  *pEICRA |= 0x2;

  *pEIMSK |= 0x01;

  *pSREG |= 0x80;

}

void print64(uint64_t value)
{
  if ( value >= 10 )
  {
    print64(value / 10);
  }

  Serial.print(static_cast<int>(value % 10));
}

uint64_t chartoUUID(unsigned char * UUID[])
{
  uint64_t newUUID = 0;
  newUUID |= (uint64_t)UUID[0];
  for (int i = 1; i < 5; i++)
  {
    newUUID = newUUID << 8;
    newUUID |= (uint64_t)UUID[i];
  }
  return newUUID;
}

bool sendmessage(Message message)
{
  unsigned char * tempUUID = message.getTo();
  uint64_t uintUUID = 0;
  for (int i = 0; i < 5; i++)
  {
    uintUUID |= tempUUID[i];
    uintUUID = uintUUID << 8;
  }
  uintUUID = uintUUID >> 8;
  print64(uintUUID);
  Serial.println();
  bool returnv;
  unsigned short payload = message.getPayload();
  radio.stopListening();
  radio.openWritingPipe(uintUUID);
  if (radio.write(&message, sizeof(Message)))
    returnv = true;
  else returnv = false;
  radio.startListening();

  return returnv;
}

void radiosetup()
{
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.maskIRQ(1, 1, 0);
  configureInterrupt();
  radio.openReadingPipe(1, 0x0111111111);
  radio.startListening();

}

ISR(INT0_vect) {
  //WHEN PIN2 READS HIGH
  tone(3, 440, 1000);
  prevstate = state;
  unsigned short payload = 0;
  while (radio.available())
    radio.read(&recievedMessage, sizeof(Message));
  //recievedMessage.setPayload(payload);
  //lcd.print("Message recieved");
  Serial.println("Message Recieved");
  Serial.println(recievedMessage.getPayload());
  // lcd.print(recievedMessage.getPayload());
  memory.saveMessage(recievedMessage);

  int index = memory.getNumberMessages() - 1;
  //Serial.println(index);
  Message tempmessage = memory.getMessage(index);
  //Serial.println(tempmessage.getPayload());
  //Serial.println(memory.getMessage(memory.getNumberMessages() - 1).getPayload());
  bool b1, b2, b3;
  radio.whatHappened(b1, b2, b3);
  state = INTERRUPT;
}

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  //radiosetup();
  lcd.begin(16, 2);

  int time = millis();
  //lcd.print("Enter UUID");
  //time = millis() - time;
  //Serial.print("Took "); Serial.print(time); Serial.println(" ms");

}

void loop() {

  switch (state)
  {

    case SETUP:
      {
        uint8_t buttons = lcd.readButtons();
        lcd.setCursor(0, 0);
        lcd.print("Enter UUID");
        char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
        static char userName[16] = {'a', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
        static int letterNum = 0;
        static int cursorRowLocation = 1;
        static int cursorColLocation = 0;
        static int stringLength = 0;

        if (buttons) {

          lcd.setCursor(cursorColLocation, cursorRowLocation);

          if (buttons & BUTTON_UP) {
            if (letterNum > 25)
            {
              letterNum = 0;
              userName[stringLength] = alphabet[letterNum];
              delay(200);
            }
            else {
              userName[stringLength] = alphabet[letterNum];
              letterNum++;
              lcd.print(userName[stringLength]);
              delay (200);
            }
          }
          if (buttons & BUTTON_DOWN) {
            if (letterNum < 0)
            {
              letterNum = 25;
              userName[stringLength] = alphabet[letterNum];

            }
            else {
              userName[stringLength] = alphabet[letterNum];
              letterNum--;
              lcd.print(userName[stringLength]);
              delay (200);
            }
          }
          if (buttons & BUTTON_RIGHT) {
            cursorColLocation++;
            lcd.setCursor(cursorColLocation, cursorRowLocation);
            //userName[stringLength] = alphabet[letterNum];
            stringLength++;
            letterNum = 0;
            delay(200);
          }
          if (buttons & BUTTON_LEFT) {
            lcd.print(' ');
            cursorColLocation--;
            lcd.setCursor(cursorColLocation, cursorRowLocation);
            userName[stringLength] = ' ';

            stringLength--;
            letterNum = 0;
            delay(200);
          }
          if (buttons & BUTTON_SELECT) {
            lcd.clear();

            for (int i = 0; i < stringLength; i++)
            {
              lcd.setCursor(i, 0);
              lcd.print(userName[i]);

            }
            delay(1000);
            lcd.clear();
            static unsigned char newUUID[5] = {};
            for (int i = 0; i < 5; i++)
            {
              newUUID[i] = 0x11;
            }
            MYUUID = chartoUUID(*newUUID);
            Contact contact(newUUID, userName);
            memory.saveNodeInformation(contact);
            radiosetup();
            state = MENU;
            //Save to EEPROM
          }

        }



        break;
      }

    case MENU:
      {
        uint8_t buttons = lcd.readButtons();
        static char option = 0;
        String menuArr[4] = {"Contacts", "Messages", "N. Contact", "About Me"};

        lcd.setCursor(0, 0);
        lcd.print("Menu:");
        if (buttons) {

          if (buttons & BUTTON_RIGHT) {

            lcd.clear();
            lcd.print("Menu:");
            option++;
            if (option > 3)
            {
              option = 0;

            }
            lcd.print(menuArr[option]);

            delay (200);
          }
          if (buttons & BUTTON_LEFT) {

            lcd.clear();
            lcd.print("Menu:");
            option--;
            if (option < 0)
            {
              option = 3;

            }
            lcd.print(menuArr[option]);

            delay (200);
          }
          if (buttons & BUTTON_SELECT)
          {
            if (option == 0)
            {
              option = 0;
              state = CONTACTS;
              lcd.clear();
              delay(200);
            }
            else if (option == 1)
            {
              option = 0;
              state = MESSAGES;
              lcd.clear();
              delay(200);
            }
            else if (option == 2)
            {
              option = 0;
              lcd.clear();
              state = NEW_CONTACT;
              delay(500);

            }
            else if (option == 3)
            {
              option = 0;
              lcd.clear();
              state = ABOUT_ME;
              delay(200);
            }
          }
        }
        break;
      }
    case ABOUT_ME:
      {
        bool Brexit = true;

        uint8_t buttons = lcd.readButtons();
        lcd.setCursor(0, 0);
        lcd.print(memory.getNodeName());
        lcd.setCursor(0, 1);
        for (int i = 0; i < 5; i++)
        {
          unsigned char hexNum;
          hexNum = memory.getNodeUUID()[i];
          lcd.print(hexNum, HEX);
        }
        //lcd.print(memory.getNodeUUID());
        delay(5000);
        state = MENU;
        lcd.clear();
      }
      break;

    case INTERRUPT:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Message Recieved");
        lcd.setCursor(0, 1);
        lcd.print(recievedMessage.getPayload());
        for (int i = 0; i < 5; i++)
        {
          Serial.print(recievedMessage.getFrom()[i]);
          Serial.print("-");
        }
        Serial.println(" ");
        for (int i = 0; i < 5; i++)
        {
          Serial.print(recievedMessage.getTo()[i]);
          Serial.print("-");
        }

        Serial.println();
        Serial.println(recievedMessage.getLength());
        memory.saveMessage(recievedMessage);
        delay(3000);
        lcd.clear();
        state = prevstate;

      }
      break;

    case MESSAGES:
      {
        uint8_t buttons = lcd.readButtons();
        static int option = 0;
        lcd.setCursor(0, 0);
        lcd.print("Message:");
        if (buttons) {

          if (buttons & BUTTON_RIGHT) {

            lcd.clear();
            lcd.print("Message:" + option);
            option++;
            if (option > memory.getNumberMessages())
            {
              option = 0;

            }
            lcd.setCursor(0, 1);
            for (int i = 0; i < 5; i++)
              lcd.print(memory.getMessage(option).getFrom()[i]);
            delay (200);
          }
          if (buttons & BUTTON_LEFT) {

            lcd.clear();
            lcd.print("Message:" + option);
            option--;
            if (option < 0)
            {
              option = memory.getNumberMessages();

            }
            //lcd.print(memory.getMessage(option));
            lcd.setCursor(0, 1);
            delay (200);
          }
          if (buttons & BUTTON_SELECT)
          {
            state = MESSAGE_OPEN;
            lcd.clear();
            delay(500);
          }
        }
      }
      break;
    case MESSAGE_OPEN:
      {
      lcd.print(recievedMessage.getPayload());
      delay(3000);
      lcd.clear();
      state = MENU;
      }
      break;
    case CONTACTS:
      {
        uint8_t buttons = lcd.readButtons();
        //lcd.clear();
        static int option = 0;
        lcd.setCursor(0, 0);
        lcd.print("Contacts:");
        if (memory.getNumberContacts() == 0)
        {
          lcd.print("You have no contacts :(");
          state = MENU;
          delay(1000);
          lcd.clear();
        }
        else
        {

          if (buttons) {

            if (buttons & BUTTON_RIGHT) {

              lcd.clear();
              lcd.print("Contacts:");
              option++;
              if (option > memory.getNumberContacts())
              {
                option = 0;

              }
              lcd.print(memory.getContact(option).getName());
              lcd.setCursor(0, 1);
              for (int i = 0; i < 5; i++)
                lcd.print(memory.getContact(option).getUUID()[i]);
              delay (200);
            }
            if (buttons & BUTTON_LEFT) {

              lcd.clear();
              lcd.print("Contacts:");
              option--;
              if (option < 0)
              {
                option = memory.getNumberContacts();

              }
              lcd.print(memory.getContact(option).getName());
              lcd.setCursor(0, 1);
              for (int i = 0; i < 5; i++)
                lcd.print(memory.getContact(option).getUUID()[i]);
              delay (200);
            }
            if (buttons & BUTTON_SELECT)
            {
              state = NEW_MESSAGE;
              lcd.clear();
              delay(500);
            }
          }
          /*
            lcd.print(memory.getContact(0).getName());
            lcd.setCursor(0, 1);
            for (int i = 0; i < 5; i++)
            {
            unsigned char hexNum;
            hexNum = memory.getContact(0).getUUID()[i];
            lcd.print(hexNum, HEX);
            }
            delay(5000);
            lcd.clear();
            state = MENU;
          */
        }
        if (state == MENU)
          break;
      }
      break;
    case NEW_CONTACT:
      {
        static bool incompleteUUID = false;
        static bool incompletename = true;

        //lcd.clear();

        if (memory.getNumberContacts() == 20)
        {

          lcd.setCursor(0, 0);
          lcd.print("Contact list");
          lcd.setCursor(0, 1);
          lcd.print("is full!");
          delay(1000);
          state = MENU;
        }
        else
        {

          static char userName[16] = {'a', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
          static unsigned char newUUID[5] = {};
          if (incompletename && !incompleteUUID)
          {
            uint8_t buttons = lcd.readButtons();
            lcd.setCursor(0, 0);
            lcd.print("New Contact   (1");
            lcd.setCursor(0, 1);
            lcd.print("Name:");



            char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

            static int letterNum = 0;
            static int cursorRowLocation = 1;
            static int cursorColLocation = 5;
            static int stringLength = 0;

            if (buttons) {

              lcd.setCursor(cursorColLocation, cursorRowLocation);

              if (buttons & BUTTON_UP) {
                if (letterNum > 25)
                {
                  letterNum = 0;
                  userName[stringLength] = alphabet[letterNum];
                  delay(200);
                }
                else {
                  userName[stringLength] = alphabet[letterNum];
                  letterNum++;
                  lcd.print(userName[stringLength]);
                  delay (200);
                }
              }
              if (buttons & BUTTON_DOWN) {
                if (letterNum < 0)
                {
                  letterNum = 25;
                  userName[stringLength] = alphabet[letterNum];

                }
                else {
                  userName[stringLength] = alphabet[letterNum];
                  letterNum--;
                  lcd.print(userName[stringLength]);
                  delay (200);
                }
              }
              if (buttons & BUTTON_RIGHT) {
                cursorColLocation++;
                lcd.setCursor(cursorColLocation, cursorRowLocation);
                //userName[stringLength] = alphabet[letterNum];
                stringLength++;
                letterNum = 0;
                delay(200);
              }
              if (buttons & BUTTON_LEFT) {
                lcd.print(' ');
                cursorColLocation--;
                lcd.setCursor(cursorColLocation, cursorRowLocation);
                userName[stringLength] = ' ';

                stringLength--;
                letterNum = 0;
                delay(200);
              }
              if (buttons & BUTTON_SELECT) {
                lcd.clear();
                incompleteUUID = true;
                incompletename = false;
                lcd.print(userName);
                stringLength = 0;
                cursorColLocation = 5;
                cursorRowLocation = 1;
                delay(2000);
              }

            }

          }

          if (incompleteUUID && !incompletename)
          {

            uint8_t buttons = lcd.readButtons();
            lcd.setCursor(0, 0);
            lcd.print("New Contact   (2");
            lcd.setCursor(0, 1);
            lcd.print("UUID:");
            static bool  mathStuff = false;
            static String UUIDvariable[10] = {" ", " ", " ", " ", " ", " ", " ", " ", " ", " "};
            static String alphabet[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
            static int letterNum = 0;
            static int cursorRowLocation = 1;
            static int cursorColLocation = 5;
            static int stringLength = 0;

            if (buttons) {

              lcd.setCursor(cursorColLocation, cursorRowLocation);

              if (buttons & BUTTON_UP) {
                if (letterNum > 15)
                {
                  letterNum = 0;
                  UUIDvariable[stringLength] = alphabet[letterNum];
                  delay(200);
                }
                else {
                  letterNum++;
                  UUIDvariable[stringLength] = alphabet[letterNum];
                  lcd.print(UUIDvariable[stringLength]);
                  delay (200);
                }
              }
              if (buttons & BUTTON_DOWN) {
                if (letterNum < 0)
                {
                  letterNum = 15;
                  UUIDvariable[stringLength] = alphabet[letterNum];

                }
                else {
                  letterNum--;
                  UUIDvariable[stringLength] = alphabet[letterNum];
                  lcd.print(UUIDvariable[stringLength]);
                  delay (200);
                }
              }
              if (buttons & BUTTON_RIGHT) {
                cursorColLocation++;
                lcd.setCursor(cursorColLocation, cursorRowLocation);
                UUIDvariable[stringLength] = alphabet[letterNum];
                stringLength++;
                letterNum = 0;
                delay(200);
              }
              if (buttons & BUTTON_LEFT) {
                lcd.print(' ');
                cursorColLocation--;
                lcd.setCursor(cursorColLocation, cursorRowLocation);
                UUIDvariable[stringLength] = ' ';

                stringLength--;
                letterNum = 0;
                delay(200);
              }
              if (buttons & BUTTON_SELECT) {
                lcd.clear();
                //incompletename = false;
                mathStuff = true;
                //buttons = false;


                delay(500);
              }

            }


            /*________________________________________________________________________________________________________________________________________________________________________________________________________________________________*/

            if (mathStuff)
            {

              static int j = 0;
              for (int i = 0; i < 10; i++)
              {

                if (UUIDvariable[i] == "0")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x0;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "1")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x1;
                  if (i % 2 != 0)
                    j++;

                }

                if (UUIDvariable[i] == "2")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x2;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "3")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x3;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "4")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x4;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "5")
                { if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x5;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "6")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x6;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "7")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x7;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "8")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x8;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "9")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0x9;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "A")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0xA;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "B")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0xB;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "C")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0xC;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "D")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0xD;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "E")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0xE;
                  if (i % 2 != 0)
                    j++;
                }
                if (UUIDvariable[i] == "F")
                {
                  if (i % 2 != 0)
                    newUUID[j] <<= 4;
                  newUUID[j] |= 0xF;
                  if (i % 2 != 0)
                    j++;

                }

              }
              mathStuff = false;
              incompleteUUID = false;
              for (int i = 0; i < 10; i++)
              {
                lcd.setCursor(i, 0);
                lcd.print(UUIDvariable[i]);
              }
              for (int i = 0; i < 5; i++)
              {
                lcd.setCursor(i, 1);
                // lcd.print(newUUID[i]);
                //Serial.print(newUUID[i]);
              }

              //Serial.print(*something);
              delay(1000);
              if (j != 0) {
                Contact contact(newUUID, userName);
                memory.saveContact(contact);
                incompleteUUID = false;
                incompletename = true;
                for (int i = 0; i < 10; i++)
                {
                  userName[i] = ' ';
                  UUIDvariable[i] = " ";
                }
                for (int i = 0; i < 5; i++)
                  newUUID[i] = 0;
                cursorRowLocation = 1;
                cursorColLocation = 5;
                stringLength = 0;
                state = MENU;
              }
              j = 0;
              lcd.clear();

            }


          }
          /*________________________________________________________________________________________________________________________________________________________________________________________________________________________________*/

          //memory.getNumberContacts()
        }

      }
      break;
    case NEW_MESSAGE:
      {
        uint8_t buttons = lcd.readButtons();
        lcd.setCursor(0, 0);
        lcd.print("Type Morse MSG:");
        char alphabet[2] = {'.', '_'};
        static char Msg[16] = {'.', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
        static int letterNum = 0;
        static int cursorRowLocation = 1;
        static int cursorColLocation = 0;
        static int stringLength = 0;
        if (buttons) {

          lcd.setCursor(cursorColLocation, cursorRowLocation);

          if (buttons & BUTTON_UP) {
            if (letterNum > 1)
            {
              letterNum = 0;
              Msg[stringLength] = alphabet[letterNum];
              delay(200);
            }
            else {
              Msg[stringLength] = alphabet[letterNum];
              letterNum++;
              lcd.print(Msg[stringLength]);
              delay (200);
            }
          }
          if (buttons & BUTTON_DOWN) {
            if (letterNum < 0)
            {
              letterNum = 1;
              Msg[stringLength] = alphabet[letterNum];

            }
            else {
              Msg[stringLength] = alphabet[letterNum];
              letterNum--;
              lcd.print(Msg[stringLength]);
              delay (200);
            }
          }
          if (buttons & BUTTON_RIGHT) {
            cursorColLocation++;
            lcd.setCursor(cursorColLocation, cursorRowLocation);
            //userName[stringLength] = alphabet[letterNum];
            stringLength++;
            letterNum = 0;
            delay(200);
          }
          if (buttons & BUTTON_LEFT) {
            lcd.print(' ');
            cursorColLocation--;
            lcd.setCursor(cursorColLocation, cursorRowLocation);
            Msg[stringLength] = ' ';

            stringLength--;
            letterNum = 0;
            delay(200);
          }
          if (buttons & BUTTON_SELECT) {
            lcd.clear();
            Message tempMessage;
            tempMessage.setTo(memory.getContact(0).getUUID());
            tempMessage.setPayload(tempMessage.stringToPayload(Msg));
            sendmessage(tempMessage);
            for (int i = 0; i < stringLength; i++)
            {
              lcd.setCursor(i, 0);
              lcd.print(Msg[i]);
              lcd.clear();
            }
            delay(1000);
            for (int i = 0; i < 16; i ++)
              Msg[i] = ' ';
            letterNum = 0;
            cursorRowLocation = 1;
            cursorColLocation = 0;
            stringLength = 0;
            state = MENU;
          }
          break;
        }
      }
  }
}
