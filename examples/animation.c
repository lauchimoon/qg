#include "qg.h"

int main()
{
  qg_init_window(800, 800, "[qg] animation");
  qg_set_fps(60);
  QGTexture t = qg_load_texture("player.png");
  QGRectangle r = { 0, 0, 96, 96 };   /* This is the texture's rectangle */
  int frames_counter = 0;

  while (qg_window_is_open()) {
    frames_counter++;
    if (frames_counter >= 10) {
      frames_counter = 0;
      r.x += 96;
      if (r.x >= 386) {
        r.x = 96;
      }
    }

    qg_begin_drawing();
    qg_clear(QG_WHITE);
    qg_draw_texture_part(t, r, (QGV2D){ 800/2-t.width/2, 800/2-t.height/2 }, QG_WHITE);
    qg_draw_text("Animations are done via rectangles. Check the source code!", 10, 10, 20, QG_BLACK);
    qg_stop_drawing();
  }

  qg_free_texture(t);
  qg_close_window();
  return 0;
}
