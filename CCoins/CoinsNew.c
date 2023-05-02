#include "CoinsNew.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//added
CoinData DeclareCoins()
{
    CoinData TempCoins;

    TempCoins.USAC[0] = 50;
    TempCoins.USAC[1] = 25;
    TempCoins.USAC[2] = 10;
    TempCoins.USAC[3] = 1;

    TempCoins.AUSC[0] = 50;
    TempCoins.AUSC[1] = 20;
    TempCoins.AUSC[2] = 10;
    TempCoins.AUSC[3] = 5;

    TempCoins.EURC[0] = 20;
    TempCoins.EURC[1] = 10;
    TempCoins.EURC[2] = 5;
    TempCoins.EURC[3] = 1;

    strcpy(TempCoins.USA, "US$");
    strcpy(TempCoins.AUS, "AU$");
    strcpy(TempCoins.EUR, "EUR");

    return TempCoins;
}
//Opens the Files using command line arguments
void OpenFile(FILE **FileIn, FILE **FileOut, char *argv[])
{
    *FileIn = fopen(argv[1], "r");
    *FileOut = fopen(argv[2], "w");
    return;
}
//reads a line and store into array of chars
void ReadLine(FILE** InFile, char Tokens[5][20])
{
    fscanf(*InFile, "%s %s %s %s %s\n",
            Tokens[0], Tokens[1], Tokens[2], Tokens[3], Tokens[4]);
}
//checks for errors
int ErrorCheck(char Tokens[5][20], CoinData Coins)
{   //checking type
    if (strcmp(Tokens[4], Coins.USA) != 0
        && strcmp(Tokens[4], Coins.AUS) != 0
        && strcmp(Tokens[4], Coins.EUR) != 0)
    {
        return -1;
    }
    int Amount;
    sscanf(Tokens[1], "%d", &Amount);
    if (Amount < 0 || Amount > 100)
    {
        return -1;
    }
    if (strcmp(Tokens[4],Coins.AUS) == 0)
    {
        if (Amount % 5 != 0)
        {
            return -1;
        }
    }
    return 1;
}
//checks the array of structs for the name from the name
int FindName(char* TempName, Information* Temp, int Entries) //can pointerfy this?
{
    if (Entries == 0)
    {
        return -1;
    }
    for (int i = 0; i < Entries; i++)
    {
        if (strcmp(TempName, Temp[i].Name) == 0)
        {
            return i;
        }
    }
    return -1;
}
//reads over the file and finds the amount of lines
int FindLines(FILE* FileIn)
{
    int Lines = 0;
    char* Buffer;
    Buffer = (char*)malloc(100 *sizeof (char));
    while(fgets(Buffer, 99, FileIn) != NULL)
    {
       Lines ++;
    }
    free (Buffer);
    return Lines;
}

//used to store information into the structs
//takes a pointer to the struct, location if the name matched, and the array of tokens
//if the name was found, checks for the type to see if there is a match
    //if there is it will add to the current amount stored
    //if not will add to the next value array cell
//if name was not found, will just store into the next information cell and 1st cell of values
void StoreInfo(Information* Temp, int Location, char Tokens[5][20])
{
    int TempAmount = 0;
    sscanf(Tokens[1], "%d", &TempAmount);
    int j = Temp[Location].AmountOfTypes;

    if (j == 0) //if the location has no information
    {
        strcpy(Temp[Location].Name, Tokens[0]); //name
    }
    for (int i = 0; i < j; i++)
    {
        if (strcmp(Temp[Location].Coins[i].Type, Tokens[4]) == 0) //if type matches
        {
            Temp[Location].Coins[i].Amount += TempAmount;
            return;
        }
    }
        Temp[Location].Coins[j].Amount = TempAmount; //amount
        strcpy(Temp[Location].Coins[j].Type, Tokens[4]); //type
        Temp[Location].AmountOfTypes ++; //inc types
}

//zeros out all the Coin data inside the struct to not mess with logic
void Zeroing(Information* Temp, int a)
{
    for (int i = 0; i < a; i ++)
    {
        Temp[i].AmountOfTypes = 0;
        for (int j = 0; j < 3; j++)
        {
            Temp[i].Coins[j].Amount = 0;
        }
    }
}
//gets a string input and stores into a char array
char* getName(char* N)
{
    printf("Enter name: ");
    char Buffer[20];
    scanf("%s", Buffer);
    N = (char*)malloc((strlen(Buffer) +1) * sizeof(char)); //termincation char?
    strcpy(N, Buffer);
    return N;
}

