#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Node{
    int value;          //stores value for VAR and line number for OP
    char string[500];   //stores VAR name for VAR
    char op;            //Unused for VAR stores OP code for OP

    int *VAR1;  
    int *VAR2;          //Three only used for OP, points to values in VAR and TEMP arrays
    int *OUT;

    struct Node *next;
}Node;

Node *INPUTVAR = NULL;
Node *OUTPUTVAR = NULL;

void insert(Node **head, int val, char* name){      //general node insert
    Node *ptr;
    Node *prev = NULL;
    Node *temp = NULL;

    if(head == NULL){
        *head = (Node*) malloc(sizeof(Node));
            (*head)->value = val;
            (*head)->next = NULL;
            strcpy((*head)->string, name);
            return;
    }
    
    ptr = *head;
    
        while(ptr != NULL){
            prev = ptr;
            ptr = ptr->next;
      }
     temp = (Node*) malloc(sizeof(Node));
            strcpy(temp->string, name);
            temp->value = val;
            temp->next = ptr;
        if(prev != NULL)
            prev->next = temp;
        else
        {
         *head = temp;
        }
}

void insertL(Node **head, int val, char* string, char OP, int *var1, int *var2, int *out){      //long node insert(for OP)
    Node *ptr;
    Node *prev = NULL;
    Node *temp = NULL;

    if(*head == NULL){
        *head = (Node*) malloc(sizeof(Node));
            (*head)->value = val;
            (*head)->next = NULL;
            (*head)->op = OP;
            strcpy((*head)->string, string);
            (*head)->VAR1 = var1;
            (*head)->VAR2 = var2;
            (*head)->OUT = out;
            return;
    }
    
    ptr = *head;
    
        while(ptr != NULL){
            prev = ptr;
            ptr = ptr->next;
      }
     temp = (Node*) malloc(sizeof(Node));
            strcpy(temp->string, string);
            temp->value = val;
            temp->next = ptr;
            temp -> op = OP;
            temp->VAR1 = var1;
            temp->VAR2 = var2;
            temp->OUT = out;
        if(prev != NULL)
            prev->next = temp;
        else
        {
         *head = temp;
        }
}

Node* getNodeN(Node** head, char* name){            //get node using N (will create node in temp if it does not exist)
    Node *ptr = *head;
    Node *varPTR = INPUTVAR;

    while((ptr != NULL) && strcmp(ptr->string, name) != 0){
            ptr = ptr->next;
      }
    if(ptr == NULL){
        while((varPTR != NULL) && strcmp(varPTR->string, name) != 0){
            varPTR = varPTR->next;
      }
        if(varPTR == NULL){
            {
            varPTR = OUTPUTVAR;
            while((varPTR != NULL) && strcmp(varPTR->string, name) != 0){
                varPTR = varPTR->next;
                }
            }
            if(varPTR == NULL){
                insert(&(*head), 9, name);
                ptr = getNodeN(&(*head), name);
            }
            else{
                return varPTR;}
        }
        else{
            return varPTR;}
    }
    return ptr;
}

Node* getNodeL(Node** head,int line){           //get node by line in OPS
    Node *ptr = *head;

    while(ptr->value != line){
            ptr = ptr->next;
      }
    return ptr;  
}

char operator(char* op){            //convers operator strings to chars for switch statement
    if(strcmp(op, "NOT") == 0)
        return '!';
    if(strcmp(op, "AND") == 0)
        return '&';
    if(strcmp(op, "OR") == 0)
        return '|';
    if(strcmp(op, "NAND") == 0)
        return 'A';
    if(strcmp(op, "NOR") == 0)
        return 'O';
    if(strcmp(op, "XOR") == 0)
        return 'X';
    return '#';
}

void freeList(Node* head)           //free linked lists (works with all list types)
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
    
    Node *TEMP = NULL;
    Node *OP = NULL;

    FILE *input;
    //char *file_name = "file name""; //DEBUGGING MODE MANUAL FILE INPUT
    char *file_name = argv[1];
    input = fopen(file_name, "r");

     if(input == NULL){     //check for NULL input
        printf("error\n");
        exit(0);
    } 
    
    /* SETUP */
        int numVars;  
        char oper[20];
        char currLine[500];
        int numVarsTemp = 0;

        fgets(currLine, 1000, input);
        char* token = strtok(currLine, " ");
        strcpy(oper, token);
        if(strcmp(oper, "INPUTVAR") == 0){          //GETS INPUT VARS
            numVarsTemp = atoi(strtok(NULL, " "));
            numVars = numVarsTemp;
            for(int i = 0; i < numVarsTemp; i++){
                token = strtok(NULL, " \n");
                insert(&INPUTVAR, 8, token);
            }
        }
         else{   printf("error\n"); exit(0);}        //NO INPUTS OR OUTPUTS GIVEN IN LINES 0-1 EXIT
        
        fgets(currLine, 1000, input);
        token = strtok(currLine, " ");
        strcpy(oper, token);
        if(strcmp(oper, "OUTPUTVAR") == 0){         //GETS OUTPUT VARS
            numVarsTemp = atoi(strtok(NULL, " "));
            for(int i = 0; i < numVarsTemp; i++){
                token = strtok(NULL, " \n");
                insert(&OUTPUTVAR, 2, token);
            }
        }
        else{   printf("error\n"); exit(0);}        //NO INPUTS OR OUTPUTS GIVEN IN LINES 0-1 EXIT
    
    char op[17];
    char op_string[500];  
    char temp_1[17];  char temp_2[17];  char tempOUT[17];
    int total_ops = 0;

