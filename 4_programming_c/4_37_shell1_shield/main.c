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
#include "array.h"
#include "retval.h"

/* Defines -------------------------------------------------------------------*/
#define INIT_SIZE   (20)
#define PROMPT_LINE "> "

/* Enum delarations ----------------------------------------------------------*/
enum msg_indx {
    /* warnings */
    msg_indx_unquot = 0,
};

enum state {
    state_out = 0,
    state_inword1,
    state_inword2,
    state_slash,
    
    state_num,
};

/* Struct declarations -------------------------------------------------------*/
struct fsm_in {
    int c;
};

struct fsm_out {
    struct array *word;
    struct list *words;
};

struct fsm {
    struct fsm_in in;
    struct fsm_out out;
    enum state cur_state;
    enum state prev_state;
    void (* const state_table[state_num])(void);
};


/* Private function prototypes -----------------------------------------------*/
static void print_words(struct list *w);
static void print_word(struct node *w, void *data);
static retval_t save_word(struct list *words, struct array *word);
static void free_words(struct list *words);
static void change_state(enum state next);
/* state functions */
static void shell1_state_out(void);
static void shell1_state_inword1(void);
static void shell1_state_inword2(void);
static void shell1_state_slash(void);

/* Local constants -----------------------------------------------------------*/
static const char *msg_strings[] = {
    "unmatched quotes",
};

/* Local variables -----------------------------------------------------------*/
static struct fsm shell1 = {
    .cur_state = state_out,
    .prev_state = state_out,
    .state_table = {
        &shell1_state_out,
        &shell1_state_inword1,
        &shell1_state_inword2,
        &shell1_state_slash,
    }
};

/* Function definitions ------------------------------------------------------*/
int main(int argc, char *argv[])
{
    shell1.out.words = list_init();
    if (shell1.out.words == NULL)
        exit(EXIT_FAILURE);
    shell1.out.word = array_init(INIT_SIZE);
    if (shell1.out.word == NULL) { 
        list_destroy(shell1.out.words);
        exit(EXIT_FAILURE);
    }
    printf(PROMPT_LINE);

    while ((shell1.in.c = getchar()) != EOF)
        shell1.state_table[shell1.cur_state]();
    printf("\n");
    array_destroy(shell1.out.word);
    free_words(shell1.out.words);
    list_destroy(shell1.out.words);
        
    return 0;
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

static retval_t save_word(struct list *words, struct array *word)
{
    char *tmp = malloc((word->count + 1) * sizeof(char));
    if (tmp == NULL) {
        perror(ERROR_ALLOCATION);
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

    assert(words != NULL);
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

static void fsm_reset(void)
{
    shell1.cur_state = state_out;
    shell1.prev_state = state_out;
}

static void change_state(enum state next)
{
    shell1.prev_state = shell1.cur_state;
    shell1.cur_state = next;
}

static void shell1_state_out(void)
{
    if (isalnum(shell1.in.c)) {
        change_state(state_inword1);
        array_save_char(shell1.out.word, shell1.in.c);
    } else if (shell1.in.c == '\"') {
        change_state(state_inword2);
    } else if (shell1.in.c == '\\') {
        change_state(state_slash);
    }
}

static void shell1_state_inword1(void)
{
    if (isalnum(shell1.in.c)) {
        array_save_char(shell1.out.word, shell1.in.c);
    } else if (shell1.in.c == '\"') {
        change_state(state_inword2);
    } else if (shell1.in.c == '\\') {
        change_state(state_slash);
    } else if (isblank(shell1.in.c)) {
        change_state(state_out);
        save_word(shell1.out.words, shell1.out.word);
        array_reset(shell1.out.word);
    } else if (shell1.in.c == '\n') {
        save_word(shell1.out.words, shell1.out.word);
        print_words(shell1.out.words);
        free_words(shell1.out.words);
        array_reset(shell1.out.word);
        fsm_reset();
        printf(PROMPT_LINE);
    }
}

static void shell1_state_inword2(void)
{
    if (isalnum(shell1.in.c) || isblank(shell1.in.c)) {
        array_save_char(shell1.out.word, shell1.in.c);
    } else if (shell1.in.c == '\"') {
        change_state(state_inword1);
    } else if (shell1.in.c == '\\') {
        change_state(state_slash);
    } else if (shell1.in.c == '\n') {
        printf("Error: %s\n", msg_strings[msg_indx_unquot]);
        printf(PROMPT_LINE);
        free_words(shell1.out.words);
        array_reset(shell1.out.word);
        fsm_reset();
    }
}

static void shell1_state_slash(void)
{
    if (shell1.in.c == '\n') {
        if (shell1.out.word->count != 0) {
            save_word(shell1.out.words, shell1.out.word);
            print_words(shell1.out.words);
            free_words(shell1.out.words);
            array_reset(shell1.out.word);
        }
        printf(PROMPT_LINE);
        fsm_reset();
    } else {
        array_save_char(shell1.out.word, shell1.in.c);
        change_state(shell1.prev_state);
    }
   
}

