/**************************************************************************************************
 INCLUDES
**************************************************************************************************/

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sl.h"
#include "ascii.h"

/**************************************************************************************************
 CONSTANTS
**************************************************************************************************/

static const char* const VALID_OPTIONS[] = { "-n", "-f", "-t" };
static const char* const INVALID_ARGS = "Invalid argument(s) passed to sl.\r\n\r\nUSAGE:\r\n\r\nsl [OPTION_NAME] [OPTION_VALUE] ... \r\n\r\nOPTIONS:\r\n\r\n-n\tSpecify which animation to view (1 though " LOGO_COUNT_STR ").\r\n\r\n-f\tThe path to a file containing ASCII art; overrides the '-n' switch.\r\n\r\n-t\tPass in text (max " MAX_TEXT_LENGTH_STR " characters) to be converted to ASCII art; overrides the '-n' and '-f' switches.\r\n\r\n";
static const char* const INVALID_ARG_N = "The value for parameter '-n' must be between 1 and " LOGO_COUNT_STR ".\r\n\r\n";
static const char* const INVALID_ARG_F = "Error opening file: '%s'\n";

/**************************************************************************************************
 STRUCTS
**************************************************************************************************/

struct option
{
    const char* name;
    const char* value;
};

struct option_array
{
    struct option* options;
    int count;
};

/**************************************************************************************************
 GLOBALS
**************************************************************************************************/

static struct ascii_art g_custom_logo;

/**************************************************************************************************
 FUNCTIONS
**************************************************************************************************/

int print_line(int y, int x, char* str)
{
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    
    return OK;
}

int update_logo_1(int x)
{
    static char* logo[LOGO1_HEIGHT] = {LOGO1_STR1, LOGO1_STR2, LOGO1_STR3, LOGO1_STR4, LOGO1_STR5, LOGO1_STR6, LOGO1_STR7, LOGO1_STR8, LOGO1_STR9, LOGO1_STR10, LOGO1_STR11};

    if (x < -LOGO1_LENGTH)  return ERR;

    int y = (LINES / 2) - (LOGO1_HEIGHT / 2); // the image will be vertically centered

    for (int i = 0; i < LOGO1_HEIGHT; ++i)
        print_line(y + i, x, logo[i]);

    return OK;
}

int update_logo_2(int x)
{
    static char* logo[LOGO2_HEIGHT] = {LOGO2_STR1, LOGO2_STR2, LOGO2_STR3, LOGO2_STR4, LOGO2_STR5, LOGO2_STR6, LOGO2_STR7, LOGO2_STR8, LOGO2_STR9};

    if (x < -LOGO2_LENGTH)  return ERR;

    static int y = 0;
    if (x % 4 == 0) y++; // will move image downward

    for (int i = 0; i < LOGO2_HEIGHT; ++i)
        print_line(y + i, x, logo[i]);

    return OK;
}

int update_logo_3(int x)
{
    if (x % 3 != 0) return OK; 

    static char* logo[LOGO3_HEIGHT] = {LOGO3_STR1, LOGO3_STR2, LOGO3_STR3, LOGO3_STR4, LOGO3_STR5, LOGO3_STR6, LOGO3_STR7, LOGO3_STR8, LOGO3_STR9};

    if (x < -LOGO3_LENGTH)  return ERR;

    x = (COLS / 2) - (LOGO3_LENGTH / 2);
    int y = LINES / 2;
    int r = rand() % 5;

    switch (r)
    {
        case 0:      // do nothing, leave the image centered
	    break;
	case 1:      // move image up and left
	    x -= 1;
	    y -= 1;
	    break;
	case 2:      // move image up and right
	    x += 1;
            y -= 1;
	    break;
        case 3:      // move image down and left
	    x -= 1;
	    y += 1;
	    break;
	case 4:      // move image down and right
	    x += 1;
	    y += 1;
	    break;
    }

    for (int i = 0; i < LOGO3_HEIGHT; ++i)
        print_line(y + i, x, logo[i]);

    return OK;
}

int update_custom_logo(int x)
{
    if (x < -g_custom_logo.length)  return ERR;

    int y = (LINES / 2) - (g_custom_logo.height / 2); // the image will be vertically centered

    for (int i = 0; i < g_custom_logo.height; ++i)
        print_line(y + i, x, g_custom_logo.data[i]);

    return OK;
}

struct option* option_new(const char* name, const char* value)
{
    struct option* result = malloc(sizeof(struct option));
    result->name = name;
    result->value = value;
    return result;
}

struct option_array* option_array_new(struct option* options, int count)
{
    struct option_array* result = malloc(sizeof(struct option_array));
    result->options = options;
    result->count = count;
    return result;
}

