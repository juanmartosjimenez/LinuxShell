#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>


#include "list.h"
#include "parse.h"
#include "ast.h"

//THIS FILE IS TAKEN FROM LECTURE 5 CACULATOR CODE, MOST OF THE FUNCTIONS IN THIS FILE ARE TAKEN FROM THAT LECTURE WITH SOME MODIFICATIONS
//
// Leaf of branch, where only relevant term is vv or command such as "echo hello"
calc_ast*
make_ast_value(list* toks)
{
    calc_ast* ast = malloc(sizeof(calc_ast));
    ast->op = NULL;
    ast->arg0 = 0;
    ast->arg1 = 0;
    ast->value = toks;
    return ast;
}

// relevant term is arg1 and arg2 that stores left and right branches ending with calc_ast leaf, op stores one of / // & && ;
calc_ast*
make_ast_op(char* op, calc_ast* a0, calc_ast* a1)
{
    calc_ast* ast = malloc(sizeof(calc_ast));
    ast->op = op;
    ast->arg0 = a0;
    ast->arg1 = a1;
    ast->value = NULL;
    return ast;
}

//LECTURE 5 CALCULATOR CODE
void
free_ast(calc_ast* ast)
{
    if (ast) {
        if (ast->arg0) {
            free(ast->arg0);
        }

        if (ast->arg1) {
            free(ast->arg1);
        }

        free(ast);
    }
}


//starter code
int execute(char* cmd[]){
    int cpid;
    if (strcmp(cmd[0], "exit")==0){
       exit(1);
    } else if (strcmp(cmd[0], "cd") == 0) {
        chdir(cmd[1]);
        return 0;
    } else {
        if ((cpid = fork())) {
            int status;
            waitpid(cpid, &status, 0);
            return status;
        }
        else {
            execvp(cmd[0], cmd);
        }
    }
}

char* getVal(calc_ast* ast){
    if (ast->value){
        return ast->value->head;
    } else{
        printf("error");
        exit(1);
    }
}

//executes without creating new process
//Potential memory leak because it is not being freed
void
execute_same(calc_ast* ast){
    if(ast->value){
        long length_t = length(ast->value);
        char* arr_toks[length_t+1];
        arr_toks[length_t] = NULL;
        list* temp = ast->value;
        int ii = 0;
        for (;temp;temp=temp->tail){
            arr_toks[ii]= temp->head;
            ii++;
        }
        execvp(arr_toks[0], arr_toks);
    }
}

//CODE FOR redirOut FUNCTION WAS TAKEN FROM LECTURE
int
redirOut(calc_ast* ast){
    int cpid;
    char* file = getVal(ast->arg1);
    if ((cpid = fork())){
        //parent
        int status;
        waitpid(cpid, &status, 0);
        return status;

    } else {
        int fd = open(file, O_CREAT | O_WRONLY, 0644);
        close(1);
        dup(fd);
        
        close(fd);
        execute_same(ast->arg0);
    }
}


//check_rv and redirIn ARE BOTH TAKEN FROM LECTURE with some modifications
void
check_rv(int rv)
{
    if (rv == -1) {
        perror("fail");
        exit(1);
    }
}

int
redirIn(calc_ast* ast){
    int rv;
    int pipefd[2];

    char* msg = getVal(ast->arg1); 
    char* cmd = getVal(ast->arg0);

    //used for interprocess communication 0 is read 1 is write
    //want to write right side to be read by left side commmand
    rv = pipe(pipefd);
    check_rv(rv);

    int p_read = pipefd[0];
    int p_write = pipefd[1];
    int cpid;
        
    if ((cpid = fork())){
        int status;
        check_rv(cpid);
        close(p_read);

        rv = write(p_write, msg, strlen(msg)+1);
        check_rv(rv);
        waitpid(cpid, &status, 0);
        return status;
    }
    else {
        close(p_write);
        char temp[100];
        //read input string
        rv = read(p_read, temp, 100);
        check_rv(rv);
        execlp(cmd,cmd, temp, NULL);

        printf("should not print here \n");
    }
}

