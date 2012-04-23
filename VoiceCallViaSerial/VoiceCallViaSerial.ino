#include <SoftwareSerial.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(10, 9, 8); // cs, dc, rst
SoftwareSerial cell(2, 3);

int x = 0, y = 0;

String number;

void setup(void) {
  Serial.begin(115200);
  Serial.print("initializing display...");

  tft.initR(INITR_REDTAB);

  Serial.println(" done.");

  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
  display("number please\r\n");
  
  cell.begin(9600);
}

void loop()
{
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      display("\r\n");
      display("dialing.");
      if (number.length() > 0) {
        cell.print("ATD");
        cell.print(number);
        cell.print("\r");
        number = "";
      }
    } else {
      number += c;
      display(c);
    }
  }
  
  while (cell.available()) {
    display(cell.read());
  }
}

void display(char *s)
{
  do {
    display(*s);
  } while (*++s);
}

void display(char c)
{
  if (c == '\n') y += 16;
  else if (c == '\r') x = 0;
  else {
    tft.drawChar(x, y, c, ST7735_WHITE, 2);
    x += 12;
    if (x > 155) {
      x = 0;
      y += 16;
    }
  }
  
  if (y > 120) {
    x = y = 0;
    tft.fillScreen(ST7735_BLACK);
  }
}

