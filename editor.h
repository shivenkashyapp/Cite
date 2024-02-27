#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "buffer.h"
#include "err.h"

extern void cite_editor_init();

/* Drawing stuff */
extern void cite_editor_repaint(int flush);
extern void cite_draw_statusbar();
extern void cite_repos();
extern void cite_clearscr(int flush);

/* TTY I/O */
extern void cite_modeset_original();
extern void cite_modeset_raw();
extern int  cite_get_winsize(int*, int*);

/* Keyboard input */
#define IS_CTRL(key) ((key) & 0x1f)

extern char cite_editor_readkey();
extern void cite_editor_handle_keys();
