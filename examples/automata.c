#include "qg.h"

int *cell_output;
int *cell_state;

int main() {
  qg_init_window(640, 480, "[qg] game of life");
  qg_set_fps(60);

  while (qg_window_is_open()) {
    qg_begin_drawing();
    qg_clear(QG_BLACK);
    qg_stop_drawing();
  }

  qg_close_window();
  return 0;
}
