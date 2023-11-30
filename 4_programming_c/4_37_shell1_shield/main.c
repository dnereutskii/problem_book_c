/*
 * Task 4.37 shell1_shield
 * Simple shell program. Takes strings from stdin, devides on words and prints
 * them. Added 2 shielded chars: " and \" 
 * Run: ./shell1
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

struct array {
    char *buf;
    size_t limit;
    size_t count;
};

union flags {
    struct {
        unsigned char wdaccum   : 1;
        unsigned char wdsave    : 1;
        unsigned char lnfin     : 1;
        unsigned char spcaccum  : 1;
        unsigned char shield    : 1;
    } bits;
    unsigned char all;
};

enum retval {
    retval_ok = 0,
    retval_err_alloc,
};

enum msg_indx {
    /* warnings */
    msg_indx_unquot = 0,
    /* errors */
    msg_indx_err_alloc,
};

enum act {
    act_nothing = 0,
    act_save_char,
    act_save_word,
    act_save_word_end_line,
    act_end_line,
    act_err_open_quot,
};

static void print_words(struct list *w);
static void print_word(struct node *w, void *data);
static enum retval array_init(struct array *arr);
static enum retval array_save_char(struct array *arr, char ch);
static void array_destroy(struct array *arr);
static enum act flags_handler(union flags *fls, char c);
static enum retval save_word(struct list *words, struct array *word);
static void free_words(struct list *words);
static void array_reset(struct array *arr);

const char *msg_strings[] = {
    "unmatched quotes",
    "allocation error",
};

int main(int argc, char *argv[])
{
    int c;
    union flags fls = {0};
    struct array word;
    struct list *words;

    words = list_init();
    if (words == NULL)
        exit(EXIT_FAILURE);
    if (array_init(&word) != retval_ok) { 
        list_destroy(words);
        exit(EXIT_FAILURE);
    }
    printf(PROMPT_LINE);

    while ((c = getchar()) != EOF) {
        int res = flags_handler(&fls, c);
        switch (res) {
        case act_nothing:
            continue;
        case act_save_char:
            array_save_char(&word, c);
            break;
        case act_err_open_quot:
            printf("Error: %s\n", msg_strings[msg_indx_unquot]);
            free_words(words);
            array_reset(&word);
            fls.all = 0;
            printf(PROMPT_LINE);
            break;
        case act_save_word:
            save_word(words, &word);
            array_reset(&word);
            fls.all = 0;
            break;
        case act_save_word_end_line:
            save_word(words, &word);
            print_words(words);
            free_words(words);
            array_reset(&word);
            fls.all = 0;
            printf(PROMPT_LINE);
            break;
        case act_end_line:
            print_words(words);
            free_words(words);
            array_reset(&word);
            fls.all = 0;
            printf(PROMPT_LINE);
            break;
        } 
    }
    printf("\n");
    array_destroy(&word);
    free_words(words);
    list_destroy(words);
        
    return 0;
}


static enum retval array_init(struct array *arr)
{
    arr->buf = malloc(INIT_SIZE * sizeof(char));
    if (arr->buf == NULL) {
        perror("array_init()");
        return retval_err_alloc;
    }
    arr->count = 0;
    arr->limit = INIT_SIZE;
    return retval_ok;
}

static enum retval array_save_char(struct array *arr, char ch)
{
    if (arr->count >= arr->limit) {
        arr->buf = realloc(arr->buf, arr->limit * SIZE_FACTOR);
        if (arr->buf == NULL) {
            perror("array_safe_char()");
            return retval_err_alloc;
        }
        arr->limit *= SIZE_FACTOR;
    }
    arr->buf[arr->count] = ch;
    arr->count++;
    return retval_ok;
}

static void array_destroy(struct array *arr)
{
    free(arr->buf);
    arr->limit = 0;
    arr->count = 0;
    arr->buf = NULL;
}

static void print_words(struct list *words)
{
    list_go_through(words, &print_word, NULL);
}

static void print_word(struct node *word, void *data)
{
    (void)data;
    printf("[%s]\n", word->val);
}

static enum act flags_handler(union flags *fls, char c)
{
    if (isalpha(c)) {
        if (!fls->bits.wdaccum)
            fls->bits.wdaccum = 1;
        return act_save_char;
    }
    if (isblank(c)) {
        if (fls->bits.wdaccum) {
            if(fls->bits.spcaccum)
                return act_save_char;
            else
                return act_save_word;
        } else if (fls->bits.spcaccum) {
            fls->bits.wdaccum = 1;
            return act_save_char;
        } else {
            return act_nothing;
        }
        
    }
    if (c == '\"') {
        fls->bits.spcaccum ^= 1;
        return act_nothing;
    }
    if (c == '\n') {
        if (fls->bits.spcaccum)
            return act_err_open_quot;
        if (fls->bits.wdaccum)
            return act_save_word_end_line;
        else
            return act_end_line;
    }
    return act_nothing;
}

static enum retval save_word(struct list *words, struct array *word)
{
    char *tmp = malloc((word->count + 1) * sizeof(char));
    if (tmp == NULL) {
        perror(msg_strings[msg_indx_err_alloc]);
        return retval_err_alloc;
    }
    memcpy(tmp, word->buf, word->count);
    tmp[word->count] = '\0';
    list_add_to_end(words, (val_t)tmp);
    return retval_ok;
}

static void free_words(struct list *words)
{
    struct node *word, *tmp;
    
    if (words->first == NULL) /* empty list */
        return;
    word = words->first;
    while (word != NULL) {
        free(word->val); /* free val */
        tmp = word;
        word = word->next;
        free(tmp); /* free node */
    }
    words->first = NULL;
    words->last = NULL;
    words->cnt = 0;
}

static void array_reset(struct array *arr)
{
    arr->count = 0;
    arr->buf[0] = '\0';
}
