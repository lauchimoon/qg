/* Extension for Quick Graphics providing tools to develop videogames */
#ifndef _QGAME_H
#define _QGAME_H

#include "qg.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {
  QGTexture tex;
  int tile_x, tile_y, columns, rows, tile_count;
} QGameTileset;

typedef struct {
  int *grid;
  QGameTileset *ts;
  int x, y, w, h;
} QGameTilemap;

#ifdef __cplusplus
extern "C" {
#endif
int qgame_rects_collide(QGRectangle rec1, QGRectangle rec2);
int qgame_circles_collide(QGCircle c1, QGCircle c2);

QGameTileset *qgame_tileset_new(char *tileset_path, int t_width, int t_height);
void qgame_tileset_free(QGameTileset *ts);
void qgame_tileset_set_size(QGameTileset *ts, int t_width, int t_height);
void qgame_tileset_draw_tile(QGameTileset *ts, int id, int x, int y);

QGameTilemap *qgame_tilemap_new();
void qgame_tilemap_free(QGameTilemap *tm);
void qgame_tilemap_init_size(QGameTilemap *tm, int w, int h);
void qgame_tilemap_set_data(QGameTilemap *tm, int *data, int data_size);
int qgame_tilemap_get_tile(QGameTilemap *tm, int x, int y);
void qgame_tilemap_set_tile(QGameTilemap *tm, int x, int y, int id);
void qgame_tilemap_clear(QGameTilemap *tm);
void qgame_tilemap_draw(QGameTilemap *tm);
void qgame_tilemap_draw_ex(QGameTilemap *tm, int x, int y, int w, int h);
void qgame_tilemap_draw_part(QGameTilemap *tm, int pos_x, int pos_y, int x, int y, int w, int h);
void qgame_tilemap_draw_grid(QGameTilemap *tm, QGColor c);

#ifdef __cplusplus
}
#endif

#define QGAME_IMPLEMENTATION

#if defined(QGAME_IMPLEMENTATION)
int qgame_rects_collide(QGRectangle rec1, QGRectangle rec2) {
  int c = false;
  if (rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x &&
      rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y) {
        c = true;
      }
  
  return c;
}

int qgame_circles_collide(QGCircle c1, QGCircle c2) {
  int c = false;
  float dist_x = c1.center_x - c2.center_x;
  float dist_y = c1.center_y - c2.center_y;
  float dist = sqrt((dist_x+dist_x) * (dist_y+dist_y));

  if (dist <= c1.radius+c2.radius) {
    c = true;
  }

  return c;
}

QGameTileset *qgame_tileset_new(char *tileset_path, int t_width, int t_height) {
  QGameTileset *ts = malloc(sizeof(QGameTileset));
  *ts = (QGameTileset){ 0 };
  ts->tex = qg_load_texture(tileset_path);
  qgame_tileset_set_size(ts, t_width, t_height);

  return ts;
}

void qgame_tileset_free(QGameTileset *ts) {
  qg_free_texture(ts->tex);
  free(ts);
}

void qgame_tileset_set_size(QGameTileset *ts, int t_width, int t_height) {
  ts->tile_x = t_width;
  ts->tile_y = t_height;
  ts->columns = ts->tex.width/t_width;
  ts->rows = ts->tex.height/t_height;
  ts->tile_count = ts->columns * ts->rows;
}

void qgame_tileset_draw_tile(QGameTileset *ts, int id, int x, int y) {
  int col = id % ts->columns;
  int row = id / ts->columns;
  QGRectangle tile_rec = {
    col * ts->tile_x,
    row * ts->tile_y,
    ts->tile_x,
    ts->tile_y
  };
  qg_draw_texture_part(ts->tex, tile_rec, (QGV2D){ (float)x, (float)y}, QG_WHITE);
}


QGameTilemap *qgame_tilemap_new() {
  QGameTilemap *tm = malloc(sizeof(QGameTilemap));
  *tm = (QGameTilemap){ 0 };
  qgame_tilemap_init_size(tm, 1, 1);

  return tm;
}

