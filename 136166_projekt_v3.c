#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_DATAF_SUBSTRING_SIZE 16
#define BUFFER_DATAF_SIZE 64
#define BUFFER_STRINGF_SIZE 8
#define BUFFER_PARSEF_SIZE 512

typedef struct
{
    char *Hodnota_ID;
    char *Hodnota_zn;
    char *Hodnota_1;
    char *Hodnota_2;
}
data_element;

typedef struct
{
    char *Poznámka_ID;
    char *Poznámka_N1;
    char *Poznámka_Hodina;
    char *Poznámka_Minuta;
    char *Poznámka_T;
}
parse_element;

typedef struct linked_list_item_struct
{
    struct linked_list_item_struct *tail;

    data_element data_value;
    char *string_value;
    parse_element parse_value;
}
LINKED_LIST_ITEM;

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

void find_initial_string_max_count(FILE *datastream, FILE *stringstream, FILE *parsestream, int *initial_string_max_count) //Find the count of strings in the original files 
{
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

    *initial_string_max_count = datastream_strings;
    if (*initial_string_max_count < stringstream_strings) *initial_string_max_count = stringstream_strings;
    if (*initial_string_max_count < parsestream_strings) *initial_string_max_count = parsestream_strings;
}

void create_data_value_from_file(data_element *data_value, FILE *datastream, int number_of_string) //Create structure for data.txt in linked list  
{
    int i;
    char buffer_dataf[BUFFER_DATAF_SIZE];

    for (i = 0; i <= number_of_string; i++)
    {
        buffer_flush(buffer_dataf, BUFFER_DATAF_SIZE);
        fgets(buffer_dataf, BUFFER_DATAF_SIZE, datastream);
    }
    rewind(datastream);
    remove_newline_symbols(buffer_dataf, BUFFER_DATAF_SIZE);

    data_value->Hodnota_1 = malloc(BUFFER_DATAF_SUBSTRING_SIZE);
    data_value->Hodnota_2 = malloc(BUFFER_DATAF_SUBSTRING_SIZE);
    data_value->Hodnota_ID = malloc(BUFFER_DATAF_SUBSTRING_SIZE);
    data_value->Hodnota_zn = malloc(BUFFER_DATAF_SUBSTRING_SIZE);

    buffer_flush(data_value->Hodnota_1, BUFFER_DATAF_SUBSTRING_SIZE);
    buffer_flush(data_value->Hodnota_2, BUFFER_DATAF_SUBSTRING_SIZE);
    buffer_flush(data_value->Hodnota_ID, BUFFER_DATAF_SUBSTRING_SIZE);
    buffer_flush(data_value->Hodnota_zn, BUFFER_DATAF_SUBSTRING_SIZE);
    
    sscanf(buffer_dataf, "%s %s %s %s", data_value->Hodnota_1, data_value->Hodnota_2, data_value->Hodnota_ID, data_value->Hodnota_zn);
}

void create_string_value_from_file(char **string_value, FILE *stringstream, int number_of_string) //Create string for string.txt in linked list... Useless, but it seems to be good in my context
{
    int i;
    char buffer_stringf[BUFFER_STRINGF_SIZE];

    for (i = 0; i <= number_of_string; i++)
    {
        buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
        fgets(buffer_stringf, BUFFER_STRINGF_SIZE, stringstream);
    }
    rewind(stringstream);
    remove_newline_symbols(buffer_stringf, BUFFER_STRINGF_SIZE);
    *string_value = malloc(BUFFER_STRINGF_SIZE);
    strcpy(*string_value, buffer_stringf);
}

