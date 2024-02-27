#include "editor.h"

struct cite_editor_config {
	int srows; 
	int scols;

	struct termios cite_state_original;

	int cx;
	int cy;
} editor;

void cite_editor_init() {
	if (cite_get_winsize(&editor.srows, &editor.scols) == -1) {
		cite_err_exit(1, "Couldn't get window size.\n");
	}

	editor.cx = 0;
	editor.cy = 0;
}

/* Render stuff */
void cite_draw_statusbar() {
	// for (int r=0; r < editor.srows; r++) {	
}

void cite_clearscr(int flush) {
	cite_buffer_append(&buf, "\x1b[2J", 4);
	// forcefully write buffer to tty output
	flush ? cite_buffer_write(STDOUT_FILENO, buf) : 0;
}

void cite_repos() {
	cite_clearscr(1);	

	cite_buffer_append(&buf, "\x1b[H", 3);	
	cite_buffer_write(STDOUT_FILENO, buf);

	cite_clearscr(0);
}

void cite_editor_repaint(int flush) {
	cite_clearscr(flush);	
	
	char b[32];
	snprintf(b, sizeof(b), "\x1b[%d;%dH", editor.cy + 1, editor.cx + 1);
	cite_buffer_append(&buf, b, strlen(b));

	cite_buffer_write(STDOUT_FILENO, buf);
}

/* TTY I/O */
void cite_state_init() {
	/* Initialize terminal config. */
	tcgetattr(STDIN_FILENO, &editor.cite_state_original);
}

void cite_modeset_original() {
	/* Revert terminal config back to the initial state. */
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor.cite_state_original);
}

void cite_modeset_raw() {
	/* Enter "raw" mode for I/O. */
	cite_state_init();
	atexit(cite_modeset_original);

	struct termios raw = editor.cite_state_original;
	
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int cite_get_winsize(int *rows, int *cols) {
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		return -1;
	}

	else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		
		return 0;
	}
}


// TODO: add enum for keys
void cite_move_cursor(char key) {
	switch (key) {
		case 'a':
			if (editor.cx != 0) editor.cx--;
			break;

		case 'd':
			if (editor.cx != editor.scols - 1) editor.cx++;
			break;

		case 'w':
			if (editor.cy != 0) editor.cy--;
			break;

		case 's':
			if (editor.cy != editor.srows - 1) editor.cy++;
			break;
	}
}

/* Keyboard input */
char cite_editor_readkey() {
	int n;
	char key;

	while ((n = read(STDIN_FILENO, &key, 1)) != 1) {
		if (n == -1 && errno != EAGAIN) {
			cite_err_exit(1, "Error whilst calling read()\n");
		}
	}


	// TODO: cleanup needed
	if (key == '\x1b') {
		char sequence[3];

		if (read(STDIN_FILENO, &sequence[0], 1) != 1 || read(STDIN_FILENO, &sequence[1], 1) != 1)
			return '\x1b';

		if (sequence[0] == '[') {
			switch (sequence[1]) {
				case 'A': return 'w';
				case 'B': return 's';
				case 'C': return 'd';
				case 'D': return 'a';
			}
		}

		return '\x1b';
	}

	else {
		return key;
	}
}


void cite_editor_handle_keys() {
	char key = cite_editor_readkey();
	switch (key) {
		case IS_CTRL('q'):
			cite_repos();
			exit(0);
			break;	

		case 'w':
		case 's':
		case 'a':
		case 'd':
			cite_move_cursor(key);
			break;
	}
}
