/*
  scroll.ino - scroll demo for ViewPort

  Created by Jozef Kutej, 27 January 2018
  Released into the public domain.

*/

#include <Arduino.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <ViewPort.h>

unsigned long uptime = -1;
uint8_t last_second = -1;

#define LCD_BACKLIGHT_PIN 10

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//LiquidCrystal_I2C lcd(0x3f, 16, 4);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
ViewPort vp(16,2,&lcd_set_cursor, &lcd_print_chr);

void lcd_print_chr(char ch) {
    lcd.print(ch);
}
void lcd_set_cursor(uint8_t x, uint8_t y) {
    lcd.setCursor(x,y);
}

void setup () {
    Serial.begin(9600);
    while (Serial.available()) { Serial.read(); }

    pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(LCD_BACKLIGHT_PIN, HIGH);

    //lcd.begin();
    lcd.begin(16,2);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("scroll demo");
    lcd.setCursor(0,1);
    lcd.print("1234567890123456");
}

void loop () {
    // count seconds
    //unsigned long cur_second = millis() / 1000;
    uint8_t cur_second = millis() / 1000;
    while (last_second != cur_second) {
        last_second++;
        uptime++;
    }

    unsigned long tmp_uptime = uptime;
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
    seconds = tmp_uptime % 60;
    tmp_uptime = tmp_uptime / 60;
    minutes = tmp_uptime % 60;
    tmp_uptime = tmp_uptime / 60;
    hours = tmp_uptime % 60;
    tmp_uptime = tmp_uptime / 60;
    days = tmp_uptime / 24;

    //digitalWrite(LCD_BACKLIGHT_PIN, (uptime % 10 < 8 ? HIGH : LOW));

    String line1_str = String("1 -> "+String(uptime)+" 123456789 123456789");   // 29 chars;
    ViewPort_line line1(line1_str.c_str());
    ViewPort_line line2("2 -> 123456");
    ViewPort_line line3("3 -> 98765432");
    String line4_str = "4 -> ox:"+String(vp.vx_offset)+",oy:"+String(vp.vy_offset);
    ViewPort_line line4(line4_str.c_str());
    String line5_str = "5 -> "+String(days)+"d "+two_dig(hours)+":"+two_dig(minutes)+":"+two_dig(seconds)+" (center)";
    ViewPort_line line5(line5_str.c_str(), align_center);
    String line6_str = "6 -> width:"+String(vp.text_width)+" height: "+String(vp.text_height);
    ViewPort_line line6(line6_str.c_str());
    ViewPort_line line7("7 -> 6789 123456789 1234567890");    // 30 chars

    vp.append_line(&line1);
    vp.append_line(&line2);
    vp.append_line(&line3);
    vp.append_line(&line4);
    vp.append_line(&line5);
    vp.append_line(&line6);
    vp.append_line(&line7);
    vp.refresh();

    int btn = read_LCD_buttons();
    if (btn == btnLEFT) {
        vp.scroll_left();
    }
    else if (btn == btnRIGHT) {
        vp.scroll_right();
    }
    else if (btn == btnUP) {
        vp.scroll_up();
    }
    else if (btn == btnDOWN) {
        vp.scroll_down();
    }

    vp.first_line = 0;

    delay(100);
}

String two_dig(uint8_t x) {
    if (x > 9) {
        return String(x);
    }
    else {
        return "0"+String(x);
    }
}

int read_LCD_buttons() {
    int adc_key_in = analogRead(0);

    if (adc_key_in > 1000) return btnNONE;
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT;

    return btnNONE;
}

