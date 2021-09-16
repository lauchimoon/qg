#include "qg.h"

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Non-standard includes */
#include "external/glfw/include/GLFW/glfw3.h"

#if defined(_WIN32)
  void __stdcall Sleep(unsigned long msTimeout);  /* Include Sleep() function signature here to avoid including windows.h */
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#define FONTSTASH_IMPLEMENTATION
#include "external/fontstash.h"

#define GLFONTSTASH_IMPLEMENTATION
#include "external/glfontstash.h"

#define MAX_BUFFERS   4
#define BUFFER_LEN    1024
#define MAX_CHAR_PRESSED_QUEUE  16
#define FORMAT_UNCOMPRESSED_GRAYSCALE     1
#define FORMAT_UNCOMPRESSED_GRAY_ALPHA    2
#define FORMAT_UNCOMPRESSED_R8G8B8        4
#define FORMAT_UNCOMPRESSED_R8G8B8A8      7

typedef struct {
  GLFWwindow *handle;
  int window_width;
  int window_height;
  char *window_title;

  int exit_key;

  double time_current, time_previous;
  double time_update, time_draw;
  double time_frame, time_target;

  int current_key_state[512];
  int previous_key_state[512];
  int current_mouse_state[3];
  int previous_mouse_state[3];

  int char_pressed_queue[MAX_CHAR_PRESSED_QUEUE];
  int char_pressed_queue_count;

  FONScontext *fs;
  QGFont default_font;
} Data;

typedef struct {
  void *data;
  int width, height, format, mipmaps;
} Image;

static void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void init_device(int width, int height);
static void wait(float ms);
static void write_png(char *filename, unsigned char *img_data, int width, int height);
static Image load_img_from_memory(char *filetype, unsigned char *data, unsigned int data_size);
static void free_img(Image img);
QGTexture load_texture_from_img(Image img);

static Data qg_data = { 0 };

/* Create OpenGL context */
void qg_init_window(int width, int height, char *title) {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
#if !defined(QG_NO_LOG)
    printf("[GLFW]: failed to init\n");
#endif
  }

  glfwWindowHint(GLFW_RESIZABLE, 0);

  qg_data.handle = glfwCreateWindow(width, height, title, NULL, NULL);

  qg_data.window_width = width;
  qg_data.window_height = height;
  qg_data.window_title = title;

  if (qg_data.handle == NULL) {
#if !defined(QG_NO_LOG)
    printf("[GLFW]: failed to init window\n");
#endif
    glfwTerminate();
  }

  glfwMakeContextCurrent(qg_data.handle);
  glfwSetKeyCallback(qg_data.handle, key_callback);
  glfwSwapInterval(0);  /* Disables vsync */
  qg_data.exit_key = GLFW_KEY_ESCAPE;

  init_device(qg_data.window_width, qg_data.window_height);

  srand(time(NULL));  /* Seed random number generator */
  qg_data.time_previous = glfwGetTime();

#if !defined(QG_NO_LOG)
  printf("[INFO]: Starting QG v%.1f\n", QG_VERSION);
  printf("[INFO]: Window size of %dx%d\n", qg_data.window_width, qg_data.window_height);
#endif
}

/* Free OpenGL window and close context */
void qg_close_window() {
  glfonsDelete(qg_data.fs);

  glfwDestroyWindow(qg_data.handle);
  glfwTerminate();

#if !defined(QG_NO_LOG)
  printf("[INFO]: Closing QG\n");
#endif
}

/* Repeat until exit_key is pressed */
bool qg_window_is_open() {
  return !glfwWindowShouldClose(qg_data.handle);
}

void qg_begin_drawing() {
  qg_data.time_current = glfwGetTime();
  qg_data.time_update = qg_data.time_current - qg_data.time_previous;
  qg_data.time_previous = qg_data.time_current;

  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
}

/* Swap buffers */
void qg_stop_drawing() {
  glfwSwapBuffers(qg_data.handle);
  glfwPollEvents();
  glFlush();

  qg_data.time_current = glfwGetTime();
  qg_data.time_draw = qg_data.time_current - qg_data.time_previous;
  qg_data.time_previous = qg_data.time_current;

  qg_data.time_frame = qg_data.time_update + qg_data.time_draw;

  double extra_time = 0;

  /* Wait for some time */
  while (qg_data.time_frame < qg_data.time_target) {
    wait((float)(qg_data.time_target - qg_data.time_frame)*1000.0);

    qg_data.time_current = glfwGetTime();
    extra_time = qg_data.time_current - qg_data.time_previous;
    qg_data.time_previous = qg_data.time_current;

    qg_data.time_frame += extra_time;
  }
}