//pipe_eval is TAKEN FROM LECTURE with some modifications
int
pipe_eval(calc_ast* ast){
    //fork
    // parent wait
    // child 
    //      pipe
    //      fork
    //      parent
    //      child 
    //      dup pipe to 1
    //      eval cmd 1
    //
    //      fork
    //      parent
    //      child
    //      dup pipe to 0
    //      eval cmd 2
    //    wait on both
    //

    //int cpid_both;
    //calc_ast* cmd0 = ast->arg1;
    //calc_ast* cmd1 = ast->arg0;
    //int pipefd[2];

    //if ((cpid_both = fork())){
    //    int rv;
    //    int status;
    //    //in from 0 
    //    //write in 1 of first process
    //    rv = pipe(pipefd);

    //    check_rv(rv);
    //    waitpid(cpid_both, &status, 0);
    //    return status;

    //}else {
    //        int cpid1, cpid2;

    //        //child 1
    //        if (cpid1 = fork()){
    //            check_rv(cpid1);
    //            
    //        } else {
    //            dup2(pipefd[0],0);
    //            execute_same(cmd0);

    //        }


    //}



}

int
and_eval(calc_ast* ast){
    int stat;
    stat = ast_eval(ast->arg0);
    if (stat ==0){
        stat = ast_eval(ast->arg1);
        return stat;
    } else
    {
        return 1;
    }

}
int
or_eval(calc_ast* ast){
    int stat;
    stat = ast_eval(ast->arg0);
    if (stat == 0){
        return 1;
    } else {
        return ast_eval(ast->arg1);
    }
}

int
background_eval(calc_ast* ast){
    long length_t = length(ast->arg0->value);
    char* arr_toks[length_t+1];
    arr_toks[length_t] = NULL;
    list* temp = ast->arg0->value;
    int ii = 0;
    for (;temp;temp=temp->tail){
        arr_toks[ii]= temp->head;
        ii++;
    }

    if ((fork())) {
        ast_eval(ast->arg1);
    }
    else {
        execvp(arr_toks[0], arr_toks);
    }
}

int
set_env(calc_ast* ast){
    char* var = ast->arg0->value->head;
    char* val = ast->arg1->value->head;
    setenv(var, val, 1);
}

int
down_tree(calc_ast* ast){
    if(ast->value){
        printf("here");
        printf("%s \n", "$HOSTNAME");
    }
    return 1;
}

int
env_print(calc_ast* ast){
    if (strcmp(ast->arg0->value->head, "echo")){
        down_tree(ast->arg1);
    }
    return 1;
}

int
ast_eval(calc_ast* ast)
{
    if(ast->value){
        long length_t = length(ast->value);
        char* arr_toks[length_t+1];
        arr_toks[length_t] = NULL;
        list* temp = ast->value;
        int ii = 0;
        for (;temp;temp=temp->tail){
            arr_toks[ii]= temp->head;
            ii++;
        }
        int stat = execute(arr_toks);
        free_list(temp);
        return stat;
    } else if (strcmp(ast->op, "=") == 0){
        return set_env(ast);
    } else if (strcmp(ast->op, "$") == 0) {
        return env_print(ast);
    } else if (strcmp(ast->op,"|") == 0) {
        return pipe_eval(ast);
    } else if (strcmp(ast->op, "||") == 0){
        return or_eval(ast); 
    } else if (strcmp(ast->op, "&") == 0){
        return background_eval(ast);
    } else if (strcmp(ast->op, "&&") == 0){
        return and_eval(ast);
    } else if (strcmp(ast->op, ";") == 0){
        ast_eval(ast->arg0);
        return ast_eval(ast->arg1);
    } else if (strcmp(ast->op, ">") == 0){
        return redirOut(ast);
    } else if (strcmp(ast->op, "<") == 0){
        return redirIn(ast);
    } else {
    }
}

