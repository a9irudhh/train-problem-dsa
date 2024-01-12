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

void welcomeMessage(void);
void menuForPassengerComfort(void);
void cityPromotions(void);
void welcomeCityPromotions(void);
void menuForCityPromotions(void);
void viewTopCity(void);
int loadFileCityPromotions(void);
void write_log(char *, char *, char *);
void quickSortForTopCity(int, int);
int partition(int, int);
void swap(CPS *, CPS *);

#endif // INVENTORY_H_INCLUDED