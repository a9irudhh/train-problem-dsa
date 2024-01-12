#include "passengerComfort.h"
#include "settings.h"

// necessary global declaration
// of the structs to make data get
// transferred to primary memory from secondary.
CPS dataToDisplay[100];
CPS duplicateDataDisplay[100];
CPS searchHistory[100];

// necessary global count
// used in maintaining overall data count
int globalCount = 0;
int historyCount = 0;
int duplicateDataCount = 0;

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
        for (int i = 0; i < 55; i++)
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

int loadFileCityPromotions(void)
{
    FILE *fp;

    // Open the file in read mode
    fp = fopen("textCityPromotionDataBase.txt", "r");

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

int bruteForceStringSearch(char *T, char *P)
{
    int n = strlen(T);
    int m = strlen(P);
    for (int i = 0; i <= n - m; i++)
    {
        int j = 0;
        while (j < m && P[j] == T[i + j])
            j++;
        if (j == m)
            return i;
    }

    return -1;
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

void searchForCity(void)
{
    globalCount = 0;
    duplicateDataCount = 0;

    status = loadFileCityPromotions();
    if (status != 1)
    {
        printf("Load Failed\n");
        return;
    }

    printf("working\n");
    char searchString[25];
    getchar(); // Consume the newline character in the input buffer

    printf("Enter Search string: ");
    fgets(searchString, sizeof(searchString), stdin);
    searchString[strcspn(searchString, "\n")] = '\0'; // Remove trailing newline, if any

    if (strlen(searchString) == 0)
    {
        // If the search string is empty, display all records
        for (int i = 0; i < globalCount; i++)
        {
            printf("RATING  REVIEW\n\n");
            printf("%.1lf\t%s\n", dataToDisplay[i].rating, dataToDisplay[i].description);
        }
        return;
    }

    for (int i = 0; i < globalCount; i++)
    {
        int k = bruteForceStringSearch(dataToDisplay[i].description, searchString);
        if (k != -1)
        {
            duplicateDataDisplay[duplicateDataCount].rating = dataToDisplay[i].rating;
            strcpy(duplicateDataDisplay[duplicateDataCount].description, dataToDisplay[i].description);
            duplicateDataCount++;
        }
    }

    for (int i = 0; i < duplicateDataCount; i++)
    {
        printf("RATING  REVIEW\n\n");
        printf("%.1lf\t%s\n", duplicateDataDisplay[i].rating, duplicateDataDisplay[i].description);
    }

    return;
}

void menuForCityPromotions(void)
{
    printf("\n------------------MENU------------------\n\n");
    printf("-- Enter 1 to View City's Top Tourist spot\n");
    printf("-- Enter 2 to Search for the Tourist Spot's Description\n");
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

void cityPromotions(void)
{

    // welcomeCityPromotions();
    choice = 0;
    while (true)
    {
        menuForCityPromotions();
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            return;

        case 1:
            viewTopCity(); // implemented quicksort
            break;
        case 2:
            searchForCity(); // implemented Bfss to search 
            break;
        case 3:
            getFeedbackOnCity(); // implemented hashing to login
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
}