void create_parse_value_from_file(parse_element *parse_value, FILE *parsestream, int number_of_string) //Create structure for parse.txt in linked list
{
    int i, j, last_splitter_index, s;
    char buffer_parsef[BUFFER_PARSEF_SIZE], *buffer_time;

    for (i = 0; i <= number_of_string; i++)
    {
        buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);
        fgets(buffer_parsef, BUFFER_PARSEF_SIZE, parsestream);
    }
    rewind(parsestream);
    remove_newline_symbols(buffer_parsef, BUFFER_PARSEF_SIZE);

    parse_value->Poznámka_ID = NULL;
    parse_value->Poznámka_N1 = NULL;
    buffer_time = NULL;
    parse_value->Poznámka_Hodina = NULL;
    parse_value->Poznámka_Minuta = NULL;
    parse_value->Poznámka_T = NULL;

    //As long as idk the sizes of substrings in parse.txt, im initializing all the fields in such an iterative way
    i = 0;
    s = 1;
    while (buffer_parsef[i] != '#')
    {
        parse_value->Poznámka_ID = realloc(parse_value->Poznámka_ID, s++);
        parse_value->Poznámka_ID[i] = buffer_parsef[i];
        i++;
    }
    
    s = 1;
    last_splitter_index = i++;
    while (buffer_parsef[i] != '#')
    {
        parse_value->Poznámka_N1 = realloc(parse_value->Poznámka_N1, s++);
        parse_value->Poznámka_N1[i - last_splitter_index - 1] = buffer_parsef[i];
        i++;
    }

    s = 1;
    last_splitter_index = i++;
    while (buffer_parsef[i] != '#')
    {
        buffer_time = realloc(buffer_time, s++);
        buffer_time[i - last_splitter_index - 1] = buffer_parsef[i];
        i++;
    }

    s = 1;
    for (j = 0; j < 2; j++)
    {
        if (buffer_time == NULL)
            break;
        parse_value->Poznámka_Hodina = realloc(parse_value->Poznámka_Hodina, s);
        parse_value->Poznámka_Minuta = realloc(parse_value->Poznámka_Minuta, s++);
        parse_value->Poznámka_Hodina[j] = buffer_time[j];
        parse_value->Poznámka_Minuta[j] = buffer_time[j + 2];
    }

    free(buffer_time);
    buffer_time = NULL;

    s = 1;
    last_splitter_index = i++;
    while (buffer_parsef[i] != '#')
    {
        parse_value->Poznámka_T = realloc(parse_value->Poznámka_T, s++);
        parse_value->Poznámka_T[i - last_splitter_index - 1] = buffer_parsef[i];
        i++;
    }
}

void create_linked_list_item_from_file(LINKED_LIST_ITEM *item, int number_of_string, LINKED_LIST_ITEM *tail, FILE *datastream, FILE *stringstream, FILE *parsestream) //I want to add some OOP style. Idk, it looks like factory method, but it's just a procedure
{
    item->tail = tail;
    create_data_value_from_file(&(item->data_value), datastream, number_of_string);
    create_string_value_from_file(&(item->string_value), stringstream, number_of_string);
    create_parse_value_from_file(&(item->parse_value), parsestream, number_of_string);
}

void create_data_value_from_string(data_element *data_value, char *string)
{
    data_value->Hodnota_1 = malloc(BUFFER_DATAF_SUBSTRING_SIZE);
    data_value->Hodnota_2 = malloc(BUFFER_DATAF_SUBSTRING_SIZE);
    data_value->Hodnota_ID = malloc(BUFFER_DATAF_SUBSTRING_SIZE);
    data_value->Hodnota_zn = malloc(BUFFER_DATAF_SUBSTRING_SIZE);

    buffer_flush(data_value->Hodnota_1, BUFFER_DATAF_SUBSTRING_SIZE);
    buffer_flush(data_value->Hodnota_2, BUFFER_DATAF_SUBSTRING_SIZE);
    buffer_flush(data_value->Hodnota_ID, BUFFER_DATAF_SUBSTRING_SIZE);
    buffer_flush(data_value->Hodnota_zn, BUFFER_DATAF_SUBSTRING_SIZE);
    
    sscanf(string, "%s %s %s %s", data_value->Hodnota_1, data_value->Hodnota_2, data_value->Hodnota_ID, data_value->Hodnota_zn);
}

void create_string_value_from_string(char **string_value, char *string)
{
    *string_value = malloc(BUFFER_STRINGF_SIZE);
    strcpy(*string_value, string);
}

