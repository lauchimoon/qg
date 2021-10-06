/* qg: Quick Graphics */
#ifndef _QG_H
#define _QG_H

#define QGAPI

/* C and C++ have different ways of filling structures
   C structures are filled like (type){}
   C++ structures are filled like type{} */
#if defined(__cplusplus)
  #define CLITERAL(type)    type
#else
  #define CLITERAL(type)    (type)
#endif

#ifndef PI
  #define PI 3.1415926
#endif

#ifndef RAD2DEG
  #define RAD2DEG (180 / PI)
#endif

#ifndef DEG2RAD
  #define DEG2RAD (PI / 180)
#endif

#define QG_VERSION  "2.0-dev"

#if defined(_WIN32)
  #define QG_DEFAULT_FONT  "C:\\Windows\\Fonts\\arial.ttf"
#elif defined(__linux__)
  #define QG_DEFAULT_FONT  "/usr/share/fonts/ubuntu/UbuntuMono-R.ttf"
#endif

#define QG_WHITE           CLITERAL(QGColor){ 255, 255, 255, 255 }
#define QG_BLACK           CLITERAL(QGColor){ 0, 0, 0, 255       }
#define QG_DARKGRAY        CLITERAL(QGColor){ 68, 68, 68, 255 }
#define QG_GRAY            CLITERAL(QGColor){ 128, 128, 128, 255 }
#define QG_LIGHTGRAY       CLITERAL(QGColor){ 192, 192, 192, 255 }
#define QG_RED             CLITERAL(QGColor){ 255, 50, 100, 255  }
#define QG_ORANGE          CLITERAL(QGColor){ 255, 153, 51, 255  }
#define QG_YELLOW          CLITERAL(QGColor){ 255, 201, 90, 255  }
#define QG_GREEN           CLITERAL(QGColor){ 102, 255, 102, 255 }
#define QG_BLUE            CLITERAL(QGColor){ 0, 128, 255, 255   }
#define QG_PURPLE          CLITERAL(QGColor){ 178, 102, 255, 255 }
#define QG_PINK            CLITERAL(QGColor){ 255, 204, 255, 255 }
#define QG_MAGENTA         CLITERAL(QGColor){ 255, 102, 255, 255 }

typedef struct {
  float x;
  float y;
} QGVec2;

typedef struct {
  float x;
  float y;
  int width;
  int height;
} QGRectangle;

