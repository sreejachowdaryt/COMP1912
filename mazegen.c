/**
 * @file mazegen.c
 * @author T Sreeja Chowdary
 * @brief Additional Challenge Task – Maze Generator​
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_DIM 100
#define MIN_DIM 5

typedef struct {
    int x;
    int y;
} Coord;

typedef struct {
    char **map;
    int height;
    int width;
    Coord start;
    Coord end;
} Maze;

// Function prototypes
Maze generate_maze(int width, int height);
void save_maze_to_file(Maze maze, char *filename);
bool is_valid_location(Maze maze, Coord location);
void dfs_generate_maze(Maze *maze, Coord location);

int main(int argc, char *argv[]) {
    // Check command-line arguments
    if (argc != 4) {
        printf("Usage: ./mazegen <filename> <width> <height>\n");
        return EXIT_FAILURE;
    }

    // Parse command-line arguments
    char *filename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    // Check dimensions
    if (width < MIN_DIM || width > MAX_DIM || height < MIN_DIM || height > MAX_DIM) {
        printf("Invalid dimensions. Width and height must be between %d and %d.\n", MIN_DIM, MAX_DIM);
        return EXIT_FAILURE;
    }

    // Generate maze
    Maze maze = generate_maze(width, height);

    // Save maze to file
    save_maze_to_file(maze, filename);

    printf("Maze generated successfully and saved to %s\n", filename);

    return EXIT_SUCCESS;
}

// Generating maze with the input height and width.
Maze generate_maze(int width, int height) {
    Maze maze;
    maze.width = width;
    maze.height = height;

    // Allocate memory for the maze map
    maze.map = (char **)malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        maze.map[i] = (char *)malloc(width * sizeof(char));
    }

    // Initialize maze with walls
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze.map[i][j] = '#';
        }
    }

    // Set random seed
    srand(time(NULL));

    // Choose random starting point (not on border)
    maze.start.x = 2 + rand() % (width - 3);
    maze.start.y = 2 + rand() % (height - 3);
    maze.map[maze.start.y][maze.start.x] = 'S';

    // Choose random ending point (not on border and not equal to start)
    do {
        maze.end.x = 2 + rand() % (width - 3);
        maze.end.y = 2 + rand() % (height - 3);
    } while (maze.end.x == maze.start.x && maze.end.y == maze.start.y);
    maze.map[maze.end.y][maze.end.x] = 'E';

    return maze;
}

bool is_valid_location(Maze maze, Coord location) {
    return location.x >= 0 && location.x < maze.width && location.y >= 0 && location.y < maze.height &&
           maze.map[location.y][location.x] == '#';
}

//Maze generating algorithm used is dfs recursive implementation.
void dfs_generate_maze(Maze *maze, Coord location) {
    // to be implemented in later iterations.
}

/*
 *https://chat.openai.com/share/1d04501d-8adc-4b09-aaa9-370bad7a55c5
 * lines 125-13 adapted from the code chatgpt has given.
*/
void save_maze_to_file(Maze maze, char *filename) {
    // Opening a new text file.
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    // Write maze to file
    for (int i = 0; i < maze.height; i++) {
        for (int j = 0; j < maze.width; j++) {
            fprintf(file, "%c", maze.map[i][j]);
        }
        fprintf(file, "\n");
    }

    // closing the file. 
    fclose(file);
}



