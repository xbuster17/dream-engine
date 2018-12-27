#ifndef __DE_WINDOW_H__
#define __DE_WINDOW_H__

#include "../libs.h" // sdl

int dwindow_init(int x, int y, int flags);
void dwindow_quit(void);

void dwindow_title(char*);
void dwindow_size(int x, int y);
void dwindow_fullscreen(bool);
bool dwindow_focus(void);
void dwindow_get_gl_info(void); // updates info strings
void dwindow_preconfig(void); // gl version settings called before sdl initialization

void dwindow_resized(int x, int y); // called by event

#endif