void qgame_tilemap_free(QGameTilemap *tm) {
  if (tm->grid) {
    free(tm->grid);
  }
  free(tm);
}

void qgame_tilemap_init_size(QGameTilemap *tm, int w, int h) {
  tm->w = w;
  tm->h = h;
  if (tm->grid) {
    free(tm->grid);
  }
  tm->grid = malloc(sizeof(int) * w * h);
  qgame_tilemap_clear(tm);
}

void qgame_tilemap_set_data(QGameTilemap *tm, int *data, int data_size) {
  int count = tm->w * tm->h > data_size? data_size : tm->w * tm->h;
  for (int i = 0; i < count; i++) {
    tm->grid[i] = data[i];
  }
}

int qgame_tilemap_get_tile(QGameTilemap *tm, int x, int y) {
  int less_x = x < 0;
  int less_y = y < 0;
  int bigger_x = x > tm->w - 1;
  int bigger_y = y > tm->h - 1;
  if (less_x || less_y || bigger_x || bigger_y) {
    return -1;
  }

  return tm->grid[tm->w * y + x];
}

void qgame_tilemap_set_tile(QGameTilemap *tm, int x, int y, int id) {
  if (id < -1 || id > tm->ts->tile_count - 1) {
    return;
  }

  int x_in = x > -1 && x < tm->w;
  int y_in = y > -1 && y < tm->h;

  if (x_in && y_in) {
    tm->grid[tm->w * y + x];
  }
}

void qgame_tilemap_clear(QGameTilemap *tm) {
  for (int i = 0; i < tm->w * tm->h; i++) {
    tm->grid[i] = -1;
  }
}

void qgame_tilemap_draw(QGameTilemap *tm) {
  int w = tm->w;
  int h = tm->h;
  int px = tm->x;
  int py = tm->y;
  int sx = tm->ts->tile_x;
  int sy = tm->ts->tile_y;

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int id = tm->grid[x + y * w];
      if (id > -1) {
        qgame_tileset_draw_tile(tm->ts, id, px+x*sx, py+y*sy);
      }
    }
  }
}

void qgame_tilemap_draw_ex(QGameTilemap *tm, int x, int y, int w, int h) {
  qgame_tilemap_draw_part(tm, tm->x, tm->y, x, y, w, h);
}

void qgame_tilemap_draw_part(QGameTilemap *tm, int pos_x, int pos_y, int x, int y, int w, int h) {
  if (x > tm->w || x + w < 0 || y > tm->h || y + h < 0) {
    return;
  }

  /* Find used cells */
  int wi = x + w < tm->w? x + w : tm->w;
  int he = y + h < tm->h? y + h : tm->h;
  x = x > 0? x : 0;
  y = y > 0? y : 0;
  int px = pos_x;
  int py = pos_y;
  int sx = tm->ts->tile_x;
  int sy = tm->ts->tile_y;
  int grid_w = tm->w;

  for (int _y = y; _y < he; _y++) {
    for (int _x = x; _x < wi; _x++) {
      int id = tm->grid[_x + _y * grid_w];
      if (id > -1) {
        qgame_tileset_draw_tile(tm->ts, tm->grid[_x + _y * grid_w], px+_x*sx, py+_y*sy);
      }
    }
  }
}

void qgame_tilemap_draw_grid(QGameTilemap *tm, QGColor c) {
    int x = tm->x;
    int y = tm->y;
    int w = tm->w;
    int h = tm->h;
    int sx = tm->ts->tile_x;
    int sy = tm->ts->tile_y;
    int len_x = w * sx;
    int len_y = h * sy;

    /* Horizontal lines */
    for (int i = 0; i < h + 1; i++) {
      qg_draw_line(x, y+sy*i, x+len_x, y+sy*i, c);
    }

    /* Vertical lines */
    for (int i = 0; i < w + 1; i++) {
      qg_draw_line(x+sx*i, y, x+sx*i, y+len_y, c);
    }
}

#endif

#endif /* _QGAME_H */