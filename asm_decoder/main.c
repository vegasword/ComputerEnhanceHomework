// TODO(vegasword): Implement the rest of the MOD instructions (including Effective Address Calculation)
// TODO(vegasword): Test pipeline (decode with NASM and check my output with it)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned uint;
typedef unsigned char uchar;

#include "utils.c"
#include "parser.c"

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
    
    
    printf(";%s\n", argv[1]);
    printf("bits %s\n", argv[2] == NULL ? "16" : argv[2]);
    parse_bin_str(binString);
    
    free(binString);
    free(hexString);
    free(buffer);
    fclose(file);
    
    return 0;
}