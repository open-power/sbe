/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/test/dilithiumalgotest/hwtestcase/main.c $                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2024                             */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define	MAX_MARKER_LEN		50
#define DILITHIUM_R2_8x7_CRYPTO_PUBLICKEYBYTES  2336
#define DILITHIUM_R2_8x7_CRYPTO_SECRETKEYBYTES  5136
#define DILITHIUM_R2_8x7_CRYPTO_BYTES           4668

uint8_t fileName[10][50] = {0};

/**
 * @brief Prints the usage message for the program.
 *
 * @param program_name The name of the program.
 */
void print_usage(const char *program_name)
{
    printf("Usage: %s [input_file output_file_name] [-h]\n", program_name);
    printf("This program convert KAT vectors to Binary file\n");
    printf("\npositional arguments:\n");
    printf("\tinput_file output_file            Convert given KAT file to output Binary file\n");
    printf("\t\tOptional arg - output_file\n");
    printf("\nOptional Arguments:\n");
    printf("-h                                  Print this help message.\n");
    printf("\nExample:\n");
    printf("  %s input.txt output.bin\n", program_name);
}


int findMarker(FILE *infile, const char *marker)
{
    char	line[MAX_MARKER_LEN];
    int		i, len;
    int curr_line;

    len = (int)strlen(marker);
    if ( len > MAX_MARKER_LEN-1 )
        len = MAX_MARKER_LEN-1;

    for ( i=0; i<len; i++ )
    {
        curr_line = fgetc(infile);
        line[i] = curr_line;
        if (curr_line == EOF )
            return 0;
    }
    line[len] = '\0';

    while ( 1 ) {
        if ( !strncmp(line, marker, len) )
            return 1;

        for ( i=0; i<len-1; i++ )
            line[i] = line[i+1];

        curr_line = fgetc(infile);
        line[len-1] = curr_line;
        if (curr_line == EOF )
            return 0;
        line[len] = '\0';
    }

    // shouldn't get here
    return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int readHex(FILE *infile, unsigned char *A, int Length, char *str)
{
    int			i, ch, started;
    unsigned char	ich;

    if ( Length == 0 ) {
        A[0] = 0x00;
        return 1;
    }
    memset(A, 0x00, Length);
    started = 0;
    if ( findMarker(infile, str) )
        while ( (ch = fgetc(infile)) != EOF ) {
            if ( !isxdigit(ch) ) {
                if ( !started ) {
                    if ( ch == '\n' )
                        break;
                    else
                        continue;
                }
                else
                    break;
            }
            started = 1;
            if ( (ch >= '0') && (ch <= '9') )
                ich = ch - '0';
            else if ( (ch >= 'A') && (ch <= 'F') )
                ich = ch - 'A' + 10;
            else if ( (ch >= 'a') && (ch <= 'f') )
                ich = ch - 'a' + 10;
            else // shouldn't ever get here
            ich = 0;

            for ( i=0; i<Length-1; i++ )
                A[i] = (A[i] << 4) | (A[i+1] >> 4);
            A[Length-1] = (A[Length-1] << 4) | ich;
        }
    else
        return 0;

    return 1;
}


typedef struct __attribute__((__packed__))
{
    uint32_t count;
    uint32_t size;
    uint32_t reserved1;
    uint32_t reserved2;
}header_t;

typedef struct __attribute__((__packed__))
{
    uint32_t vectorSize;
    uint32_t msgLen;
}packet_t;

uint32_t toBigEndian32(uint32_t value) {
    return ((value << 24) & 0xFF000000) |
           ((value << 8)  & 0x00FF0000) |
           ((value >> 8)  & 0x0000FF00) |
           ((value >> 24) & 0x000000FF);
}

int generateHeader(FILE * i_file, uint32_t i_count, uint32_t i_size)
{
    header_t header =
    {
        .count = toBigEndian32(i_count),
        .size  = toBigEndian32(i_size),
        .reserved1 = 0,
        .reserved2 = 0
    };

    return (fwrite(&header, sizeof(uint8_t), sizeof(header_t), i_file));
}

int updateHeader(const FILE * i_file, uint32_t i_count, uint32_t i_size)
{
    header_t header =
    {
        .count = toBigEndian32(i_count),
        .size  = toBigEndian32(i_size),
        .reserved1 = 0,
        .reserved2 = 0
    };
    FILE * file = (FILE *) i_file;
    rewind( file );
    fwrite(&header, sizeof(uint8_t), sizeof(header_t), file);
}

FILE * addDataToBinFile(char * i_outputFileName, uint8_t * i_msg, uint32_t i_msgLen, uint8_t * i_pk, uint8_t * i_sm)
{
    const uint32_t maxSizeOfFile = 307200; // 300 KB
    static uint32_t count = 0;
    static uint32_t fileCount = 0;
    static uint32_t bytesWrittenToFile = 0;
    static FILE *outputFile = NULL;
    static FILE *headerFilePtr = NULL;

    uint32_t vectorSize = i_msgLen + DILITHIUM_R2_8x7_CRYPTO_PUBLICKEYBYTES + DILITHIUM_R2_8x7_CRYPTO_BYTES;

    if (bytesWrittenToFile > (maxSizeOfFile - vectorSize))
    {
        // Update the header
        updateHeader(outputFile, count, bytesWrittenToFile);
        fclose(outputFile);

        printf ("Closing file, Count: %d, bytesWrittenToFile: %d\n", count, bytesWrittenToFile);

        count = 0;
        bytesWrittenToFile = 0;
    }

    if (bytesWrittenToFile == 0)
    {
        char str[100] = {0};  // Buffer to hold the resulting string
        char str1[100] = {0};
        strcpy(str, i_outputFileName);

        uint8_t * dotPosition = strstr(str, ".");
        if (dotPosition)
        {
            *dotPosition = 0x00;
            sprintf(str1, "%s_%d.%s", str, fileCount, ++dotPosition);
        }
        else
        {
            sprintf(str1, "%s_%d", str, fileCount);
        }

        outputFile = fopen(str1, "wb");
        if (outputFile == NULL)
        {
            perror("Error opening output file\n");
            return NULL;
        }

        printf ("Create output file: %s\n", str1);

        strcpy(fileName[fileCount], str1);
        fileName[fileCount+1][0] = 0x00; // Null termination

        // Generate dummy header
        bytesWrittenToFile += generateHeader(outputFile, 0, 0);

        // Number of file count increment
        fileCount++;
    }

    count++;

    /*
        vector packet Binary format:
        +-------------------------------+
        | vector size   - 4 bytes       |
        +-------------------------------+
        | message len   - 4 bytes       |
        +-------------------------------+
        | message       - variable      |
        +-------------------------------+
        | public key    - 2336 bytes    |
        +-------------------------------+
        | signature     - 4668 bytes    |
        +-------------------------------+
    */
    packet_t packet = {
        .vectorSize = toBigEndian32(vectorSize),
        .msgLen = toBigEndian32(i_msgLen)
    };
    bytesWrittenToFile += fwrite(&packet, sizeof(uint8_t), sizeof(packet), outputFile);
    bytesWrittenToFile += fwrite(i_msg, sizeof(uint8_t), i_msgLen, outputFile);
    bytesWrittenToFile += fwrite(i_pk, sizeof(uint8_t), DILITHIUM_R2_8x7_CRYPTO_PUBLICKEYBYTES, outputFile);
    bytesWrittenToFile += fwrite(i_sm, sizeof(uint8_t), DILITHIUM_R2_8x7_CRYPTO_BYTES, outputFile);

    fpos_t pos;
    fgetpos(outputFile, &pos);
    updateHeader(outputFile, count, bytesWrittenToFile);
    fsetpos(outputFile, &pos);

    printf ("updateHeader:: Count: %d, bytesWrittenToFile: %d\n", count, bytesWrittenToFile);

    return outputFile;
}


/**
 * @brief Generate binary files out of given KAT file
 *
 *  Binary file format:
 *  +-------------------------------+
 *  |           Header              |
 *  +-------------------------------+
 *  |           Vector 0            |
 *  +-------------------------------+
 *  |           .....               |
 *  +-------------------------------+
 *  |           Vector N            |
 *  +-------------------------------+
 *
 *
 *  Header packet format:
 *  +---------------------------------------+
 *  | Number of vector in file  | 4 bytes   |
 *  +---------------------------+-----------+
 *  | Size of total vector      | 4 bytes   |
 *  +---------------------------+-----------+
 *  | Reserved                  | 8 bytes   |
 *  +---------------------------+-----------+
 *
 *
 *  Vector packet format:
 *  +-------------------------------+
 *  | vector size   | 4 bytes       |
 *  +---------------+---------------+
 *  | message len   | 4 bytes       |
 *  +---------------+---------------+
 *  | message       | variable      |
 *  +---------------+---------------+
 *  | public key    | 2336 bytes    |
 *  +---------------+---------------+
 *  | signature     | 4668 bytes    |
 *  +---------------+---------------+
 */
int generateBinaryFile(char * i_katFile, char * i_outputFileName)
{
    int                 errorCount = 0;
    int                 error_code =0;
    int                 done = 0;
    int                 idx = 0;
    int                 count = 0;
    int                 ret_val;

    uint8_t             msg[3300];
    uint8_t             pk[DILITHIUM_R2_8x7_CRYPTO_PUBLICKEYBYTES];
    uint8_t             sign[DILITHIUM_R2_8x7_CRYPTO_BYTES];

    size_t              mlen;

    FILE * outBinFile = NULL;
    FILE *katFile = fopen(i_katFile, "r");
    if (katFile == NULL)
    {
        perror("Error opening input file\n");
        return 1; // Exit with error code
    }

    do
    {
        // get the cout value from FILE
        if ( findMarker(katFile, "count = ") )
        {
            if (fscanf(katFile, "%d", &count) != 1)
            {
                printf ("can't find count value in %d\n", __LINE__);
                done = 1;
                break;
            }
        }
        else
        {
            printf ("Read full file\n");
            done = 1;
            break;
        }
        printf ("count: %d\n", count);

         // get the message length value from FILE
        if ( findMarker(katFile, "mlen = ") )
        {
            fscanf(katFile, "%zd", &mlen);
        }
        else
        {
            printf("ERROR: unable to read 'mlen' from <%s>\n", katFile);
            return -2;
        }

        // Read message from file and convert it to hex
        if ( !readHex(katFile, msg, (int)mlen, "msg = ") )
        {
            printf("ERROR: unable to read 'msg' from <%s>\n", katFile);
            return -3;
        }

        // Read public key from KAT file and convert it to hex
        if ( !readHex(katFile, pk, DILITHIUM_R2_8x7_CRYPTO_PUBLICKEYBYTES, "pk = ") )
        {
            printf("ERROR: unable to read 'pk' from <%s>\n", katFile);
            return -4;
        }

        uint32_t maxSignLen = mlen + DILITHIUM_R2_8x7_CRYPTO_BYTES;
        // Allocate space for signature data
        uint8_t * sm = (unsigned char *)calloc(maxSignLen, sizeof(unsigned char));

        if ( !readHex(katFile, sm, maxSignLen, "sm = ") )
        {
            printf("ERROR: unable to read 'sm' from <%s>\n", katFile);
            return -5;
        }

        outBinFile = addDataToBinFile(i_outputFileName, msg, mlen, pk, sm);
    }while(!done);

    if (outBinFile)
    {
        fclose(outBinFile);
    }

    fclose(katFile);
    return 0;
}


/**
 * @brief Main function of the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 on success, non-zero on failure.
 */
int main(int argc, char *argv[])
{
    // Check if the help option is provided
    if (argc == 2 && (strcmp(argv[1], "-h") == 0))
    {
        print_usage(argv[0]);
        return 0;
    }

    // Random vector test
    else if (argc == 3)
    {
        char *inputFilename  = argv[1];
        char *outputFilename = argv[2];

        printf ("\nConvert KAT file to Binary file...\n");
        printf ("Input file : %s\n", inputFilename);
        printf ("Output file: %s\n", outputFilename);

        generateBinaryFile(inputFilename, outputFilename);
    }

    // Print success message to stdout
    printf("--- Program ended ---\n");

    return 0;
}




