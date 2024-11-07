// Created by: Jeff Chenenko - A00745858
// COMP 2511 - Procedural Programming with C
// Assignment 8 - index.c

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

    FILE* dataFile = fopen(argv[1], "rb");
    if (!dataFile) { // if data file doesn't exist display error and exit the program
        printf("Error opening data file '%s'\n", argv[1]);
        return 1;
    }

    int recordCount = fileSize(dataFile) / sizeof(Account); // calculate number of records in datafile
    IndexRecord* indexRecords = (IndexRecord*)malloc(recordCount * sizeof(IndexRecord)); // allocate memory for array of records

    if (indexRecords == NULL) {
        printf("memory allocation failed.\n");
        fclose(dataFile);
        return 1;
    }

    // initialize index header
    IndexHeader indexHeader = { APP_NAME, recordCount };

    // iterate through each record in datafile and populate index record
    for (int i = 0; i < recordCount; ++i) {

        Account account;
        long pos = ftell(dataFile);

        if (fread(&account, sizeof(Account), 1, dataFile) != 1) {
            printf("Error reading account\n");
            free(indexRecords);
            fclose(dataFile);
            return 1;
        }

        indexRecords[i].accountBalance = account.accountBalance;
        indexRecords[i].filePos = pos;
    }

    //sort index record array
    qsort(indexRecords, recordCount, sizeof(IndexRecord), compareIndexRecords);

    FILE* indexFile = fopen(argv[2], "wb");
    if (!indexFile) { // if index doesn't exist display error and exit the program
        printf("Error opening index file '%s'\n", argv[2]);
        free(indexRecords);
        fclose(dataFile);
        return 1;
    }

    // write header and records
    fwrite(&indexHeader, sizeof(IndexHeader), 1, indexFile);
    fwrite(indexRecords, sizeof(IndexRecord), recordCount, indexFile);

    printf("Index file '%s' created successfully.\n", argv[2]);

    // close files and free
    fclose(dataFile);
    fclose(indexFile);
    free(indexRecords);

    return 0;
}


// compare index records function
int compareIndexRecords(const void* a, const void* b) {

    // cast qsort pointers back to actual type
    const IndexRecord* ia = (const IndexRecord*)a;
    const IndexRecord* ib = (const IndexRecord*)b;

    // compare account balances for descending order display
    if (ia->accountBalance > ib->accountBalance) {
        return -1;
    }
    if (ia->accountBalance < ib->accountBalance) {
        return 1;
    }

    return 0;
}


// calculate file size function and rewind file
long fileSize(FILE* file) {

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    return size;
}