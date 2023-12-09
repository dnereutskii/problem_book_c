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
#define SIZE_FACTOR (2)
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
    enum state cur_state;
    enum state prev_state;
    void (*state_table[state_num])(struct fsm_in *, struct fsm_out *); 
};


/* Private function prototypes -----------------------------------------------*/
static void print_words(struct list *w);
static void print_word(struct node *w, void *data);
static retval_t save_word(struct list *words, struct array *word);
static void free_words(struct list *words);
static void change_state(enum state cur, enum stete next);
/* state functions */
static void shell1_state_out(struct fsm_in *in, struct fsm_out *out);
static void shell1_state_inword1(struct fsm_in *in, struct fsm_out *out);
static void shell1_state_inword2(struct fsm_in *in, struct fsm_out *out);
static void shell1_state_slash(struct fsm_in *in, struct fsm_out *out)

/* Local constants -----------------------------------------------------------*/
static const char *msg_strings[] = {
    "unmatched quotes",
};

/* Local variables -----------------------------------------------------------*/
struct fsm shell1 = {
    .cur_state = state_out,
    .state_table = {
        &state_func_out,
        &state_func_inword1,
        &state_func_inword2,
        %state_func_slash,
    }
};

int main(int argc, char *argv[])
{
    int c;
    struct array *word;
    struct list *words;

    words = list_init();
    if (words == NULL)
        exit(EXIT_FAILURE);
    word = array_init(INIT_SIZE);
    if (word == NULL) { 
        list_destroy(words);
        exit(EXIT_FAILURE);
    }
    printf(PROMPT_LINE);

    while ((c = getchar()) != EOF) {
        shell1.state_table[shell1.cur_state](c);
    }
    printf("\n");
    array_destroy(word);
    free_words(words);
    list_destroy(words);
        
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

static void change_state(enum state cur, enum stete next)
{
    shell1.prev_state = cur;
    shell1.cur_state = next;
}

static void shell1_state_out(struct fsm_in *in, struct fsm_out *out)
{
    if (isalnum(in->c)) {
        change_state(state_out, state_inword1);
        array_save_char(out->word, in->c);
    } else if (in->c == '\"') {
        change_state(state_out, state_inword2);
    } else if (in->c == '\\') {
        change_state(state_out, state_slash);
    }
}

static void shell1_state_inword1(struct fsm_in *in, struct fsm_out *out)
{
    if (isalnum(in->c)) {
        change_state(state_inword1, state_inword1);
        array_save_char(out->word, in->c);
    } else if (in->c == '\"') {
        change_state(state_inword1, state_inword2);
    } else if (in->c == '\\') {
        change_state(state_inword1, state_slash);
    } else if (isblank(in->c)) {
        change_state(state_inword1, state_out);
        save_word(out->words, out->word);
        array_reset(out->word);
    } else if (in->c == '\n') {
        save_word(out->words, out->word);
        print_words(out->words);
        ree_wors(out->wors)
        fsm_reset(shell1);
        
    }
}

static void shell1_state_inword2(struct fsm_in *in, struct fsm_out *out)
{
    
}

static void shell1_state_slash(struct fsm_in *in, struct fsm_out *out)
{

   
}

