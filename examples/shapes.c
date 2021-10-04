#include "qg.h"

int main()
{
  qg_init_window(800, 800, "[qg] shapes");
  qg_set_fps(60);

  while (qg_window_is_open()) {
    qg_begin_drawing();
    qg_clear(QG_WHITE);

    /* q */
    qg_draw_rectangle(90, 250, 160, 28, QG_BLACK);
    qg_draw_rectangle(250, 240, 28, 350, QG_BLACK);
    qg_draw_rectangle(90, 250, 28, 150, QG_BLACK);
    qg_draw_rectangle(90, 400, 165, 28, QG_BLACK);

    /* g */
    qg_draw_rectangle(370, 250, 160, 28, QG_BLACK);
    qg_draw_rectangle(530, 240, 28, 350, QG_BLACK);
    qg_draw_rectangle(370, 250, 28, 150, QG_BLACK);
    qg_draw_rectangle(370, 400, 165, 28, QG_BLACK);
    qg_draw_rectangle(370, 562, 165, 28, QG_BLACK);

    qg_draw_circle(650, 300, 50.0, QG_RED);
    
    qg_end_drawing();
  }

  qg_close_window();
  return 0;
}
