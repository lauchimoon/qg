#include "qg.h"

int main()
{
  qg_init_window(800, 800, "[qg] pixels");

  while (qg_window_is_open()) {
    qg_begin_drawing();
    qg_clear(QG_WHITE);
    for (int x = 0; x < 800; x++) {
      for (int y = 0; y < 800; y++) {
        QGColor c = (QGColor){ qg_random_int(0, 255), qg_random_int(0, 255), qg_random_int(0, 255), 255 };
        qg_draw_point(x, y, c);
      }
    }
    qg_end_drawing();
  }

  qg_close_window();
  return 0;
}
