#include "passengerComfort.h"
#include "settings.h"

// necessary global declaration
// of the structs to make data get
// transferred to primary memory from secondary.
CPS dataToDisplay[100];

// necessary global count
// used in maintaining overall data count
int globalCount = 0;

// definitions of all the functions which I have used in my code

// Function:     open_log
// Description:  opens the log file to write the success or failure status
// Input param:  NULL
// Return Type:  NULL
void open_log()
{
    // Open the file
    flog = fopen("log.txt", "a+");

    // If unable to open the file, alert the user
    if (flog == NULL)
        printf("Unable to open log file. No log status will be captured.");
    else
        fprintf(flog, "%s", "_________________________START OF ITERATION________________________");

    fprintf(flog, "%s", "\n\n");
}

// Function:     close_log
// Description:  closes the log file if opened
// Input param:  NULL
// Return Type:  NULL
void close_log()
{
    if (flog != NULL)
    {
        // Add a message to indicate end of a iteration
        fprintf(flog, "%s", "_________________________END OF ITERATION________________________");
        fprintf(flog, "%s", "\n\n");

        // Close the file pointer
        fclose(flog);
    }
}

void write_log(char *function_name, char *status, char *message)
{
    // Write the appropriate log message with associated run time
    myTime = time(NULL);
    fprintf(flog, "%s %s : %s --> %s\n\n", ctime(&myTime), function_name, status, message);
}

void welcomeMessage(void)
{
    system("cls");
    for (tindex = 0; tindex < 80; tindex++)
        printf("*");

    printf("\n\n\t\t    WELCOME TO PASSENGER COMFORT MODULE\n\n");

    for (tindex = 0; tindex < 80; tindex++)
        printf("*");

    for (int i = 0; i < 1000000; i++)
        for (int i = 0; i < 25; i++)
            ;

    return;
}

void menuForPassengerComfort(void)
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
    ;
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

    printf("Enter 0 to go back \n  =>");
    return;
}

int loadFileCityPromotions(void)
{
    FILE *fp;

    // Open the file in read mode
    fp = fopen("cityPromotionDataBase.txt", "r");

    // Check if the file was successfully opened
    if (fp == NULL)
    {
        write_log("loadFileCityPromotions", "FILE_OPEN_ERROR", "Unable to open the city promotion file");
        return FAILURE;
    }

    // Until the end of the file, read all the file data
    while (!feof(fp))
    {
        fscanf(fp, "%lf\t%[^\n]s", &dataToDisplay[globalCount].rating, dataToDisplay[globalCount].description);
        globalCount++;
    }

    fclose(fp);
    return 1;
}

/**
 * Function Name: swap
 * Input Params: Pointers to struct 1 and struct 2
 * Return Type: void
 * Description: Uses a temporary struct to swap the input 2 structs
 **/
void swap(CPS *a, CPS *b)
{
    CPS temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Function Name: partition
 * Input Params: Lower limit and higher limit of the array
 * Return Type: int
 * Description: Sub-function of the quick sort which partitions the array and returns the index
 **/
int partition(int low, int high)
{
    double p = dataToDisplay[low].rating;
    int i = low;
    int j = high + 1;

    do
    {
        do
        {
            i++;
        } while (dataToDisplay[i].rating < p);

        do
        {
            j--;
        } while (dataToDisplay[j].rating > p);

        if (i < j)
        {
            swap(&dataToDisplay[i], &dataToDisplay[j]);
        }

    } while (i < j);

    swap(&dataToDisplay[low], &dataToDisplay[j]);

    return j;
}

/**
 * Function Name: quickSortForTopCity
 * Input Params: Lower limit and higher limit of the array
 * Return Type: void
 * Description: Efficient recursive approach to sort a given array
 **/
void quickSortForTopCity(int low, int high)
{
    if (low < high)
    {
        int pivot = partition(low, high);

        quickSortForTopCity(low, pivot - 1);
        quickSortForTopCity(pivot + 1, high);
    }
}

void viewTopCity(void)
{
    status = loadFileCityPromotions();
    if (status != 1)
    {
        printf("Load Failed\n");
        return;
    }

    quickSortForTopCity(0, globalCount - 1);

    printf("RATING  REVIEW\n\n");
    printf("%.1lf\t%s\n", dataToDisplay[globalCount - 1].rating, dataToDisplay[globalCount - 1].description);

    return;
}

void cityPromotions(void)
{
    open_log();

    // welcomeCityPromotions();
    choice = 0;
    while (true)
    {
        menuForCityPromotions();
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            close_log();
            return;

        case 1:
            viewTopCity();
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
}
