// necessary headerfiles needed for implementing the below functionalities

// passengerComfort.h for function declaration
#include "passengerComfort.h"
// settings.h to include Macros like SUCCESS, FAILURE etc
#include "settings.h"
// locals.h to include local variables which are used multiple times
#include "locals.h"

// necessary global declaration
// of the structs to make data get
// transferred to primary memory from secondary.
CPS dataToDisplay[100];
CPS duplicateDataDisplay[100];
CPS searchHistory[100];
HPS hashedPasswordsCopy[100];
SNS placesList[100];
DDT *root = NULL;

// These are used when user is prompted to enter the details wherever required
char userName[20];
char password[7];
char cityName[30];
char dormitoryType[30];

// weight matrix to show the distance between any 2 platforms in meters
int weightMatrix[10][10] = {
    {0, 200, 999999, 999999, 999999, 999999, 500, 999999, 999999, 999999},
    {200, 0, 300, 999999, 200, 999999, 999999, 1000, 999999, 999999},
    {999999, 300, 0, 500, 999999, 999999, 800, 999999, 999999, 200},
    {999999, 999999, 500, 0, 300, 999999, 999999, 999999, 999999, 999999},
    {999999, 200, 999999, 300, 0, 500, 999999, 700, 999999, 999999},
    {999999, 999999, 999999, 999999, 500, 0, 999999, 999999, 400, 200},
    {500, 999999, 800, 999999, 999999, 999999, 0, 300, 999999, 999999},
    {999999, 1000, 999999, 999999, 700, 999999, 300, 0, 600, 999999},
    {999999, 999999, 999999, 999999, 999999, 400, 999999, 600, 0, 200},
    {999999, 999999, 200, 999999, 999999, 200, 999999, 999999, 200, 0}};

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
        fprintf(flog, "%s", "-----------------------START OF ITERATION-----------------------");

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
        fprintf(flog, "%s", "-----------------------END OF ITERATION-----------------------");
        fprintf(flog, "%s", "\n\n");

        // Close the file pointer
        fclose(flog);
    }
}

// Function:     writeLog
// Description:  Opens the log file to write the success or failure status along with a message.
// Input params:
//   - char *functionName: Name of the function or operation being logged.
//   - char *status: Success or failure status to be logged.
//   - char *message: Additional information or message to be logged.
// Return Type:  void
void writeLog(char *functionName, char *status, char *message)
{
    // Write the appropriate log message with associated run time
    myTime = time(NULL);
    fprintf(flog, "%s %s : %s -> %s\n\n", ctime(&myTime), functionName, status, message);
}

// Function:     welcomeMessage
// Description:  Displays a welcome message for the Passenger Comfort Module, clearing the screen and presenting a title.
// Input param:  None
// Return Type:  None
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

// Function:     menuForPassengerComfort
// Description:  Displays a menu for the Passenger Comfort Module, providing options for user interaction.
// Input param:  None
// Return Type:  None
void menuForPassengerComfort(void)
{
    // Display the menu with different options
    printf("\n\t\t\t  \033[1;36m------------------MENU------------------\033[0m\n\n");
    printf("=> Enter 1 to View the City's Unique Attractions\n");
    printf("=> Enter 2 to Get Platform Assistance\n");
    printf("=> Enter 3 to For Personalised NoteTaking\n\n");
    printf("Enter 0 to exit the program :( \n  =>");
    return;
}

// Function:     loadFileCityPromotions
// Description:  Reads data from the "textCityPromotionDataBase.txt" file to populate the City Promotion data structure.
// Input param:  None
// Return Type:  int - Returns SUCCESS (1) if the file is successfully loaded, otherwise returns FAILURE (0).
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
        fscanf(fp, "%lf\t%[^\n]s", &dataToDisplay[glbCntForTopCities].rating, dataToDisplay[glbCntForTopCities].description);
        glbCntForTopCities++;
    }

    fclose(fp);
    return SUCCESS;
}

/**
 * Function Name: swapCPS
 * Input Params: Pointers to struct 1 (a) and struct 2 (b)
 * Return Type: void
 * Description: Swaps the content of two CPS (City Promotion Struct) instances using a temporary struct.
 *
 * CPS* a: Pointer to the first CPS struct
 * CPS* b: Pointer to the second CPS struct
 *
 * The function creates a temporary CPS struct and swaps the content of struct a with struct b, and vice versa.
 **/
void swapCPS(CPS *a, CPS *b)
{
    // a temporary CPS struct to use it for copying values
    CPS temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Function Name: partition
 * Input Params: Lower limit (low) and higher limit (high) of the array
 * Return Type: int - Returns the index used for partitioning the array in the quicksort algorithm
 * Description: Sub-function of the quicksort algorithm that partitions the array based on a pivot element's rating.
 *
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
            swapCPS(&dataToDisplay[i], &dataToDisplay[j]);
        }

    } while (i < j);

    swapCPS(&dataToDisplay[low], &dataToDisplay[j]);

    return j;
}