/* Clear the background's color */
void qg_clear(QGColor c) {
  /* Transforming values from 0-1 to 0-255 */
  float r = (float)c.r / 255;
  float g = (float)c.g / 255;
  float b = (float)c.b / 255;
  float a = (float)c.a / 255;

  glClearColor(r, g, b, a);
}

void qg_set_exit_key(int key) {
  qg_data.exit_key = key;
}

int qg_random_int(int min, int max) {
  if (min > max) {
    int tmp = max;
    max = min;
    min = tmp;
  }
  return (rand()%(abs(max-min)+1) + min);
}

/* Note: no extension (.png) required in argument filename */
void qg_take_screenshot(char *filename) {
  char buffer[20];
  int fb_w, fb_h;
  unsigned char *img_data;

  glfwGetFramebufferSize(qg_data.handle, &fb_w, &fb_h);

  img_data = (unsigned char *)malloc(fb_w * fb_h * sizeof(unsigned char) * 4);

  glReadPixels(0, 0, fb_w, fb_h, GL_RGBA, GL_UNSIGNED_BYTE, img_data);

  unsigned char *img_data_flip = (unsigned char *)malloc(fb_w * fb_h * sizeof(unsigned char) * 4);

  for (int y = fb_h-1; y >= 0; y--) {
    for (int x = 0; x < (fb_w * 4); x++) {
      img_data_flip[x + (fb_h-y-1)*fb_w*4] = img_data[x + (y*fb_w*4)];
    }
  }

  free(img_data);

  sprintf(buffer, "%s.png", filename);

  write_png(buffer, img_data_flip, fb_w, fb_h);
  free(img_data_flip);
#if !defined(QG_NO_LOG)
  printf("[INFO]: Screenshot '%s' taken\n", buffer);
#endif
}

QGV2D qg_get_mouse_pos() {
  double xpos, ypos;
  glfwGetCursorPos(qg_data.handle, &xpos, &ypos);

  QGV2D mouse = { (float)xpos, (float)ypos };
  return mouse;
}

int qg_screen_width() {
  return qg_data.window_width;
}

int qg_screen_height() {
  return qg_data.window_height;
}

