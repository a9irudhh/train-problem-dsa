#ifndef PASSENGERCOMFORT_H_INCLUDED
#define PASSENGERCOMFORT_H_INCLUDED

// Including all the built-in required header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>

// Variable to capture the time details
time_t myTime;

// Keeps a log of activity. Any errors can be checked in this file
FILE *flog;

// struct declarations for the City Promotion functionalities
//  to use Primary Memory instead of Secondary
typedef struct cityPromotionStruct
{
    double rating;
    char description[300];
} CPS;

// Function Declarations

void writeLog(char *, char *, char *);
void openLog();
void closeLog();
void welcomeMessage(void);
void welcomeCityPromotions(void);
void menuForPassengerComfort(void);
void menuForCityPromotions(void);
void cityPromotions(void);
void viewTopCity(void);
int loadFileCityPromotions(void);
void quickSortForTopCity(int, int);
int partition(int, int);
void swap(CPS *, CPS *);
void searchForCity(void);
void getFeedbackOnCity(void);
int addFeedback(char *, char *, char *);
char *createAccount(void);
char *getLoginCredentials(void);
void giveFeedbackPromt(char *);

#endif // PASSENGERCOMFORT_H_INCLUDED