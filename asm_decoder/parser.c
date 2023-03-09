void parse_register(char wField, char* regField, char* dest) {
    char prefix = 0, suffix = 0;
    
    if (wField == '0') {
        const char* prefixes = "acdbacdb";
        prefix = prefixes[strtol(regField, NULL, 2)];
        suffix = (regField[0] == '0') ? 'l' : 'h';
    }
    else {
        const char* prefixes = "acdbsbsd";
        prefix = prefixes[strtol(regField, NULL, 2)];
        suffix = (regField[0] == '0') ? 'x' : (regField[1] == '0') ? 'p' : 'i';
    }
    
    dest[0] = prefix;
    dest[1] = suffix;
    dest[2] = '\0';
}

void parse_bin_str(char* binString) {
    int tmp = 0;
    
    char* cur = &binString[0];
    while (*cur != '\0')
    {
        // Immediate to register
        if (str_cmp(cur, "1011")) {
            printf("mov ");
            
            char wField = cur[4];
            char regField[4];
            strncpy(regField, &cur[5], 3);
            
            char reg[3];
            parse_register(wField, regField, reg);
            printf("%s, ", reg);
            
            if (wField == '0') {
                char data[9];
                strncpy(data, &cur[8], 8);
                printf("%ld\n", strtol(data, NULL, 2));
                cur += 16;
            }
            else {
                char data[17];
                strncpy(data, &cur[8], 16);
                printf("%ld\n", strtol(data, NULL, 2));
                cur += 24;
            }
        }
        // Register/memory to/from register
        else if (str_cmp(cur, "100010")) {
            printf("mov ");
            
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
        // Immediate to register/memory
        else if (str_cmp(cur, "1100011")) {
            printf("mov ");
            
            char wField = cur[7];
            char modField[3];
            strncpy(modField, &cur[8], 2);
            char rmField[4];
            strncpy(rmField, &cur[13], 3);
            
            if (str_cmp(modField, "11")) {
                
                char rm[3];
                parse_register(wField, rmField, rm);
                printf("%s, ", rm);
            }
            // Effective Adress Calculation
            else {
                if (str_cmp(modField, "00") && str_cmp(rmField, "110")) {
                    // TODO(vegasword): Direct adress extra case
                }
                if (rmField[0] == '0') {
                    char eac[11];
                    sprintf(eac, "[b%c + %ci]\n\0",
                            (rmField[1] == '0') ? 'x' : 'p',
                            (rmField[2] == '0') ? 's' : 'd');
                    // TODO(vegasword): Data handling
                }
                else {
                    char eac[6];
                    if (rmField[1] == '0') {
                        sprintf(eac, "[%ci]\n\0",
                                (rmField[2] == '0') ? 's' : 'd');
                    }
                    else {
                        sprintf(eac, "[b%c]\n\0",
                                (rmField[2] == '0') ? 'p' : 'x');
                    }
                    // TODO(vegasword): Data handling
                }
            }
            
            if (wField == '0') {
                char data[9];
                strncpy(data, &cur[29], 8);
                printf("%lli\n", strtol(data, NULL, 2));
                if (cur - &binString[0] <= sizeof binString)
                    cur += 24;
            }
            else {
                char data[17];
                strncpy(data, &cur[29], 16);
                printf("%ld\n", strtol(data, NULL, 2));
                cur += 32;
            }
        }
        
        // Safety condition
        if (tmp > 11) return;
        else tmp++;
    }
}