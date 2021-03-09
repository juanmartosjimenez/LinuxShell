#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/wait.h>

#include "tokens.h"
#include "ast.h"
#include "parse.h"
#include "list.h"

//LINE 13 to 84 ARE TAKEN FROM NAT TUCK LECTURE WITH SOME SLIGHT MODIFICATIONS
char*
read_number(const char* text,long  ii){
    int nn= 0;
    while (isalnum(text[ii+nn]) || 45 == text[ii+nn] || 46 == text[ii+nn] || 47 == text[ii+nn] || 95 == text[ii+nn]){
        ++nn;
    }
    char* num = malloc(nn + 1);
    memcpy(num, text + ii, nn);
    num[nn] = 0;
    return num;
}

list* tokenize(const char* text){
    list* xs = 0;

    int nn =strlen(text);
    int ii = 0;
    while (ii< nn){
        if (isblank(text[ii])){
            ++ii;
            continue;
        }
        if(isalnum(text[ii]) || 45 == text[ii] || 46 == text[ii] || 47 == text[ii] || 95 == text[ii]){
            char* num = read_number(text, ii);
            xs = cons(num, xs);
            ii += strlen(num);
            free(num);
            continue;
        }
        if(text[ii]==38){
            if (text[ii+1] == 38) {
                char op[4] = "&&";
                xs = cons(op, xs);
                ii += 2;
                continue;
            } else {
                char op[4] = "&";
                xs = cons(op, xs);
                ii +=1;
                continue;
            }

        }

        if(text[ii]==124){
            if (text[ii+1] == 124) {
                char op[4] = "||";
                xs = cons(op, xs);
                ii += 2;
                continue;
            } else {
                char op[4] = "|";
                xs = cons(op, xs);
                ii +=1;
                continue;
            }

        }
        //else, operator
        char op[4] = "x";//two bytes: 'x', '\0' x and null byte
        op[0] = text[ii];
        xs = cons(op, xs);
        ++ii;

    }

    return rev_free(xs);

}
//LECTURE CODE ENDS HERE 


void
chomp(char* text)
{
    int ln = strlen(text) -1;
    if (*text && text[ln] == '\n'){
        text[ln] = '\0';
    }
}


//STARTER CODE WITH SOME MODIFICATIONS, THE FORMAT OF THIS CODE IS VERY SIMILAR AND HEAVILY RELIANT ON CACULATOR CODE FROM LECTURE 5
int
main(int argc, char* argv[])
{
    char cmd[256];
    int out = 0;
    if(argc == 1){
        while(1){
            printf("nush$ ");
            fflush(stdout);
            char* rv =  fgets(cmd, 256, stdin);
            if (!rv){
                break;
            }
        chomp(cmd);
        list* tokens = tokenize(cmd);
       
        //parse and eval 
        calc_ast* ast = parse(tokens);
        out = ast_eval(ast);
        free_ast(ast);
        free_list(tokens);
        }
    } else {
        FILE* f= fopen(argv[1], "r");

        if (f == NULL){
            printf("File does not exists \n");
            exit(0);
        }

        while(fgets(cmd, 256, f)){
            chomp(cmd);
            list* tokens = tokenize(cmd);
            //parse 
            calc_ast* ast = parse(tokens);
            //eval
            out = ast_eval(ast);
            free_ast(ast);
            free_list(tokens);
            }

        fclose(f);
    }
    return out;
}

