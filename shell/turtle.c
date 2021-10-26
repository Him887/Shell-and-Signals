#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for (i = 0; i < strlen(line); i++) {

        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0) {
                tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        }
        else {
            token[tokenIndex++] = readChar;
        }
    }

    free(token);
    tokens[tokenNo] = NULL ;
    return tokens;
}

void cd(char** tok)
{
    if (tok[2] != NULL)
    {
        printf("Wrong Number of Arguments passed\n");
        return;
    }
    if (tok[1] == NULL)
    {
        printf("Pass atleast one argument\n");
        return;
    }
    char we[1024];
    if (chdir(tok[1]) < 0)
        printf("No such directory\n");
    getcwd(we, 1024);
    printf("%s\n", we);
}

void exec(char **tok)
{
    int i = 0;
    int fork_val = fork();
    if (fork_val == 0) {
        execvp(tok[0], tok);
        printf("Wrong command\n");
        exit(0);
    } else {
        int k;
        waitpid(fork_val, &k, WUNTRACED);
    }
}



void main(void)
{
    char  line[MAX_INPUT_SIZE];
    char  **tokens;
    int i;
    char buf[1024];
    while (1) {

        printf("HimAgrawal> ");
        bzero(line, MAX_INPUT_SIZE);
        gets(line);
        // printf("Got command %s\n", line);
        line[strlen(line)] = '\n'; //terminate with new line
        char* line1 = strtok (line, ";");
        while (1)
        {
            strcpy(buf, line1);
            tokens = tokenize(buf);
            int std_inp = dup(0), std_out = dup(1);
            bool out_red = 0, over = 0, app = 0;
            int i = 0;
            while (tokens[i++] != NULL)
            {
                if (out_red)
                {
                    int fd_out;
                    if (tokens[i - 1] == NULL) {
                        printf("Wrong file Specified");
                        break;
                    }
                    if (over)
                        fd_out = open(tokens[i - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    dup2(fd_out, 1);
                    out_red = 0;
                    tokens[i - 1] = NULL;
                    tokens[i - 2] = NULL;
                }
                else if (strcmp(tokens[i - 1], ">") == 0)
                {
                    over = 1;
                    out_red = 1;
                }
            }
            //do whatever you want with the commands, here we just print them
            if (strcmp(tokens[0], "cd") == 0)
            {
                cd(tokens);
            } else {
                exec(tokens);
            }
            dup2(std_inp, 0);
            dup2(std_out, 1);
            // Freeing the allocated memory
            for (i = 0; tokens[i] != NULL; i++) {
                free(tokens[i]);
            }
            free(tokens);
            line1 = strtok (line1 + strlen (line1) + 1, ";");
            if (line1 == NULL)
                break;
        }
    }
}