/**
 * Function Name: quickSortForTopCity
 * Input Params: Lower limit (low) and higher limit (high) of the array
 * Return Type: void
 * Description: Efficient recursive approach to sort the top city data array using the quicksort algorithm.
 *
 */
void quickSortForTopCity(int low, int high)
{
    if (low < high)
    {
        int pivot = partition(low, high);

        quickSortForTopCity(low, pivot - 1);
        quickSortForTopCity(pivot + 1, high);
    }
}

/**
 * Function Name: mergeSortForPassword
 * Description: Initiates the merge sort algorithm for the hashed passwords array.
 * Input param: NULL
 * Return Type: NULL
 **/

void mergeSortForPassword(void)
{
    if (glbCntForHashedPasswords > 1)
        mergeSort(hashedPasswordsCopy, glbCntForHashedPasswords);

    return;
}

/**
 * Function Name: merge
 * Description: Merges two sorted arrays B and C into a single array A.
 * Input Params:
 *   - HPS *B: Pointer to the first sorted array
 *   - int p: Size of array B
 *   - HPS *C: Pointer to the second sorted array
 *   - int q: Size of array C
 *   - HPS *A: Pointer to the merged array
 * Return Type: NULL*/

void merge(HPS *B, int p, HPS *C, int q, HPS *A)
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

/**
 * Function Name: mergeSort
 * Description: Applies the merge sort algorithm to efficiently sort an array of hashed passwords.
 * Input Params:
 *   - HPS *A: Pointer to the array of hashed passwords to be sorted
 *   - int n: Size of the array A
 * Return Type: NULL
 **/

