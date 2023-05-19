#ifndef _SHELL_H_
#define _SHELL_H_


#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define END_OF_FILE -2
#define EXIT -3

/* Global environemnt */
extern char **environ;
/* Global program name */
char *name;
/* Global history counter */
int hist;

/* Global variables */
extern char *name;
extern int hist;
extern alias_t *aliases;

typedef struct alias_s {
    char *name;
    char *value;
    struct alias_s *next;
} alias_t;

typedef struct list_s {
    char *dir;
    struct list_s *next;
} list_t;

/*Linked List */
alias_t *add_alias_end(alias_t **head, char *name, char *value);
void free_alias_list(alias_t *head);
list_t *add_node_end(list_t **head, char *dir);
void free_list(list_t *head);

/*Main procs */
void sig_handler(int sig);
int execute(char **args, char **front);
int create_error(char **args, int error_code);
char *get_location(char *command);
void free_args(char **args, char **front);
void free_env(void);
int handle_args(int *exe_ret);
int proc_file_commands(char *filename, int *exe_ret);


/* Other function declarations and macros */

#endif /* SHELL_H */
