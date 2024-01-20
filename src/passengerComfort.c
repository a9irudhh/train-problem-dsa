#include "passengerComfort.h"
#include "settings.h"
#include "locals.h"

// necessary global declaration
// of the structs to make data get
// transferred to primary memory from secondary.
CPS dataToDisplay[100];
CPS duplicateDataDisplay[100];
CPS searchHistory[100];
LHP hashedPasswordsCopy[100];

// necessary global count
// used in maintaining overall data count
int globalCount = 0;
int glbCntForHashedPasswords = 0;
int maxHashedPsswords = 0;
int historyCount = 0;
int duplicateDataCount = 0;
int feedBackCount = 0;
char userName[20];
char password[7];
// definitions of all the functions which I have used in my code

// Function:     openLog
// Description:  opens the log file to write the success or failure status
// Input param:  NULL
// Return Type:  NULL
void openLog()
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

// Function:     closeLog
// Description:  closes the log file if opened
// Input param:  NULL
// Return Type:  NULL
void closeLog()
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

void writeLog(char *function_name, char *status, char *message)
{
    // Write the appropriate log message with associated run time
    myTime = time(NULL);
    fprintf(flog, "%s %s : %s --> %s\n\n", ctime(&myTime), function_name, status, message);
}

void welcomeMessage(void)
{
    system("cls");
    for (tindex = 0; tindex < 100; tindex++)
        printf("*");

    printf("\n\n\t\t\t      WELCOME TO PASSENGER COMFORT MODULE\n\n");

    for (tindex = 0; tindex < 100; tindex++)
        printf("*");

    return;
}

