/**---------------MODULE NAME: PASSENGER COMFORT/ ASSISTANCE-------------**/

#include "locals.h"
#include "passengerComfort.c"

// main function to initiate the module
int main()
{
    // Open the log file which tracks the error which occur during
    // run time of the code
    openLog();

    // Displaying welcome message
    welcomeMessage();

    // Menu Driven approach for giving Console based project a good look
    while (true)
    {
        // Display menu for passenger comfort
        menuForPassengerComfort();

        // Get user choice
        scanf("%d", &choice);

        // Clear the console screen
        system("cls");

        // Process user choice using a switch statement
        switch (choice)
        {
        case 0:
            // User chose to exit, close the log file and exit
            closeLog();
            exit(0);
            break;

        case 1:
            // User chose city promotions
            cityPromotions();
            break;

        case 2:
            // User chose to get platform assistance
            getPlatformAssistance();
            break;

        case 3:
            // User chose an option that is currently commented out
            noteTaking();
            break;
            
        default:
            // User entered an invalid choice
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }

    return 0;
}
