cite: Cite/cite.c
	echo "Compiling ..." && $(CC) -Iinclude/ Cite/*.c -o cite -Wall -Wextra -pedantic -std=c99 && echo "Running Cite ..." && ./cite && echo
