#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void v(FILE **datastream, FILE **stringstream, FILE **parsestream, char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int string_max_count)
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

    int mode;
    scanf("%d", &mode);
    switch (mode) //Choosing of the mode of the function
    {
        case 1:
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

                remove_newline_symbols(buffer_dataf, BUFFER_DATAF_SIZE);
                remove_newline_symbols(buffer_stringf, BUFFER_STRINGF_SIZE);
                remove_newline_symbols(buffer_parsef, BUFFER_PARSEF_SIZE);
                
                sscanf(buffer_dataf, "%s %s %s %s\n", &buffer_dataf_substring_1, &buffer_dataf_substring_2, 
                    &buffer_dataf_substring_3, &buffer_dataf_substring_4);

                printf("ID. mer. modulu: %s\nHodnota 1: %s\nHodnota 2: %s\nPoznamka: %s\n\n", buffer_stringf, 
                    buffer_dataf_substring_3, buffer_dataf_substring_4, buffer_parsef);
                
                buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
                buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);
            }
            
            rewind(*datastream);
            rewind(*stringstream);
            rewind(*parsestream);
            break;

        case 2:
            if (*d_datastream == NULL || *d_stringstream == NULL || *d_parsestream == NULL)
            {
                printf("V2: Nenaplnene polia.\n");
                break;
            }
            for (i = 0; i < string_max_count; i++)
            {
                sscanf((*d_datastream)[i], "%s %s %s %s\n", buffer_dataf_substring_1, buffer_dataf_substring_2, 
                        buffer_dataf_substring_3, buffer_dataf_substring_4);

                    remove_newline_symbols(buffer_dataf_substring_4, BUFFER_DATAF_SUBSTRING_SIZE);
                    remove_newline_symbols((*d_stringstream)[i], BUFFER_STRINGF_SIZE);
                    remove_newline_symbols((*d_parsestream)[i], BUFFER_PARSEF_SIZE);

                printf("ID. mer. modulu: %s\nHodnota 1: %s\nHodnota 2: %s\nPoznamka: %s\n\n", (*d_stringstream)[i], 
                    buffer_dataf_substring_3, buffer_dataf_substring_4, (*d_parsestream)[i]);

                rewind(*datastream);
                rewind(*stringstream);
                rewind(*parsestream);
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
            if(uppers[i] != 0) printf("%c : %d\n", (char)(i + 'A'), uppers[i]);
        for (i = 0; i < 26; i++)
            if(lowers[i] != 0) printf("%c : %d\n", (char)(i + 'a'), lowers[i]);
        for (i = 0; i < 10; i++)
            if(figures[i] != 0) printf("%c : %d\n", (char)(i + '0'), figures[i]);
            
        rewind(stringstream);
    }
}



void n(FILE *datastream, FILE *stringstream, FILE *parsestream, char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int *string_max_count, int *initial_string_max_count)
{
    if (datastream == NULL || stringstream == NULL || parsestream == NULL) //Break if file isn't open
    {
        printf("N: Neotvoreny subor.\n");
        return;
    }
    if (*d_datastream != NULL || *d_stringstream != NULL || *d_parsestream != NULL)
    {
        int i;
        for (i = 0; i < *string_max_count; i++)
        {
            free((*d_datastream)[i]);
            free((*d_stringstream)[i]);
            free((*d_parsestream)[i]);
            (*d_datastream)[i] = NULL;
            (*d_stringstream)[i] = NULL;
            (*d_parsestream)[i] = NULL;
        }
     
        *string_max_count = 0;
        *d_datastream = NULL;
        *d_stringstream = NULL;
        *d_parsestream = NULL;
    }
    
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
    
    rewind(datastream);
    rewind(stringstream);
    rewind(parsestream);

    *string_max_count = datastream_strings;
    if (*string_max_count < stringstream_strings) *string_max_count = stringstream_strings;
    if (*string_max_count < parsestream_strings) *string_max_count = parsestream_strings;

    *initial_string_max_count = *string_max_count;
    #pragma endregion

    #pragma region //Preparation of dynamical arrays
    *d_datastream = (char **)malloc(*string_max_count * sizeof(char *));
    *d_stringstream = (char **)malloc(*string_max_count * sizeof(char *));
    *d_parsestream = (char **)malloc(*string_max_count * sizeof(char *));

    for (i = 0; i < *string_max_count; i++)
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
        remove_newline_symbols((*d_datastream)[i], BUFFER_DATAF_SIZE);
        i++;
    }
    
    i = 0;
    while (fgets(buffer_stringf, BUFFER_STRINGF_SIZE, stringstream) != NULL)
    {            
        for (j = 0; j < BUFFER_STRINGF_SIZE; j++)
            (*d_stringstream)[i][j] = buffer_stringf[j];
        buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
        remove_newline_symbols((*d_stringstream)[i], BUFFER_STRINGF_SIZE);
        i++;
    }
    
    i = 0;
    while (fgets(buffer_parsef, BUFFER_PARSEF_SIZE, parsestream) != NULL)
    {
        for (j = 0; j < BUFFER_PARSEF_SIZE; j++)
            (*d_parsestream)[i][j] = buffer_parsef[j];
        buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);
        remove_newline_symbols((*d_parsestream)[i], BUFFER_PARSEF_SIZE);
        i++;
    }
    #pragma endregion
    rewind(datastream);
    rewind(stringstream);
    rewind(parsestream);
}

