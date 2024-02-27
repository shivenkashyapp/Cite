#include "editor.h"


int main() {
	cite_editor_init();
	cite_modeset_raw();

	for (;;) {
		cite_editor_repaint(1);
		cite_editor_handle_keys();	
	}
	
	return 0;
}
