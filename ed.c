/*
 * A clone of the ed editor
 * 
 * by pharus (pintomachado@tecnico.ulisboa.pt)
 *
 * ed -> A study on line oriented editing
 * */
#include <stdio.h>
#include <stdlib.h>

#define SIZE_OF_BUFFER 100

typedef struct _Buffer {
	char *buf;
	int cap;
	struct _Buffer *ext;
} *Buffer;

typedef struct _Line {
	int n;
	Buffer b;
	struct _Line *next;
} *Line;


Buffer
BufferCreate(int cap)
{
	Buffer b = (Buffer) malloc(sizeof(struct _Buffer));

	if (!b) {
		perror("Buffer: Failed to allocate instance");
		return NULL;
	}

	b->buf = (char*) malloc(sizeof(char) * cap);

	if (!b->buf) {
		perror("Buffer: Failed to allocate buffer");
		free(b);
		return NULL;
	}

	b->cap = cap;
	b->ext = NULL;
	b->buf[0] = '\0';

	return b;	
}

void
BufferDestroy(Buffer b)
{
	Buffer aux = NULL;
	while (b) {
		aux = b->ext;
		free(b->buf);
		free(b);
		b = aux;
	}
}

int
BufferReadline(Buffer b, FILE * f)
{
	int c;
	int i = 0; /* count characters in buffer */

	while ((c = fgetc(f)) != EOF) {
		b->buf[i] = c;
		i++;
		if (i >= b->cap - 1) {
			if (!b->ext) {	
				b->ext = BufferCreate(SIZE_OF_BUFFER);
				if (!b->ext)
					return 1;
			}
			b->buf[i] = '\0';
			b = b->ext;
			i = 0;
		}
		if (c == '\n')
			break;
	}
	b->buf[i] = '\0';

	return 0;
}

Line
LineCreate(int n, int len)
{
	Line l = (Line) malloc(sizeof(struct _Line));

	if (!l) {
		perror("Line: Failed to allocate instance");
		return NULL;
	}
	l->b = BufferCreate(len);

	if (!l->b) {
		perror("Line: Failed to allocate buffer");
		free(l);
		return NULL;
	}
	l->n = n;
	l->next = NULL;
	return l;
}

void
LineDestroy(Line l)
{
	Line aux = NULL;
	while (l) {
		BufferDestroy(l->b);
		aux = l->next;	
		free(l);
		l = aux;
	}
}

Line
LineReadText(int len, FILE *f)
{
	Line l = NULL, b = NULL, a = NULL;
	int i = 0;


	while (!feof(f)) {
		if (!l) {
			l = LineCreate(i, len);
			b = l;
		} else {
			a = l;
			l->next = LineCreate(i, len);
			l = l->next;
		}
		BufferReadline(l->b, f);

		if (!l->b->buf[0]) {
			a->next = NULL;
			LineDestroy(l);
		}
		i++;
	}

	return b;
}

int
main(int argc, char *argv[])
{
	FILE * f;
	Line l, ptr;

	if (argc > 1) {
		printf("Trying to open %s\n", argv[1]);
		f = fopen(argv[1], "r");

		if (!f) {
			perror(argv[1]);
			exit(1);
		}

		l = LineReadText(SIZE_OF_BUFFER,f);
		ptr = l;
		printf("%p\n", (void*) ptr);
		ptr = l;

		while (ptr) {
			printf("%d -> %s", ptr->n, ptr->b->buf);
			if (!ptr->b->buf[0])
				printf("Shit\n");
			ptr = ptr->next;
		}

		LineDestroy(l);

		fclose(f);
	} else {
		puts("Two arguments please");
	}
	

	return 0;
}