void menuForPassengerComfort(void)
{
    // system("cls");
    printf("\n\t\t\t  ------------------MENU------------------\n\n");
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
        writeLog("loadFileCityPromotions", "FILE_OPEN_ERROR", "Unable to open the city promotion file");
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

void mergeSortForPassword(void)
{
    if (glbCntForHashedPasswords > 1)
        mergeSort(hashedPasswordsCopy, glbCntForHashedPasswords);

    return;
}

void merge(LHP *B, int p, LHP *C, int q, LHP *A)
{
    int i = 0, j = 0, k = 0;

    while (i < p && j < q)
    {
        if (B[i].loadedHashedPassword <= C[j].loadedHashedPassword)
        {
            A[k] = B[i];
            i++;
        }
        else
        {
            A[k] = C[j];
            j++;
        }
        k++;
    }

    if (i == p)
    {
        while (j < q)
        {
            A[k] = C[j];
            j++;
            k++;
        }
    }
    else
    {
        while (i < p)
        {
            A[k] = B[i];
            i++;
            k++;
        }
    }
}

void mergeSort(LHP *A, int n)
{
    if (n > 1)
    {
        int mid = n / 2;

        // Create arrays B and C to hold the two halves
        LHP *B = (LHP *)malloc(mid * sizeof(LHP));
        LHP *C = (LHP *)malloc((n - mid) * sizeof(LHP));

        // Copy elements to B and C
        for (int i = 0; i < mid; i++)
        {
            B[i] = A[i];
        }

        for (int i = mid; i < n; i++)
        {
            C[i - mid] = A[i];
        }

        // Recursively apply merge sort to B and C
        mergeSort(B, mid);
        mergeSort(C, n - mid);

        // Merge the sorted arrays B and C back into A
        merge(B, mid, C, n - mid, A);

        // Free the allocated memory for arrays B and C
        free(B);
        free(C);
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

    // printf("working\n");
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

    if (duplicateDataCount == 0)
    {
        printf("No Cities with name %s\n", searchString);
        return;
    }
    for (int i = 0; i < duplicateDataCount; i++)
    {

        printf("RATING  REVIEW\n\n");
        printf("%.1lf\t%s\n", duplicateDataDisplay[i].rating, duplicateDataDisplay[i].description);
    }

    return;
}

int addFeedback(char *username, char *city, char *feedback)
{
    FILE *file = fopen("textFeedbackDataBase.txt", "a+");

    if (file == NULL)
        return FAILURE;

    fprintf(file, "User: %s, City: %s, Feedback: %s\n", username, city, feedback);
    fclose(file);

    printf("Feedback added successfully.\n");
    return SUCCESS;
}

int addHashedPasswordToFile(char *username, unsigned long hashedValue)
{
    FILE *file = fopen("textPasswordData.txt", "a+");

    if (file == NULL)
        return FAILURE;

    fprintf(file, "%s\t%lu\n", username, hashedValue);
    fclose(file);

    printf("\nPassword Accepted\n");
    return SUCCESS;
}

void loadHshdpsdFromFile(void)
{
    FILE *fhash = fopen("textPasswordData.txt", "r");

    if (fhash == NULL)
    {
        perror("Error opening file");
        return;
    }

    while (fscanf(fhash, "%19s\t%lu", hashedPasswordsCopy[glbCntForHashedPasswords].loadedName, &hashedPasswordsCopy[glbCntForHashedPasswords].loadedHashedPassword) == 2)
        glbCntForHashedPasswords++;

    fclose(fhash);
}

int countWords(char *input)
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

unsigned long getHashValueDjb2(char *tempPassword)
{

    unsigned long hash = 5381;
    int c;

    while ((c = *tempPassword++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

char *createAccount(void)
{
    system("cls");
    unsigned long secreteNumber;
moreThan2Words:
    printf("Please Enter Your Name[single word]: ");
    scanf(" %19[^\n]s", userName); // Corrected format specifier

    userName[strcspn(userName, "\n")] = 0;

    // count = 0;
    count = countWords(userName);
    if (count > 1)
    {
        printf("Don't include spaces in Username\n");
        goto moreThan2Words;
    }

    status = searchInFile(userName);
    if (status == 1)
    {
        printf("Username already exists.\n");
        goto moreThan2Words;
    }
    printf("Enter password: ");
    int i = 0;
    int attempts = 7;
    while (attempts > 0)
    {
        password[i] = getch();
        printf("*");
        i++;
        attempts--;
    }

    secreteNumber = getHashValueDjb2(password);
    status = addHashedPasswordToFile(userName, secreteNumber);
    if (status != 1)
    {
        printf("Password Database Updation error\n");
        return NULL;
    }

    // Clear the input buffer
    while (getchar() != '\n')
        ;

    return userName;
}

int searchInFile(char *target)
{
    FILE *file = fopen("textPasswordData.txt", "r");

    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    char line[100];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        lineNumber++;

        // Extract the first string (token) from the line
        char *token1 = strtok(line, "\t");
        if (token1 != NULL)
        {
            // Compare the token with the target string
            if (rabinKarpSearch(token1, target) == 1)
            {
                fclose(file);
                return SUCCESS;
            }
        }
    }

    fclose(file);
    return FAILURE;
}

int rabinKarpSearch(char *token, char *target)
{
    // text-> token, pattern ->target
    int tokenSize = strlen(token);
    int targetSize = strlen(target);
    unsigned long long targetHash = createHash(target, targetSize);
    unsigned long long tokenHash = createHash(token, targetSize); // why targetsize => bcz it is the window size

    for (int i = 0; i <= tokenSize - targetSize; i++)
    {
        if (tokenHash == targetHash)
        {
            int match = 1;
            for (int j = 0; j < targetSize; j++)
                if (token[i + j] != target[j])
                {
                    match = 0;
                    break;
                }

            if (match)
                return SUCCESS; // Pattern found at index i
        }

        if (i < tokenSize - targetSize)
            tokenHash = recalculateHash(token, i, i + targetSize, tokenHash, targetSize);
    }

    return -1; // Pattern not found
}

unsigned long long recalculateHash(char *str, int oldIndex, int newIndex, unsigned long long oldHash, int patternLength)
{
    unsigned long long newHash = oldHash - str[oldIndex] * pow(101, patternLength - 1);
    newHash *= 101;
    newHash += str[newIndex];

    return newHash;
}

unsigned long long createHash(char *str, int length)
{
    unsigned long long hash = 0;

    for (int i = 0; i < length; i++)
        hash += str[i] * pow(101, length - 1 - i);

    return hash;
}

int binarySearchForUnrecUser(unsigned long match)
{
    int start = 0, end = glbCntForHashedPasswords;

    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (hashedPasswordsCopy[mid].loadedHashedPassword == match)
            return SUCCESS;
        else if (hashedPasswordsCopy[mid].loadedHashedPassword > match)
            end = mid - 1;
        else
            start = mid + 1;
    }
    return FAILURE;
}

int rabinKarpSearchInitiater(char *userName)
{
    char tempUsername[20];
    unsigned long secreteNumber;
    strcpy(tempUsername, userName);
    status = searchInFile(userName);
    if (status != 1)
    {
        printf("Account with Name %s doesnt exist\n", tempUsername);
        return FAILURE;
    }

    printf("Enter password: ");
    int i = 0;
    int attempts = 7;
    while (attempts > 0)
    {
        password[i] = getch();
        printf("*");
        i++;
        attempts--;
    }

    secreteNumber = getHashValueDjb2(password);
    loadHshdpsdFromFile();

    mergeSortForPassword();
    status = binarySearchForUnrecUser(secreteNumber);
    if (status != 1)
    {
        printf("\nPassowrd Didn't Match.");
        return FAILURE;
    }
    return SUCCESS;
}

char *getLoginCredentials(void)
{
    printf("Please Enter Your Name: ");
    scanf(" %19[^\n]s", userName);

    while (getchar() != '\n')
        ;
    status = rabinKarpSearchInitiater(userName);
    if (status != 1)
        return NULL;

    return userName;
}

void giveFeedbackPromt(char *userName)
{
    char userDescription[100];
    char userEnteredCity[25];

    printf("\nEnter the city Name:\n");
    if (scanf("%24[^\n]s", userEnteredCity) != 1)
    {
        printf("Error reading city name.\n");
        return;
    }

    printf("Enter the city description:\n");
    if (scanf(" %99[^\n]s", userDescription) != 1)
    {
        printf("Error reading city description.\n");
        return;
    }

    status = addFeedback(userName, userEnteredCity, userDescription);
    if (status != SUCCESS)
    {
        printf("Feedback couldn't be added.\n");
        return;
    }

    return;
}

void getFeedbackOnCity(void)
{
    system("cls");
    char userChoice;
    while (true)
    {
    wrongInput:
        printf("Are you a New User? [Y/N]\nEnter 0 to return\n =>");
        scanf(" %c", &userChoice);

        getchar();
        userChoice = toupper(userChoice);
        if (userChoice == 'Y')
        {
            getString = createAccount();
            if (getString == NULL)
                return;

            maxHashedPsswords++;
            giveFeedbackPromt(getString);
            return;
        }
        else if (userChoice == 'N')
        {
            getString = getLoginCredentials();
            if (getString == NULL)
                return;
            giveFeedbackPromt(getString);
            return;
        }
        else if (userChoice == '0')
        {
            system("cls");
            return;
        }
        else
        {
            printf("Please choose [Y/N]\n");
            goto wrongInput;
        }
    }
}

void menuForCityPromotions(void)
{
    printf("\n\t\t------------------MENU------------------\n\n");
    printf("-- Enter 1 to View City's Top Tourist spot\n");
    printf("-- Enter 2 to Search for the Tourist Spot's Description\n");
    printf("-- Enter 3 to Give Feedback on a City\n\n");
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
            getFeedbackOnCity(); //hashing, rabinKarp, constructive algo
            break;
        case 4:
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
}
