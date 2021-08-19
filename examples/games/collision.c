#define QGAME_IMPLEMENTATION
#include "../../extensions/qgame.h"

int main() {
  qg_init_window(800, 800, "[qgame] collisions");
  qg_set_fps(60);
  QGRectangle r1 = { 50, 50, 64, 64 };
  QGRectangle r2 = { 450, 450, 128, 64 };
  QGColor c = { 128, 128, 255, 255 };
  QGColor rec_color = QG_BLACK;

  char *text;

  while (qg_window_is_open()) {
    rec_color = QG_BLACK;
    text = "Get to the rectangle below with WASD";
    if (qgame_rects_collide(r1, r2)) {
      rec_color = c;
      text = "Collision!";
    }

    if (qg_is_key_down(KEY_W)) { r1.y -= 5; }
    if (qg_is_key_down(KEY_A)) { r1.x -= 5; }
    if (qg_is_key_down(KEY_S)) { r1.y += 5; }
    if (qg_is_key_down(KEY_D)) { r1.x += 5; }

    qg_begin_drawing();
    qg_clear(QG_WHITE);
    qg_draw_rectangle(r2.x, r2.y, r2.width, r2.height, QG_RED);
    qg_draw_rectangle(r1.x, r1.y, r1.width, r1.height, rec_color);
    qg_draw_text(text, 10, 10, 20, QG_BLACK);
    qg_draw_text(qg_text_format("X: %f/Y: %f", r1.x, r1.y), 10, 35, 20, QG_BLACK);
    qg_stop_drawing();
  }

  qg_close_window();
  return 0;
}