void q(char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int *string_max_count)
{
    if (*d_datastream == NULL || *d_stringstream == NULL || *d_parsestream == NULL)
        printf("Q: Polia nie su vytvorene.\n");
    else
    {
        #pragma region //Variables
        int i, j;
        char buffer_dataf[BUFFER_DATAF_SIZE];
        char buffer_stringf[BUFFER_STRINGF_SIZE];
        char buffer_parsef[BUFFER_PARSEF_SIZE];

        int Y;
        scanf("%d", &Y);
        #pragma endregion

        #pragma region //Reallocation
        (*string_max_count)++; 
        *d_datastream = (char **)realloc(*d_datastream, (*string_max_count) * sizeof(char *));
        *d_stringstream = (char **)realloc(*d_stringstream, (*string_max_count) * sizeof(char *));
        *d_parsestream = (char **)realloc(*d_parsestream, (*string_max_count) * sizeof(char *));

        (*d_datastream)[*string_max_count - 1] = (char *)malloc(BUFFER_DATAF_SIZE * sizeof(char));
        (*d_stringstream)[*string_max_count - 1] = (char *)malloc(BUFFER_STRINGF_SIZE * sizeof(char));
        (*d_parsestream)[*string_max_count - 1] = (char *)malloc(BUFFER_PARSEF_SIZE * sizeof(char));
        #pragma endregion

        buffer_flush((*d_datastream)[*string_max_count - 1], BUFFER_DATAF_SIZE);
        buffer_flush((*d_stringstream)[*string_max_count - 1], BUFFER_STRINGF_SIZE);
        buffer_flush((*d_parsestream)[*string_max_count - 1], BUFFER_PARSEF_SIZE);
        
        Y = (Y <= (*string_max_count) ? Y - 1 : (*string_max_count) - 1);
        
        //Move down
        for (i = (*string_max_count) - 1; i > Y; i--)
        {
            for (j = 0; j < BUFFER_DATAF_SIZE; j++)
                (*d_datastream)[i][j] = (*d_datastream)[i - 1][j];
            for (j = 0; j < BUFFER_STRINGF_SIZE; j++)
                (*d_stringstream)[i][j] = (*d_stringstream)[i - 1][j];
            for (j = 0; j < BUFFER_PARSEF_SIZE; j++)
                (*d_parsestream)[i][j] = (*d_parsestream)[i - 1][j];
        }


        //Rewriting
        scanf("%s\n", buffer_stringf);
        fgets(buffer_dataf, BUFFER_DATAF_SIZE, stdin);
        fgets(buffer_parsef, BUFFER_PARSEF_SIZE, stdin);

        remove_newline_symbols(buffer_dataf, BUFFER_DATAF_SIZE);
        remove_newline_symbols(buffer_stringf, BUFFER_STRINGF_SIZE);
        remove_newline_symbols(buffer_parsef, BUFFER_PARSEF_SIZE);

        for (i = 0; i < BUFFER_DATAF_SIZE; i++)
            (*d_datastream)[Y][i] = buffer_dataf[i];
        for (i = 0; i < BUFFER_STRINGF_SIZE; i++)
            (*d_stringstream)[Y][i] = buffer_stringf[i];
        for (i = 0; i < BUFFER_PARSEF_SIZE; i++)
            (*d_parsestream)[Y][i] = buffer_parsef[i];
    }
}



