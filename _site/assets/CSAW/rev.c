#include <stdio.h>
#include <string.h>


long long printbinchar(char a1);
long long xoring(long long a1);

char flag[32];    //flag size
int index_flag = 0;     //initial position= 0

int main(int argc, const char **argv, const char **envp)
{
    unsigned short v4[8];
    int v5;
    int v6;
    int i;

    strcpy((char *)v4, "BgApYb7nCswD");
    ((char *)&v4[6])[1] = 0; // HIBYTE(v4[6])=0
    v4[7] = 0; 
    v5 = 0;
    v6 = 12;
    printf("That is incorrect :(");
    for (i = 0; i < v6; ++i)
    {
        if ((i & 1) == 0 && i)
            printbinchar(*((char *)v4 + i));
    }
    printf(flag);  // print flag 
    return 0;
}

long long printbinchar(char a1)
{
    long long v2[4];
    int v3;
    char v4;
    int i;

    memset(v2, 0, sizeof(v2));
    v4 = a1;

    for (i = 0; i <= 7; ++i)
    {
        v3 = (v4 << i >> 7) & 1;
        ((int *)v2)[i] = v3;
    }

    return xoring((long long)v2);
}

long long xoring(long long a1)
{
    long long v2[2];
    long long v3[3];
    int j;
    int i;

    v3[0] = 0LL;
    v3[1] = 0LL;
    v2[0] = 0LL;
    v2[1] = 0LL;

    for (i = 0; i <= 3; ++i)
    {
        ((int *)v3)[i] = *((int *)(4LL * i + a1));
        ((int *)v2)[i] = *((int *)(4 * (i + 4LL) + a1));
    }

    for (j = 0; j <= 3; ++j)
    {
        if (((int *)v3)[j] == ((int *)v2)[j])
            flag[index_flag] = '0';
        else
            flag[index_flag] = '1';
        ++index_flag;
    }

    return 0LL;
}
//csawctf{01011100010001110000} - flag
