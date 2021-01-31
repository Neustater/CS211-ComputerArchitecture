#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
unsigned short input = atoi(argv[1]);
//unsigned short input = 384;

unsigned short rev = 0;
unsigned short temp = input;

for(int i = 0; i < 16; i++)
{   rev = rev<<1;
    rev = rev|(temp&1);
    temp = temp>>1;
}

if(rev == input)
{
    printf("Is-Palindrome");
}
else
{
    printf("Not-Palindrome");
}

return(0);

}