void w(char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int *string_max_count)
{
    if (*d_datastream == NULL || *d_stringstream == NULL || *d_parsestream == NULL)
        printf("W: Polia nie su vytvorene.\n");
    else
    {
        #pragma region //Variables
        int i, j, *found_index, found_index_count, pointer, result_of_comparison;
        char buffer_stringf[BUFFER_STRINGF_SIZE];

        found_index = NULL;
        found_index_count = 0;
        pointer = 0;
        #pragma endregion

        scanf("%s", buffer_stringf);

        //Find strings' numbers
        for (i = 0; i < *string_max_count; i++)
        {
            result_of_comparison = strcmp((*d_stringstream)[i], buffer_stringf);
            if(result_of_comparison == 0)
            {
                found_index = (int *)realloc(found_index, (++found_index_count) * sizeof(int));
                found_index[found_index_count - 1] = i;
            }
        }

        #pragma region //Moving records
        if (found_index_count > 0)
        {
            for (i = 0; i < *string_max_count; i++)
            {
                if (i < found_index[0]) continue;
                if (i == found_index[pointer]) pointer++;
                else
                {
                    for (j = 0; j < BUFFER_DATAF_SIZE; j++)
                        (*d_datastream)[i - pointer][j] = (*d_datastream)[i][j];
                    for (j = 0; j < BUFFER_STRINGF_SIZE; j++)
                        (*d_stringstream)[i - pointer][j] = (*d_stringstream)[i][j];
                    for (j = 0; j < BUFFER_PARSEF_SIZE; j++)
                        (*d_parsestream)[i - pointer][j] = (*d_parsestream)[i][j];
                }
            }

            //Memory free
            for (j = *string_max_count - (found_index[pointer] + 1); j < *string_max_count; j++)
            {
                free((*d_datastream)[j]);
                free((*d_stringstream)[j]);
                free((*d_parsestream)[j]);
                (*d_datastream)[j] = NULL;
                (*d_stringstream)[j] = NULL;
                (*d_parsestream)[j] = NULL;
            }

            *string_max_count -= found_index_count;
            *d_datastream = (char **)realloc(*d_datastream, (*string_max_count) * sizeof(char *));
            *d_stringstream = (char **)realloc(*d_stringstream, (*string_max_count) * sizeof(char *));
            *d_parsestream = (char **)realloc(*d_parsestream, (*string_max_count) * sizeof(char *));
            #pragma endregion
        }

        printf("W: Vymazalo sa : %d zaznamov !\n", found_index_count);
    }
}



void e(char ***d_parsestream, int *string_max_count, int *initial_string_max_count, FILE *parsestream)
{
    if (*d_parsestream == NULL)
        printf("E: Polia nie su vytvorene.\n");
    else
    {
        char buffer_parsef[BUFFER_PARSEF_SIZE];
        char buffer_parsef_input[BUFFER_PARSEF_SIZE];
        char **buffer_parsef_used;
        int i, j;
        buffer_parsef_used = NULL;

        //Check if buffer_parsef_input is a substring of (*d_parsestream)[i] with strstr
        scanf("%s", buffer_parsef_input);
        for (i = 0; i < *string_max_count; i++)
        {
            if (strstr((*d_parsestream)[i], buffer_parsef_input) != NULL)
            {
                for (j = 0; j < *initial_string_max_count; j++)
                {
                    fgets(buffer_parsef, BUFFER_PARSEF_SIZE, parsestream);
                    remove_newline_symbols(buffer_parsef, BUFFER_PARSEF_SIZE);

                    if (strcmp((*d_parsestream)[i], buffer_parsef) == 0)
                    {
                        printf("%s\n", (*d_parsestream)[i]);
                    }
                }
            }
        }
    }
}

int main()
{
    #pragma region //Variables
    FILE *dataf, *strf, *parsef;
    dataf = NULL;
    strf = NULL;
    parsef = NULL;

    char **d_dataf, **d_strf, **d_parsef;
    d_dataf = NULL;
    d_strf = NULL;
    d_parsef = NULL;

    int string_max_count, initial_string_max_count, i;
    #pragma endregion

    while (1)
    {
        switch (getchar())
        {
            case 'v':
                v(&dataf, &strf, &parsef, &d_dataf, &d_strf, &d_parsef, string_max_count);
                break;
            case 'h':
                h(strf);
                break;
            case 'n':
                n(dataf, strf, parsef, &d_dataf, &d_strf, &d_parsef, &string_max_count, &initial_string_max_count);
                break;
            case 'q':
                q(&d_dataf, &d_strf, &d_parsef, &string_max_count);
                break;
            case 'w':
                w(&d_dataf, &d_strf, &d_parsef, &string_max_count);
                break;
            case 'e':
                e(&d_parsef, &string_max_count, &initial_string_max_count, parsef);
                break;
            case 'k':
                for (i = 0; i < string_max_count; i++)
                {
                    free(d_dataf[i]);
                    free(d_strf[i]);
                    free(d_parsef[i]);
                    d_dataf[i] = NULL;
                    d_strf[i] = NULL;
                    d_parsef[i] = NULL;
                }

                free(d_dataf);
                free(d_strf);
                free(d_parsef);
                d_dataf = NULL;
                d_strf = NULL;
                d_parsef = NULL;

                fclose(dataf);
                fclose(strf);
                fclose(parsef);
                return 0;
        }
    }
}