#include <stdio.h>
#include <stdlib.h>

#define BUFFER_DATAF_SUBSTRING_SIZE 16
#define BUFFER_DATAF_SIZE 64
#define BUFFER_STRINGF_SIZE 8
#define BUFFER_PARSEF_SIZE 512

void buffer_flush(char *buffer, int buffer_size) //Buffer clear procedure
{
    int i;
    for (i = 0; i < buffer_size; i++)
        buffer[i] = 0;

}

void remove_newline_symbols(char *buffer, int buffer_size) 
{
    int i;
    for (i = 0; i < buffer_size; i++)
        if(buffer[i] == '\n') buffer[i] = 0;
}

void v(FILE **datastream, FILE **stringstream, FILE **parsestream)
{
    #pragma region //Declaration of variables
    char buffer_dataf[BUFFER_DATAF_SIZE];

    char buffer_dataf_substring_1[BUFFER_DATAF_SUBSTRING_SIZE];
    char buffer_dataf_substring_2[BUFFER_DATAF_SUBSTRING_SIZE];
    char buffer_dataf_substring_3[BUFFER_DATAF_SUBSTRING_SIZE];
    char buffer_dataf_substring_4[BUFFER_DATAF_SUBSTRING_SIZE];

    char buffer_stringf[BUFFER_STRINGF_SIZE];
    char buffer_parsef[BUFFER_PARSEF_SIZE];

    buffer_flush(buffer_dataf, BUFFER_DATAF_SIZE);
    buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
    buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);
    #pragma endregion

    getchar();
    switch (getchar()) //Choosing of the mode of the function
    {
        case '1':
            if ((*datastream = fopen("data.txt", "r")) == NULL || 
                (*stringstream = fopen("string.txt", "r")) == NULL || 
                (*parsestream = fopen("parse.txt", "r")) == NULL) //Break if files cannot be opened
            {
                printf("V1: Neotvorene txt subory.\n");
                break;
            }
            
            while (fgets(buffer_dataf, BUFFER_DATAF_SIZE, *datastream) != NULL)
            {
                fgets(buffer_stringf, BUFFER_STRINGF_SIZE, *stringstream);
                fgets(buffer_parsef, BUFFER_PARSEF_SIZE, *parsestream);

                //I've no idea how to split line from data.txt as 4 nums with saving format, so I use 4 variables as subbuffers
                sscanf(buffer_dataf, "%s %s %s %s\n", &buffer_dataf_substring_1, &buffer_dataf_substring_2, 
                    &buffer_dataf_substring_3, &buffer_dataf_substring_4);

                remove_newline_symbols(buffer_dataf_substring_4, BUFFER_DATAF_SUBSTRING_SIZE);
                remove_newline_symbols(buffer_stringf, BUFFER_STRINGF_SIZE);
                remove_newline_symbols(buffer_parsef, BUFFER_PARSEF_SIZE);
                
                printf("ID. mer. modulu: %s\nHodnota 1: %s\nHodnota 2: %s\nPoznamka:%s\n\n", buffer_stringf, 
                    buffer_dataf_substring_3, buffer_dataf_substring_4, buffer_parsef);
                
                buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
                buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);
            }
            break;

        default:
            printf("V: Nesprávne volba vypisu.");
            break;
    }
}

void h(FILE **stringstream)
{
    if (*stringstream == NULL) //Break if file isn't open
        printf("H: Neotvoreny subor.\n");
    else
    {
        char figures[10];
        char uppers[26];
        char lowers[26];
        char buffer;
        int i;
       
        buffer_flush(figures, 10);
        buffer_flush(uppers, 26);
        buffer_flush(lowers, 26);
        fseek(*stringstream, 0, SEEK_SET); //I've figured out, that if I want to use my pointer from main, I need to reset my pointer back to the start

        while (!feof(*stringstream))
        {
            buffer = getc(*stringstream);
            if (buffer >= '0' && buffer <= '9')
                figures[buffer - '0']++;
            else if (buffer >= 'A' && buffer <= 'Z')
                uppers[buffer - 'A']++;
            else if (buffer >= 'a' && buffer <= 'z')
                lowers[buffer - 'a']++;
        }

        for (i = 0; i < 26; i++)
            if(uppers[i] != 0) printf("%c: %d\n", (char)(i + 'A'), uppers[i]);
        for (i = 0; i < 26; i++)
            if(lowers[i] != 0) printf("%c: %d\n", (char)(i + 'a'), lowers[i]);
        for (i = 0; i < 10; i++)
            if(figures[i] != 0) printf("%c: %d\n", (char)(i + '0'), figures[i]);
    }
}

int main()
{
    FILE *dataf, *strf, *parsef;
    dataf = NULL;
    strf = NULL;
    parsef = NULL;

    FILE **p_dataf, **p_strf, **p_parsef;
    p_dataf = &dataf;
    p_strf = &strf;
    p_parsef = &parsef;

    while (1)
    {
        switch (getchar())
        {
            case 'v':
                v(p_dataf, p_strf, p_parsef);
                break;
            case 'h':
                h(p_strf);
                break;
        }
    }
    
    return 0;
}