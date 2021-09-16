#include "qg.h"

#define NULL  ((void *)0)

int main()
{
  qg_init_window(800, 800, "[qg] text");
  qg_set_fps(60);
  QGColor c = { 128, 255, 128, 255 };
  QGFont f = qg_load_font("font.ttf");
  int val = 0;

  while (qg_window_is_open()) {
    val++;
    if (val >= 100) {
      val = 0;
    }

    if (qg_is_key_pressed(KEY_S)) {
      qg_take_screenshot("my_screenshot");
    }

    qg_begin_drawing();
    qg_clear(QG_WHITE);
    qg_draw_fps(10, 10, c);
    qg_draw_text("This is some text. Above me is the fps counter", 10, 60, 30, QG_BLACK);
    qg_draw_text_ex(f, "And here is a custom font!", 10, 120, 20, QG_BLACK);
    qg_draw_text(qg_text_format("When the number reaches 100, it resets: %d", val), 10, 160, 25, QG_BLACK);
    qg_draw_text_ex(f, "Press S to take a screenshot!", 15, 300, 30, QG_RED);
    qg_stop_drawing();
  }

  qg_close_window();
  return 0;
}
