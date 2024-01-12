#include "locals.h"
#include "passengerComfort.c"

int main()
{
    welcomeMessage();
    while (true)
    {
        menuForPassengerComfrot();
        scanf("%d", &choice);
        system("cls");

        switch (choice)
        {
        case 0:
            exit(USER_EXIT);
            break;
        
        case 1: 
            cityPromotions();
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
    
}

















