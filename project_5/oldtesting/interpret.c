#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE *input;
   //char *file_name = "tk15.asm"; //DEBUGGING MODE MANUAL FILE INPUT
   char *file_name = argv[1];
    input = fopen(file_name, "r");

     if(input == NULL){     //check for NULL input
        printf("error\n");
        exit(0);
    } 
    int lines = 0;

    char ch;
    while((ch = fgetc(input))!=EOF) { //determines line count
      if(ch=='\n')
         lines++;
   }
    fclose(input);

    char** ops = malloc((lines + 1)*sizeof(char*)); //matrix that holds operators
    char** x = malloc((lines + 1)*sizeof(char*));   //matrix that holds x
    char** y = malloc((lines + 1)*sizeof(char*));   //matrix that holds y 
    char** L = malloc((lines + 1)*sizeof(char*));   //matrix that holds L

   for(int i = 0; i < lines; i++){      //malloc 2D of matrixes
    ops[i] = malloc(6 * sizeof(char));
    x[i] = malloc(6 * sizeof(char));
    y[i] = malloc(6 * sizeof(char));
    L[i] = malloc(6 * sizeof(char));
    }

    input = fopen(file_name,"r");   //restarts input stream

    for(int i = 0; i < lines; i++){
        char currLine[100];
        fgets(currLine, 100, input);

        int spaces = 0;
        for(int j = 0; j < strlen(currLine); j++){  //spaces perline to determine number of entries
            if(currLine[j] == ' ')
                spaces++;
        }

        if((strcmp(currLine, "\n") != 0)){      //reads inputs if
            strcpy(ops[i], strtok(currLine, " "));
         //printf("%s \t",ops[i]);              //debug only
            strcpy(x[i], strtok(NULL, " \n"));
         //printf("%d \t",atoi(x[i]));          //debug only
         if(spaces > 1){
            strcpy(y[i], strtok(NULL, " \n"));
            //printf("%d \t",atoi(y[i]));       //debug only
         }
            else{
                strcpy(y[i], "z");              //z is blank field
            }
        if(spaces > 2){
            strcpy(L[i], strtok(NULL, " \n"));
            //printf("%s \t",L[i]);             //debug only
        }
            else{
                strcpy(L[i], "z");              //z is blank field
            }
        }
        else
        {
            strcpy(ops[i], "blank");
            strcpy(x[i], "z");
            strcpy(y[i], "z");    //HANDLE BLANKS
            strcpy(L[i], "z");
        }
        
    }
    int ptr = 0;


//registers
short ax = 0;
short bx = 0;
short cx = 0;
short dx = 0;
short tempx = 0; //temp x register (prevents interaction with matrix)
short tempy = 0; //temp y register
    
int di = 0;

    while(ptr < lines){

        if(ops[ptr][0] == 'j'){                 //JUMP OPERATUONS 
            if((strcmp(ops[ptr],"jmp") == 0)){  //(formatted with redundency to match std operations)
                ptr = atoi(x[ptr]);
                
            }
        else
        {
        char curr_x = y[ptr][0];    //current x reads from y because move shifts param
        short *xi = NULL;
        short readx = (short)atoi(y[ptr]);
        switch(curr_x) {        //determines if x-val is a register or short
            case 'a'  : xi = &ax; break; //ax register
            case 'b'  : xi = &bx; break; //bx resister
            case 'c'  : xi = &cx; break; //cx register
            case 'd'  : xi = &dx; break; //dx register
            case 'z'  : break;           //blank line
            default   : tempx = readx; xi = &tempx; //value given
            }
        
        char curr_y = L[ptr][0]; //current y reads from L because move shifts param
        short *yi = NULL;
        short ready = (short)atoi(L[ptr]);
        switch(curr_y) {        //determines if x-val is a register or short
            case 'a'  : yi = &ax; break; //ax register
            case 'b'  : yi = &bx; break; //bx register
            case 'c'  : yi = &cx; break; //cx register
            case 'd'  : yi = &dx; break; //dx register
            case 'z'  : break;          //blank line
            default   : tempy = ready; yi = &tempy; //value given
            }

        int Li = atoi(x[ptr]);

        char curr_op;
        curr_op = ops[ptr][1];

        switch(curr_op) {
            case 'e'  : if(*xi == *yi) ptr = Li; else ptr++; break;    //equal
            case 'n'  : if(*xi != *yi) ptr = Li; else ptr++; break;    //not equal
            case 'g'  : if(*xi > *yi){ ptr = Li; break;}   //greater than
                        if(strlen(ops[ptr]) == 3)              //greater than or equal
                            if(*xi == *yi) {ptr = Li; break;}
                        ptr++; break;
            case 'l'  : if(*xi < *yi){ ptr = Li; break;}   //less than
                        if(strlen(ops[ptr]) == 3)              //less than or equal
                            if(*xi == *yi) {ptr = Li; break;}
                        ptr++; break;
                }
            }
        }
        else    //NON JUMP OPERATIONS
        {
        if(!(strcmp(ops[ptr],"blank") == 0)){
        char curr_x = x[ptr][0];    //current x
        short *xi = NULL;
        short readx = (short)atoi(x[ptr]);
        switch(curr_x) {        //determines if x-val is a register or short
            case 'a'  : xi = &ax; break; //ax register
            case 'b'  : xi = &bx; break; //bx resister
            case 'c'  : xi = &cx; break; //cx register
            case 'd'  : xi = &dx; break; //dx register
            case 'z'  : break;           //blank line
            default   : tempx = readx; xi = &tempx; //value given
            }
        char curr_y = y[ptr][0]; //current y
        short *yi = NULL;
        short ready = (short)atoi(y[ptr]);
        switch(curr_y) {        //determines if x-val is a register or short
            case 'a'  : yi = &ax; break; //ax register
            case 'b'  : yi = &bx; break; //bx register
            case 'c'  : yi = &cx; break; //cx register
            case 'd'  : yi = &dx; break; //dx register
            case 'z'  : break;          //blank line
            default   : tempy = ready; yi = &tempy; //value given
            }
            char curr_op;
            if((strcmp(ops[ptr],"mul") == 0)) //changes m to n for switch
                curr_op = 'n';
            else
                curr_op = ops[ptr][0];
        
           /* short t;
            if(di == 0){
           t = 12;  di++;}   //FOR DEBUG USE ONLY
           else t = 6;*/

        switch(curr_op) {
            case 'm'  : *yi = *xi; break;           //move
            case 'a'  : *yi = *xi + *yi; break;     //add
            case 's'  : *yi = *yi - *xi; break;     //subtract
            case 'n'  : *yi = (*xi)*(*yi); break;   //mulitply
            case 'd'  : *yi = *xi / *yi; break;     //divide
            case 'r' : scanf("%hi", xi); break;   //read
            //case 'r' : *xi = t; break;              //FOR DEBUG USE ONLY read
            case 'p': printf("%d", *xi); break;            //print
            case 'b': break;                        //blank line
            }
        }
        ptr++;
       }
    }

    //CLEANUP
    printf("\n");
    for(int i = 0; i < lines; i++){
    free(ops[i]); 
    free(x[i]); 
    free(y[i]);
    free(L[i]);
    }
    free(ops);
    free(x);
    free(y);
    free(L);
    fclose(input);
    return 0;
 }
