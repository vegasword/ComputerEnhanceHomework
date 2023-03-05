// TODO(vegasword): Decoding Multiple Instructions and Suffixes
// Link: https://www.computerenhance.com/p/decoding-multiple-instructions-and

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char uchar;

char* create_hex_str(uchar* buffer, long size) {
    char* hexString = (char*) malloc(size * 2 + 1);
    if (!hexString) {
        return NULL;
    }
    memset(hexString, 0, sizeof hexString);
    
    for (int i=0; i < size; i++)
    {
        char c[3];
        sprintf(c, "%02X", (uchar)buffer[i]);
        strcat(hexString, c);
    }
    
    return hexString;
}

char* hex_to_bin(char* hexString) {
    static const char* hexDigits = "0123456789ABCDEF";
    static const char* binDigits[16] = {
        "0000", "0001", "0010", "0011",
        "0100", "0101", "0110", "0111",
        "1000", "1001", "1010", "1011",
        "1100", "1101", "1110", "1111"
    };
    
    size_t hexLen = strlen(hexString);
    char* binString = malloc(hexLen * 4 + 1);
    if (!binString) {
        return NULL;
    }
    memset(binString, 0, sizeof binString);
    
    for (size_t i = 0; i < hexLen; i++) {
        const char* hexPtr = strchr(hexDigits, toupper(hexString[i]));
        if (!hexPtr) {
            free(binString);
            return NULL;
        }
        int index = hexPtr - hexDigits;
        strcat(binString, binDigits[index]);
    }
    
    return binString;
}

void parse_register(char bWField, char* bRegister, char* dest) {
    char prefix = 0, suffix = 0;
    
    if (bWField == '0') {
        const char* prefixes = "ACDBACDB";
        prefix = prefixes[strtol(bRegister, NULL, 2)];
        suffix = bRegister[0] == '0' ? 'L' : 'H';
    }
    else {
        const char* prefixes = "ACDBSBSD";
        prefix = prefixes[strtol(bRegister, NULL, 2)];
        suffix = bRegister[0] == '0' ? 'X' : (bRegister[1] == '0' ? 'P' : 'I');
    }
    
    dest[0] = prefix;
    dest[1] = suffix;
    dest[2] = '\0';
}

// We handle only the Register/memory to/from register MOV instruction
// (MOD = 11) case for now
// TODO(vegasword): Handle other memory modes
void parse_bin_str(char* binString, char* path, char* bitsDirective) {
    printf(";%s\n", path);
    printf("bits %s\n", bitsDirective);
    
    char bOpCode[7];
    strncpy(bOpCode, binString, 6);
    char bDField = binString[6];
    char bWField = binString[7];
    
    char bModField[3];
    strncpy(bModField, &binString[8], 2);
    char bRegField[4];
    strncpy(bRegField, &binString[10], 3);
    char bRmField[4];
    strncpy(bRmField, &binString[13], 3);
    
    // TODO(vegasword): parse_operation
    if (strcmp(bOpCode, "100010") == 0) {
        printf("MOV ");
    }
    
    char reg[3], rm[3];
    parse_register(bWField, bRegField, reg);
    parse_register(bWField, bRmField, rm);
    
    if (bDField == '0') {
        printf("%s, %s\n", rm, reg);
    }
    else {
        printf("%s, %s\n", reg, rm);
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: decode_to_asm [PATH] [BITS DIRECTIVE (default=16)]\n");
        return 1;
    }
    
    FILE* file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Failed to open binary file!\n");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    uchar* buffer = (uchar*) malloc(sizeof uchar * fileSize);
    if (buffer == NULL) {
        printf("Unable to allocate memory to main buffer!\n");
        return 1;
    }
    memset(buffer, 0, fileSize);
    
    size_t result = fread(buffer, 1, fileSize, file); 
    if (result != fileSize) {
        printf("Failed to read file!\n");
        free(buffer);
        return 1;
    }
    
    char* hexString = create_hex_str(buffer, fileSize);
    if (!hexString) {
        printf("Failed to create the hexadecimal string!\n");
        return 1;
    }
    
    char* binString = hex_to_bin(hexString);
    if (!binString) {
        printf("Failed to create the binary string!\n");
        return 1;
    }
    
    size_t opCount = fileSize / 2;
    parse_bin_str(binString, argv[1], (argv[2] == NULL ? "16" : argv[2]));
    
    free(binString);
    free(hexString);
    free(buffer);
    fclose(file);
    
    return 0;
}