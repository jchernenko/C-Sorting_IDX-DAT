// Created by: Jeff Chenenko - A00745858
// COMP 2511 - Procedural Programming with C
// Assignment 8 - assign08.h


#ifndef ASSIGN08_H
#define ASSIGN08_H

#define APP_NAME "Assign08"

// account struct
typedef struct {
    int accountNumber;
    char firstName[50];
    char lastName[50];
    float accountBalance;
    float lastPaymentAmount;
} Account;

// index record struct
typedef struct {
    float accountBalance;
    long filePos;
} IndexRecord;

// index header struct
typedef struct {
    char appName[20];
    int recCount;
} IndexHeader;

// function prototypes for index.c
int compareIndexRecords(const void* a, const void* b);
long fileSize(FILE* file);

// function prototypes for display.c
void displayAccounts(FILE* dataFile, FILE* indexFile, int sortOrder);
void displayMenu(void);
void displayNatural(FILE* dataFile, FILE* indexFile);
void displayDescending(FILE* dataFile, FILE* indexFile);
void exitProgram(FILE* dataFile, FILE* indexFile);
typedef void (*Menu)(FILE*, FILE*);

#endif // ASSIGN08_H
