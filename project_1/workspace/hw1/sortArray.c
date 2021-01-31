#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
 
    FILE *input;
    
    int c;
    int *evens; //even array
    int *odds; //odd array
    
    int curr;
    int eptr=0; //current position in even array
    int optr=0; //current position in odd array

  input = fopen(argv[1],"r");
  //input = fopen("/Users/michaelneustater/Users/michaelneustater/CS211/project_1/autograder/hw1/test2.txt","r");

     if (input == NULL){
        perror("no file found");
        exit(0);
  }
    fscanf(input,"%d\n", &c);

    evens = (int*) malloc(sizeof(int) * c);
    odds = (int*) malloc(sizeof(int) * c);

    while(fscanf(input, "%d", &curr) == 1){
        if(curr % 2 == 0){
            evens[eptr] = curr;
            eptr++;
            }
        else{
             odds[optr] = curr;
             optr++;
            }
    }
    int currpos = 0;
    int holder = 0;
    int max = eptr;

    while(max > 1){ //sort evens array
        currpos = 0;
        do{
            holder = 0;
            if(evens[currpos] > evens[currpos + 1])
                {
                    holder = evens[currpos + 1];
                    evens[currpos + 1] = evens[currpos];
                    evens[currpos] = holder;
                }
                currpos++;
        } while((currpos + 1) < max);
        max--;
    }
    
    
    currpos = 0;
    holder = 0;
    int min = optr;

    while(min > 1){ //sort odds array
        currpos = 0;
        do{
            holder = 0;
            if(odds[currpos] < odds[currpos + 1])
                {
                    holder = odds[currpos + 1];
                    odds[currpos + 1] = odds[currpos];
                    odds[currpos] = holder;
                }
                currpos ++;
        } while((currpos + 1) < min);
        min--;
    }
    for (int i = 0; i < eptr; i++){
    printf("%d\t", evens[i]);
    }

    for (int i = 0; i < optr; i++){
    printf("%d\t", odds[i]);
    }

    printf("\n");
    free(odds);//freeing the number array that was malloced
    free(evens);//freeing the number array that was malloced
    fclose(input);
    return 0;

    
}
