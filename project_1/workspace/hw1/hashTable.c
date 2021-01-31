#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int value;
    struct Node *next;
}Node;

int insCol = 0;
int succSearch = 0;
Node *hashtable[10000] = { NULL };

void insert(int val){
    Node *ptr;

    int pos = val % 10000;
    if(pos < 0)
        pos = pos+10000;

    ptr = hashtable[pos];
    if(ptr != NULL)
        insCol++;

    Node *temp = (Node*) malloc(sizeof(Node));
        temp->value = val;
        temp->next = ptr;
    hashtable[pos] = temp;
}

void search(int val){
    Node *ptr;
    int pos = val % 10000;
    if(pos < 0 )
        pos = pos+10000;

    ptr = hashtable[pos];

    while(ptr != NULL){
        if(ptr->value == val){
            succSearch++;
            return;
        }
        ptr = ptr->next;
    }
}

void freeList(Node *head)
{
  Node* tmp;

  while (head != NULL)
    {
      tmp = head;
      head = head->next;
      free(tmp);
    }

}

int main(int argc, char* argv[])
{
    FILE *input;

  input = fopen(argv[1],"r");
  //input = fopen("/Users/michaelneustater/Users/michaelneustater/CS211/project_1/provided/test1.txt","r");

     if(input == NULL){
        printf("error\n");
        exit(0);
    }

    char state;
    int val;

     while(fscanf(input, "%c\t", &state) == 1){
         fscanf(input, "%d\n", &val);
            if(state == 'i')
                insert(val);
            if(state == 's')
                search(val);
     }

     printf("%d\t\n%d\t\n",insCol,succSearch);

    fclose(input);
    for(int i = 0; i<10000; i++)
        freeList(hashtable[i]);
    return 0;
}

