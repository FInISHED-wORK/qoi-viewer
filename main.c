#include <stdio.h>
#include <string.h>

#include "raylib.h"

#define QOI_IMPLEMENTATION
#include "qoi.h"

int check_qoi_get_channels(const char* filepath)
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("ERROR: Couldn't read file %s\n", filepath);
        exit(1);
    }

    char *magic = malloc(sizeof(char) * 4); 
    fseek(file, 0, SEEK_SET);
    fread(magic, sizeof(char), 4, file);

    if(strcmp(magic, "qoif") != 0) {
        printf("ERROR: Given file isn't a valid QOI image.\n");
        exit(1);
    }

    free(magic);

    char *channels = malloc(sizeof(char)); 
    fseek(file, 12, SEEK_SET);
    fread(channels, sizeof(char), 1, file);

    int ret = atoi(channels);

    free(channels);
    fclose(file);

    return ret;
}

int main(int argc, char **argv)
{
    SetTraceLogLevel(LOG_ERROR);

    (void)argc;
    (void)*argv++;

    if (*argv == NULL)
    {
        printf("USAGE: ./qoi-viewer <input>.qoi\n");
        exit(1);
    }

    const char *filepath = *argv++;

    int channels = check_qoi_get_channels(filepath);

    qoi_desc desc = {0};

    unsigned char *qoi = qoi_read(filepath, &desc, channels);

    if (qoi == NULL)
    {
        printf("ERROR: Couldn't read QOI image.\n");
        exit(1);
    }

    InitWindow(desc.width, desc.height, "qoi-viewer");
    SetWindowTitle(filepath);

    printf("QOI Desc: %dx%d, channels %d\n", desc.width, desc.height, desc.channels);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        unsigned int y = 0;
        unsigned int x = 0;

        size_t amount = (desc.width * desc.height) * desc.channels;

        unsigned int pixel = 0;
        for (size_t i = 0; i < amount;)
        {
            unsigned char r = qoi[i++];
            unsigned char g = qoi[i++];
            unsigned char b = qoi[i++];
            unsigned char a = 255;

            if (desc.channels == 4)
                a = qoi[i++];

            x = pixel % desc.width;
            y = (pixel - x) / desc.width;

            DrawPixel(x, y, (Color){r, g, b, a});

            pixel++;
        }

        EndDrawing();
    }

    free(qoi);

    CloseWindow();
}