void create_parse_value_from_string(parse_element *parse_value, char *string)
{
    int i, j, last_splitter_index, s;
    char *buffer_time;

    parse_value->Poznámka_ID = NULL;
    parse_value->Poznámka_N1 = NULL;
    buffer_time = NULL;
    parse_value->Poznámka_Hodina = NULL;
    parse_value->Poznámka_Minuta = NULL;
    parse_value->Poznámka_T = NULL;

    //As long as idk the sizes of substrings in parse.txt, im initializing all the fields in such an iterative way
    i = 0;
    s = 1;
    while (string[i] != '#')
    {
        parse_value->Poznámka_ID = realloc(parse_value->Poznámka_ID, s++);
        parse_value->Poznámka_ID[i] = string[i];
        i++;
    }
    
    s = 1;
    last_splitter_index = i++;
    while (string[i] != '#')
    {
        parse_value->Poznámka_N1 = realloc(parse_value->Poznámka_N1, s++);
        parse_value->Poznámka_N1[i - last_splitter_index - 1] = string[i];
        i++;
    }

    s = 1;
    last_splitter_index = i++;
    while (string[i] != '#')
    {
        buffer_time = realloc(buffer_time, s++);
        buffer_time[i - last_splitter_index - 1] = string[i];
        i++;
    }

    s = 1;
    for (j = 0; j < 2; j++)
    {
        if (buffer_time == NULL)
            break;
        parse_value->Poznámka_Hodina = realloc(parse_value->Poznámka_Hodina, s);
        parse_value->Poznámka_Minuta = realloc(parse_value->Poznámka_Minuta, s++);
        parse_value->Poznámka_Hodina[j] = buffer_time[j];
        parse_value->Poznámka_Minuta[j] = buffer_time[j + 2];
    }

    free(buffer_time);
    buffer_time = NULL;

    s = 1;
    last_splitter_index = i++;
    while (string[i] != '#')
    {
        parse_value->Poznámka_T = realloc(parse_value->Poznámka_T, s++);
        parse_value->Poznámka_T[i - last_splitter_index - 1] = string[i];
        i++;
    }
}

void create_linked_list_item_from_string(LINKED_LIST_ITEM *item, LINKED_LIST_ITEM *tail, char *data_string, char *string_string, char *parse_string)
{
    item->tail = tail;
    create_data_value_from_string(&(item->data_value), data_string);
    create_string_value_from_string(&(item->string_value), string_string);
    create_parse_value_from_string(&(item->parse_value), parse_string);
}

void free_data_element(data_element *data_value)
{
    free(data_value->Hodnota_1);
    free(data_value->Hodnota_2);
    free(data_value->Hodnota_ID);
    free(data_value->Hodnota_zn);

    data_value->Hodnota_1 = NULL;
    data_value->Hodnota_2 = NULL;
    data_value->Hodnota_ID = NULL;
    data_value->Hodnota_zn = NULL;
}

void free_parse_element(parse_element *parse_value)
{
    free(parse_value->Poznámka_ID);
    free(parse_value->Poznámka_N1);
    free(parse_value->Poznámka_Hodina);
    free(parse_value->Poznámka_Minuta);
    free(parse_value->Poznámka_T);

    parse_value->Poznámka_ID = NULL;
    parse_value->Poznámka_N1 = NULL;
    parse_value->Poznámka_Hodina = NULL;
    parse_value->Poznámka_Minuta = NULL;
    parse_value->Poznámka_T = NULL;
}

