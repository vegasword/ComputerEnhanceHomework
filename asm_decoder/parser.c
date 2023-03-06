void parse_register(char bWField, char* bRegister, char* dest) {
    char prefix = 0, suffix = 0;
    
    if (bWField == '0') {
        const char* prefixes = "acdbacdb";
        prefix = prefixes[strtol(bRegister, NULL, 2)];
        suffix = bRegister[0] == '0' ? 'l' : 'h';
    }
    else {
        const char* prefixes = "acdbsbsd";
        prefix = prefixes[strtol(bRegister, NULL, 2)];
        suffix = bRegister[0] == '0' ? 'x' : (bRegister[1] == '0' ? 'p' : 'i');
    }
    
    dest[0] = prefix;
    dest[1] = suffix;
    dest[2] = '\0';
}

void parse_bin_str(char* binString) {
    char* cur = &binString[0];
    while (*cur != '\0')
    {
        printf("mov ");
        
        // Register/memory to/from register case
        if (memcmp(cur, "100010", 7))
        {
            char dField = cur[6];
            char wField = cur[7];
            char modField[3];
            strncpy(modField, &cur[8], 2);
            char regField[4];
            strncpy(regField, &cur[10], 3);
            char rmField[4];
            strncpy(rmField, &cur[13], 3);
            
            char reg[3], rm[3];
            parse_register(wField, regField, reg);
            parse_register(wField, rmField, rm);
            
            if (dField == '0') {
                printf("%s, %s\n", rm, reg);
            }
            else {
                printf("%s, %s\n", reg, rm);
            }
            cur += 16;
        }
        
        if (cur - &binString[0] > 2056)
            break;
    }
}