#include "passengerComfort.h"
#include "settings.h"

// struct declarations for the City Promotion functionalities
//  to use Primary Memory instead of Secondary
typedef struct cityPromotionStruct
{
    double rating;
    char description[200];
} CPS;

// necessary global declaration
// of the structs to make data get
// transferred to primary memory from secondary.
CPS dataToDisplay[100];




// necessary global count 
// used in maintaining overall data count
int globalCount = 0;


// definitions of all the functions which I have used in my code

void welcomeMessage(void)
{
    system("cls");
    for (tindex = 0; tindex < 80; tindex++)
        printf("*");

    printf("\n\n\t\t~ ~ ~ WELCOME TO PASSENGER COMFORT MODULE ~ ~ ~\n\n");

    for (tindex = 0; tindex < 80; tindex++)
        printf("*");

    for (int i = 0; i < 1000000; i++)
        for (int i = 0; i < 25; i++)
            ;

    return;
}

void menuForPassengerComfrot(void)
{
    // system("cls");
    printf("\n------------------MENU------------------\n\n");
    printf("-- Enter 1 to View the City's Unique Attractions\n");
    printf("-- Enter 2 to        \n");
    printf("-- Enter 3 to        \n");
    printf("-- Enter 4 to        \n");
    printf("-- Enter 5 to        \n");
    printf("-- Enter 6 to        \n");
    printf("-- Enter 7 to        \n");
    printf("-- Enter 8 to        \n");
    printf("-- Enter 9 to        \n\n");

    printf("Enter 0 to exit the program :( \n  =>");
    return;
}

void welcomeCityPromotions(void)
{
}

void menuForCityPromotions(void)
{
    printf("\n------------------MENU------------------\n\n");
    printf("-- Enter 1 to View City's Top Tourist spot\n");
    printf("-- Enter 2 to        \n");
    printf("-- Enter 3 to        \n");
    printf("-- Enter 4 to        \n");
    printf("-- Enter 5 to        \n");
    printf("-- Enter 6 to        \n");
    printf("-- Enter 7 to        \n");
    printf("-- Enter 8 to        \n");
    printf("-- Enter 9 to        \n\n");

    printf("Enter 0 to exit the program :( \n  =>");
    return;
}

void loadFileCityPromotions(void)
{
    FILE *fp;

    // Open the file in read mode
    fp = fopen("../dataBase.txt", "r");

    // Check if the file was successfully opened
    if (fp == NULL)
    {
        printf("File Not found\n");
        return;
    }

    // Until the end of the file, read all the file data
    while (!feof(fp))
    {
        fscanf(fp, "%lf\t%[^\n]s", &dataToDisplay[globalCount].rating, dataToDisplay[globalCount].description);
        globalCount++;
    }

    fclose(fp);
}
void viewTopCity(void)
{
    loadFileCityPromotions();
}
void cityPromotions(void)
{
    welcomeCityPromotions();
    choice = 0;
    while (true)
    {
        menuForCityPromotions();
        scanf("%d", &choice);
        system("cls");

        switch (choice)
        {
        case 0:
            return;

        case 1:
            viewTopCity();
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }

    return;
}