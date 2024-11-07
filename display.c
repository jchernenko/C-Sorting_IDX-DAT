// Created by: Jeff Chenenko - A00745858
// COMP 2511 - Procedural Programming with C
// Assignment 8 - display.c

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assign08.h"


// main function
int main(int argc, char* argv[]) {

    // check for correct number of arguments and exit if incorrect
    if (argc != 3) {
        printf("To run program use: %s datafilename indexfilename\n", argv[0]);
        return 1;
    }

    // open files via arguments
    char* dataFilename = argv[1];
    char* indexFilename = argv[2];
    FILE* dataFile = fopen(dataFilename, "rb");
    FILE* indexFile = fopen(indexFilename, "rb");

    if (!dataFile) { // if data file doesn't exist display error and exit the program
        printf("Error - data file '%s' does not exist.\n", dataFilename);
        return 1;
    }
    if (!indexFile) { // if index file doesn't exist display error and exit the program
        printf("Error - index file '%s' does not exist.\n", indexFilename);
        return 1;
    }

    // read and validate index file header
    IndexHeader idxHeader;

    // check that application versions match
    if (fread(&idxHeader, sizeof(IndexHeader), 1, indexFile) != 1 || strcmp(idxHeader.appName, APP_NAME) != 0) {
        printf("Error - data and index files are not compatible.\n");
        fclose(dataFile);
        fclose(indexFile);
        return 1;
    }

    Menu menu[] = { displayNatural, displayDescending, exitProgram }; // function pointers for menu

    int input;
    int result;

    // menu control and selection
    do {
        displayMenu();
        result = scanf("%d", &input);

        if (result == 1) {
            if (input > 0 && input <= sizeof(menu) / sizeof(Menu)) {
                menu[input - 1](dataFile, indexFile);
            }
            else {
                printf("Invalid choice. Please try again.\n");
            }
        }
        else { // non-numeric error handling
            printf("Error reading input. Please enter a number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    } while (input != 3);

    fclose(dataFile);
    fclose(indexFile);
    return 0;
}


// display menu function
void displayMenu() {
    printf("\nDisplay Options:\n");
    printf("1) Natural Order\n");
    printf("2) Account Balance Descending\n");
    printf("3) Exit\n");
    printf("Enter Choice: ");
}


// display accounts function
void displayAccounts(FILE* dataFile, FILE* indexFile, int sortOrder) {

    Account account;
    IndexRecord index;

    fseek(dataFile, 0, SEEK_SET); // start of data file

    if (sortOrder == 1 && indexFile != NULL) { // descending order
        printf("AccountNumber FirstName   LastName   AccountBalance LastPaymentAmount\n");
        printf("---------------------------------------------------------------------\n");

        while (fread(&index, sizeof(IndexRecord), 1, indexFile)) {
            if (fseek(dataFile, index.filePos, SEEK_SET) != 0) {
                printf("Seek error.\n");
                break;
            }
            if (fread(&account, sizeof(Account), 1, dataFile) != 1) {
                printf("Error reading account data.\n");
                break;
            }

            printf("%-13d %-11s %-11s %13.2f %16.2f\n", account.accountNumber, account.firstName, account.lastName, account.accountBalance, account.lastPaymentAmount);
        }
    }
    else { // natural order
        printf("AccountNumber FirstName   LastName   AccountBalance LastPaymentAmount\n");
        printf("---------------------------------------------------------------------\n");

        while (fread(&account, sizeof(Account), 1, dataFile)) {
            printf("%-13d %-11s %-11s %13.2f %16.2f\n", account.accountNumber, account.firstName, account.lastName, account.accountBalance, account.lastPaymentAmount);
        }
    }
}


// display natural order
void displayNatural(FILE* dataFile, FILE* indexFile) {
    puts("");
    displayAccounts(dataFile, NULL, 0);
}


// display descending order
void displayDescending(FILE* dataFile, FILE* indexFile) {
    puts("");
    displayAccounts(dataFile, indexFile, 1);
}


// exit program
void exitProgram(FILE* dataFile, FILE* indexFile) {
    // placeholder because main deals with exiting
}