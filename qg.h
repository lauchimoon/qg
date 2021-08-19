/* qg: Quick Graphics
   2D Graphics library based on OpenGL for Linux.
   Focused on simplicity, inspired by raylib.
*/
#ifndef _QG_H
#define _QG_H

#if defined(_WIN32)
  #define QGAPI
#else
  #define QGAPI
#endif


/* C and C++ have different ways of filling structures
   C structures are filled like (type){}
   C++ structures are filled like type{} */
#if defined(__cplusplus)
  #define CLITERAL(type)    type
#else
  #define CLITERAL(type)    (type)
#endif

#define PI 3.1415926
#define RAD2DEG (180 / PI)
#define DEG2RAD (PI / 180)
#define QG_VERSION  1.0

#if defined(_WIN32)
  #define QG_DEFAULT_FONT  "C:\\Windows\\Fonts\\arial.ttf"
#elif defined(__linux__)
  #define QG_DEFAULT_FONT  "/usr/share/fonts/ubuntu/UbuntuMono-R.ttf"
#endif

typedef struct {
  float x;
  float y;
} QGV2D;

typedef struct {
  float x;
  float y;
  int width;
  int height;
} QGRectangle;

typedef struct {
  float center_x, center_y, radius;
} QGCircle;

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} QGColor;

typedef struct {
  int width;
  int height;
  unsigned int id;
} QGTexture;

typedef int QGFont;

#ifndef __cplusplus
  #define false 0
  #define true 1
#endif

/* The unknown key */
#define KEY_UNKNOWN            -1

/* Printable keys */
#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348

#define MOUSE_LEFT             0
#define MOUSE_RIGHT            1
#define MOUSE_MID              2

#define QG_WHITE    CLITERAL(QGColor){ 255, 255, 255, 255 }
#define QG_BLACK    CLITERAL(QGColor){ 0, 0, 0, 255 }
#define QG_RED      CLITERAL(QGColor){ 240, 50, 25, 255 }

#if defined(__cplusplus)
extern "C" {
#endif
/* Core functions */

QGAPI void qg_init_window(int width, int height, char *title);
QGAPI void qg_close_window();
QGAPI int qg_window_is_open();
QGAPI void qg_begin_drawing();
QGAPI void qg_stop_drawing();
QGAPI void qg_clear(QGColor c);
QGAPI void qg_set_exit_key(int key);
QGAPI int qg_random_int(int min, int max);
QGAPI void qg_take_screenshot(char *filename);

QGAPI int qg_is_key_pressed(int key);
QGAPI int qg_is_key_down(int key);
QGAPI int qg_is_mouse_pressed(int button);
QGAPI int qg_is_mouse_down(int button);

/* Shapes */

QGAPI void qg_draw_point(int x, int y, QGColor c);
QGAPI void qg_draw_line(int sx, int sy, int ex, int ey, QGColor c);
QGAPI void qg_draw_rectangle(int x, int y, int w, int h, QGColor c);
QGAPI void qg_draw_circle(int cx, int cy, float r, QGColor c);
QGAPI void qg_draw_triangle(QGV2D v1, QGV2D v2, QGV2D v3, QGColor c);

/* Time */

QGAPI void qg_set_fps(int fps);
QGAPI float qg_get_fps();

/* Textures */

QGAPI QGTexture qg_load_texture(char *path);
QGAPI void qg_free_texture(QGTexture t);
QGAPI void qg_draw_texture(QGTexture t, int x, int y, QGColor tint);
QGAPI void qg_draw_texture_ex(QGTexture t, QGV2D pos, float scale, float rot, QGColor tint);
QGAPI void qg_draw_texture_part(QGTexture t, QGRectangle rec, QGV2D pos, QGColor tint);

/* Text */

QGAPI QGFont qg_load_font(char *path);
QGAPI void qg_draw_text(char *text, int x, int y, int font_size, QGColor c);
QGAPI void qg_draw_text_ex(QGFont fnt, char *text, int x, int y, int font_size, QGColor c);
QGAPI char *qg_text_format(char *text, ...);
QGAPI void qg_draw_fps(int x, int y, QGColor c);

#if defined(__cplusplus)
}
#endif

#endif /* _QG_H */
