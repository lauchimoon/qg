# Quick Graphics

## Features
- CROSS-PLATFORM! qg is available for Windows and Linux.
- EASY! In just 15 lines of code you can get an OpenGL context up and running.
- SMALL! qg offers just the necessary for quick graphics development.
- PERFORMANT! A very simple qg application will consume almost no CPU.
- With "qgame" you have a small framework to develop games with qg, including tilemaps and collisions.
- Inspired by raylib.
- Developed in pure C.

## Example
This example should create a window of 800x800 with a white background and some text.
```c
#include "qg.h"

int main()
{
  qg_init_window(800, 800, "Basic window");
  qg_set_fps(60);

  while (qg_window_is_open()) {
    qg_begin_drawing();
    qg_clear(QG_WHITE);
    qg_draw_text("hello from qg!", 10, 10, 30, QG_BLACK);
    qg_stop_drawing();
  }

  qg_close_window();
  return 0;
}
```

## Building
- Run `make` on Linux.
- Run `mingw32-make` on Windows.

## Including in projects
qg gets built as a static library. To use it in your projects makes it really simple:

- Linux: `gcc main.c path/to/libqg.a -lGL -lm -ldl -lpthread -o main`
- Windows: `gcc main.c path/to/libqg.a -lopengl32 -lgdi32 -o main.exe`