struct option_array* get_options(int argc, char* argv[])
{
    if (argc == 0 || argc == 1) return option_array_new(NULL, 0);
    
    if ((argc - 1) % 2 != 0)
    {
        printf(INVALID_ARGS);        
        return NULL;
    }

    int option_count = (argc - 1) / 2;
    int option_index = 0;
    
    struct option* options;
    options = malloc(option_count * sizeof(struct option));
    
    for (int i = 1; i < argc - 1; i += 2)
    {
        options[option_index].name = argv[i];
	options[option_index++].value = argv[i+1];
    }

    struct option_array* result = option_array_new(options, option_count);
    
    int valid_option_count = sizeof(VALID_OPTIONS) / sizeof(VALID_OPTIONS[0]);
    for (int i = 0; i < result->count; ++i)
    {
	bool found = false;
        for (int j = 0; j < valid_option_count; ++j)
	{
	    if (strcmp(result->options[i].name, VALID_OPTIONS[j]) == 0)
	    {
		found = true;
		break;
            }
	}

	if (!found)
	{
	    printf(INVALID_ARGS);
	    return NULL;
	}
    }

    return result;
}

struct option* get_option(struct option_array* options, const char* name)
{
    for (int i = 0; i < options->count; ++i)
        if (strcmp(options->options[i].name, name) == 0) return &options->options[i];

    return NULL;
}

int get_logo(int argc, char* argv[])
{
    struct option_array* options = get_options(argc, argv);
    if (options == NULL) return -1;

    int logo = 0;
    struct option* n = get_option(options, "-n");
    if (n != NULL)
    {
        logo = (int)strtol(n->value, (char**)NULL, 10);
	if (logo < 1 || logo > LOGO_COUNT)
	{
	    printf(INVALID_ARG_N);
	    return -1;
	}
    }

    struct option* f = get_option(options, "-f");
    if (f != NULL)
    {
    	char *line_buf = NULL;
	size_t line_buf_size = 0;
        int line_count = 0;
	ssize_t line_size;
	
	FILE* file = fopen(f->value, "r");

        if (!file)
	{
	    fprintf(stderr, INVALID_ARG_F, f->value);
	    return ERR;
	}
        
	int max_line_size = 0;
	line_size = getline(&line_buf, &line_buf_size, file);
        max_line_size = max_line_size > line_size ? max_line_size : line_size;
	
	while (line_size >= 0)
	{
	    line_count++;
	    line_size = getline(&line_buf, &line_buf_size, file);
	    max_line_size = max_line_size > line_size ? max_line_size : line_size;
	}

	free(line_buf);
	line_buf = NULL;
	fclose(file);
	
	g_custom_logo.data = calloc(line_count, sizeof(char*));
        g_custom_logo.length = max_line_size;
	g_custom_logo.height = line_count;
	line_size = max_line_size = line_count = 0;

	file = fopen(f->value, "r");

        if (!file)
	{
	    fprintf(stderr, INVALID_ARG_F, f->value);
	    return ERR;
	}
                 
	line_size = getline(&line_buf, &line_buf_size, file);
	
	while (line_size >= 0)
	{
	    g_custom_logo.data[line_count] = calloc(g_custom_logo.length, sizeof(char));
	    memcpy(g_custom_logo.data[line_count++], line_buf, line_size);
	    line_size = getline(&line_buf, &line_buf_size, file);
	}

	free(line_buf);
	line_buf = NULL;
	fclose(file);
        
	logo = 100;
    }

    struct option* t = get_option(options, "-t");
    if (t != NULL)
    {
        if (strlen(t->value) > MAX_TEXT_LENGTH)
        {
            printf(INVALID_ARGS);
            return ERR;
        }

        g_custom_logo = get_ascii_art(t->value);
        logo = 100;
    }

    return logo;
}

void init()
{
    initscr();
    signal(SIGINT, SIG_IGN);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);
}

int animate(int logo)
{
    if (logo == 0)
    {
	time_t t;
	srand((unsigned) time(&t));
	logo = (rand() % LOGO_COUNT) + 1;
    }
    
    for (int x = COLS - 1; ; --x) 
    {
	switch (logo)
	{
	    case 1:
                if (update_logo_1(x) == ERR) return ERR;
                break;
	    case 2:
		if (update_logo_2(x) == ERR) return ERR;
		break;
	    case 3:
		if (update_logo_3(x) == ERR) return ERR;
                break;
	    case 100:
		if (update_custom_logo(x) == ERR) return ERR;
		break;
        }

        getch();
        refresh();
        usleep(40000);
    }

    return OK;
}

void finalize()
{
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
}

int main(int argc, char* argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    int logo = get_logo(argc, argv);
    if (logo == -1) return -1;
    
    init();
    animate(logo);
    finalize();

    return 0;
}