void qg_show_cursor() {
  glfwSetInputMode(qg_data.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void qg_hide_cursor() {
  glfwSetInputMode(qg_data.handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}


bool qg_is_key_pressed(int key) {
  bool res;

  qg_data.current_key_state[key] = qg_is_key_down(key);
  if (qg_data.current_key_state[key] != qg_data.previous_key_state[key]) {
    if (qg_data.current_key_state[key]) {
      res = true;
    }
    qg_data.previous_key_state[key] = qg_data.current_key_state[key];
  } else {
    res = false;
  }

  return res;
}

bool qg_is_key_down(int key) {
  if (glfwGetKey(qg_data.handle, key) == GLFW_PRESS) {
    return true;
  } else {
    return false;
  }
}

bool qg_is_mouse_pressed(int button) {
  bool res;

  qg_data.current_mouse_state[button] = qg_is_mouse_down(button);
  if (qg_data.current_mouse_state[button] != qg_data.previous_mouse_state[button]) {
    if (qg_data.current_mouse_state[button]) {
      res = true;
    }
    qg_data.previous_mouse_state[button] = qg_data.current_mouse_state[button];
  } else {
    res = false;
  }

  return res;
}

bool qg_is_mouse_down(int button) {
  if (glfwGetMouseButton(qg_data.handle, button) == GLFW_PRESS) {
    return true;
  } else {
    return false;
  }
}

int qg_get_char_pressed() {
  int value = 0;

  if (qg_data.char_pressed_queue_count > 0) {
    // Get character
    value = qg_data.char_pressed_queue[0];

    // Shift elements 1 step toward the head
    for (int i = 0; i < (qg_data.char_pressed_queue_count - 1); i++)
      qg_data.char_pressed_queue[i] = qg_data.char_pressed_queue[i + 1];

    // Reset last character in queue
    qg_data.char_pressed_queue[qg_data.char_pressed_queue_count] = 0;
    qg_data.char_pressed_queue_count--;
  }

  return value;
}



/* Draw a pixel */
void qg_draw_point(int x, int y, QGColor c) {
  glBegin(GL_POINTS);
    glColor4ub(c.r, c.g, c.b, c.a);
    glVertex2i(x, y);
  glEnd();
}

/* Draw a line */
void qg_draw_line(int sx, int sy, int ex, int ey, QGColor c) {
  glBegin(GL_LINES);
    glColor4ub(c.r, c.g, c.b, c.a);
    glVertex2i(sx, sy);
    glVertex2i(ex, ey);
  glEnd();
}

/* Draw a filled rectangle */
void qg_draw_rectangle(int x, int y, int w, int h, QGColor c) {
  glBegin(GL_QUADS);
    glColor4ub(c.r, c.g, c.b, c.a);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
  glEnd();
}

/* Draw a filled circle */
void qg_draw_circle(int cx, int cy, float r, QGColor c) {
  if (r <= 0) {
    r = 0.1;
  }

  glBegin(GL_TRIANGLE_FAN);
    glColor4ub(c.r, c.g, c.b, c.a);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 360; i++) {
      glVertex2f(cx + sin(DEG2RAD*i) * r, cy + cos(DEG2RAD*i) * r);
    }
  glEnd();
}

/* Draw a filled triangle */
void qg_draw_triangle(QGV2D v1, QGV2D v2, QGV2D v3, QGColor c) {
  glBegin(GL_TRIANGLES);
    glColor4ub(c.r, c.g, c.b, c.a);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v3.x, v3.y);
  glEnd();
}

bool qg_point_vs_rec(QGV2D point, QGRectangle rec) {
  bool res = false;

  if (point.x >= rec.x && point.x <= rec.x+rec.width &&
      point.y >= rec.y && point.y <= rec.y+rec.height) {
    res = true;
  }

  return res;
}

bool qg_rec_vs_rec(QGRectangle rec1, QGRectangle rec2) {
  bool res = false;

  if (rec1.x < rec2.x+rec2.width && rec1.x+rec1.width > rec2.x &&
      rec1.y < rec2.y+rec2.height && rec1.y+rec1.height > rec2.y) {
    res = true;
  }

  return res;
}



void qg_set_fps(int fps) {
  qg_data.time_target = 1 / (float)fps;
#if !defined(QG_NO_LOG)
  printf("[INFO]: Screen refreshes every %f seconds\n", qg_data.time_target);
#endif
}

float qg_get_fps() {
  return (1/(float)qg_data.time_target);
}

QGAPI float qg_get_delta() {
  return (float)qg_data.time_frame;
}



QGTexture qg_load_texture(char *path) {
  QGTexture t;

  int width, height, bpp;

  unsigned char *data = stbi_load(path, &width, &height, &bpp, 4);

  if (data == NULL) {
#if !defined(QG_NO_LOG)
    printf("[WARNING]: Failed to load file %s\n", path);
#endif
  }
  
  GLuint id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  /* Free the loaded data */
  stbi_image_free(data);

  t.id = id;
  t.width = width;
  t.height = height;
#if !defined(QG_NO_LOG)
  printf("[INFO]: Loaded texture [ID: %d]\n", t.id);
#endif

  return t;
}

QGTexture qg_load_texture_from_memory(char *filetype, unsigned char *data, unsigned int data_size) {
  QGTexture t = { 0 };
  Image dummy = load_img_from_memory(filetype, data, data_size);
  t = load_texture_from_img(dummy);

  free_img(dummy);
  return t;
}

void qg_free_texture(QGTexture t) {
  glDeleteTextures(1, &t.id);

  if (t.id != 0) {
#if !defined(QG_NO_LOG)
    printf("[INFO]: Unloaded texture [ID: %d]\n", t.id);
#endif
  }
}

void qg_draw_texture(QGTexture t, int x, int y, QGColor tint) {
  qg_draw_texture_ex(t, (QGV2D){ x, y }, 1.0, 0.0, tint);
}

void qg_draw_texture_ex(QGTexture t, QGV2D pos, float scale, float rot, QGColor tint) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, t.id);

  glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.0);
    glScalef(scale, scale, 1.0);
    glRotatef(rot, 0.0, 0.0, 1.0);

    glBegin(GL_QUADS);
      glColor4ub(tint.r, tint.g, tint.b, tint.a);
      glNormal3f(0.0, 0.0, 0.0);
      glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex2f(t.width, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex2f(t.width, t.height);
      glTexCoord2f(0, 1.0); glVertex2f(0, t.height);
    glEnd();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}

