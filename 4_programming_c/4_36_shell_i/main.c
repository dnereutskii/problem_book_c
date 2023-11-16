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
    size_t word_cnt;
};

enum retval {
    retval_ok = 0,
    retval_err_alloc,
};

enum msg_indx {
    msg_indx_unquot = 0,
    /* errors */
    msg_indx_err_alloc,
};

static void print_words(struct list *w);
static void print_word(struct node *w, void *data);
static enum retval line_init(struct line *l);
static enum retval line_save_char(struct line *l, char ch);
static void line_destroy(struct line *l);
static void line_fill_words(struct list *li, struct line *ln);

const char *msg_strings[] = {
    "unmatched quotes",
    "allocation error",
};

int main(int argc, char *argv[])
{
    int c;
    bool spc = true, quot = false;
    struct line ln;
    struct list *words;

    words = list_init();
    if (words == NULL)
        exit(EXIT_FAILURE);
    if (line_init(&ln) != retval_ok) { 
        list_destroy(words);
        exit(EXIT_FAILURE);
    }
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
            if (!quot) {
                line_save_char(&ln, '\0');
                line_fill_words(words, &ln);
                print_words(words);
            } else {
                printf("%s\n", msg_strings[msg_indx_unquot]);
            }
            printf(PROMPT_LINE);
            line_destroy(&ln);
            list_free(words);
            if (line_init(&ln) != retval_ok) {
                list_destroy(words);
                exit(EXIT_FAILURE);
            }
            spc = true;
            continue;
        }
        line_save_char(&ln, c);
        if (spc)
            spc = false;     
    }
    printf("\n");
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
    l->word_cnt = 0;
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
    if (ch == '\0')
        l->word_cnt++;
    return retval_ok;
}

static void line_destroy(struct line *l)
{
    free(l->buf);
    l->limit = 0;
    l->count = 0;
    l->word_cnt = 0;
    l->buf = NULL;
}

static void line_fill_words(struct list *li, struct line *ln)
{
    assert(ln != NULL);
    assert(li != NULL);
    char *word = ln->buf;
    for (size_t i = 0; i < ln->word_cnt; i++) {
        list_add_to_end(li, (val_t)word);
        word = strchr(word, 0);
        if (word == NULL)
            return;
        word++;    
    }
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

