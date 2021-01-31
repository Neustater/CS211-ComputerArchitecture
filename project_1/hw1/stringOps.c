#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char value;
    struct Node *next;
}Node;

Node *head = NULL;

int numNodes = 0;


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
    char *currWord;
    int wordLen;
    Node *ptr;
    Node *vowel;
    int pos;

    for(int i = 1; i < argc; i++){
        currWord = argv[i];
            wordLen = strlen(currWord);
            pos = 0;

                while(pos < wordLen){
                        if(currWord[pos] == 'a' || currWord[pos] == 'e'|| currWord[pos] == 'i' || currWord[pos] == 'o'|| currWord[pos] == 'u'|| currWord[pos] == 'A' || currWord[pos] == 'E'|| currWord[pos] == 'I' || currWord[pos] == 'O'|| currWord[pos] == 'U'){
                            if(head == NULL){
                                head = (Node*) malloc(sizeof(Node));
                                    head->value = currWord[pos];
                                    head->next = NULL;
                                numNodes ++;
                            }
                            else
                            {
                                ptr = head;
                                while(ptr->next != NULL){
                                    ptr = ptr->next;
                                } 

                                vowel = (Node*) malloc(sizeof(Node));
                                    vowel->value = currWord[pos];
                                    vowel->next = NULL;
                                ptr->next = vowel;
                                numNodes++;
                            } 
                        }
                    pos++;
                }
    }
    ptr = head;
    while(ptr != NULL){
        printf("%c",ptr->value);
         ptr = ptr->next;
     }
     printf("\n");

    freeList(head);
    return 0;
}
