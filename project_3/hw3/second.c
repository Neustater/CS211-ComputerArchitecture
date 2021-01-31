#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
unsigned short input = atoi(argv[1]);
//unsigned short input = 1000;

int pairs = 0;
int state = 0;
int oBits = 0;
for(int i = 0; i < 16; i++)
{
    if((1&input) == 1){
        oBits++;
        if(state == 1){
            pairs++;
            state = 0;
        }
        else
        {
            state = 1;
        }
    }
    else 
    {
        state = 0;
    }
    input = input>>1;
}

//printf("%d\n",oBits);
if((oBits%2) == 0)
    printf("Even-Parity\t%d\n",pairs);
else
    printf("Odd-Parity\t%d\n",pairs);

return(0);

}
