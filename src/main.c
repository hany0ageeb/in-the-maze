#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
/**
 * struct App_s - App_s
 * @window: window
 * @renderer: renderer
*/
typedef struct App_s {
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint32_t *colorBuffer;
    SDL_Texture *colorBufferTexture;
} App_t;
typedef struct Point_s {
    int x;
    int y;
} Point_t;
typedef struct Pixel_s {
    Point_t point;
    uint32_t color;
} Pixel_t;
/*
typedef struct Rectangle_s {

} Rectangle_s;
typedef struct Player_s {

}Player_t;
*/
typedef struct Map_s {
    int **map;
    int columns;
    int rows;
} Map_t;
/**
 * splitLine - split line (string) based on sep
 * @line: a string to split
 * @sep: separateor char
 * Return: array of null terminated strings
*/
char **splitLine(const char *line, const char sep, const size_t max_len)
{
    size_t start = 0, size = 0, end = 0, index = 0;
    char **result = NULL;

    if (line == NULL || *line == '\0')
        return (NULL);
    while (line[start] != '\0')
    {
        if (line[start] == sep)
            size++;
        start++;
    }
    if (size == 0)
        size = 1;
    result = malloc(sizeof(char *) * (size + 1));
    if (result != NULL)
    {
        start = 0;
        end = 0;
        while(line[end] != '\0' && index < max_len)
        {
            while(line[end] != sep)
            {
                end++;
            }
            result[index] = substring(line, start, end - 1);
            index++;
            if (result[end] == '\0')
                break;
            start = end + 1;
            end++;
        }
        result[size] = NULL;
        *len = size;
    }
    return (result);
}
/**
 * substring - substring source from start - end inclusive
 * @source: source string
 * @start: start index >= 0 inclusive
 * @end: end index inclusive
 * Return: the substring or NULL
*/
char *substring(const char *source, size_t start, size_t end)
{
    char *sub = NULL;
    size_t len, index;

    if (source != NULL && *source != '\0' && start <= end)
    {
        len = (end - start) + 2;
        sub = malloc(sizeof(char) * len);
        if (sub !=  NULL)
        {
            index = 0;
            while(start <= end)
            {
                sub[index] = source[start];
                index++;
                start++;
            }
            sub[len - 1] = '\0';
        }
    }
    return (sub);
}
Map_t *createMapFromFile(const char *filePath)
{
    FILE *fp = NULL;
    char *line = NULL;
    char **result;
    size_t len = 0, index = 0, columns;
    ssize_t read;
    Map_t *map = NULL;

    fp = fopen(filename, "r");
    if (fp == NULL)
        return (NULL);
    if ((read = getline(&line, &len, fp)) == -1)
        return (NULL);
    if (map == NULL)
        return (NULL);
    result = splitLine(line, ' ', 2);
    if (result == NULL)
        return (NULL);
    map = malloc(sizeof(Map_t));
    map->row = atoi(result[0]);
    map->column = atoi(result[1]);
    map->map = malloc(sizeof(int *) * map->row);
    index = 0;
    while ((read = getline(&line, &len, fp)) != -1 && index < map->row)
    {
        result = splitLine(line, &columns, ' ');
        map->columns = columns;
        map[index] = malloc(sizeof(int) * columns);
    }
    free(line);
    return (map);
}
App_t *createApp(const char *title)
{
    App_t *app = NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL Init Error: %s\n", SDL_GetError());
        return (NULL);
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(window == NULL)
    {
        fprintf(stderr, "SDL Create window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return (NULL);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "SDL Create Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return (NULL);
    }
    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
    {
        fprintf(stderr, "SDL Set RendererDrawBlendMode: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestrotyWindow(window);
        SDL_Quit();
        return (NULL);
    }
    texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL)
    {
        fprintf("SDL Create Texture Error: %s\n", SDL_GetError());
        SDL_DestrotyTexture(texture);
        SDL_DestrotyRenderer(renderer);
        SDL_DestroyWindow(window);
        return (NULL);
    }
    app = malloc(sizeof(App_t));
    if (app != NULL)
    {
        app->window = window;
        app->renderer = renderer;
        app->colorBuffer = malloc(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);
        app->colorBufferTexture = texture;
    }
    return (app);
}
void destroyApp(App_t *app)
{
    if (app != NULL)
    {
        if(app->colorBufferTexture != NULL)
        {
            SDL_DestroyTexture(app->colorBufferTexture);
        }
        if(app->window != NULL)
        {
            SDL_DestroyWindow(app->window);
        }
        if (app->renderer != NULL)
        {
            SDL_DestroyRenderer(app->renderer);
        }
        if(app->coloBuffer != NULL)
        {
            free(app->colorBuffer);
        }
        free(app);
    }
    SDL_Quit();
}
/**
 * main - Entry point
 * @argc: arguments count
 * @argv: argv
 * Return: 0 if success otherwiseher
*/
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    const char *windowTitle = "In the Maze!";
    int quit = 0;
    SDL_Event e;
    App_t *app = createApp(windowTitle);

    if (app == NULL)
    {
        fprintf(stderr, "Could not Create App Instance - Exit the program\n");
        exit(EXIT_FAILURE);
    }
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if (e.type == (Uint32)SDL_QUIT)
            {
                quit = 1;
                break;
            }
            switch(e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_w:
                case SDLK_UP:
                    break;
                case SDLK_DOWN:
		        case SDLK_s:
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    break;
                default:
                    break;
            }
        }
    }
    destroyApp(app);
    app = NULL;
    exit(EXIT_SUCCESS);
}