typedef struct {
  float center_x;
  float center_y;
  float radius;
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
  typedef enum { false, true } bool;
#endif

typedef enum {
  KEY_UNKNOWN        =   -1,
  KEY_SPACE          =   32, 
  KEY_APOSTROPHE     =   39,
  KEY_COMMA          =   44,
  KEY_MINUS          =   45,
  KEY_PERIOD         =   46,
  KEY_SLASH          =   47,
  KEY_0              =   48,
  KEY_1              =   49,
  KEY_2              =   50,
  KEY_3              =   51,
  KEY_4              =   52,
  KEY_5              =   53,
  KEY_6              =   54,
  KEY_7              =   55,
  KEY_8              =   56,
  KEY_9              =   57,
  KEY_SEMICOLON      =   59,
  KEY_EQUAL          =   61,
  KEY_A              =   65,
  KEY_B              =   66,
  KEY_C              =   67,
  KEY_D              =   68,
  KEY_E              =   69,
  KEY_F              =   70,
  KEY_G              =   71,
  KEY_H              =   72,
  KEY_I              =   73,
  KEY_J              =   74,
  KEY_K              =   75,
  KEY_L              =   76,
  KEY_M              =   77,
  KEY_N              =   78,
  KEY_O              =   79,
  KEY_P              =   80,
  KEY_Q              =   81,
  KEY_R              =   82,
  KEY_S              =   83,
  KEY_T              =   84,
  KEY_U              =   85,
  KEY_V              =   86,
  KEY_W              =   87,
  KEY_X              =   88,
  KEY_Y              =   89,
  KEY_Z              =   90,
  KEY_LEFT_BRACKET   =   91,
  KEY_BACKSLASH      =   92,
  KEY_RIGHT_BRACKET  =   93,
  KEY_GRAVE_ACCENT   =   96,
  KEY_ESCAPE         =   256,
  KEY_ENTER          =   257,
  KEY_TAB            =   258,
  KEY_BACKSPACE      =   259,
  KEY_INSERT         =   260,
  KEY_DELETE         =   261,
  KEY_RIGHT          =   262,
  KEY_LEFT           =   263,
  KEY_DOWN           =   264,
  KEY_UP             =   265,
  KEY_PAGE_UP        =   266,
  KEY_PAGE_DOWN      =   267,
  KEY_HOME           =   268,
  KEY_END            =   269,
  KEY_CAPS_LOCK      =   280,
  KEY_SCROLL_LOCK    =   281,
  KEY_NUM_LOCK       =   282,
  KEY_PRINT_SCREEN   =   283,
  KEY_PAUSE          =   284,
  KEY_F1             =   290,
  KEY_F2             =   291,
  KEY_F3             =   292,
  KEY_F4             =   293,
  KEY_F5             =   294,
  KEY_F6             =   295,
  KEY_F7             =   296,
  KEY_F8             =   297,
  KEY_F9             =   298,
  KEY_F10            =   299,
  KEY_F11            =   300,
  KEY_F12            =   301,
  KEY_F13            =   302,
  KEY_F14            =   303,
  KEY_F15            =   304,
  KEY_F16            =   305,
  KEY_F17            =   306,
  KEY_F18            =   307,
  KEY_F19            =   308,
  KEY_F20            =   309,
  KEY_F21            =   310,
  KEY_F22            =   311,
  KEY_F23            =   312,
  KEY_F24            =   313,
  KEY_F25            =   314,
  KEY_KP_0           =   320,
  KEY_KP_1           =   321,
  KEY_KP_2           =   322,
  KEY_KP_3           =   323,
  KEY_KP_4           =   324,
  KEY_KP_5           =   325,
  KEY_KP_6           =   326,
  KEY_KP_7           =   327,
  KEY_KP_8           =   328,
  KEY_KP_9           =   329,
  KEY_KP_DECIMAL     =   330,
  KEY_KP_DIVIDE      =   331,
  KEY_KP_MULTIPLY    =   332,
  KEY_KP_SUBTRACT    =   333,
  KEY_KP_ADD         =   334,
  KEY_KP_ENTER       =   335,
  KEY_KP_EQUAL       =   336,
  KEY_LEFT_SHIFT     =   340,
  KEY_LEFT_CONTROL   =   341,
  KEY_LEFT_ALT       =   342,
  KEY_LEFT_SUPER     =   343,
  KEY_RIGHT_SHIFT    =   344,
  KEY_RIGHT_CONTROL  =   345,
  KEY_RIGHT_ALT      =   346,
  KEY_RIGHT_SUPER    =   347,
  KEY_MENU           =   348
} KeyboardKeys;

typedef enum {
  MOUSE_LEFT           =  0,
  MOUSE_RIGHT          =  1,
  MOUSE_MID            =  2
} MouseButtons;

typedef enum {
  QGTEXTURE_FILTER_POINT = 0,
  QGTEXTURE_FILTER_BILINEAR
} TextureFilters;

#if defined(__cplusplus)
extern "C" {
#endif
/* Core functions */

QGAPI void qg_init_window(int width, int height, const char *title);
QGAPI void qg_close_window(void);
QGAPI bool qg_window_is_open(void);
QGAPI void qg_begin_drawing(void);
QGAPI void qg_end_drawing(void);
QGAPI void qg_clear(QGColor c);
QGAPI void qg_set_exit_key(int key);
QGAPI void qg_set_title(const char *title);
QGAPI int qg_random_int(int min, int max);
QGAPI void qg_take_screenshot(const char *filename);
QGAPI QGVec2 qg_get_mouse_pos(void);
QGAPI int qg_screen_width(void);
QGAPI int qg_screen_height(void);
QGAPI void qg_show_cursor(void);
QGAPI void qg_hide_cursor(void);

QGAPI bool qg_is_key_pressed(int key);
QGAPI bool qg_is_key_down(int key);
QGAPI bool qg_is_mouse_pressed(int button);
QGAPI bool qg_is_mouse_down(int button);
QGAPI int qg_get_char_pressed(void);

/* Shapes */

QGAPI void qg_draw_point(int x, int y, QGColor c);
QGAPI void qg_draw_line(int sx, int sy, int ex, int ey, QGColor c);
QGAPI void qg_draw_rectangle(int x, int y, int w, int h, QGColor c);
QGAPI void qg_draw_rectangle_rec(QGRectangle rec, QGColor c);
QGAPI void qg_draw_rectangle_lines(QGRectangle rec, QGColor c);
QGAPI void qg_draw_circle(int cx, int cy, float r, QGColor c);
QGAPI void qg_draw_triangle(QGVec2 v1, QGVec2 v2, QGVec2 v3, QGColor c);
QGAPI bool qg_point_vs_rec(QGVec2 point, QGRectangle rec);
QGAPI bool qg_rec_vs_rec(QGRectangle rec1, QGRectangle rec2);

/* Time */

QGAPI void qg_set_fps(int fps);
QGAPI float qg_get_fps(void);
QGAPI float qg_get_delta(void);

/* Textures */

QGAPI QGTexture qg_load_texture(const char *path);
QGAPI QGTexture qg_load_texture_from_header(const char *filetype, unsigned char *data, unsigned int data_size);
QGAPI void qg_free_texture(QGTexture t);
QGAPI void qg_draw_texture(QGTexture t, int x, int y, QGColor tint);
QGAPI void qg_draw_texture_ex(QGTexture t, QGVec2 pos, float scale, float rot, QGColor tint);
QGAPI void qg_draw_texture_part(QGTexture t, QGRectangle rec, QGVec2 pos, QGColor tint);
QGAPI void qg_set_texture_filter(QGTexture t, int filter);

/* Text */

QGAPI QGFont qg_load_font(const char *path);
QGAPI void qg_draw_text(const char *text, int x, int y, int font_size, QGColor c);
QGAPI void qg_draw_text_ex(QGFont fnt, const char *text, int x, int y, int font_size, QGColor c);
QGAPI char *qg_text_format(const char *text, ...);
QGAPI void qg_draw_fps(int x, int y, QGColor c);

#if defined(__cplusplus)
}
#endif

#endif /* _QG_H */
