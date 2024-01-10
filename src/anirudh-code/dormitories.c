/*  CityPromotion Display Code
    Written By: Anirudh R. H.
    Date: 05/01/2024
*/
#include <stdio.h>

// struct declaration
typedef struct DormitoryData
{
    char dormitoryName[100];
    int dormitoryBedCount;
    double dormitoryRent;
    int dormitoryAvailability;
    double dormitoryRating;
    char description[200];
} DD;

// necessary globalDeclarations
DD dataToDisplay[100];
int globalCount = 0;

/**
 * Function Name: loadFromFile
 * Input Params: NILL
 * Return Type: void
 * Description: Loads all the data present in the Search Engine into Appropriate DS
 **/
void loadFromFile()
{
    FILE *fp;

    // Open the file in read mode
    fp = fopen("dataBase.txt", "r");

    // Check if the file was successfully opened
    if (fp == NULL)
    {
        printf("File Not found\n");
        return;
    }

    // Until the end of the file, read all the file data
    while (!feof(fp))
    {
        fscanf(fp, "%s\t%d\t%lf\t%d\t%lf\t%[^\n]s",
               dataToDisplay[globalCount].dormitoryName,
               &dataToDisplay[globalCount].dormitoryBedCount,
               &dataToDisplay[globalCount].dormitoryRent,
               &dataToDisplay[globalCount].dormitoryAvailability,
               &dataToDisplay[globalCount].dormitoryRating,
               dataToDisplay[globalCount].description);
        globalCount++;
    }

    fclose(fp);
}

/**
 * Function Name: swap
 * Input Params: Pointers to struct 1 and struct 2
 * Return Type: void
 * Description: Uses a temporary struct to swap the input 2 structs
 **/
void swap(DD *a, DD *b)
{
    DD temp = *a;
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
    double p = dataToDisplay[low].dormitoryRating;
    int i = low;
    int j = high + 1;

    do
    {
        do
        {
            i++;
        } while (dataToDisplay[i].dormitoryRating < p);

        do
        {
            j--;
        } while (dataToDisplay[j].dormitoryRating > p);

        if (i < j)
        {
            swap(&dataToDisplay[i], &dataToDisplay[j]);
        }

    } while (i < j);

    swap(&dataToDisplay[low], &dataToDisplay[j]);

    return j;
}

/**
 * Function Name: quickSort
 * Input Params: Lower limit and higher limit of the array
 * Return Type: void
 * Description: Efficient recursive approach to sort a given array
 **/
void quickSort(int low, int high)
{
    if (low < high)
    {
        int pivot = partition(low, high);

        quickSort(low, pivot - 1);
        quickSort(pivot + 1, high);
    }
}

/**
 * Function Name: displayData
 * Input Params: NILL
 * Return Type: void
 * Description: Displays the Data present in the Struct
 **/
void displayData()
{
    printf("DORMITORY NAME  BED COUNT  RENT  AVAILABILITY  RATING  REVIEW\n");
    for (int i = globalCount - 1; i > 0; i--)
    {
        printf("%-15s %-10d %-6.2lf %-13d %-6.2lf %s\n",  //used padding of spaces to maintain clean output 
               dataToDisplay[i].dormitoryName,
               dataToDisplay[i].dormitoryBedCount,
               dataToDisplay[i].dormitoryRent,
               dataToDisplay[i].dormitoryAvailability,
               dataToDisplay[i].dormitoryRating,
               dataToDisplay[i].description);
    }
}

/**
 * Main function to call the UDF
 */
int main()
{
    loadFromFile();
    quickSort(0, globalCount - 1);
    displayData();
}
