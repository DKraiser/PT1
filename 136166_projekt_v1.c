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

void v(FILE **datastream, FILE **stringstream, FILE **parsestream, char ***p_d_datastream, char ***p_d_stringstream, char ***p_d_parsestream, int max_strings)
{
    #pragma region //Declaration of variables
    char buffer_dataf[BUFFER_DATAF_SIZE];

    char buffer_dataf_substring_1[BUFFER_DATAF_SUBSTRING_SIZE];
    char buffer_dataf_substring_2[BUFFER_DATAF_SUBSTRING_SIZE];
    char buffer_dataf_substring_3[BUFFER_DATAF_SUBSTRING_SIZE];
    char buffer_dataf_substring_4[BUFFER_DATAF_SUBSTRING_SIZE];

    char buffer_stringf[BUFFER_STRINGF_SIZE];
    char buffer_parsef[BUFFER_PARSEF_SIZE];

    int i;

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
            
            fseek(*datastream, 0, SEEK_SET);
            fseek(*stringstream, 0, SEEK_SET);
            fseek(*parsestream, 0, SEEK_SET);
            break;

        case '2':
            for (i = 0; i < max_strings; i++)
            {
                sscanf((*p_d_datastream)[i], "%s %s %s %s\n", &buffer_dataf_substring_1, &buffer_dataf_substring_2, 
                        &buffer_dataf_substring_3, &buffer_dataf_substring_4);

                    remove_newline_symbols(buffer_dataf_substring_4, BUFFER_DATAF_SUBSTRING_SIZE);
                    remove_newline_symbols((*p_d_stringstream)[i], BUFFER_STRINGF_SIZE);
                    remove_newline_symbols((*p_d_parsestream)[i], BUFFER_PARSEF_SIZE);

                printf("ID. mer. modulu: %s\nHodnota 1: %s\nHodnota 2: %s\nPoznamka:%s\n\n", (*p_d_stringstream)[i], 
                    buffer_dataf_substring_3, buffer_dataf_substring_4, (*p_d_parsestream)[i]);

                fseek(*datastream, 0, SEEK_SET);
                fseek(*stringstream, 0, SEEK_SET);
                fseek(*parsestream, 0, SEEK_SET);
            }
            break;

        default:
            printf("V: Nesprávne volba vypisu.\n");
            break;
    }
}

void h(FILE *stringstream)
{
    if (stringstream == NULL) //Break if file isn't open
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

        while (!feof(stringstream))
        {
            buffer = getc(stringstream);
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
            
        fseek(stringstream, 0, SEEK_SET);
    }
}

void n(FILE *datastream, FILE *stringstream, FILE *parsestream, char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int *max_strings)
{
    if (datastream == NULL || stringstream == NULL || parsestream == NULL) //Break if file isn't open
        printf("N: Neotvoreny subor.\n");

    else
    {
        #pragma region //Variables
        int i, j;
        char buffer_dataf[BUFFER_DATAF_SIZE];
        char buffer_stringf[BUFFER_STRINGF_SIZE];
        char buffer_parsef[BUFFER_PARSEF_SIZE];

        int datastream_strings, stringstream_strings, parsestream_strings;
        datastream_strings = 0;
        stringstream_strings = 0;
        parsestream_strings = 0;

        while (fgets(buffer_dataf, BUFFER_DATAF_SIZE, datastream) != NULL) datastream_strings++;
        while (fgets(buffer_stringf, BUFFER_STRINGF_SIZE, stringstream) != NULL) stringstream_strings++;
        while (fgets(buffer_parsef, BUFFER_PARSEF_SIZE, parsestream) != NULL) parsestream_strings++;
        fseek(datastream, 0, SEEK_SET);
        fseek(stringstream, 0, SEEK_SET);
        fseek(parsestream, 0, SEEK_SET);

        *max_strings = datastream_strings;
        if (*max_strings < stringstream_strings) *max_strings = stringstream_strings;
        if (*max_strings < parsestream_strings) *max_strings = parsestream_strings;
        #pragma endregion

        #pragma region //Preparation of dynamical arrays
        *d_datastream = (char **)malloc(*max_strings * sizeof(char *));
        *d_stringstream = (char **)malloc(*max_strings * sizeof(char *));
        *d_parsestream = (char **)malloc(*max_strings * sizeof(char *));
        for (i = 0; i < *max_strings; i++)
        {
            (*d_datastream)[i] = malloc(BUFFER_DATAF_SIZE * sizeof(char));
            (*d_stringstream)[i] = malloc(BUFFER_STRINGF_SIZE * sizeof(char));
            (*d_parsestream)[i] = malloc(BUFFER_PARSEF_SIZE * sizeof(char));
        }
        #pragma endregion

        #pragma region //Initialization of dynamical arrays with data
        i = 0;
        while (fgets(buffer_dataf, BUFFER_DATAF_SIZE, datastream) != NULL)
        {
            for (j = 0; j < BUFFER_DATAF_SIZE; j++)
                (*d_datastream)[i][j] = buffer_dataf[j];
            buffer_flush(buffer_dataf, BUFFER_DATAF_SIZE);
            i++;
        }

        i = 0;
        while (fgets(buffer_stringf, BUFFER_STRINGF_SIZE, stringstream) != NULL)
        {            
            for (j = 0; j < BUFFER_STRINGF_SIZE; j++)
                (*d_stringstream)[i][j] = buffer_stringf[j];
            buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
            i++;
        }
        
        i = 0;
        while (fgets(buffer_parsef, BUFFER_PARSEF_SIZE, parsestream) != NULL)
        {
            for (j = 0; j < BUFFER_PARSEF_SIZE; j++)
                (*d_parsestream)[i][j] = buffer_parsef[j];
            buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);
            i++;
        }
        #pragma endregion

        fseek(datastream, 0, SEEK_SET);
        fseek(stringstream, 0, SEEK_SET);
        fseek(parsestream, 0, SEEK_SET);
    }
}

int main()
{
    FILE *dataf, *strf, *parsef, **p_dataf, **p_strf, **p_parsef;
    dataf = NULL;
    strf = NULL;
    parsef = NULL;
    p_dataf = &dataf;
    p_strf = &strf;
    p_parsef = &parsef;

    char **d_dataf, **d_strf, **d_parsef, ***p_d_dataf, ***p_d_strf, ***p_d_parsef;
    d_dataf = NULL;
    d_strf = NULL;
    d_parsef = NULL;
    p_d_dataf = &d_dataf;
    p_d_strf = &d_strf;
    p_d_parsef = &d_parsef;

    int max_strings;

    while (1)
    {
        switch (getchar())
        {
            case 'v':
                v(p_dataf, p_strf, p_parsef, p_d_dataf, p_d_strf, p_d_parsef, max_strings);
                break;
            case 'h':
                h(strf);
                break;
            case 'n':
                n(dataf, strf, parsef, p_d_dataf, p_d_strf, p_d_parsef, &max_strings);
                break;
        }
    }
    
    free(p_dataf);
    free(p_strf);
    free(p_parsef);
    free(d_dataf);
    free(d_strf);
    free(d_parsef);

    fclose(dataf);
    fclose(strf);
    fclose(parsef);

    return 0;
}