/*
 * Task 4.36 shell_I
 * Simple shell program. Takes strings from stdin, devides on words and prints
 * them.
 * Run: ./shi
 *
 * @author rhetti
 * @date 11.2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "list.h"

#define INIT_SIZE   (20)
#define SIZE_FACTOR (2)
#define PROMPT_LINE "> "

struct line {
    char *buf;
    size_t limit;
    size_t count;
};

enum retval {
    retval_ok = 0,
    retval_err_alloc,
};

static void print_words(struct list *w);
static void print_word(struct node *w, void *data);
static enum retval line_init(struct line *l);
static enum retval line_save_char(struct line *l, char ch);
static void line_destroy(struct line *l);

int main(int argc, char *argv[])
{
    int c;
    bool spc = true, quot = false;
    struct line ln;
    struct list *words;

    words = list_init();
    if (words == NULL)
        exit(EXIT_FAILURE);
    if (line_init(&ln) != retval_ok) 
        exit(EXIT_FAILURE);
    printf(PROMPT_LINE);
    while ((c = getchar()) != EOF) {
        if (c == '"') {
            quot = !quot;
            continue;
        }
        if (isblank(c)) {
            if (quot) {
                line_save_char(&ln, c);
                continue;
            }
            if (spc)
                continue;
            line_save_char(&ln, '\0');
            spc = true;
            continue;
        }
        if (c == '\n') {
            line_save_char(&ln, '\0');
            print_words(words);
            printf("> ");
            line_destroy(&ln);
            list_free(words);
            if (line_init(&ln) != retval_ok)
                exit(EXIT_FAILURE);
            spc = true;
            continue;
        }
        line_save_char(&ln, c);
        if (spc) {
            list_add_to_end(words, (val_t)&ln.buf[ln.count - 1]);
            spc = false;
        }       
    }
    line_destroy(&ln);
    list_destroy(words);
    
    return 0;
}


static enum retval line_init(struct line *l)
{
    l->buf = malloc(INIT_SIZE * sizeof(char));
    if (l->buf == NULL) {
        perror("line_init()\n");
        return retval_err_alloc;
    }
    l->count = 0;
    l->limit = INIT_SIZE;
    return retval_ok;
}

static enum retval line_save_char(struct line *l, char ch)
{
    if (l->count >= l->limit) {
        l->buf = realloc(l->buf, l->limit * SIZE_FACTOR);
        if (l->buf == NULL) {
            perror("safe_char()\n");
            return retval_err_alloc;
        }
        l->limit *= SIZE_FACTOR;
    }
    l->buf[l->count] = ch;
    l->count++;
    return retval_ok;
}

static void line_destroy(struct line *l)
{
    free(l->buf);
    l->limit = 0;
    l->count = 0;
    l->buf = NULL;
}

static void print_words(struct list *w)
{
    list_go_through(w, &print_word, NULL);
}

static void print_word(struct node *w, void *data)
{
    (void)data;
    printf("[%s]\n", w->val);
}
