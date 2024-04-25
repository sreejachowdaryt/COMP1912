#include <stdio.h>
#include <stdlib.h>

// Define maximum and minimum dimensions for the maze
#define MAX_DIM 100
#define MIN_DIM 5

// Define exit codes for error handling
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

// Define structures for coordinates and the maze
typedef struct __Coord {
    int x;
    int y;
} coord;

typedef struct __Maze {
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

// Function to get the width of the maze from the file
int get_width(FILE *file) {
    int width = 0;
    fseek(file, 0, SEEK_SET);
    while (fgetc(file) != '\n') width++;
    return width;
}

// Function to get the height of the maze from the file
int get_height(FILE *file) {
    int height = 0;
    fseek(file, 0, SEEK_SET);
    while (fgetc(file) != EOF) {
        if (fgetc(file) == '\n') height++;
    }
    return height;
}

// Function to read the maze from the file into the maze struct
int read_maze(maze *this, FILE *file) {
    // Get width and height of the maze
    this->width = get_width(file);
    this->height = get_height(file);

    // Allocate memory for the maze map
    this->map = (char **)malloc(this->height * sizeof(char *));
    if (this->map == NULL) return EXIT_MAZE_ERROR;

    // Read each line of the maze from the file
    for (int i = 0; i < this->height; i++) {
        // Allocate memory for each row of the maze
        this->map[i] = (char *)malloc((this->width + 1) * sizeof(char));
        if (this->map[i] == NULL) return EXIT_MAZE_ERROR;
        // Read the row from the file
        fseek(file, i * (this->width + 1), SEEK_SET);
        fread(this->map[i], sizeof(char), this->width + 1, file);
    }
    return EXIT_SUCCESS;
}

// Function to print the maze with the player's position
void print_maze(maze *this, coord *player) {
    printf("\n");
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (player->x == j && player->y == i) {
                printf("X");
            } else {
                printf("%c", this->map[i][j]);
            }
        }
        printf("\n");
    }
}

// Function to free memory allocated for the maze
void free_maze(maze *this) {
    for (int i = 0; i < this->height; i++) {
        free(this->map[i]);
    }
    free(this->map);
}

// Function to handle player movement within the maze
void move(maze *this, coord *player, char direction) {
    int new_x = player->x;
    int new_y = player->y;

    // Update player position based on the specified direction
    switch (direction) {
        case 'W':
        case 'w':
            new_y--;
            break;
        case 'A':
        case 'a':
            new_x--;
            break;
        case 'S':
        case 's':
            new_y++;
            break;
        case 'D':
        case 'd':
            new_x++;
            break;
        default:
            printf("Invalid direction!\n");
            return;
    }

    // Check if the new position is within the maze and not a wall
    if (new_x >= 0 && new_x < this->width && new_y >= 0 && new_y < this->height &&
        this->map[new_y][new_x] != '#') {
        player->x = new_x;
        player->y = new_y;
    } else {
        printf("Cannot move in that direction!\n");
    }
}

// Function to check if the player has reached the exit
int has_won(maze *this, coord *player) {
    return player->x == this->end.x && player->y == this->end.y;
}

// Main function
int main(int argc, char *argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc != 2) {
        printf("Usage: %s <maze_file>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    // Open the maze file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return EXIT_FILE_ERROR;
    }

    // Initialize maze and player variables
    maze this_maze;
    coord player;
    player.x = -1;
    player.y = -1;

    // Read the maze from the file
    int result = read_maze(&this_maze, file);
    if (result != EXIT_SUCCESS) {
        printf("Error reading maze from file!\n");
        fclose(file);
        return result;
    }

    // Find the starting position of the player in the maze
    for (int i = 0; i < this_maze.height; i++) {
        for (int j = 0; j < this_maze.width; j++) {
            if (this_maze.map[i][j] == 'S') {
                player.x = j;
                player.y = i;
                break;
            }
        }
    }

    // Check if the starting position is found
    if (player.x == -1 || player.y == -1) {
        printf("Starting point 'S' not found in maze!\n");
        free_maze(&this_maze);
        fclose(file);
        return EXIT_MAZE_ERROR;
    }

    // Game loop
    while (!has_won(&this_maze, &player)) {
        print_maze(&this_maze, &player);
        char direction;
        printf("Enter move (W/A/S/D): ");
        scanf(" %c", &direction);
        move(&this_maze, &player, direction);
    }

    // Display win message
    printf("Congratulations! You've won the game!\n");

    // Free allocated memory and close file
    free_maze(&this_maze);
    fclose(file);

    return EXIT_SUCCESS;
}