void v(FILE **datastream, FILE **stringstream, FILE **parsestream, char ***d_datastream, char ***d_stringstream, char ***d_parsestream, LINKED_LIST_ITEM *linked_list, int *l_string_max_count, int *d_string_max_count, int *initial_string_max_count)
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

    LINKED_LIST_ITEM *current_list_item;
    current_list_item = linked_list;

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
            
            find_initial_string_max_count(*datastream, *stringstream, *parsestream, initial_string_max_count);

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
            for (i = 0; i < *d_string_max_count; i++)
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

        case 3:
            if (linked_list == NULL)
            {
                printf("V3: Nenaplnený spajany zoznam.\n");
                break;
            }
            while (current_list_item != NULL)
            {
                printf("ID. mer. modulu: %s\n", current_list_item->string_value);
                printf("Hodnota 1: %s\n", current_list_item->data_value.Hodnota_ID);
                printf("Hodnota 2: %s\n", current_list_item->data_value.Hodnota_zn);
                printf("Poznámka ID: %s\n", (current_list_item->parse_value.Poznámka_ID != NULL ? current_list_item->parse_value.Poznámka_ID : "NaN"));
                printf("Poznámka C: %s : %s => %s\n", (current_list_item->parse_value.Poznámka_Hodina != NULL ? current_list_item->parse_value.Poznámka_Hodina : "-1"), (current_list_item->parse_value.Poznámka_Minuta != NULL ? current_list_item->parse_value.Poznámka_Minuta : "-1"), (current_list_item->parse_value.Poznámka_N1 != NULL ? current_list_item->parse_value.Poznámka_N1 : "-1"));
                printf("Poznámka T: %s\n\n", (current_list_item->parse_value.Poznámka_T != NULL ? current_list_item->parse_value.Poznámka_T : "NaN"));
                current_list_item = current_list_item->tail;
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



void n(FILE *datastream, FILE *stringstream, FILE *parsestream, char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int *d_string_max_count, int initial_string_max_count)
{
    if (datastream == NULL || stringstream == NULL || parsestream == NULL) //Break if file isn't open
    {
        printf("N: Neotvoreny subor.\n");
        return;
    }
    if (*d_datastream != NULL || *d_stringstream != NULL || *d_parsestream != NULL)
    {
        int i;
        for (i = 0; i < *d_string_max_count; i++)
        {
            free((*d_datastream)[i]);
            free((*d_stringstream)[i]);
            free((*d_parsestream)[i]);
            (*d_datastream)[i] = NULL;
            (*d_stringstream)[i] = NULL;
            (*d_parsestream)[i] = NULL;
        }
     
        *d_string_max_count = 0;
        *d_datastream = NULL;
        *d_stringstream = NULL;
        *d_parsestream = NULL;
    }
    
    #pragma region //Variables
    int i, j;
    char buffer_dataf[BUFFER_DATAF_SIZE];
    char buffer_stringf[BUFFER_STRINGF_SIZE];
    char buffer_parsef[BUFFER_PARSEF_SIZE];
    *d_string_max_count = initial_string_max_count; 

    #pragma endregion

    #pragma region //Preparation of dynamical arrays
    *d_datastream = (char **)malloc(*d_string_max_count * sizeof(char *));
    *d_stringstream = (char **)malloc(*d_string_max_count * sizeof(char *));
    *d_parsestream = (char **)malloc(*d_string_max_count * sizeof(char *));

    for (i = 0; i < *d_string_max_count; i++)
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



void q(char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int *d_string_max_count)
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
        (*d_string_max_count)++; 
        *d_datastream = (char **)realloc(*d_datastream, (*d_string_max_count) * sizeof(char *));
        *d_stringstream = (char **)realloc(*d_stringstream, (*d_string_max_count) * sizeof(char *));
        *d_parsestream = (char **)realloc(*d_parsestream, (*d_string_max_count) * sizeof(char *));

        (*d_datastream)[*d_string_max_count - 1] = (char *)malloc(BUFFER_DATAF_SIZE * sizeof(char));
        (*d_stringstream)[*d_string_max_count - 1] = (char *)malloc(BUFFER_STRINGF_SIZE * sizeof(char));
        (*d_parsestream)[*d_string_max_count - 1] = (char *)malloc(BUFFER_PARSEF_SIZE * sizeof(char));
        #pragma endregion

        buffer_flush((*d_datastream)[*d_string_max_count - 1], BUFFER_DATAF_SIZE);
        buffer_flush((*d_stringstream)[*d_string_max_count - 1], BUFFER_STRINGF_SIZE);
        buffer_flush((*d_parsestream)[*d_string_max_count - 1], BUFFER_PARSEF_SIZE);
        
        Y = (Y <= (*d_string_max_count) ? Y - 1 : (*d_string_max_count) - 1);
        
        //Move down
        for (i = (*d_string_max_count) - 1; i > Y; i--)
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



void w(char ***d_datastream, char ***d_stringstream, char ***d_parsestream, int *d_string_max_count)
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
        for (i = 0; i < *d_string_max_count; i++)
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
            for (i = 0; i < *d_string_max_count; i++)
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
            for (j = *d_string_max_count - (found_index[pointer] + 1); j < *d_string_max_count; j++)
            {
                free((*d_datastream)[j]);
                free((*d_stringstream)[j]);
                free((*d_parsestream)[j]);
                (*d_datastream)[j] = NULL;
                (*d_stringstream)[j] = NULL;
                (*d_parsestream)[j] = NULL;
            }

            *d_string_max_count -= found_index_count;
            *d_datastream = (char **)realloc(*d_datastream, (*d_string_max_count) * sizeof(char *));
            *d_stringstream = (char **)realloc(*d_stringstream, (*d_string_max_count) * sizeof(char *));
            *d_parsestream = (char **)realloc(*d_parsestream, (*d_string_max_count) * sizeof(char *));

            free(found_index);
            found_index = NULL;
            #pragma endregion
        }

        printf("W: Vymazalo sa : %d zaznamov !\n", found_index_count);
    }
}



