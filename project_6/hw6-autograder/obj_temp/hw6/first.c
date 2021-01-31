#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*******************************************************************************************
    L1 Cache simulator
    Simulates Assoc, N-Assoc, and Direct caches with LRU and FIFO replacements
    Designed to accept up to 48 bit memory strings (can be modified to handle 64 bit)
    ALL ERRORS USE OUTPUT error FOR AUTOGRADER
    INPUT FORMAT:   ./first <cache size> <associativity> <replace policy> <block size> <trace
    @author: Michael Neustater (Rutgers University: CS211)
********************************************************************************************/

typedef struct Node{ //NODES for blocks
    int v;
    long long tag;
    long long block;
    struct Node *next ;

}Node;

int main(int argc, char* argv[])
{
    /*
    char *associativity = "direct";
    char *replace_policy = "fifo";
    int C = 32; //cache size
    int B = 4; //block size
    int b = log2(B); //block bits (offset)              //USED FOR TESTING IN VSCODE
    int S; //sets                                       //DEBUG ONLY
    int s; //set bits
    int E; //n value (number of blocks per set)
    int t; //tag bits
    //int M = pow(2,48);
    */

    char *associativity = argv[2];
    char *replace_policy = argv[3];
    int C = atoi(argv[1]);  //cache size
    int B = atoi(argv[4]);  //block size
    int b = log(B)/log(2);  //block bits                 //STANDARD INPUT FOR TERMINAL EXECUTION
    int S;                  //sets
    int s;                  //set bits
    int E;                  //n value (number of blocks per set)
    int t;                  //tag bits
    //int M = pow(2,48);

    double b_check = ceil(log2(B));
    double s_check;

    if(b_check != b){   //if Block size is not a power of 2 EXIT
        printf("error\n");
        exit(0);
    }

    if(B > C){          //if Block size is bigger than cache EXIT
        printf("error\n");
        exit(0);
    }

    if(strcmp(associativity,"direct") == 0)             //checks associativity types
        E = 1;
    else if(strcmp(associativity,"assoc") == 0)
        E = C / B;
    else {
        char assoc_check[7];
        char findE[strlen(associativity-5)];
        int len = strlen(associativity);

        if(strlen(associativity) < 7){              //if input is shorter than assoc:N exit
            printf("error\n");
            exit(0);
        }

        for(int i = 0; i<6; i++){                   //gets what should be "assoc:"
            assoc_check[i] = associativity[i];
        }

        if(strcmp("assoc:", assoc_check)!= 0){      //if input is not assoc: EXIT
            printf("error\n");
            exit(0);
        }

        for(int i = 6; i<len; i++){                 //Extracts E (N) from assoc:N
            findE[i-6] = associativity[i];
        }
        E = atoi(findE);                            //Converts extracted string to int
    }


    S = C / (E * B);            //number of sets (size of array of LLs)
    s = log2(S);                //bits for s
    t = 48 - s - b;

    s_check = ceil(log2(S));
    if(s_check != s){           //if Sets is not a power of 2 EXIT
        printf("error\n");
        exit(0);
    }

    Node** memory = malloc(S * sizeof(Node));           //malloc array of nodes (SETS)
    for(int i = 0; i < S; i++){
        memory[i] = NULL;
    }
    Node *gen_ptr;

    for(int i = 0; i < S; i++){                         //Generate Empty Blocks (NODES) in Set
        memory[i] = (Node*) malloc(sizeof(Node));
        memory[i]->v = 0;
        memory[i]->next = NULL;
        gen_ptr = memory[i];
        for(int j = 1; j < E; j++){
            Node *temp = (Node*) malloc(sizeof(Node));
            temp->v = 0;
            temp->next = gen_ptr;
            memory[i]= temp;
            gen_ptr = temp;
        }
    }

    FILE *input;
    //char *file_name = "/Users/michaelneustater/Users/michaelneustater/CS211/project_6/hw6-autograder/testcases/first/input/trace4.txt"; //DEBUGGING MODE MANUAL FILE INPUT
    char *file_name = argv[5];
    input = fopen(file_name, "r");

     if(input == NULL){         //check for NULL input EXIT if NULL
        printf("error\n");
        exit(0);
        }

    char request;                       //Read or Write
    unsigned long long data;            //stores 48 bit data string
    int eof_state = 0;                  //1 if eof is found

    int hit = 0;
    int miss = 0;
    int read = 0;
    int write = 0;

    long long tag;      //tag created from input (long long used to store 48 bits)
    long long set;      //set created from input

    while(eof_state == 0){
    Node *ptr;
    Node *prev_ptr;

    int hit_state = 0;

    char currLine[35];
    fgets(currLine, 35, input);

    char *reader  = strtok(currLine," \n"); //reads input, throws out PC
    if(strcmp(reader,"#eof") == 0){
        eof_state = 1;
        break;
    }

    reader = strtok(NULL, " ");             //reads operation
    request = reader[0];
    reader = strtok(NULL, " ");             //reads data
    data = (long long)strtol(reader, NULL, 0);

    tag = data >> (48 - t);                 //shifts to determine tags and set
    tag = tag & 281474976710655;
    set = data << t;
    set = set & 281474976710655;
    set = set >> (t + b);

   /*if(set >= S){
        printf("Set: %lld \n\n\n S: %d C: %d E: %d B:%d\n",set,S,C,E,B);        //DEBUG OUTPUT
        exit(0);
    }*/
    ptr = memory[set];                      //Begin memory request
    prev_ptr = NULL;
    while(ptr != NULL){
        if(ptr->v == 1){
            if(ptr->tag == tag){
                hit++;                      //DETERMINES IF HIT
                hit_state = 1;
                break;
            }
        }
        else{
            miss++;                         //MISS BY V = 0
            read++;                         //must read from memory request
            hit_state = 0;
            break;
        }
        prev_ptr = ptr;
        ptr = ptr->next;
    }

    if(ptr == NULL){                        //MISS BY NOT EXISTANT IN SET (NULL reached)
        miss++;
        read++;                             //must read from memory
        hit_state = 0;
    }

    if(request == 'W')                      //increments if write request
        write++;

    Node *curr_node;
    Node *ptr_front;
        if(hit_state == 1){
            curr_node = ptr;
        } else
        {
            curr_node = (Node*) malloc(sizeof(Node));
            curr_node->v = 1;
            curr_node-> block = data;
            curr_node-> tag = tag;
        }
    //Replacement proccess begins
    if(strcmp(replace_policy, "lru") == 0){
        ptr_front = memory[set];
        if(hit_state == 1){                     //swap if hit
            if(prev_ptr != NULL){
            prev_ptr->next = curr_node->next;
            memory[set] = curr_node;
            curr_node->next = ptr_front;
            }
        } else{                                 //insert new node remove replaced node
            curr_node->next = ptr_front;
            memory[set] = curr_node;
            ptr = curr_node;
            while(ptr->next != NULL){
                prev_ptr = ptr;
                ptr = ptr->next;
            }
                prev_ptr->next = NULL;
                free(ptr);
        }
    } else if(strcmp(replace_policy, "fifo") == 0){
        if(hit_state == 1){                     //if hit nothing
            //DO NOTHING
        }else{                                  //if new node place at end remove first
               if(prev_ptr != NULL){
                    prev_ptr->next = curr_node;

                   if(ptr != NULL){
                    curr_node->next = ptr->next;
                   }else{
                    curr_node->next = NULL;
                    ptr = memory[set];
                    memory[set] = ptr->next;
                   }
               } else{
                   memory[set] = curr_node;
                   curr_node->next = ptr->next;
               }
               free(ptr);
            }
    } else 
        {
        printf("error\n");
        exit(0);
        }
}

//outputs
printf("Memory reads: %d\n", read);
printf("Memory writes: %d\n", write);
printf("Cache hits: %d\n", hit);
printf("Cache misses: %d\n", miss);

//CLEAN-UP
for(int i = 0; i < S; i++){
        Node *free_ptr = memory[i];
         Node *temp_ptr;

        while(free_ptr != NULL){
            temp_ptr = free_ptr;
            free_ptr = free_ptr->next;
            free(temp_ptr);
        }
    }
    free(memory);
    return 0;
}
