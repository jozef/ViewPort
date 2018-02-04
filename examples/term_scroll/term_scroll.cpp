#include <iostream>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>

#include <ViewPort.h>

using namespace std;

uint8_t display_width = 16;
uint8_t display_height = 2;

void lcd_print_chr(char ch) {
    printf("%c", ch);
}
void lcd_set_cursor(uint8_t x, uint8_t y) {
    y += 7;
    x += 4;
    printf("%c[%d;%df",0x1B,y,x);
}

int main(int argc, char* argv[]) {
    system ("clear");
    printf("press 'q' to exit\n");
    printf("press arrows to scroll\n");
    printf("display width: %d, display height: %d\n", display_width, display_height);

    // draw square around viewport
    lcd_set_cursor(-3,-2);
    printf("*"); for (uint8_t i = 0; i < display_width+2+2; i++) { printf("-"); } printf("*");
    for (uint8_t i = 0; i < display_height+2; i++) {
        lcd_set_cursor(-3,-1+i); printf("|");
        lcd_set_cursor(-3+display_width+2+2+1,-1+i); printf("|");
    }
    lcd_set_cursor(-3,-2+display_height+2+1);
    printf("*"); for (uint8_t i = 0; i < display_width+2+2; i++) { printf("-"); } printf("*");

    // unbuffered input
    struct termios oldSettings, newSettings;
    tcgetattr( fileno( stdin ), &oldSettings );
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr( fileno( stdin ), TCSANOW, &newSettings );

    ViewPort vp(display_width, display_height, &lcd_set_cursor, &lcd_print_chr);

    ViewPort_line line1("1 -> abcdefgh <");
    ViewPort_line line2("2 -> 6789 123456789 (right) <", align_right);
    ViewPort_line line4("4 -> last line (center) <", align_center);
    ViewPort_line line3("3 -> 9876 987654321 987654321 <", align_left, &line4);

    vp.append_line(&line1);
    vp.append_line(&line2);
    vp.append_line(&line3);

    char kbd_key = 0;
    while (kbd_key != 113) {
        time_t rawtime;
        time (&rawtime);
        struct tm* timeinfo = localtime (&rawtime);
        char line1_str[255];
        char datetime_str[255];
        strftime(datetime_str, 100, "%H:%M:%S %Y-%m-%d",timeinfo);
        sprintf(line1_str, "1 -> %s <", datetime_str);
        line1.line = line1_str;
        vp.refresh();

        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        int ret = select(1, &rfds, NULL, NULL, &tv);
        lcd_set_cursor(0, 16);
        cout << "ret: " << ret << "\n";
        if (ret <= 0) {
            continue;
        }

        kbd_key = cin.get();

        if (kbd_key == 27) {
            kbd_key = cin.get();
            if (kbd_key == 27) {
                break;
            }
            else if (kbd_key == 91) {
                kbd_key = cin.get();
                if (kbd_key == 65) {
                    vp.scroll_up();
                }
                else if (kbd_key == 66) {
                    vp.scroll_down();
                }
                else if (kbd_key == 67) {
                    vp.scroll_right();
                }
                else if (kbd_key == 68) {
                    vp.scroll_left();
                }
            }
        }

        lcd_set_cursor(0, 15);
        cout << "keypressed: " << int(kbd_key) << "\n";
    }

    // restore terminal
    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
    return 0;
}