void e(char ***d_parsestream, int *d_string_max_count, int *initial_string_max_count, FILE *parsestream)
{   
    getchar(); //Fix of clicking Enter
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
        buffer_flush(buffer_parsef_input, BUFFER_PARSEF_SIZE);
        fgets(buffer_parsef_input, BUFFER_PARSEF_SIZE, stdin);
        remove_newline_symbols(buffer_parsef_input, BUFFER_PARSEF_SIZE);
        for (i = 0; i < *d_string_max_count; i++)
        {
            if (strstr((*d_parsestream)[i], buffer_parsef_input) != NULL)
            {
                for (j = 0; j < *initial_string_max_count; j++)
                {
                    buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);
                    fgets(buffer_parsef, BUFFER_PARSEF_SIZE, parsestream);
                    remove_newline_symbols(buffer_parsef, BUFFER_PARSEF_SIZE);

                    if (strcmp((*d_parsestream)[i], buffer_parsef) == 0)
                    {
                        printf("%s\n", (*d_parsestream)[i]);
                    }
                }
            }
            rewind(parsestream);
        }
    }
}



void m(FILE *datastream, FILE *stringstream, FILE *parsestream, LINKED_LIST_ITEM **first_list_item, int *d_string_max_count, int initial_string_max_count)
{
    int i;
    LINKED_LIST_ITEM *current_list_item, *tail_list_item;

    if (datastream == NULL || stringstream == NULL || parsestream == NULL) //Break if file isn't open
    {
        printf("M: Neotvoreny subor.\n");
        return;
    }
    
    if (first_list_item != NULL) //Freeing old linked list
    {
        LINKED_LIST_ITEM *next;
        next = *first_list_item;
        while (next != NULL)
        {
            free_data_element(&(next->data_value));
            free(next->string_value);
            next->string_value = NULL; 
            free_parse_element(&(next->parse_value));

            next = next->tail;
        }

        *d_string_max_count = 0;
    }
    
    current_list_item = malloc(sizeof(LINKED_LIST_ITEM));
    create_linked_list_item_from_file(current_list_item, initial_string_max_count - 1, NULL, datastream, stringstream, parsestream); //Creating last item of linked list

    for (i = initial_string_max_count - 2; i >= 0; i--) //Creating new linked list
    {
        tail_list_item = current_list_item;
        current_list_item = malloc(sizeof(LINKED_LIST_ITEM));
        create_linked_list_item_from_file(current_list_item, i, tail_list_item, datastream, stringstream, parsestream);
    }

    *first_list_item = current_list_item;
    current_list_item = NULL;
    tail_list_item = NULL;

    *d_string_max_count = initial_string_max_count;
    printf("M: Nacitalo sa %d zaznamov.\n", initial_string_max_count);
}