void qg_draw_texture_part(QGTexture t, QGRectangle rec, QGV2D pos, QGColor tint) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, t.id);

  glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.0);

    glBegin(GL_QUADS);
      glColor4ub(tint.r, tint.g, tint.b, tint.a);
      glNormal3f(0.0, 0.0, 0.0);

      glTexCoord2f((float)rec.x / t.width, (float)rec.y / t.height);
      glVertex2f(0.0, 0.0);
      
      glTexCoord2f((float)(rec.x + rec.width) / t.width, (float)rec.y / t.height);
      glVertex2f(rec.width, 0.0);
      
      glTexCoord2f((float)(rec.x + rec.width) / t.width, (float)(rec.y + rec.height) / t.height);
      glVertex2f(rec.width, rec.height);
      
      glTexCoord2f((float)rec.x / t.width, (float)(rec.y + rec.height) / t.height);
      glVertex2f(0.0, rec.height);
    glEnd();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}



QGFont qg_load_font(char *path) {
  QGFont fn = fonsAddFont(qg_data.fs, "font", path);

  if (fn == FONS_INVALID) {
#if !defined(QG_NO_LOG)
    printf("[FONTSTASH]: Could not load font %s\n", path);
#endif
  }

  return fn;
}

/* By default it uses /usr/share/fonts/ubuntu/UbuntuMono-R.ttf */
void qg_draw_text(char *text, int x, int y, int font_size, QGColor c) {
  qg_draw_text_ex(qg_data.default_font, text, x, y, font_size, c);
}

/* If a font was loaded, the user may use it in this function */
void qg_draw_text_ex(QGFont fnt, char *text, int x, int y, int font_size, QGColor c) {
  unsigned int color = glfonsRGBA(c.r, c.g, c.b, c.a);

  fonsClearState(qg_data.fs);
  fonsSetFont(qg_data.fs, fnt);

  fonsSetSize(qg_data.fs, (float)font_size);
  fonsSetColor(qg_data.fs, color);
  fonsDrawText(qg_data.fs, (float)x, (float)y+font_size-6, text, NULL);
}

char *qg_text_format(char *text, ...) {
  char buffers[MAX_BUFFERS][BUFFER_LEN] = { 0 };
  int index = 0;

  char *current_buffer = buffers[index];
  memset(current_buffer, 0, MAX_BUFFERS);

  va_list args;
  va_start(args, text);
  vsnprintf(current_buffer, BUFFER_LEN, text, args);
  va_end(args);

  index++;
  if (index >= MAX_BUFFERS) {
    index = 0;
  }

  return current_buffer;
}

void qg_draw_fps(int x, int y, QGColor c) {
  qg_draw_text(qg_text_format("%.0f fps", qg_get_fps()), x, y, 40, c);
}



/* Redirect errors to here */
static void error_callback(int error, const char *description) {
  printf(description);
}

/* Specific keyboard events */
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS && key == qg_data.exit_key) {
    glfwSetWindowShouldClose(qg_data.handle, 1);
  }
}

/* OpenGL setup */
static void init_device(int width, int height) {
  glfwGetFramebufferSize(qg_data.handle, &width, &height);
  glViewport(0, 0, width, height);

  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glEnable(GL_BLEND);   /* Enable color blending */
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  qg_data.fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
  if (qg_data.fs == NULL) {
#if !defined(QG_NO_LOG)
    printf("[FONTSTASH]: Could not create fontstash instance\n");
#endif
  }

  qg_data.default_font = qg_load_font(QG_DEFAULT_FONT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void wait(float ms) {
#if defined(_WIN32)
  Sleep((unsigned int)ms);
#elif defined(__linux__)
  struct timespec ts = { 0, ms * 1000000 };
  nanosleep(&ts, NULL);
#endif
}

static void write_png(char *filename, unsigned char *img_data, int width, int height) {
  stbi_write_png(filename, width, height, 4, img_data, width*4);
}

static Image load_img_from_memory(char *filetype, unsigned char *data, unsigned int data_size) {
  Image image = { 0 };

  if (data != NULL) {
    int comp = 0;
    image.data = stbi_load_from_memory(data, data_size, &image.width, &image.height, &comp, 0);

    if (image.data != NULL) {
      image.mipmaps = 1;

      switch (comp) {
      case 1: image.format = FORMAT_UNCOMPRESSED_GRAYSCALE; break;
      case 2: image.format = FORMAT_UNCOMPRESSED_GRAY_ALPHA; break;
      case 3: image.format = FORMAT_UNCOMPRESSED_R8G8B8; break;
      case 4: image.format = FORMAT_UNCOMPRESSED_R8G8B8A8; break;
      default: break;
      }
    }
  }

  return image;
}

static void free_img(Image img) {
  free(img.data);
}

QGTexture load_texture_from_img(Image img) {
  QGTexture t;

  GLuint id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);

  t.id = id;
  t.width = img.width;
  t.height = img.height;

  return t; 
}
