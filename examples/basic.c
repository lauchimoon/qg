#include "qg.h"

int main()
{
  qg_init_window(800, 800, "[qg] basic", false);
  qg_set_fps(60);

  while (qg_window_is_open()) {
    qg_begin_drawing();
    qg_clear(QG_WHITE);
    qg_draw_text("Hello from qg!", 10, 10, 30, QG_BLACK);
    qg_end_drawing();
  }

  qg_close_window();
  return 0;
}
