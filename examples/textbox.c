#include "qg.h"
#include <stdio.h>

#define MAX_INPUT_CHARS   16

int main() {
  qg_init_window(800, 450, "[qg] textbox");
  qg_set_fps(60);

  char text[MAX_INPUT_CHARS+1] = "\0";
  int letter_count = 0;
  QGRectangle rec = { 800/2 - 225/2, 180, 225, 50 };
  bool mouse_on_text = false;

  while (qg_window_is_open()) {
    // Update
    if (qg_point_vs_rec(qg_get_mouse_pos(), rec)) {
      mouse_on_text = true;
    } else {
      mouse_on_text = false;
    }

    if (mouse_on_text) {
      int key = qg_get_char_pressed();
      
      while (key > 0) {
        if ((key >= 32) && (key <= 125) && (letter_count < MAX_INPUT_CHARS)) {
          text[letter_count] = (char)key;
          text[letter_count+1] = '\0';
          letter_count++;
        }

        key = qg_get_char_pressed();
      }

      if (qg_is_key_pressed(KEY_BACKSPACE)) {
        letter_count--;
        if (letter_count < 0) {
          letter_count = 0;
        }
        text[letter_count] = '\0';
      }
    }

    // Draw
    qg_begin_drawing();
    qg_clear(QG_WHITE);
    qg_draw_rectangle_rec(rec, QG_LIGHTGRAY);
    qg_draw_rectangle_lines(rec, mouse_on_text? QG_RED : QG_BLACK);
    qg_draw_text(text, rec.x+5, rec.y+8, 20, QG_BLACK);
    qg_draw_text("Type something on the grey box!", 10, 10, 50, QG_BLACK);
    qg_stop_drawing();
  }

  qg_close_window();
  return 0;
}
