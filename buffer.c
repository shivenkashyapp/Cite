#include "buffer.h"

struct buffer buf = CITE_BUF_INIT;

void cite_buffer_write(int fd, struct buffer buf) {
	write(fd, buf.s, buf.len);
}

void cite_buffer_append(struct buffer *buf, const char *s, int len) {
	char *cnew = (char*) realloc(buf->s, buf->len + len);
	if (cnew == NULL) return;
	
	memcpy(&cnew[buf->len], s, len);
	
	buf->s = cnew;
	buf->len += len;
}