void a(LINKED_LIST_ITEM **first_list_item, int *l_string_max_count)
{
    #pragma region //Variables
    int i, j;
    char buffer_dataf[BUFFER_DATAF_SIZE];
    char buffer_stringf[BUFFER_STRINGF_SIZE];
    char buffer_parsef[BUFFER_PARSEF_SIZE];

    LINKED_LIST_ITEM *new_element, *current_element, *prev_element;
    int Y;
    #pragma endregion

    (*l_string_max_count)++;
    current_element = *first_list_item;
    prev_element = NULL;

    //Checking whether LL is created or not
    if (*first_list_item != NULL)
    {
        scanf("%d", &Y);
        Y = (Y <= (*l_string_max_count) ? Y - 1 : (*l_string_max_count) - 1);
    }
    else
        Y = 0;

    #pragma region //Preparation of buffers
    buffer_flush(buffer_dataf, BUFFER_DATAF_SIZE);
    buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
    buffer_flush(buffer_parsef, BUFFER_PARSEF_SIZE);

    scanf("%s\n", buffer_stringf);
    fgets(buffer_dataf, BUFFER_DATAF_SIZE, stdin);
    fgets(buffer_parsef, BUFFER_PARSEF_SIZE, stdin);

    remove_newline_symbols(buffer_dataf, BUFFER_DATAF_SIZE);
    remove_newline_symbols(buffer_stringf, BUFFER_STRINGF_SIZE);
    remove_newline_symbols(buffer_parsef, BUFFER_PARSEF_SIZE);
    #pragma endregion

    for (i = 0; i < Y; i++)
    {
        prev_element = current_element;
        current_element = current_element->tail;
    }

    //Creation of new element
    new_element = malloc(sizeof(LINKED_LIST_ITEM));
    create_linked_list_item_from_string(new_element, current_element, buffer_dataf, buffer_stringf, buffer_parsef);

    if (prev_element == NULL)
        *first_list_item = new_element;
    else
        prev_element->tail = new_element;
}



void s(LINKED_LIST_ITEM **first_list_item, int *l_string_max_count)
{
    if (*first_list_item == NULL)
    {
        printf("S: Spajany zoznam nie je vytvorený.\n");
        return;
    }

    #pragma region //Variables
    int i, j, *found_index, found_index_count, pointer, result_of_comparison;
    char buffer_stringf[BUFFER_STRINGF_SIZE];
    LINKED_LIST_ITEM *current_element, *previous_element, *previous_element_tail, **found_elements;

    current_element = *first_list_item;
    previous_element = NULL;
    found_index = NULL;
    found_index_count = 0;
    pointer = 0;
    #pragma endregion

    //Find strings' numbers
    i = 0;
    buffer_flush(buffer_stringf, BUFFER_STRINGF_SIZE);
    scanf("%s", buffer_stringf);
    remove_newline_symbols(buffer_stringf, BUFFER_STRINGF_SIZE);

    while (current_element != NULL)
    {
        result_of_comparison = strcmp(current_element->string_value, buffer_stringf);
        if(result_of_comparison == 0)
        {
            found_index = (int *)realloc(found_index, (++found_index_count) * sizeof(int));
            found_index[found_index_count - 1] = i;
        }
        i++;
        current_element = current_element->tail;
    }

    #pragma region //Moving records
    found_elements = malloc(found_index_count * sizeof(LINKED_LIST_ITEM*));
    current_element = *first_list_item;
    if (found_index_count > 0)
    {
        for (i = 0; i < *l_string_max_count - 1; i++)
        {
            if (i != found_index[pointer])
            {
                previous_element = current_element;
                current_element = current_element->tail;
                continue;
            }
            
            found_elements[pointer++] = current_element;
            current_element = current_element->tail;
            if (previous_element != NULL)
                previous_element->tail = current_element;
        }

        //Special for last item in list
        if (i == found_index[pointer])
        {
            found_elements[pointer++] = current_element;
            if (previous_element != NULL)
                previous_element->tail = NULL;
        }
    }
    #pragma endregion

    #pragma region //Memory freeing
    for (i = 0; i < found_index_count; i++)
    {
        if (*first_list_item == found_elements[i])
            *first_list_item = (*first_list_item)->tail; 
        free_data_element(&(found_elements[i]->data_value));
        free(found_elements[i]->string_value);
        found_elements[i]->string_value = NULL;
        free_parse_element(&(found_elements[i]->parse_value));
        free(found_elements[i]);
        found_elements[i] = NULL;
    }

    free(found_elements);
    free(found_index);
    found_elements = NULL;
    found_index = NULL;
    #pragma endregion

    *l_string_max_count -= found_index_count;
    printf("S: Vymazalo sa : %d zaznamov !\n", found_index_count);
}



