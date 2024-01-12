/* SEARCH ENGINE SIMULATION */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Struct definitions
typedef struct searchHistory
{
    char history[100];
} HS;

typedef struct engineData
{
    int priority;
    char searchFeed[150];
} ED;

// Global declarations
HS search_history_data[100];
ED searchList[300];
ED searchDuplicateList[100];

int globalCount = 0;
int inputCount = 0;
int nextRelCount = 0;
int sizeOfDuplicateList = 100;
char input[100];

/**
 * Function Name: welcome
 * Input Params: NILL
 * Return Type: void
 * Description: Prints a welcome message
 **/
void welcome()
{
    int i;
    for (i = 0; i < 80; i++)
        printf("*");

    printf("\n\n\t\t\t\tIII SEARCH ENGINE\n\n");

    for (i = 0; i < 80; i++)
        printf("*");

    printf("\n");
}

/**
 * Function Name: loadFromFile
 * Input Params: NILL
 * Return Type: void
 * Description: Loads all the data available with the search engine into the appropriate data structure
 **/
void loadFromFile()
{
    FILE *fp;

    // Open the file in read mode
    fp = fopen("SearchIndex.txt", "r");

    // Check if the file was successfully opened
    if (fp == NULL)
    {
        printf("File Not found\n");
        return;
    }

    // Until the end of the file, read all the file data
    while (!feof(fp))
    {
        fscanf(fp, "%d\t%[^\n]s", &searchList[globalCount].priority, searchList[globalCount].searchFeed);
        globalCount++;
    }

    fclose(fp);
}

/**
 * Function Name: countWords
 * Input Params: input string
 * Return Type: int
 * Description: Calculates the tokens present in the string (token in the sense of words)
 *              and returns the count
 **/
int countWords(char input[])
{
    int count = 0;
    // Determines where to split the input string, here after a blank space " ".
    // Returns a pointer to the first token found in the input string.
    char *token = strtok(input, " ");

    while (token != NULL)
    {
        count++;
        // Modifies the input string by replacing the delimiter (" ") with null characters
        token = strtok(NULL, " ");
    }

    return count;
}

/**
 * Function Name: getInput
 * Input Params: int n (flag variable)
 * Return Type: void
 * Description: If n is 0 then it asks for a valid 2-word string until the user enters it,
 *              else it takes input from recent search and displays the result
 **/
void getInput(int n)
{
    do
    {
        if (n == 0)
        {
            printf("Enter two words: ");
            scanf(" %99[^\n]s", input);
        }
        else
            strcpy(input, search_history_data[inputCount - 1].history);

        strcpy(search_history_data[inputCount].history, input);
        inputCount++;

        // Remove trailing newline character (commonly used when fgets is used)
        input[strcspn(input, "\n")] = 0;

        // Count the words
        int wordCount = countWords(input);

        // Check if exactly two words were entered
        if (wordCount > 2)
        {
            // Display error message and reprompt
            printf("\n\t\tError: Please enter exactly two words.\n\n");
        }
        else
        {
            // Success message and break the infinite loop
            printf("\n\t\tSuccess\n\n");
            break;
        }
    } while (1); // Infinite loop until valid input is provided
}

/**
 * Function Name: bfss
 * Input Params: Target String and Pattern String
 * Return Type: int
 * Description: Brute force method to search for a pattern in the given string
 *              Returns the index if the pattern is found else returns -1
 **/
int bfss(char *T, char *P)
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

/**
 * Function Name: swap
 * Input Params: Pointers to struct 1 and struct 2
 * Return Type: void
 * Description: Uses a temporary struct to swap the input 2 structs
 **/
