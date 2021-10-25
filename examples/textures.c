#include "qg.h"

int main()
{
  qg_init_window(800, 800, "[qg] textures", false);
  QGTexture t = qg_load_texture("logo.png");

  while (qg_window_is_open()) {
    qg_begin_drawing();
    qg_draw_texture(t, 0, 0, QG_WHITE);
    qg_draw_text("This is a texture!", 10, 10, 30, QG_BLACK);
    qg_end_drawing();
  }

  qg_free_texture(t);
  qg_close_window();
  return 0;
}
