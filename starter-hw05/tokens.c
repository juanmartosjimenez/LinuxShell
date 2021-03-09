#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokens.h"
#include "list.h"


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

void
chomp(char* text)
{
    int ln = strlen(text) -1;
    if (*text && text[ln] == '\n'){
        text[ln] = '\0';
    }
}
int
main(int _ac, char* _av[])
{
    char line[100];
    while(1){
        //readline
        printf("tokens$ ");
        fflush(stdout);
        char* rv = fgets(line, 96, stdin);
        //If EOF stop reading 
        if (!rv){
            break;
        }
        chomp(line);

        //tokenize
        list* tokens = tokenize(line);
         
        list* new = reverse(tokens);
        print_list(new);
        //parse
        
        

        //eval

    }
}
