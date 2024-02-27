#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct buffer {
	char *s;
	int len;
};

extern struct buffer buf;

#define CITE_BUF_INIT {NULL, 0}

extern void cite_buffer_write(int fd, struct buffer buf);
extern void cite_buffer_append(struct buffer *buf, const char *s, int len);
