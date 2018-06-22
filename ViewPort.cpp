/* see ViewPort.h */

#include "ViewPort.h"
#include <stdint.h>
#include <string.h>

ViewPort_line::ViewPort_line(const char* l, text_align a, ViewPort_line* n)
    : line(l), align(a), next(n) {}

ViewPort::ViewPort(uint8_t dx, uint8_t dy, void (*sc)(uint8_t x, uint8_t y), void (*pr)(char ch))
    : first_line(nullptr), vx_offset(0), vy_offset(0), dmaxx(dx), dmaxy(dy), set_cursor(sc), print(pr),
      text_width(dx), text_height(dy) {}

void ViewPort::append_line(ViewPort_line* line) {
    ViewPort_line** last_line = &first_line;
    while (*last_line != nullptr) {
        last_line = &((*last_line)->next);
    }
    (*last_line) = line;
}

void ViewPort::clear() {
    first_line = nullptr;
    refresh();
}

void ViewPort::scroll_down() {
    if (text_height - vy_offset > dmaxy) {
        vy_offset++;
        refresh();
    }
}

void ViewPort::scroll_up() {
    if (vy_offset) {
        vy_offset--;
        refresh();
    }
}

void ViewPort::scroll_right() {
    if (text_width - vx_offset > dmaxx) {
        vx_offset++;
        refresh();
    }
}

void ViewPort::scroll_left() {
    if (vx_offset) {
        vx_offset--;
        refresh();
    }
}

void ViewPort::refresh() {
    ViewPort_line* cur_line = first_line;
    uint8_t cur_text_width = 0;
    text_height = 0;
    for (uint8_t row = 0; row < vy_offset; row++) {
        if (!cur_line) break;
        cur_line = cur_line->next;
        text_height++;
    }

    for (uint8_t row = 0; row < dmaxy; row++) {
        set_cursor(0,row);

        if (!cur_line) {
            for (uint8_t col = 0; col < dmaxx; col++) {
                print(' ');
            }
            continue;
        }

        uint8_t row_text_width = 0;
        uint8_t row_padding = 0;

        const char* text = cur_line->line;
        if (cur_line->align == align_right) {
            row_padding = text_width - strlen(text);
        }
        else if (cur_line->align == align_center) {
            row_padding = (text_width - strlen(text)) / 2;
        }
        for (uint8_t col = 0; col < vx_offset; col++) {
            if (!text[0]) break;
            if (row_padding) {
                row_padding--;
            }
            else {
                text++;
            }
            row_text_width++;
        }

        for (uint8_t col = 0; col < dmaxx; col++) {
            char ch = ' ';
            if (row_padding) {
                row_padding--;
            }
            else if (text && text[0]) {
                ch = text[0];
                text++;
                row_text_width++;
            }
            print(ch);
        }

        while (text[0]) {
            text++;
            row_text_width++;
        }
        if (row_text_width > text_width) text_width = row_text_width;

        cur_line = cur_line->next;
        text_height++;
    }

    while (cur_line) {
        uint8_t row_text_width = strlen(cur_line->line);
        if (row_text_width > text_width) text_width = row_text_width;

        cur_line = cur_line->next;
        text_height++;
    }
}
