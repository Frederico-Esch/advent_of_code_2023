#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct pos {
    size_t grid_index, line_index;
    size_t depth;
};

enum pipe_t {
    HORIZONTAL = '-',
    VERTICAL = '|',
    LEFT_DOWN = '7', LEFT_UP = 'J',
    RIGHT_DOWN = 'F', RIGHT_UP = 'L',
    NO_PIPE = '.',
    START = 'S'
};

enum plug_t {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum pipe_t grid[1024][1024] = {0};
size_t dgrid[1024][1024] = {0}; //depth grid

size_t line_size = 0;
size_t grid_size = 0;
struct pos s_pos = {.line_index = 0, .grid_index = 0 };
size_t queue_size = 0;


void fill_grid(void) {
    FILE* input = fopen("../input-teste.txt", "r");

    size_t size = 0;
    char* line = NULL;
    while (1) {
        size = 0; line = NULL;
        size = getline(&line, &size, input);
        if ((ssize_t)size == -1) break;
        if (line_size == 0) line_size = strlen(line) -1;

        for (size_t i = 0; i < line_size; i++) {
            enum pipe_t pipe = line[i];
            if (pipe == START) {
                s_pos = (struct pos){ .line_index = i, .grid_index = grid_size, .depth = 0 };
            }
            grid[grid_size][i] = pipe;
        }
        grid_size++;
        free(line);
    }
    free(line);

    fclose(input);
}

void print_grid() {
    for (size_t i = 0; i < grid_size; i++) {
        for (size_t j = 0; j < line_size; j++) {
            putchar(grid[i][j]);
        }
        putchar('\n');
    }
}

void enqueue(struct pos* queue, size_t amount_nodes, struct pos value) {
    if (amount_nodes == queue_size) {
        puts("AMOUNT OF NODES REACHED");
        exit(1);
    }
    queue[queue_size++] = value;
}

struct pos dequeue(struct pos* queue, size_t amount_nodes) {
    struct pos value = queue[0];
    if (queue_size == 0) {
        puts("theres nothing in the queue");
        exit(1);
    }
    memmove(queue, &queue[1], (amount_nodes-1)*sizeof(struct pos));
    queue_size--;
    return value;
}

bool should_enqueue(enum pipe_t curr, enum pipe_t next, enum plug_t plug) {
    if (next == NO_PIPE) {
        return false;
    }

    switch (plug) {

    case UP:
    break;

    case DOWN:
    break;

    case LEFT:
    break;

    case RIGHT:
    break;
    }
    return false;
}

void look_around(struct pos current, struct pos* queue, bool* visited, size_t amount_nodes) {
    ssize_t grid_index;
    ssize_t line_index;

    grid_index = (ssize_t)current.grid_index - 1; //up
    line_index = current.line_index;
    if (grid_index > 0) {
        enum pipe_t curr = grid[current.grid_index][current.line_index];
        enum pipe_t next = grid[grid_index][line_index];
        if (should_enqueue(curr, next, UP)) { }
    }

    grid_index = current.grid_index + 1; //down
    line_index = current.line_index;
    if (grid_index < grid_size) {
        enum pipe_t curr = grid[current.grid_index][current.line_index];
        enum pipe_t next = grid[grid_index][line_index];
        if (should_enqueue(curr, next, DOWN)) { }
    }

    grid_index = current.grid_index;
    line_index = current.line_index + 1; //right
    if (line_index < line_size) {
        enum pipe_t curr = grid[current.grid_index][current.line_index];
        enum pipe_t next = grid[grid_index][line_index];
        if (should_enqueue(curr, next, RIGHT)) { }
    }

    grid_index = current.grid_index;
    line_index = (ssize_t)current.line_index - 1; //left
    if (line_index > 0) {
        enum pipe_t curr = grid[current.grid_index][current.line_index];
        enum pipe_t next = grid[grid_index][line_index];
        if (should_enqueue(curr, next, LEFT)) { }
    }
}

void verify_node(struct pos current, struct pos* queue, bool* visited, size_t amount_nodes) {
    visited[current.grid_index*line_size + current.line_index] = true;
    look_around(current, queue, visited, amount_nodes);
}

int main(void) {
    fill_grid();

    printf("%zu x %zu\n", grid_size, line_size);

    size_t amount_nodes = grid_size*line_size;
    bool visited[amount_nodes]; //i + j*grid_size -> i = 0, grid_size-1 and j = 0, line_size-1 |
                                      //                                                             | grid_size-1 + (line_size-1 * grid_size)
                                      //                                                             | grid_size-1 + line_size*grid_size - grid_size
                                      //                                                             | line_size*grid_size -1 -> perfect

    struct pos queue[amount_nodes];
    for (size_t i = 0; i < amount_nodes; i++) {
        visited[i] = false;
        queue[i] = (struct pos){ .grid_index = 0, .line_index = 0 }; //as each node can only be visited once, then I don't need to have a queue bigger then the amount of nodes in the grid
    }
    puts("");

    enqueue(queue, amount_nodes, s_pos);
    while (queue_size > 0) {
        struct pos current = dequeue(queue, amount_nodes);

        verify_node(current, queue, visited, amount_nodes);
    }

    return 0;
}
