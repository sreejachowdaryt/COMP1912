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
void shuffle_directions(int directions[][2]);

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

    // Generate maze using modified DFS algorithm
    dfs_generate_maze(&maze, maze.start);

    return maze;
}

void dfs_generate_maze(Maze *maze, Coord location) {
    int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // Up, down, left, right
    shuffle_directions(directions); // Shuffle directions randomly

    // Try each direction
    for (int i = 0; i < 4; i++) {
        Coord next = {location.x + directions[i][0], location.y + directions[i][1]};
        Coord wall = {location.x + directions[i][0] / 2, location.y + directions[i][1] / 2};
        if (is_valid_location(*maze, next)) {
            maze->map[next.y][next.x] = ' ';
            maze->map[wall.y][wall.x] = ' '; // Modify to create passage instead of leaving a wall
            dfs_generate_maze(maze, next);
        }
    }
}

bool is_valid_location(Maze maze, Coord location) {
    return location.x >= 0 && location.x < maze.width && location.y >= 0 && location.y < maze.height &&
           maze.map[location.y][location.x] == '#';
}

void save_maze_to_file(Maze maze, char *filename) {
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

    fclose(file);
}

void shuffle_directions(int directions[][2]) {
    // Shuffle directions randomly
    for (int i = 0; i < 4; i++) {
        int j = rand() % (i + 1);
        int temp[2];
        temp[0] = directions[i][0];
        temp[1] = directions[i][1];
        directions[i][0] = directions[j][0];
        directions[i][1] = directions[j][1];
        directions[j][0] = temp[0];
        directions[j][1] = temp[1];
    }
}
