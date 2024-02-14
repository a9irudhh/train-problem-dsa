#ifndef LOCALS_H_INCLUDED
#define LOCALS_H_INCLUDED

// This is a local variable file
// Following variables can be used in the main program

// Variable for take choice input from the user
// Can be used for:
//  - switch cases 
//  - menu options
//  - Any other scenario where user is prompted for a choice to input
int choice = 0;

// The return status of success or failure of any function can be
// captured in status variable
int status = 0;

// getString is a variable to capture the string 
// Memory for this has to be allocated as per need based on
// the string size
char *getString;

// The tindex variable can be used as a index in loops
// It stands for temporary index
int tindex = 0;

// The variable count is used to hold the counters
int count = 0;

// necessary various global counts
// used in maintaining overall data count
int glbCntForTopCities = 0;
int glbCntForHashedPasswords = 0;
int glbCntforPlacesList = 0;
int maxHashedPsswords = 0;
int historyCount = 0;
int duplicateDataCount = 0;
int feedBackCount = 0;
int nodeCount = 0;

#endif // LOCALS_H_INCLUDED