#include "passengerComfort.h"
#include "settings.h"
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
// necessary global count
// used in maintaining overall data count
int globalCount = 0;
int glbCntForHashedPasswords = 0;
int glbCntforPlacesList = 0;
int maxHashedPsswords = 0;
int historyCount = 0;
int duplicateDataCount = 0;
int feedBackCount = 0;
char userName[20];
char password[7];
char cityName[30];
char dormitoryType[30];
// weight matrix to show the distance between any 2 platforms
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

// adjacency Matrix to show the connection between two platforms
int adjacencyMatrix[10][10] = {
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 0, 1, 0, 0, 1},
    {0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 0}};

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

void writeLog(char *functionName, char *status, char *message)
{
    // Write the appropriate log message with associated run time
    myTime = time(NULL);
    fprintf(flog, "%s %s : %s -> %s\n\n", ctime(&myTime), functionName, status, message);
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
    printf("\n\t\t\t  \033[1;36m------------------MENU------------------\033[0m\n\n");
    printf("=> Enter 1 to View the City's Unique Attractions\n");
    printf("=> Enter 2 to Get Platform Assistance\n");
    printf("=> Enter 3 to        \n");
    printf("=> Enter 4 to        \n");
    printf("=> Enter 5 to        \n\n");

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
void swapCPS(CPS *a, CPS *b)
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
            swapCPS(&dataToDisplay[i], &dataToDisplay[j]);
        }

    } while (i < j);

    swapCPS(&dataToDisplay[low], &dataToDisplay[j]);

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

    globalCount = 0;
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
    FILE *fAddHash = fopen("textPasswordData.txt", "a+");

    if (fAddHash == NULL)
        return FAILURE;

    fprintf(fAddHash, "%s\t%lu\n", username, hashedValue);
    fclose(fAddHash);

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
    printf("\n\t\t\033[1;36m------------------MENU------------------\033[0m\n\n");
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
            system("cls");
            return;

        case 1:
            viewTopCity(); // implemented quicksort
            break;
        case 2:
            searchForCity(); // implemented Bfss to search
            break;
        case 3:
            getFeedbackOnCity(); // hashing, rabinKarp, constructive algo
            break;
        case 4:
            break;
        default:
            printf("Please Enter a Valid Choice\n\n");
            break;
        }
    }
}

// --------------------------------------------new functionality related

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
 * Description: Implementation of Dijkstra's Algorithm to find the shortest path
 **/
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

void swapSNS(SNS *a, SNS *b)
{
    SNS temp = *a;
    *a = *b;
    *b = temp;
}

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
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0)
            {
                len = lps[len - 1];

                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}
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

int knuthMorisPrattToSearchForPlace(char *enteredName)
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

void searchForSpotNearPlatform(void)
{
    printf("Enter the Place Name: ");
    scanf(" %29[^\n]s", cityName);

    getchar();
    status = knuthMorisPrattToSearchForPlace(cityName);
    if (status == -1)
    {
        return;
    }

    guideToPlatform(status);
}

void bfs(int startPlatform, int endPlatform, int numPlatforms)
{
    int *visited = (int *)malloc(sizeof(int) * numPlatforms);
    int *parent = (int *)malloc(sizeof(int) * numPlatforms);

    for (int i = 0; i < numPlatforms; i++)
    {
        visited[i] = 0;
        parent[i] = -1; // Initialize parent array to -1
    }

    // Create a queue for BFS
    int *queue = (int *)malloc(sizeof(int) * numPlatforms);
    int front = -1, rear = -1;

    visited[startPlatform] = 1;
    queue[++rear] = startPlatform;

    while (front != rear)
    {
        int currentPlatform = queue[++front];

        for (int neighbor = 0; neighbor < numPlatforms; neighbor++)
        {
            if (!visited[neighbor] && weightMatrix[currentPlatform][neighbor] != 999999)
            {
                visited[neighbor] = 1;
                parent[neighbor] = currentPlatform;
                queue[++rear] = neighbor;

                if (neighbor == endPlatform)
                {
                    // Path found, print the path and return
                    printBFSPath(parent, startPlatform, endPlatform);
                    free(visited);
                    free(parent);
                    free(queue);
                    return;
                }
            }
        }
    }

    // If BFS completes without finding the destination
    printf("No path found between Platform %d and Platform %d.\n", startPlatform, endPlatform);

    // Free allocated memory
    free(visited);
    free(parent);
    free(queue);
}

void printBFSPath(int *parent, int startPlatform, int endPlatform)
{
    // Build the path
    int numPlatforms = 10;
    int currentPlatform = endPlatform;
    int *path = (int *)malloc(sizeof(int) * (numPlatforms));
    int pathLength = 0;

    while (currentPlatform != -1)
    {
        path[pathLength++] = currentPlatform; // Adjust to 1-indexed form
        currentPlatform = parent[currentPlatform];
    }

    // Print the path in reverse order
    printf("Shortest path between Platform %d and Platform %d: ", startPlatform, endPlatform);
    for (int i = pathLength - 1; i >= 0; i--)
    {
        printf("%d", path[i]);
        if (i > 0)
            printf(" -> ");
    }

    printf("\n");

    free(path);
}
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
            printf("Please choose [Y/N]\n");
            goto wrongInput;
        }
    }
}

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
DDT *insertIntoBST(DDT *root, NODE *nodeToInsert)
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
        memcpy(&(root->data), nodeToInsert, sizeof(NODE));
        root->left = NULL;
        root->right = NULL;
        return root;
    }

    DDT *curr = root, *parent = NULL;
    while (curr != NULL)
    {
        parent = curr;
        if (nodeToInsert->dormitoryRating < curr->data->dormitoryRating)
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
    memcpy(&(newNode->data), nodeToInsert, sizeof(NODE));
    newNode->left = newNode->right = NULL;

    if (nodeToInsert->dormitoryRating < parent->data->dormitoryRating)
        parent->left = newNode;
    else
        parent->right = newNode;

    return root;
}

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

    while (!feof(fdorm))
    {
        NODE data;

        fscanf(fdorm, "%49s\t%d\t%lf\t%d\t%lf\t%199[^\n]s\n",
               data.dormitoryName,
               &data.dormitoryBedCount,
               &data.dormitoryRent,
               &data.dormitoryAvailability,
               &data.dormitoryRating,
               data.description);

        root = insertIntoBST(root, &data);

    }

    // Traverse to the leftmost node
    DDT *cur = root;
    while (cur->left != NULL)
    {
        cur = cur->left;
    }

    // Print the data of the leftmost node
    printf("%s\t%d\t%lf\t%d\t%lf\t%s\n",
           cur->data->dormitoryName,
           cur->data->dormitoryBedCount,
           cur->data->dormitoryRent,
           cur->data->dormitoryAvailability,
           cur->data->dormitoryRating,
           cur->data->description);

    fclose(fdorm);
    return SUCCESS;
}
void lookForDormitories(void)
{
    status = loadDormitoryDataIntoTree();
    if (status != 1)
        return;

    printf("Enter what kind of Dormitory Are you looking for: ");
    scanf(" %29[^\n]s", dormitoryType);

    return;
}
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