#include "qg.h"

void reload();

QGTexture t = { 0 };

int main()
{
  qg_init_window(800, 800, "[qg] texture reloading", false);
  qg_set_fps(60);
  t = qg_load_texture("logo.png");

  while (qg_window_is_open()) {
    if (qg_is_key_pressed(KEY_R)) {
      reload();
    }

    qg_begin_drawing();
    qg_clear(QG_WHITE);
    qg_draw_texture(t, 0, 0, QG_WHITE);
    qg_draw_text("Modify the texture 'logo.png' and press r to see the changes!", 10, 10, 24, QG_BLACK);
    qg_end_drawing();
  }

  qg_free_texture(t);
  qg_close_window();
  return 0;
}

void reload() {
  qg_free_texture(t);
  t = qg_load_texture("logo.png");
}
