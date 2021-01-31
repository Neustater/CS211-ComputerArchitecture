#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int value;
    struct Node *left;
    struct Node *right;

}Node;

Node *head = NULL;

void inorder(Node *head) 
{ 
    if (head != NULL) 
    { 
        inorder(head->left); 
        printf("%d\t", head->value); 
        inorder(head->right); 
    } 
} 

void inorderFree(Node* head) //needs work
{ 
    if (head != NULL) 
    { 
        inorder(head->left); 
        inorder(head->right); 
        free(head); 
    } 
} 


void insert(int val, Node *ptr){
   //Node *prev = NULL;
    Node *temp = NULL;

    if(ptr->value > val && ptr->left != NULL){
        insert(val, ptr->left);
        return;
    }
    else if(ptr->value < val && ptr->right != NULL){
        insert(val, ptr->right);
        return;
    }
    else if(ptr->value == val) return;
    
     temp = (Node*) malloc(sizeof(Node));
            temp->value = val;
            temp->right = NULL;
            temp->left = NULL;

        if(ptr->value < val)
            ptr->right = temp;
        else
        {
            ptr->left = temp;
        }
        
}


int main(int argc, char* argv[])
{
    FILE *input;

    input = fopen(argv[1],"r");
    //input = fopen("/Users/michaelneustater/Users/michaelneustater/CS211/project_1/provided/test2.txt","r");

    if(input == NULL){
        printf("error\n");
        exit(0);
    }

    char state; //not important state is always i
    int val;

     while(fscanf(input, "%c\t", &state) == 1){
         fscanf(input, "%d\n", &val);
            if(head == NULL){
                 head = (Node*) malloc(sizeof(Node));
                head->value = val;
                head->left = NULL;
                head->right = NULL;
            }
              else
                insert(val, head);
     }

    inorder(head);
    fclose(input);
    printf("\n");
    return 0;

}
