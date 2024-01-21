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
            closeLog();
            exit(0);
            break;

        case 1:
            cityPromotions();
            break;

        case 2:
            getPlatformAssistance();
            break;

        case 3:
        //     noteTaking();
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }

    return 0;
}
