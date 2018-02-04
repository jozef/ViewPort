/* VERSION 0.01; 1.1.2018; see below for description and documentation */
#ifndef ViewPort_h
#define ViewPort_h

#include <stdint.h>

enum text_align { align_left, align_center, align_right };

class ViewPort_line {
    public:
        ViewPort_line(const char* l, text_align a = align_left, ViewPort_line* n = 0);
        const char* line;
        text_align align;
        ViewPort_line* next;
};

class ViewPort {
    public:
        ViewPort_line* first_line;
        uint8_t vx_offset;
        uint8_t vy_offset;
        uint8_t dmaxx;
        uint8_t dmaxy;
        uint8_t text_width;
        uint8_t text_height;
        void (*set_cursor)(uint8_t x, uint8_t y);
        void (*print)(char ch);

        ViewPort(uint8_t dx, uint8_t dy, void (*sc)(uint8_t x, uint8_t y), void (*pr)(char ch));
        void clear();
        void refresh();
        void scroll_down();
        void scroll_up();
        void scroll_right();
        void scroll_left();
        void append_line(ViewPort_line* line);
};

#endif

/*

=head1 NAME

    ViewPort.h - render text on limited viewports

=head1 SYNOPSIS

TODO

=head1 DESCRIPTION

TODO

=head1 EXAMPLES

=head2 examples/Arduino_scroll/Arduino_scroll.ino

Arduino display ViewPort with text scrolling.

=head2 examples/term_scroll/

Linux terminal ViewPort with text scrolling.

=head1 LICENSE

This is free software, licensed under the MIT License.

=head1 AUTHOR

    Jozef Kutej

=cut

*/