//handles all the logic relating to the menu
//unsure if this should have been in main or not, put in main for c++ version
void Menu(Information* Info, int E, CoinData Data, FILE** FileOut)
{
    int* Change;
    char* Name;
    char c = '0';
    int Location;
    do{
        Change = (int*)malloc(4 * sizeof(int));
        printf("1. Enter Name \n2. Exit");
        fflush(stdin);
        c = getchar();
        switch (c)
        {
        case '1':
            Name = getName(Name); //should be fine, returning pointer location
            Location = FindName(Name, Info, E);
            if (Location !=-1)
            {
                for(int i  = 0; i < Info[Location].AmountOfTypes; i++)
                //if name was found, will check the types of coins that name has stored for a match
                //then calc and print coins for that type
                {
                if (strcmp(Info[Location].Coins[i].Type, Data.USA) == 0)
                        {
                            CalcCoins(Info[Location].Coins[i], Data.USAC, Change);
                            PrintConsole(Info[Location], Data.USAC, Change, i); //with usa info
                        }
                        else
                            if (strcmp(Info[Location].Coins[i].Type, Data.AUS) == 0)
                            {
                                CalcCoins(Info[Location].Coins[i], Data.AUSC, Change);
                                PrintConsole(Info[Location], Data.AUSC, Change, i);
                            }
                        else
                            if (strcmp(Info[Location].Coins[i].Type, Data.EUR) == 0)
                            {
                               CalcCoins(Info[Location].Coins[i], Data.EURC, Change);
                               PrintConsole(Info[Location], Data.EURC, Change, i);
                            }
                }
                free (Change);
            }
            else
            {
                printf("Name not found\n");
            }
            free (Name);
            break;
        case '2':
            for (int i =0; i < E; i ++)
            {
                //for each information, will check for the types stored, calc and print the results
                for (int j = 0; j < Info[i].AmountOfTypes; j++)
                {
                if (strcmp(Info[i].Coins[j].Type, Data.USA) == 0)
                        {
                            CalcCoins(Info[i].Coins[j], Data.USAC, Change);
                            PrintFile(Info[i], Data.USAC, Change, j, FileOut); //with usa info
                        }
                        else
                            if (strcmp(Info[i].Coins[j].Type, Data.EUR) == 0)
                            {
                                CalcCoins(Info[i].Coins[j], Data.EURC, Change);
                                PrintFile(Info[i], Data.EURC, Change, j, FileOut);
                            }
                        else
                           if (strcmp(Info[i].Coins[j].Type, Data.AUS) == 0)
                            {
                               CalcCoins(Info[i].Coins[j], Data.AUSC, Change);
                               PrintFile(Info[i], Data.AUSC, Change, j, FileOut);
                            }
                    }
            }
            free (Change);
            break;
        default :
            printf("\nInvalid Input\n");
            break;
        }
    }while(c != '2');
}

//calcs the coins and stores into an array of int
//takes the value from the struct, coin data and a pointer to an int array to store results
void CalcCoins(Values V, int* Data, int* Change)
{
    int TempAmount = V.Amount;
    for (int i = 0; i < 4; i++)
    {
        Change[i] = V.Amount / Data[i];
        V.Amount = TempAmount % Data[i];
        TempAmount = V.Amount;
    }
}

//prints to the console
//takes the Information, pointer to the coin amount, pointer to the array and location inside the value array
void PrintConsole(Information Info, int* Amounts, int* Change, int j)
{
    if (j == 0)
    {
       printf("\nCustomer :\n");
    }
    printf("%s %d cents in %s\n", Info.Name, Info.Coins[j].Amount, Info.Coins[j].Type);
    printf("\nChange:\n");
    for (int i = 0; i < 4; i ++)
    {
        if(Change[i] != 0)
        {
            printf("%d Cents: %d\n", Amounts[i], Change[i]);
        }
    }
    printf("\n");
}

//prints to the file
//takes an instance of information, pointer to the coin amounts, pointer to change, location inside the value array, and out file details
void PrintFile(Information Info, int* Amounts, int* Change, int i, FILE** FileOut)
{
    fprintf(*FileOut, "%s, the change for %d cents in %s is %d,%d,%d,%d\n",
            Info.Name, Info.Coins[i].Amount, Info.Coins[i].Type,
            Change[0], Change[1], Change[2], Change[3]);
}