void swap(ED *a, ED *b)
{
    ED temp = *a;
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
    int p = searchDuplicateList[low].priority;
    int i = low;
    int j = high + 1;

    do
    {
        do
        {
            i++;
        } while (searchDuplicateList[i].priority < p);

        do
        {
            j--;
        } while (searchDuplicateList[j].priority > p);

        if (i < j)
        {
            swap(&searchDuplicateList[i], &searchDuplicateList[j]);
        }

    } while (i < j);

    swap(&searchDuplicateList[low], &searchDuplicateList[j]);

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
 * Function Name: displayArray
 * Input Params: int n (flag variable to decide whether it's 1st search result or of the next)
 * Return Type: void
 * Description: Iterates through the array and prints the info if priority isn't 0
 **/
void displayArray(int n)
{
    int flag = 0;

    if (n == 1)
    {
        if (nextRelCount >= sizeOfDuplicateList)
        {
            printf("\n\t\tNo more relevant news to display.\n\n");
            return;
        }
    }

    int cnt = searchDuplicateList[nextRelCount].priority;

    printf("\nPriority\tSearch Result\n");
    for (int i = 0; i < sizeOfDuplicateList; i++)
    {
        if (searchDuplicateList[i].priority == cnt)
        {
            printf("%d\t%s\n", searchDuplicateList[i].priority, searchDuplicateList[i].searchFeed);
            flag++;
        }
    }

    nextRelCount++;

    if (flag == 0)
        printf("\n\t\tNo data to print\n\n");
}

/**
 * Function Name: searchEngine
 * Input Params: NILL
 * Return Type: void
 * Description: Uses bfss function to find the pattern, if the pattern is found then
 *              it uses another struct to store the searches found and uses
 *              quickSort function to sort the search results based on priority
 *              and finally uses displayArray function to print the search Results
 **/
void searchEngine()
{
    sizeOfDuplicateList = 0;
    nextRelCount = 0; // Reset nextRelCount when a new search is performed
    int i = 0, j = 0;
    for (; i < globalCount; i++)
    {
        int k = bfss(searchList[i].searchFeed, input);
        if (k != -1)
        {
            // printf("Search Feed: %s, Match Index: %d\n", searchList[i].searchFeed, searchList[i].priority);
            // Create another DS and load it print it
            searchDuplicateList[j].priority = searchList[i].priority;
            strcpy(searchDuplicateList[j].searchFeed, searchList[i].searchFeed);
            sizeOfDuplicateList++;
            j++;
        }
    }
    quickSort(0, j - 1);

    displayArray(0);
}

/**
 * Function Name: displayHistory
 * Input Params: NILL
 * Return Type: void
 * Description: Traverses and prints through the Search History if present
 **/
void displayHistory()
{
    int validity = inputCount;

    if (inputCount == 0)
    {
        printf("\n\n----Search History Empty----\n\n");
        return;
    }

    int i = 0;
    printf("\n----Following are the Latest Searches----\n\n");
    while (validity > 0)
    {
        validity--;
        printf("(%d) %s\n", inputCount - (++i), search_history_data[validity].history);
    }
}

/**
 * Function Name: initSearchHistory
 * Input Params: NILL
 * Return Type: void
 * Description: Initializes the Search History
 **/
void initSearchHistory()
{
    int k = inputCount;
    for (int i = 0; i < k; i++)
    {
        strcpy(search_history_data[i].history, "");
        inputCount--;
    }
    nextRelCount = 0;
}

/**
 * Function Name: deleteParticularUsingIndex
 * Input Params: NILL
 * Return Type: void
 * Description: Deletes a particular search Result based on the input index
 **/
void deleteParticularUsingIndex()
{
    int i;
    if (inputCount == 0)
    {
        printf("\n\n----Search History Empty----\n\n");
        return;
    }
    printf("\n\n----Enter the index of the Search Result that is to be deleted----\n\n");

    if (scanf("%d", &i) != 1)
    {
        // User did not enter a number
        printf("\n\n----Invalid Input. Please enter a valid index number----\n\n");
        // Clear the input buffer
        while (getchar() != '\n')
            ;
        return;
    }

    if (i <= 0 || i > inputCount)
    {
        printf("\n\n----Index Not Found----\n\n");
        return;
    }

    if (i > inputCount)
    {
        printf("\n\n----Index Not Found----\n\n");
        return;
    }
    strcpy(search_history_data[i].history, "");

    if (i != inputCount)
    {
        for (int j = i; j < inputCount; j++)
        {
            strcpy(search_history_data[j].history, search_history_data[j + 1].history);
        }
    }
    nextRelCount--;
    inputCount--;
}

/**
 * Function Name: deleteParticularUsingString
 * Input Params: NILL
 * Return Type: void
 * Description: Deletes a particular search Result based on the input String
 **/
void deleteParticularUsingString()
{
    char deletionStr[100];
    int flag = 0, cnt = 0;

    if (inputCount == 0)
    {
        printf("\n\n----Search History Empty----\n\n");
        return;
    }
    printf("\n\n----Enter the String Which is to Be deleted\n=>");
    scanf(" %99[^\n]s", deletionStr);

    for (int i = 0; i < inputCount; i++)
    {
        if (strcmp(deletionStr, search_history_data[i].history) == 0)
        {
            flag = 1;
            strcpy(search_history_data[i].history, "");
            cnt++;
        }
    }
    if (flag == 0)
    {
        printf("\n\n----String Not Found----\n\n");
        return;
    }

    int newIndex = 0;
    for (int i = 0; i < inputCount; i++)
    {
        if (strcmp("", search_history_data[i].history) != 0)
        {
            strcpy(search_history_data[newIndex].history, search_history_data[i].history);
            newIndex++;
        }
    }
    nextRelCount--;
    inputCount -= cnt;
}

/* MENU FOR THE CODE */
void menuForTheCode()
{
    int query;
    while (1)
    {
        printf("\n----Enter the Query----\n\n"
               "1. To Search\n"
               "2. Print Search History\n"
               "3. Print Next Relevant News\n"
               "4. Print From Latest Search\n"
               "5. Delete Based on Index\n"
               "6. Delete Based on String\n"
               "7. Delete Search History\n"
               "----Press 0 to Exit----\n\n"
               "=> ");
        scanf("%d", &query);

        switch (query)
        {
        case 1:
            getInput(0);    // Used some string functions to get valid input(0 denotes user input)
            searchEngine(); // Uses bfss to search from the database in txt file
            break;
        case 2:
            displayHistory(); // Displays the history available
            break;
        case 3:
            displayArray(1); // Displays next relevant Search Result
            break;
        case 4:
            getInput(1);    // Used some string functions to get valid input(1 denotes recent search)
            searchEngine(); // Uses bfss to search from the database in txt file
            break;
        case 5:
            deleteParticularUsingIndex(); // Deletes particular history based on the index provided
            break;
        case 6:
            deleteParticularUsingString(); // Deletes particular history based on the search string
            break;
        case 7:
            initSearchHistory(); // Deletes history completely
            break;
        case 0:
            exit(0); // Exits the code
        default:
            printf("----Invalid Input----\n\n");
            break;
        }
    }
}

/*Main Function*/

int main()
{
    welcome();      // prints a welcome message
    loadFromFile(); // used the local file to load data

    menuForTheCode(); // accepts the query and operates accordingly
    return 0;         // eoc
}