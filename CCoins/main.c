#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CoinsNew.h"

int main(int argc, char *argv[])
{
    FILE *FileIn;
    FILE *FileOut;
    OpenFile(&FileIn, &FileOut, argv);
    if(!FileIn || !FileOut)
    {
        printf("Input File did not open");
        return -1;
    }

    int Entries = 0;
    int Lines = 0;
    Lines = FindLines(FileIn);
    rewind(FileIn);
    char Tokens[5][20]; //can malloc maybe, use buffer in find lines and then malloc based off the results of first line?
    Information* Test;
    CoinData Coins = DeclareCoins(); //coin data declreation
    Test = (Information*)malloc(Lines * sizeof(Information));
    Zeroing(Test, Lines);
    rewind(FileIn);
    for(int i = 0; i < Lines; i++)
    {
        ReadLine(&FileIn, Tokens);
        if (ErrorCheck(Tokens, Coins) == -1)
        {
            printf("error on Line %d", (i +1));
            continue;
        }
        int Location = (FindName(Tokens[0], Test, Entries));
        if (Location == -1)
        {
            Test[Entries].Name = (char*)malloc((strlen(Tokens[0]) +1) * sizeof(char));
            StoreInfo(Test, Entries, Tokens);
            Entries++;
        }
        else
        {
            StoreInfo(Test, Location, Tokens);
        }
    }
    Menu(Test, Entries, Coins, &FileOut);
    for (int i = 0; i < Entries; i ++)
    {
        free (Test[i].Name);
    }
    free (Test);
    fclose(FileIn);
    fclose(FileOut);
    exit(2);
}