while(fgets(op_string, 500, input) != NULL){        //read operations and fill LL (ALSO CREATES TEMPS SET TO DEBUG VALS )
    total_ops++;                                                                       //{0 MUST BE RESET EACH ITERATION}
    strcpy(op , strtok(op_string, " "));

    if(strcmp("NOT", op) == 0){                 //READ ONE VAR AND OUT IF NOT
        strcpy(temp_1 , strtok(NULL, " "));
        strcpy(tempOUT , strtok(NULL, " \n"));
        int *VAR_1 = &(getNodeN(&TEMP, temp_1)->value);
        int *OUT = &(getNodeN(&TEMP, tempOUT)->value);
        insertL(&OP, total_ops, op_string, operator(op), VAR_1, NULL, OUT); 
   }
    else{                                       //READ TWO VARS AND OUT FOR ALL ELSE
        token = strtok(NULL, " ");
        strcpy(temp_1 , token);
        token = strtok(NULL, " ");
        strcpy(temp_2 , token);
        token = strtok(NULL, " \n");
        strcpy(tempOUT , token);
        Node* tempNode = getNodeN(&TEMP, temp_1);
        int *VAR_1 = &(tempNode->value);          
        tempNode = getNodeN(&TEMP, temp_2);
        int *VAR_2 = &(tempNode->value);
        tempNode = getNodeN(&TEMP, tempOUT);
        int *OUT = &(tempNode->value);
        insertL(&OP, total_ops, op_string, operator(op), VAR_1, VAR_2, OUT);
    }
}

/* EXECUTION */
int numInputs = pow(2, numVars);        //total number of input variations
for(int j = 0; j < numInputs; j++){     //LOOPS THROUGH EVERY POSSIBLE COMBONATION OF BINARY INPUTS (IN ORDER)
    Node *ptr = INPUTVAR;
    for(int z = 1; z<=numVars; z++){                //DETERMINES INPUT VARS
        int shiftedJ = j;
        shiftedJ = shiftedJ >> (numVars- z);        //uses bit shifts to get all binary combos starting from 0
        shiftedJ = shiftedJ & 1;                    //of one and zero for the vars
        ptr->value = shiftedJ;
        ptr = ptr->next;
    }

    ptr = OUTPUTVAR;
    while(ptr != NULL){
        ptr->value = 0;                             //RESETS OUTPUT VARS TO 0
        ptr = ptr->next;
    }

    ptr = TEMP;
    while(ptr != NULL){
        ptr->value = 0;                             //RESETS TEMP VARS TO 0
        ptr = ptr->next;
    }

    char operation;                    
    int eof = 0;

    for(int i = 0; i < total_ops; i++){             //USES LLs to get VARS and OP
        eof = 0;
        Node* tempNode = getNodeL(&OP, i+1);
        char operation = (tempNode->op);
        unsigned int tempOne = *(tempNode->VAR1);
        unsigned int tempTwo = 0;
        if(operation != '!')                       //ONLY ONE VAR FOR NOT
            tempTwo = *(tempNode->VAR2);
        int *tempOT = tempNode->OUT;               //POINTER TO OUTPUT (TEMP OR OUTPUT)

        //int proct_length = pow(2, numVars) - 1;

        switch(operation) {        //determines if x-val is a register or short
            case '!'  : *tempOT = (~tempOne) & 1; break;                    //NOT
            case '&'  : *tempOT = tempOne & tempTwo; break;                 //AND
            case '|'  : *tempOT = tempOne | tempTwo; break;                 //OR
            case 'A'  : *tempOT = (~(tempOne & tempTwo) & 1); break;        //NAND
            case 'O'  : *tempOT = (~(tempOne | tempTwo) & 1) ; break;       //NOR
            case 'X'  : *tempOT = tempOne ^ tempTwo; break;                 //XOR
            case '#'  : eof = 1;               //end of operations (fall back case)
            }

        if(eof == 1)
            break;

    }

    ptr = INPUTVAR;
    while(ptr != NULL){
        printf("%d ", ptr->value);      //PRINTS INPUTS LL
        ptr = ptr->next;
    }

    ptr = OUTPUTVAR;
    while(ptr != NULL){
        printf("%d ", ptr->value);     //PRINTS OUTPUTS LL
        ptr = ptr->next;
    }
    printf("\n");
}

    //CLEANUP
    freeList(INPUTVAR);
    freeList(OUTPUTVAR);
    freeList(TEMP);
    freeList(OP);

    return 0;
}