void mergeSort(HPS *A, int n)
{
    if (n > 1)
    {
        int mid = n / 2;

        // Create arrays B and C to hold the two halves
        HPS *B = (HPS *)malloc(mid * sizeof(HPS));
        HPS *C = (HPS *)malloc((n - mid) * sizeof(HPS));

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

/**
 * Function Name: bruteForceStringSearch
 * Description: Performs a brute-force string search to find the first occurrence of a pattern in a text.
 * Input Params:
 *   - char *T: Pointer to the text where the search is performed
 *   - char *P: Pointer to the pattern being searched for
 * Return Type: int - Returns the index of the first occurrence of the pattern in the text, or -1 if not found.
 **/
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

/**
 * Function Name: viewTopCity
 * Description: Loads and displays information about the top-rated city promotion.
 * Input Params: None
 * Return Type: None
 * */
void viewTopCity(void)
{
    status = loadFileCityPromotions();
    if (status != 1)
    {
        printf("Load Failed\n");
        return;
    }

    quickSortForTopCity(0, glbCntForTopCities - 1);

    printf("RATING  REVIEW\n\n");
    printf("%.1lf\t%s\n", dataToDisplay[glbCntForTopCities - 1].rating, dataToDisplay[glbCntForTopCities - 1].description);

    glbCntForTopCities = 0;
    return;
}

/**
 * Function Name: searchForCity
 * Description: Searches and displays city promotions based on a user-entered search string.
 * Input Params: None
 * Return Type: None
 **/
void searchForCity(void)
{
    glbCntForTopCities = 0;
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
        for (int i = 0; i < glbCntForTopCities; i++)
        {
            printf("RATING  REVIEW\n\n");
            printf("%.1lf\t%s\n", dataToDisplay[i].rating, dataToDisplay[i].description);
        }
        return;
    }

    for (int i = 0; i < glbCntForTopCities; i++)
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

/**
 * Function Name: addFeedback
 * Description: Adds user feedback to a feedback database file.
 * Input Params:
 *   - char *username: Pointer to the username providing the feedback
 *   - char *city: Pointer to the city associated with the feedback
 *   - char *feedback: Pointer to the user's feedback
 * Return Type: int - Returns SUCCESS (1) if feedback is added successfully, otherwise FAILURE (0).
 * */
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

/**
 * Function Name: addHashedPasswordToFile
 * Description: Adds a hashed password entry to a password data file.
 * Input Params:
 *   - char *username: Pointer to the username associated with the hashed password
 *   - unsigned long hashedValue: The hashed password value to be added
 * Return Type: int - Returns SUCCESS (1) if the hashed password entry is added successfully, otherwise FAILURE (0).
 * */
int addHashedPasswordToFile(char *username, unsigned long hashedValue)
{
    FILE *fAddHash = fopen("textPasswordData.txt", "a+");

    if (fAddHash == NULL)
        return FAILURE;

    fprintf(fAddHash, "%s\t%lu\n", username, hashedValue);
    fclose(fAddHash);

    printf("\nPassword Accepted\n");
    return SUCCESS;
}
/**
 * Function Name: loadHshdpsdFromFile
 * Description: Loads hashed passwords from a file into an array.
 * Input Params: None
 * Return Type: None
 * */
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

/**
 * Function Name: countWords
 * Description: Counts the number of words in a given input string.
 * Input Params:
 *   - char *input: Pointer to the input string
 * Return Type: int - Returns the count of words in the input string.
 * */
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

/**
 * Function Name: getHashValueDjb2
 * Description: Calculates the DJB2 hash value for a given input string.
 * Input Params:
 *   - char *tempPassword: Pointer to the input string for which the hash is calculated
 * Return Type: unsigned long - Returns the DJB2 hash value for the input string.
 * */
unsigned long getHashValueDjb2(char *tempPassword)
{

    unsigned long hash = 5381;
    int c;

    while ((c = *tempPassword++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash;
}

/**
 * Function Name: createAccount
 * Description: Handles the process of creating a user account.
 * Input Params: None
 * Return Type: char * - Returns a pointer to the created username upon success, or NULL in case of errors.
 * */
char *createAccount(void)
{
    system("cls");
    unsigned long secreteNumber;
moreThan2Words:
    printf("Please Enter Your Username[single word]: ");
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

/**
 * Function Name: searchInFile
 * Description: Searches for a target string in a file using a specific search algorithm.
 * Input Params:
 *   - char *target: Pointer to the target string to be searched in the file
 * Return Type: int - Returns SUCCESS (1) if the target string is found, otherwise FAILURE (0) or an error code (-1).
 * */
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

/**
 * Function Name: rabinKarpSearch
 * Description: Implements the Rabin-Karp string search algorithm to find a target string in a token.
 * Input Params:
 *   - char *token: Pointer to the token (text) where the search is performed
 *   - char *target: Pointer to the target string (pattern) to be searched in the token
 * Return Type: int - Returns SUCCESS (1) if the target string is found, otherwise -1.
 * */
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

/**
 * Function Name: recalculateHash
 * Description: Recalculates the hash value for a rolling hash based on a sliding window.
 * Input Params:
 *   - char *str: Pointer to the string for which the hash is recalculated
 *   - int oldIndex: Index of the character to be removed from the current hash window
 *   - int newIndex: Index of the character to be included in the current hash window
 *   - unsigned long long oldHash: Previous hash value before recalculation
 *   - int patternLength: Length of the pattern (size of the hash window)
 * Return Type: unsigned long long - Returns the recalculated hash value.
 * */
unsigned long long recalculateHash(char *str, int oldIndex, int newIndex, unsigned long long oldHash, int patternLength)
{
    unsigned long long newHash = oldHash - str[oldIndex] * pow(101, patternLength - 1);
    newHash *= 101;
    newHash += str[newIndex];

    return newHash;
}

/**
 * Function Name: createHash
 * Description: Creates a hash value for a given string using polynomial rolling hash.
 * Input Params:
 *   - char *str: Pointer to the string for which the hash is created
 *   - int length: Length of the string
 * Return Type: unsigned long long - Returns the hash value for the given string.
 * */
unsigned long long createHash(char *str, int length)
{
    unsigned long long hash = 0;

    for (int i = 0; i < length; i++)
        hash += str[i] * pow(101, length - 1 - i);

    return hash;
}

/**
 * Function Name: binarySearchForUnrecUser
 * Description: Performs binary search for an unmatched hashed password in the password database.
 * Input Params:
 *   - unsigned long match: The hashed password to be searched for
 * Return Type: int - Returns SUCCESS (1) if the hashed password is found, otherwise FAILURE (0).
 * */
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

/**
 * Function Name: rabinKarpSearchInitiater
 * Description: Initiates the Rabin-Karp string search for a user's hashed password.
 * Input Params:
 *   - char *userName: Pointer to the username for which the password search is initiated
 * Return Type: int - Returns SUCCESS (1) if the hashed password is found, otherwise FAILURE (0).
 * */
int rabinKarpSearchInitiater(char *userName)
{
    char tempUsername[20];
    unsigned long secreteNumber;
    strcpy(tempUsername, userName);
    status = searchInFile(userName);
    if (status != 1)
    {
        printf("Account with Username %s doesnt exist\n", tempUsername);
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

/**
 * Function Name: getLoginCredentials
 * Description: Obtains login credentials from the user, including the username and password.
 * Input Params: NULL
 * Return Type: char* - Returns the username if login is successful, otherwise returns NULL.
 * */

char *getLoginCredentials(void)
{
    printf("Please Enter Your User name: ");
    scanf(" %19[^\n]s", userName);

    while (getchar() != '\n')
        ;
    status = rabinKarpSearchInitiater(userName);
    if (status != 1)
        return NULL;

    return userName;
}

/**
 * Function Name: giveFeedbackPromt
 * Description: Prompts the user to provide feedback for a specific city and adds it to the feedback database.
 * Input Params:
 *   - char *userName: Pointer to the username for which feedback is provided
 * Return Type: NULL
 *
 */
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

/**
 * Function Name: getFeedbackOnCity
 * Description: Facilitates the process of obtaining feedback on a city based on user input.
 * Input Params: NULL
 * Return Type: NULL
 * */
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
            printf("Please choose [Y/N] or 0\n");
            goto wrongInput;
        }
    }
}

/**
 * Function Name: viewTopNCities
 * Description: Loads and displays information about the top-rated city promotion where range is taken from the user.
 * Input Params: None
 * Return Type: None
 * */
void insertionSort(int number)
{
    int i, j;
    CPS key;

    for (i = 1; i < number; i++)
    {
        key = dataToDisplay[i];
        j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key.rating
        // to one position ahead of their current position
        while (j >= 0 && dataToDisplay[j].rating < key.rating)
        {
            dataToDisplay[j + 1] = dataToDisplay[j];
            j = j - 1;
        }
        dataToDisplay[j + 1] = key;
    }
}

/**
 * Function Name: viewTopNCities
 * Description: Loads and displays information about the top-rated city promotion where range is taken from the user.
 * Input Params: None
 * Return Type: None
 * */
void viewTopNcities(void)
{
    status = loadFileCityPromotions();
    if (status != 1)
    {
        printf("Load Failed\n");
        return;
    }

    printf("Enter the range in which you want to see the List(1-?): ");
    int topNcity;
    scanf("%d", &topNcity);

    insertionSort(glbCntForTopCities);

    for (int i = 0; i < topNcity; i++)
    {
        printf("RATING  REVIEW\n\n");
        printf("%.1lf\t%s\n", dataToDisplay[i].rating, dataToDisplay[i].description);
    }

    glbCntForTopCities = 0;
}

/**
 * Function Name: menuForCityPromotions
 * Description: Displays a menu for city promotions, allowing users to choose various options.
 * Input Params: NULL
 * Return Type: NULL
 * */
void menuForCityPromotions(void)
{
    printf("\n\t\t\033[1;36m------------------MENU------------------\033[0m\n\n");
    printf("-- Enter 1 to View City's Top Tourist spot\n");
    printf("-- Enter 2 to View City's Top N Tourist spots\n");
    printf("-- Enter 3 to Search for the Tourist Spot's Description\n");
    printf("-- Enter 4 to Give Feedback on a City\n\n");
    printf("Enter 0 to go back \n  =>");
    return;
}

/**
 * Function Name: cityPromotions
 * Description: Facilitates city promotions by presenting a menu and handling user choices.
 * Input Params: NULL
 * Return Type: NULL
 *
 */
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
            system("cls");
            return;

        case 1:
            viewTopCity(); // implemented quicksort
            break;
        case 2:
            viewTopNcities();
            break;
        case 3:
            searchForCity(); // implemented Bfss to search
            break;
        case 4:
            getFeedbackOnCity(); // hashing, rabinKarp, constructive algo
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
}

// --------------------------------------------new functionality related

/**
 * Function Name: menuForPlatformAssistance
 * Description: Displays a menu for platform assistance, allowing users to choose various options.
 * Input Params: NULL
 * Return Type: NULL
 * */
void menuForPlatformAssistance(void)
{
    printf("\n\t\t\033[1;36m------------------MENU------------------\033[0m\n\n");
    printf("-- Enter 1 to Guide you for Inter-Platform Commutation\n");
    printf("-- Enter 2 to Guide you to the Platform which is Nearer to a Tourist Spot\n");
    printf("-- Enter 3 to Search For Dormitories\n\n");
    printf("Enter 0 to go back \n  =>");
}

/**
 * Function Name: dijkstra
 * Input Params:
 *   - weightMatrix: Adjacency matrix representing the weight between platforms
 *   - src: Source platform
 *   - dest: Destination platform
 *   - numPlatforms: Number of platforms
 * Return Type: void
 * Description: Implementation of Dijkstra's Algorithm to find the shortest path between two platforms.
 *   This function calculates the shortest path between the source (src) and destination (dest) platforms in a given graph
 * represented by the weightMatrix adjacency matrix */
void dijkstra(int src, int dest, int numPlatforms)
{
    int *dist = (int *)malloc(sizeof(int) * numPlatforms);
    int *visited = (int *)malloc(sizeof(int) * numPlatforms);
    int *parent = (int *)malloc(sizeof(int) * numPlatforms);

    for (int i = 0; i < numPlatforms; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1; // Initialize parent array to -1
    }

    dist[src] = 0;

    for (int count = 0; count < numPlatforms - 1; count++)
    {
        int minDist = INT_MAX, minIndex;

        for (int v = 0; v < numPlatforms; v++)
        {
            if (!visited[v] && dist[v] <= minDist)
            {
                minDist = dist[v];
                minIndex = v;
            }
        }

        visited[minIndex] = 1;

        for (int v = 0; v < numPlatforms; v++)
        {
            if (!visited[v] && weightMatrix[minIndex][v] && dist[minIndex] != INT_MAX &&
                dist[minIndex] + weightMatrix[minIndex][v] < dist[v])
            {
                dist[v] = dist[minIndex] + weightMatrix[minIndex][v];
                parent[v] = minIndex; // Update parent array
            }
        }
    }

    // Build the path
    int currentPlatform = dest;
    int *path = (int *)malloc(sizeof(int) * numPlatforms);
    int pathLength = 0;

    while (currentPlatform != -1)
    {
        path[pathLength++] = currentPlatform;
        currentPlatform = parent[currentPlatform];
    }

    // Print the path in reverse order
    printf("Shortest distance between Platform %d and Platform %d is: %d meters\n", src + 1, dest + 1, dist[dest]);
    printf("Path: ");
    for (int i = pathLength - 1; i >= 0; i--)
    {
        printf("%d", path[i] + 1);
        if (i > 0)
            printf(" -> ");
    }
    printf("\n");

    // Free allocated memory
    free(dist);
    free(visited);
    free(parent);
    free(path);
}

/**
 * Function Name: interPlatformCommute
 * Description: Guides the user for inter-platform commute using Dijkstra's algorithm.
 * Input Params: None
 * Return Type: void
 **/
void interPlatformCommute(void)
{
    int initialPlatform, finalPlatform;

    printf("Enter Current platform (1-10): ");
    scanf("%d", &initialPlatform);

    printf("Enter the Desired Destination Platform (1-10): ");
    scanf("%d", &finalPlatform);

    if (initialPlatform < 1 || initialPlatform > 10 || finalPlatform < 1 || finalPlatform > 10)
    {
        printf("Invalid platform numbers. Platforms should be between 1 and 10.\n");
        return;
    }

    if (finalPlatform == initialPlatform)
    {
        printf("You are Already In the Destination.\n");
        return;
    }

    dijkstra(initialPlatform - 1, finalPlatform - 1, 10);
    return;
}

/**
 * Function Name: interPlatformCommute
 * Description: Guides the user for inter-platform commute using Dijkstra's Algorithm.
 * Input Params: NULL
 * Return Type: void
 * */
int loadSpotsNearPlatformFile(void)
{
    FILE *fSpots;

    // Open the file in read mode
    fSpots = fopen("textSpotsNearPlatform.txt", "r");

    // Check if the file was successfully opened
    if (fSpots == NULL)
    {
        writeLog("loadFileCityPromotions", "FILE_OPEN_ERROR", "Unable to open the city promotion file");
        return FAILURE;
    }

    // Until the end of the file, read all the file data
    while (!feof(fSpots))
    {
        fscanf(fSpots, "%d\t%[^\n]s", &placesList[glbCntforPlacesList].platform, placesList[glbCntforPlacesList].places);
        glbCntforPlacesList++;
    }

    fclose(fSpots);
    return SUCCESS;
}

/**
 * Function Name: swapSNS
 * Description: Swaps the content of two SNS (Places) structures.
 * Input Params:
 *   - a: Pointer to the first SNS structure
 *   - b: Pointer to the second SNS structure
 * Return Type: void
 **/
void swapSNS(SNS *a, SNS *b)
{
    SNS temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Function Name: heapify
 * Description: Maintains the max-heap property for the PlacesList array.
 * Input Params:
 *   - n: Number of elements in the array
 *   - i: Index of the current element
 * Return Type: void
 * */
void heapify(int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && placesList[left].platform > placesList[largest].platform)
        largest = left;
    if (right < n && placesList[right].platform > placesList[largest].platform)
        largest = right;

    if (largest != i)
    {
        swapSNS(&placesList[i], &placesList[largest]);
        heapify(n, largest);
    }
}

/**
 * Function Name: heapSortForSpotsNearPlatform
 * Description: Sorts the 'placesList' array using the Heap Sort algorithm.
 * Input Params: None
 * Return Type: void
 **/
void heapSortForSpotsNearPlatform(void)
{
    for (int i = glbCntforPlacesList / 2 - 1; i >= 0; i--)
        heapify(glbCntforPlacesList, i);

    for (int i = glbCntforPlacesList - 1; i > 0; i--)
    {
        swapSNS(&placesList[0], &placesList[i]);
        heapify(i, 0);
    }
}

/**
 * Function Name: computeLPSArray
 * Description: Computes the Longest Prefix Suffix (LPS) array for a given pattern.
 * Input Params:
 *   - pat: The pattern for which the LPS array is calculated.
 *   - M: Length of the pattern.
 *   - lps: Array to store the Longest Prefix Suffix values.
 * Return Type: void
 * */
void computeLPSArray(char *pat, int M, int *lps)
{
    // length of the previous longest prefix suffix
    int len = 0;

    lps[0] = 0; // lps[0] is always 0

    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
                len = lps[len - 1];
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

/**
 * Function Name: kmpSearch
 * Description: Performs the Knuth-Morris-Pratt (KMP) pattern searching algorithm.
 * Input Params:
 *   - txt: The text in which the pattern is searched.
 *   - pat: The pattern to be searched in the text.
 * Return Type: int (SUCCESS or FAILURE)
 * */
int kmpSearch(char *txt, char *pat)
{
    int M = strlen(pat);
    int N = strlen(txt);

    int *lps = (int *)malloc(sizeof(int) * M);

    computeLPSArray(pat, M, lps);

    int i = 0;
    int j = 0;
    while ((N - i) >= (M - j))
    {
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            free(lps);
            return SUCCESS;
        }

        // mismatch after j matches
        else if (i < N && pat[j] != txt[i])
        {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
    free(lps);
    return FAILURE;
}

/**
 * Function Name: knuthMorrisPrattToSearchForPlace
 * Description: Searches for a place in the placesList using the Knuth-Morris-Pratt (KMP) algorithm.
 * Input Params:
 *   - enteredName: The name of the place to be searched.
 * Return Type: int (SUCCESS with platform number or FAILURE)
 * */
int knuthMorrisPrattToSearchForPlace(char *enteredName)
{
    for (int i = 0; i < glbCntforPlacesList; i++)
    {
        // Get the place name from the current struct in placesList
        char *placeNames = placesList[i].places;
        int searchedPlatform = placesList[i].platform;

        // Compare the place name with the target string
        if (kmpSearch(placeNames, enteredName) == 1)
        {
            printf("%s is near Platform-%d.\n", enteredName, searchedPlatform);
            return searchedPlatform;
        }
    }

    // No match found
    printf("No City with %s near any of the platforms.\n", enteredName);
    return FAILURE;
}

// Function:     openLog
// Description:  opens the log file to write the success or failure status
// Input param:  NULL
// Return Type:  NULL
void searchForSpotNearPlatform(void)
{
    printf("Enter the Place Name: ");
    scanf(" %29[^\n]s", cityName);

    getchar();
    status = knuthMorrisPrattToSearchForPlace(cityName);
    if (status == -1)
    {
        return;
    }

    guideToPlatform(status);
}

/**
 * Function Name: guideToPlatform
 * Description: Guides the user to a specific platform using Dijkstra's Algorithm or terminates the guide based on user choice.
 * Input Params:
 *   - guideToPlatform: The target platform to guide the user to
 * Return Type: void
 * */
void guideToPlatform(int guideToPlatform)
{
    char userChoice;
    int startPlatform;
    while (true)
    {
    wrongInput:
        printf("Do You want Us to Guide you to Platform-%d? [Y/N]\n =>", guideToPlatform);
        scanf(" %c", &userChoice);

        getchar();
        userChoice = toupper(userChoice);
        if (userChoice == 'Y')
        {
            printf("In Which Platform Are you right Now?\n");
            scanf("%d", &startPlatform);
            // bfs(startPlatform, guideToPlatform, 10);

            dijkstra(startPlatform - 1, guideToPlatform - 1, 10);
            return;
        }
        else if (userChoice == 'N')
        {
            system("cls");
            return;
        }
        else
        {
            printf("Please choose [Y/N] or 0\n");
            goto wrongInput;
        }
    }
}

/**
 * Function Name: touristSpotNearPlatform
 * Description: Displays tourist spots near platforms, allows the user to search for a specific spot, and guides to the chosen spot if desired.
 * Input Params: NULL
 * Return Type: void
 * */
void touristSpotNearPlatform(void)
{
    status = loadSpotsNearPlatformFile();
    if (status != 1)
    {
        printf("File Open Error.\n");
        return;
    }

    heapSortForSpotsNearPlatform();
    for (int i = 1; i < glbCntforPlacesList; i++)
        printf("%d\t%s\n", placesList[i].platform, placesList[i].places);

    searchForSpotNearPlatform();

    glbCntforPlacesList = 0;
    return;
}

/**
 * Function Name: insertIntoBST
 * Description: Inserts a new node into a Binary Search Tree (BST) based on dormitory ratings.
 * Input Params:
 *   - root: Root of the Binary Search Tree
 *   - nodeToInsert: Node to be inserted into the BST
 * Return Type: DDT* (Updated root of the BST)
 * */
DDT *insertIntoBST(DDT *root, NODE nodeToInsert)
{
    if (root == NULL)
    {
        // Allocate memory for a new DDT node
        root = (DDT *)malloc(sizeof(DDT));
        if (root == NULL)
        {
            printf("Memory allocation failed\n");
            return NULL; // Return NULL to indicate failure
        }

        // Allocate memory for the data and copy it
        root->data = (NODE *)malloc(sizeof(NODE));
        if (root->data == NULL)
        {
            printf("Memory allocation failed\n");
            free(root);  // Free previously allocated memory
            return NULL; // Return NULL to indicate failure
        }

        root->data->index = nodeCount;
        strcpy(root->data->description, nodeToInsert.description);
        root->data->dormitoryAvailability = nodeToInsert.dormitoryAvailability;
        root->data->dormitoryBedCount = nodeToInsert.dormitoryBedCount;
        strcpy(root->data->dormitoryName, nodeToInsert.dormitoryName);
        root->data->dormitoryRating = nodeToInsert.dormitoryRating;
        root->data->dormitoryRent = nodeToInsert.dormitoryRent;

        root->left = NULL;
        root->right = NULL;
        return root;
    }

    DDT *curr = root, *parent = NULL;
    while (curr != NULL)
    {
        parent = curr;
        if (nodeToInsert.dormitoryRating < curr->data->dormitoryRating)
            curr = curr->left;
        else
            curr = curr->right;
    }

    // Allocate memory for a new DDT node
    DDT *newNode = (DDT *)malloc(sizeof(DDT));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        return root; // Return the original root to maintain the tree structure
    }

    // Allocate memory for the data and copy it
    newNode->data = (NODE *)malloc(sizeof(NODE));
    if (newNode->data == NULL)
    {
        printf("Memory allocation failed\n");
        free(newNode); // Free previously allocated memory
        return root;   // Return the original root to maintain the tree structure
    }

    newNode->data->index = nodeCount;
    strcpy(newNode->data->description, nodeToInsert.description);
    newNode->data->dormitoryAvailability = nodeToInsert.dormitoryAvailability;
    newNode->data->dormitoryBedCount = nodeToInsert.dormitoryBedCount;
    strcpy(newNode->data->dormitoryName, nodeToInsert.dormitoryName);
    newNode->data->dormitoryRating = nodeToInsert.dormitoryRating;
    newNode->data->dormitoryRent = nodeToInsert.dormitoryRent;

    newNode->left = NULL;
    newNode->right = NULL;

    if (nodeToInsert.dormitoryRating < parent->data->dormitoryRating)
        parent->left = newNode;
    else
        parent->right = newNode;

    nodeCount++;
    return root;
}

// Function:     openLog
// Description:  opens the log file to write the success or failure status
// Input param:  NULL
// Return Type:  NULL
int loadDormitoryDataIntoTree(void)
{
    FILE *fdorm;

    // Open the file in read mode
    fdorm = fopen("textDormitoriesDataBase.txt", "r");

    // Check if the file was successfully opened
    if (fdorm == NULL)
    {
        writeLog("loadFileCityPromotions", "FILE_OPEN_ERROR", "Unable to open the city promotion file");
        return FAILURE;
    }
    NODE data;
    while (!feof(fdorm))
    {

        fscanf(fdorm, "%d\t%49s\t%d\t%lf\t%d\t%lf\t%199[^\n]s\n",
               &data.index,
               data.dormitoryName,
               &data.dormitoryBedCount,
               &data.dormitoryRent,
               &data.dormitoryAvailability,
               &data.dormitoryRating,
               data.description);

        root = insertIntoBST(root, data);
    }

    fclose(fdorm);
    return SUCCESS;
}

void printDormitoryInfo(NODE* nodeInfoToPrint)
{
    printf("%d\t%49s\t%d\t%lf\t%d\t%lf\t%s\n",
               nodeInfoToPrint->index,
            nodeInfoToPrint->dormitoryName,
               nodeInfoToPrint->dormitoryBedCount,
               nodeInfoToPrint->dormitoryRent,
               nodeInfoToPrint->dormitoryAvailability,
               nodeInfoToPrint->dormitoryRating,
            nodeInfoToPrint->description);
}
/**
 *Function Name: bfsForDormitoryType
 *Description: Performs BFS on a tree to find a dormitory with a specific type.
 * @param Input Params:
 *   - startNode: The starting node for BFS
 *   - dormitoryType: The type of dormitory to search for
 * Return Type: void
 * */
DDT *bfsForDormitoryType(DDT *startNode, char *dormitoryType, int numberOfNodes)
{
    int *visited = (int *)malloc(numberOfNodes * sizeof(int));
    DDT **queue = (DDT **)malloc(numberOfNodes * sizeof(DDT *));
    int front = -1, rear = -1;

    for (int i = 0; i < numberOfNodes; i++)
    {
        visited[i] = 0;
    }

    queue[++rear] = startNode;

    while (front <= rear)
    {
        DDT *currentNode = queue[++front];

        if (currentNode != NULL)
        {
            // Check if dormitory type matches
            if (kmpSearch(currentNode->data->description, dormitoryType) == 1)
            {
                // Dormitory with the specified type found
                free(visited);
                free(queue);
                return currentNode; // Return the found node
            }

            // Enqueue left and right children if not visited
            if (currentNode->left && !visited[currentNode->left->data->index])
            {
                visited[currentNode->left->data->index] = 1;
                queue[++rear] = currentNode->left;
            }

            if (currentNode->right && !visited[currentNode->right->data->index])
            {
                visited[currentNode->right->data->index] = 1;
                queue[++rear] = currentNode->right;
            }
        }
    }

    // Free allocated memory
    free(visited);
    free(queue);
    return NULL; // Return NULL if the dormitory type is not found
}

/**
 * Function Name: loadDormitoryDataIntoTree
 * Description: Loads dormitory data from a file into a Binary Search Tree (BST).
 * Input Params: None
 * Return Type: int (SUCCESS or FAILURE)
 * */
void lookForDormitories(void)
{
    status = loadDormitoryDataIntoTree();
    if (status != 1)
        return;

    printf("Enter what kind of Dormitory Are you looking for: ");
    scanf(" %29[^\n]s", dormitoryType);

    DDT* foundNode = bfsForDormitoryType(root, dormitoryType, nodeCount);
    if(foundNode == NULL)
    {
        printf("No Dormitory with %s type found.\n", dormitoryType);
        return;
    }

    printDormitoryInfo(foundNode->data);
    return;
}

/**
 * Function Name: getPlatformAssistance
 * Description: Provides assistance related to platform services to the user.
 * Input Params: None
 * Return Type: void
 * */
void getPlatformAssistance(void)
{
    choice = 0;
    while (true)
    {
        menuForPlatformAssistance();
        scanf(" %d", &choice);

        switch (choice)
        {
        case 0:
            root = NULL;
            system("cls");
            return;

        case 1:
            interPlatformCommute();
            break;

        case 2:
            touristSpotNearPlatform();
            break;

        case 3:
            lookForDormitories();
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
}

// ------------------------------------------

/**
 * Function Name: addNotesToFile
 * Description: Adds a note associated with the user's username to a file.
 * Input Params:
 *    - char *userName: The username associated with the note.
 * Return Type:
 *    void
 */
void addNotesToFile(char *userName)
{
    system("cls");
    printf("\nEnter the Note That you want to add: ");
    char userNote[150];
    scanf("%149[^\n]s", userNote);
    getchar(); // Consume the newline character from the input buffer

    FILE *fadd = fopen("textUserData.txt", "a");

    if (fadd == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    fprintf(fadd, "%s\t%s\n", userName, userNote);
    printf("\nNote added Successfully.\n");
    fclose(fadd); // Don't forget to close the file after writing
    return;
}

/**
 * Function Name: addNotes
 * Description: Allows the user to add notes based on whether they are a new user or an existing user.
 * Input Params: None
 * Return Type: void
 */
void addNotes(void)
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
            addNotesToFile(getString);
            return;
        }
        else if (userChoice == 'N')
        {
            getString = getLoginCredentials();
            if (getString == NULL)
                return;
            addNotesToFile(getString);
            return;
        }
        else if (userChoice == '0')
        {
            system("cls");
            return;
        }
        else
        {
            printf("Please choose [Y/N] or 0\n");
            goto wrongInput;
        }
    }
}

/**
 * Function Name: viewTheListInNotes
 * Description: Displays the list of notes associated with a user after successful login.
 * Input Params: None
 * Return Type: void
 */

void viewTheListInNotes(void)
{
    getString = getLoginCredentials();
    if (getString == NULL)
        return;

    FILE *file = fopen("textUserData.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    char currentLine[170];
    char currentUsername[20];
    char currentString[150];
    int flag = 0, i = 1;
    while (fgets(currentLine, sizeof(currentLine), file) != NULL)
    {
        sscanf(currentLine, "%19s\t%149[^\n]s", currentUsername, currentString);
        printf("working\n");
        if (kmpSearch(currentUsername, getString) == 1)
        {
            flag = 1;
            printf("List %d %s: %s\n", i, getString, currentString);
            i++;
        }
    }

    if (flag == 0)
        printf("No Data Found.\n");
    fclose(file);
    return;
}

/**
 * Function Name: menuForNoteTaking
 * Description: Displays the menu options for note-taking.
 * Input Params: None
 * Return Type: void
 */
void menuForNoteTaking(void)
{
    printf("\n\t\t\033[1;36m------------------MENU------------------\033[0m\n\n");
    printf("-- Enter 1 to Add Notes(Check List)\n");
    printf("-- Enter 2 to To View the Note at top\n");
    printf("Enter 0 to go back \n  =>");
}

/**
 * Function Name: noteTaking
 * Description: Manages the note-taking functionality by displaying a menu and handling user choices.
 * Input Params: None
 * Return Type: void
 */
void noteTaking(void)
{
    choice = 0;
    while (true)
    {
        menuForNoteTaking();
        scanf(" %d", &choice);

        switch (choice)
        {
        case 0:
            system("cls");
            return;

        case 1:
            addNotes();
            break;

        case 2:
            viewTheListInNotes();
            break;

        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
}