void d(LINKED_LIST_ITEM **first_list_item, int *l_string_max_count)
{
    //Variables
    int i, c1, c2, t;
    LINKED_LIST_ITEM *i1, *i2, temp, *prev_i1, *prev_i2, *temp_prev;
    scanf("%d %d", &c1, &c2);

    if (c1 > *l_string_max_count || c1 <= 0 || c2 > *l_string_max_count || c2 <= 0 || c1 == c2)
        return;

    if (c1 > c2) //If c2 > c1, there is a cycle, and Im too lazy to figure out another algorithm.
    {
        t = c1;
        c1 = c2;
        c2 = t;
    }
    #pragma region //Setting pointers i1, i2, temp to items we need 
    c1--;
    c2--;
    i1 = *first_list_item;
    i2 = *first_list_item;
    prev_i1 = NULL;
    prev_i2 = NULL;

    for (i = 0; i < c1; i++)
    {
        if(i == c1 - 1 && c1 != 0)
            prev_i1 = i1;
        i1 = i1->tail;
    }

    for (i = 0; i < c2; i++)
    {
        if (i == c2 - 1 && c2 != 0)
            prev_i2 = i2;
        i2 = i2->tail;
    }
    #pragma endregion    

    #pragma region //Pointer swap
    temp = *i2;
    if (prev_i1 == NULL)
        *first_list_item = i2;
    else
        prev_i1->tail = i2;

    if (c2 - c1 != 1)
        i2->tail = i1->tail;
    else i2->tail = i1;
    prev_i2->tail = i1;
    i1->tail = temp.tail;
    #pragma endregion
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

    LINKED_LIST_ITEM *linked_list;
    linked_list = NULL;

    int l_string_max_count, d_string_max_count, initial_string_max_count, i;
    l_string_max_count = 0;
    d_string_max_count = 0;
    initial_string_max_count = 0;

    #pragma endregion

    while (1)
    {
        switch (getchar())
        {
            case 'v':
                v(&dataf, &strf, &parsef, &d_dataf, &d_strf, &d_parsef, linked_list, &l_string_max_count, &d_string_max_count, &initial_string_max_count);
                break;
            case 'h':
                h(strf);
                break;
            case 'n':
                n(dataf, strf, parsef, &d_dataf, &d_strf, &d_parsef, &d_string_max_count, initial_string_max_count);
                break;
            case 'q':
                q(&d_dataf, &d_strf, &d_parsef, &d_string_max_count);
                break;
            case 'w':
                w(&d_dataf, &d_strf, &d_parsef, &d_string_max_count);
                break;
            case 'e':
                e(&d_parsef, &d_string_max_count, &initial_string_max_count, parsef);
                break;
            case 'm':
                m(dataf, strf, parsef, &linked_list, &l_string_max_count, initial_string_max_count);
                break;
            case 'a':
                a(&linked_list, &l_string_max_count);
                break;
            case 's':
                s(&linked_list, &l_string_max_count);
                break;
            case 'd':
                d(&linked_list, &l_string_max_count);
                break;
            case 'k':
                if (dataf != NULL)
                    fclose(dataf);
                if (strf != NULL)
                    fclose(strf);
                if (parsef != NULL)
                    fclose(parsef);

                if (d_dataf != NULL)
                {
                    for (i = 0; i < d_string_max_count; i++)
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
                }

                if (linked_list != NULL)
                {
                    LINKED_LIST_ITEM *next, *cur;
                    cur = linked_list;
                    while (cur != NULL)
                    {
                        next = cur->tail;

                        free_data_element(&(cur->data_value));
                        free(cur->string_value);
                        cur->string_value = NULL; 
                        free_parse_element(&(cur->parse_value));
                        free(cur);

                        cur = next;
                    }

                    cur = NULL;
                    next = NULL;
                    linked_list = NULL;
                }
                return 0;
        }
    }
}