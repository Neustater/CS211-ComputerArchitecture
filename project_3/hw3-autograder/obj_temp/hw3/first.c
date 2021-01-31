#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  FILE *input;

  input = fopen(argv[1],"r");
  //input = fopen("/Users/michaelneustater/Users/michaelneustater/CS211/project_3/hw3-autograder/testcases/first/test2.txt","r");
     
    if(input == NULL){
        printf("error\n");
        exit(0);
    }
    unsigned short num = 0;
    fscanf(input, "%hu\n", &num);

    char op[10];
    int n = 0;
    int v = 0;

    while(fscanf(input, "%s\t", op) == 1){
         fscanf(input, "%d\t", &n);
         fscanf(input, "%d\n", &v);
            if(op[0] == 'g'){
                unsigned short pos = num;
                pos = pos>>n;   //shifting for getting that bit
                printf("%d\n",(pos&1));
                }
            if(op[0] == 'c'){
                unsigned short pos = num;
                pos = pos>>n;   //shifting for getting that bit
                unsigned short modified = (1&pos);
                if(modified == 1){
                modified = modified << n;
                modified = ~modified;
                num = num&modified;
                }
                else
                {
                modified = 1;
                modified = modified << n;
                num = num|modified;
                }
                printf("%d\n",num);
                }
            if(op[0] == 's'){
                unsigned short modified = v;
                if(modified == 1){
                modified = modified << n;
                num = num | modified;
                }
                else
                {
                modified = 1;
                modified = modified << n;
                modified = ~modified;
                num = num&modified;
                }
                printf("%d\n",num);
                }
    }
    return(0);
}
