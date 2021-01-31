#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    int value;
    struct Node *next;
}Node;

Node *head = NULL;

int numNodes = 0;

void delete(int val){
    Node *ptr;
    Node *prev;
    
    if(head == NULL)
        return;
    
    ptr = head;
    //Node *temp = NULL;
    
    while(ptr != NULL && (ptr->value) != val){
        prev = ptr;
        ptr = ptr->next;
    }

    if(ptr == NULL)
        return;
    if(head == ptr)
        head = head->next;
    else
        prev->next = ptr->next;

    numNodes--;

    free(ptr);
}

void insert(int val){
    Node *ptr;
    Node *prev = NULL;
    Node *temp = NULL;

    if(head == NULL){
        head = (Node*) malloc(sizeof(Node));
            head->value = val;
            head->next = NULL;
            numNodes ++;
            return;
    }
    
    ptr = head;
    
        while(ptr != NULL && (ptr->value) < val){
            prev = ptr;
            ptr = ptr->next;
      }

     temp = (Node*) malloc(sizeof(Node));
            temp->value = val;
            temp->next = ptr;
        if(prev != NULL)
            prev->next = temp;
        else
        {
         head = temp;
        }
        
    numNodes++;
}

void freeList(Node* head)
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
            if(state == 'd')
                if(head != NULL)
                    delete(val);
            if(state == 'i')
                insert(val);

     }
    Node *ptr = head;
    int prev;

    printf("%d\n",numNodes);

     while(ptr != NULL){
        if((ptr->value) != prev)
             printf("%d\t",ptr->value);
        prev = ptr->value;
         ptr = ptr->next;
     }

     fclose(input);
     freeList(head);
     printf("\n");
     return 0;
}
