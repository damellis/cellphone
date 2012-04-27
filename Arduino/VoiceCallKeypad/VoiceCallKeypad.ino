#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {A3, A2, A1, A0};
byte colPins[cols] = {5, 6, 7};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

Adafruit_ST7735 tft = Adafruit_ST7735(10, 9, 8); // cs, dc, rst
SoftwareSerial cell(2, 3);

int x = 0, y = 0;
unsigned long lastCellCharTime = 0;
boolean displayedNumber = true;

String number;

void setup(void) {
  //Serial.begin(115200);
  //Serial.print("initializing display...");

  tft.initR(INITR_REDTAB);

  //Serial.println(" done.");

  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);
  display("number please: ");
  
  cell.begin(9600);
  cell.print("ATZ\r");
}

void loop()
{
  char key = keypad.getKey();
  
  if (key == '*') {
    if (number.length() > 0) {
      number = number.substring(0, number.length() - 1);
      display('\b');
    } else {
      display("\r\nhanging up.");
      cell.print("ATH\r");
    }
  }
  else if (key == '#') {
    if (number.length() > 0) {
      display("\r\n");
      display("dialing: ");
      display(number);
      cell.print("ATD");
      cell.print(number);
      cell.print("\r");
      number = "";
    } else {
      display("\r\nanswering.");
      cell.print("ATA\r");
    }
  } else if (key) {
    display(key);
    number += key;
  }
  
  while (cell.available()) {
    display(cell.read());
    lastCellCharTime = millis();
    displayedNumber = false;
  }
  
  if (millis () - lastCellCharTime > 100 && !displayedNumber) {
    display("number please: ");
    display(number);
    displayedNumber = true;
  }
}

void display(char *s)
{
  do {
    display(*s);
  } while (*++s);
}

void display(String s)
{
  for (int i = 0; i < s.length(); i++) display(s[i]);
}

void display(char c)
{
  if (c == '\n') y += 8;
  else if (c == '\r') x = 0;
  else if (c == '\b') {
    if (x > 0) {
      x -= 6;
      tft.fillRect(x, y, 6, 8, ST7735_BLACK);
    }
  } else {
    tft.drawChar(x, y, c, ST7735_WHITE);
    x += 6;
    if (x > 155) {
      x = 0;
      y += 8;
    }
  }
  
  if (y > 120) {
    x = y = 0;
    tft.fillScreen(ST7735_BLACK);
  }
}

