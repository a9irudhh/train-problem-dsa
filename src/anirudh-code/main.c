#include "locals.h"
#include "passengerComfort.c"

int main()
{
    welcomeMessage();
    while (true)
    {
        menuForPassengerComfrot();
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            exit(-1);
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

















