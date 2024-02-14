#ifndef PASSENGERCOMFORT_H_INCLUDED
#define PASSENGERCOMFORT_H_INCLUDED

// Including all the built-in required header files
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <limits.h>

// Variable to capture the time details for maintaining log file
time_t myTime;

// Keeps a log of activity. Any errors can be checked in this file
FILE *flog;

// struct declaration for the City Promotion functionalities
//  to use Primary Memory instead of Secondary
typedef struct cityPromotionStruct
{
    double rating;
    char description[300];
} CPS;

// struct declaration for getting the Data which is hashed and saved in file
//  this increases readability by using Primary Memory rather than Secondary
typedef struct hashedPasswordsStruct
{
    char loadedName[20];
    unsigned long loadedHashedPassword;
} HPS;

// struct declarations for storing the Data of Places in Main Memory
// which are near a particular Platform
typedef struct spotsNearPlatformStruct
{
    int platform;
    char places[200];
} SNS;

// Structure which holds the information about the data present in the node
// of the Binary Search Tree
typedef struct nodeOfTree
{
    char dormitoryName[50];
    int dormitoryBedCount;
    double dormitoryRent;
    int dormitoryAvailability;
    double dormitoryRating;
    char description[200];
    int index;
} NODE;

// Structure declaration that gives complete info about the tree node
typedef struct dormitoryDataTree
{
    NODE *data;
    struct dormitoryDataTree *left;
    struct dormitoryDataTree *right;
} DDT;

// Function Declarations

// Function to write log entries
void writeLog(char *, char *, char *); //done

// Function to open the log file
void openLog(void);//done

// Function to close the log file
void closeLog(void);//done

// Function to display welcome message
void welcomeMessage(void);//done

// Function to display the main menu for passenger comfort
void menuForPassengerComfort(void);//done

// Function to display the menu for city promotions
void menuForCityPromotions(void);//done

// Function to handle city promotions
void cityPromotions(void);

// Function to view top-rated city promotions
void viewTopCity(void);

// Function to load city promotions from a file
int loadFileCityPromotions(void);

// Function to perform quicksort for top city promotions
void quickSortForTopCity(int, int);

// Function to partition the array for quicksort
int partition(int, int);

// Function to swap two city promotion structures
void swapCPS(CPS *, CPS *);

// Function to swap two SNS structures
void swapSNS(SNS *, SNS *);

// Function to search for a city promotion
void searchForCity(void);

// Function to get user feedback on a city promotion
void getFeedbackOnCity(void);

// Function to add feedback to the system
int addFeedback(char *, char *, char *);

// Function to create a user account
char *createAccount(void);

// Function to Check if username isnt valid
int unacceptableUserName(char *);

// Function to get login credentials
char *getLoginCredentials(void);

// Function to prompt the user for feedback
void giveFeedbackPromt(char *);

// Function to add a hashed password to the file
int addHashedPasswordToFile(char *, unsigned long);

// Function to calculate the hash value using the djb2 algorithm
unsigned long getHashValueDjb2(char *);

// Function to initiate the Rabin-Karp search algorithm
int rabinKarpSearchInitiater(char *);

// Function to perform the Rabin-Karp search algorithm
int rabinKarpSearch(char *, char *);

// Function to recalculate the hash value (Uses Rolling Hash)
unsigned long long recalculateHash(char *, int, int, unsigned long long, int);

// Function to create a hash value(Normal Hash Function)
unsigned long long createHash(char *, int);

// Function to count words in a string
int countWords(char *);

// Function to search for a string in a file
int searchInFile(char *);

// Function to load hashed passwords from a file
void loadHshdpsdFromFile(void);

// Function that initiates merge sort for hashed passwords
void mergeSortForPassword(void);

// Function to merge two arrays for merge sort
void merge(HPS *, int, HPS *, int, HPS *);

// Function to perform merge sort
void mergeSort(HPS *, int);

// Function to search for an unrecognized user in optimised way
int binarySearchForUnrecUser(unsigned long);

// Function to handle platform assistance
void getPlatformAssistance(void);

// Function to display the menu for platform assistance
void menuForPlatformAssistance(void);

// Function for inter-platform commute
void interPlatformCommute(void);

// Function to perform Dijkstra's algorithm
void dijkstra(int, int, int);

// Function to find tourist spots near a platform
void touristSpotNearPlatform(void);

// Function to load spots near a platform from a file
int loadSpotsNearPlatformFile(void);

// Function to perform heap sort for spots near a platform
void heapSortForSpotsNearPlatform(void);

// Function to search for a spot near a platform
void searchForSpotNearPlatform(void);

// Function to initiate the Knuth-Morris-Pratt search algorithm
int knuthMorrisPrattToSearchForPlace(char *);

// Function to perform Knuth Morris Pratt's algorithm
int kmpSearch(char *, char *);

// Function to guide the user to a platform
void guideToPlatform(int);

// Function to recommend dormitory based on User Search
void lookForDormitories(void);

// Function to load dormitory data into the binary search tree
int loadDormitoryDataIntoTree(void);

// Function to insert a node into the binary search tree
DDT *insertIntoBST(DDT *, NODE);

// Function to perform inorder traversal of the binary search tree
void inorder(DDT *);

// Function for handling note-taking functionality
void noteTaking(void);//done

// Function for viewing the top N cities
void viewTopNcities(void);

// Function to display the menu for note-taking
void menuForNoteTaking(void);//done

// Function for initiating adding notes
void addNotes(void);

// Function to add notes to a file
void addNotesToFile(char *);

// Function for viewing the list of notes of a particular user
void viewTheListInNotes(void);

//prints the data present in the NODE
void printDormitoryInfo(NODE *);

#endif // PASSENGERCOMFORT_H_INCLUDED
