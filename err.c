#include "err.h"

void cite_err_exit(int code, const char *msg) {
	if (code) {
		perror(msg);
		exit(code);
	}
}
