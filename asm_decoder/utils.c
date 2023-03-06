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