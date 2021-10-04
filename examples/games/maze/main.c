/* Small game to show what qgame can do.
 * Audio is not included in this demo. Graphics only.
*/
#include "qg.h"

#define QGAME_IMPLEMENTATION
#include "../../../extensions/qgame.h"

#define MAP_WIDTH   20
#define MAP_HEIGHT  19
#define TILE_SIZE   32
#define ALPHA_COLOR (QGColor){ 255, 255, 255, 100 }

enum {
  SCREEN_TITLE = 0,
  SCREEN_GAME,
  SCREEN_END
};

typedef struct {
  QGVec2 pos;
  QGTexture t;
} Player;

void init_resources();
void deinit_resources();
void init_screen(int s);
void update_screen(int s);
void draw_screen(int s);

void update_player();

int map[MAP_HEIGHT][MAP_WIDTH] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
  {1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1},
  {2, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 3},
  {1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

QGameTileset *ts;
QGameTilemap *tm;
QGTexture fog;
QGFont fnt;
QGVec2 fog_pos;
Player p;
int current_screen = SCREEN_TITLE;
int w, h;

int main() {
  w = MAP_WIDTH*TILE_SIZE;
  h = MAP_HEIGHT*TILE_SIZE;
  qg_init_window(w, h, "maze - qgame");
  qg_set_fps(60);
  init_resources();
  init_screen(current_screen);

  while (qg_window_is_open()) {
    update_screen(current_screen);
 
    qg_begin_drawing();
    qg_clear(QG_BLACK);
    draw_screen(current_screen);
    qg_end_drawing();
  }

  deinit_resources();
  qg_close_window();
  return 0;
}

void init_resources() {
  ts = qgame_tileset_new("res/tileset.png", TILE_SIZE, TILE_SIZE);
  tm = qgame_tilemap_new();
  tm->ts = ts;
  tm->x = 0;
  tm->y = 0;
  qgame_tilemap_init_size(tm, MAP_WIDTH, MAP_HEIGHT);
  qgame_tilemap_set_data(tm, *map, MAP_HEIGHT*MAP_WIDTH);

  fnt = qg_load_font("res/font.ttf");
  p.t = qg_load_texture("res/player.png");
  fog = qg_load_texture("res/fog.png");
}

void deinit_resources() {
  qgame_tileset_free(ts);
  qgame_tilemap_free(tm);
  qg_free_texture(p.t);
  qg_free_texture(fog);
}

void init_screen(int s) {
  switch (s) {
  case SCREEN_TITLE: case SCREEN_END:
    p.pos = (QGV2D){ (w/2)/2 + 50, (h/2)/2 };
    break;
  case SCREEN_GAME:
    p.pos = (QGV2D){ 0+32, 0+32*9 };
    break;
  default:
    break;
  }
}

void update_screen(int s) {
  switch (s) {
  case SCREEN_TITLE:
    if (qg_is_key_pressed(KEY_SPACE)) {
      current_screen = SCREEN_GAME;
      init_screen(SCREEN_GAME);
    }
    break;
  case SCREEN_GAME:
    update_player();
    fog_pos.x = p.pos.x-fog.width/2 + 16;
    fog_pos.y = p.pos.y-fog.height/2 + 16;
    break;
  case SCREEN_END:
    break;
  default:
    break;
  }
}

void draw_screen(int s) {
  switch (s) {
  case SCREEN_TITLE:
    qg_draw_text_ex(fnt, "ESCAPE THE MAZE", 45, 30, 40, QG_WHITE);
    qg_draw_text_ex(fnt, "a game by fuserr", 10, 540, 20, QG_WHITE);
    qg_draw_text_ex(fnt, "powered by qgame and qg", 10, 575, 20, QG_WHITE);
    qg_draw_text_ex(fnt, "arrow keys to move", 145, 430, 20, QG_WHITE);
    qg_draw_text_ex(fnt, "press space to begin", 125, 400, 20, QG_WHITE);
    qg_draw_texture_ex(p.t, p.pos, 7.0, 0.0, QG_WHITE);
    break;
  case SCREEN_GAME:
    qgame_tilemap_draw_ex(tm, 0, 0, w, h);
    qg_draw_texture(p.t, p.pos.x, p.pos.y, QG_WHITE);
    qg_draw_texture(fog, fog_pos.x, fog_pos.y, QG_WHITE);
    break;
  case SCREEN_END:
    qg_draw_texture_ex(p.t, p.pos, 7.0, 0.0, ALPHA_COLOR);
    qg_draw_text_ex(fnt, "Congratulations!", 45, 30, 40, QG_WHITE);
    qg_draw_text_ex(fnt, "You have escaped!", 20, 95, 40, QG_WHITE);
    qg_draw_text_ex(fnt, "Press Escape to leave", 40, 450, 30, QG_WHITE);
    break;
  default:
    break;
  }
}


/* Check for collisions, movement, etc */
void update_player() {
  int p_tile_x = p.pos.x / TILE_SIZE;
  int p_tile_y = p.pos.y / TILE_SIZE;
  int rule_north = qgame_tilemap_get_tile(tm, p_tile_x, p_tile_y-1) != 1;
  int rule_east = qgame_tilemap_get_tile(tm, p_tile_x+1, p_tile_y) != 1;
  int rule_south = qgame_tilemap_get_tile(tm, p_tile_x, p_tile_y+1) != 1;
  int rule_west = qgame_tilemap_get_tile(tm, p_tile_x-1, p_tile_y) != 1;

  int rule_no_escape = qgame_tilemap_get_tile(tm, p_tile_x-1, p_tile_y) != 2;
  int rule_finish = qgame_tilemap_get_tile(tm, p_tile_x, p_tile_y) == 3;

  if (qg_is_key_pressed(KEY_UP) && rule_north) {
    p.pos.y -= 32;
  }
  
  if (qg_is_key_pressed(KEY_RIGHT) && rule_east) {
    p.pos.x += 32;
  }
  
  if (qg_is_key_pressed(KEY_DOWN) && rule_south) {
    p.pos.y += 32;
  }

  if (qg_is_key_pressed(KEY_LEFT) && rule_west && rule_no_escape) {
    p.pos.x -= 32;
  }

  if (rule_finish) {
    current_screen = SCREEN_END;
    init_screen(SCREEN_END);
  }
}
