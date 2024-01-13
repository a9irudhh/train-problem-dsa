#include "locals.h"
#include "passengerComfort.c"

int main()
{
    openLog();
    
    welcomeMessage();
    while (true)
    {
        menuForPassengerComfort();
        scanf("%d", &choice);
        system("cls");

        switch (choice)
        {
        case 0:
            exit(0);
            break;

        case 1:
            cityPromotions();
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }

    system("cls");

    closeLog();
    return 0;
}
