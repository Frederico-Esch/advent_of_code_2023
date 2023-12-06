#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char** grid = NULL;
size_t* sizes = NULL;

bool look_around(int x, int y, int x_size, int y_size) {

    int around[][2] = {
        {-1, 0},
        { 1, 0},

        {0, -1},
        {0,  1},

        {-1, -1},
        { 1,  1},

        {-1,  1},
        { 1,  -1},
    };

    for (int i = 0; i < 8; i ++) {
        if (x + around[i][0]  >= 0      && y + around[i][1]  >= 0 &&
            x + around[i][0]  <  x_size && y + around[i][1]  <  y_size) {
            if (!isdigit(grid[y+around[i][1]][x+around[i][0]]) && grid[y+around[i][1]][x+around[i][0]] != 0) {
                return true;
            }
        }
    }

    return false;
}

void sanitize(char* line, size_t size) {

    for (int i = 0; i < size; i++) {
        if (line[i] == '.' || line[i] == '\n') {
            line[i] = 0;
            putchar(' ');
        }
        else putchar(line[i]);
    }
    putchar('\n');

}

int get_sum_line(char* line, size_t size, int line_i, int line_n) {
    int sum = 0;

    bool in_num = false;
    int num_i = 0;
    for (int i =0; i < size; i++) {
        if (isdigit(line[i]) && !in_num) {
            in_num = true;
            num_i = i;
        }

        if (isdigit(line[i])) {
            //printf("%s\n", &line[i]);
            if (look_around(i, line_i, size, line_n)) {
                //printf("CHAR %d IN LINE %d IS DIGIT\n", i, line_i);
                int value = atoi(&line[num_i]);
                printf("valid %d in line %d", value, line_i);
                //for (int j = 0; j < size; j++) if (line[j] == 0) putchar(' '); else putchar(line[j]);
                putchar('\n');
                sum += value;
                while (isdigit(line[i])) i++;
                i--;
            }
        }
        else {
            in_num = false;
        }
    }

    return sum;
}

int main() {

    int line_n = 0;

    FILE* input = fopen("../input.txt", "r");

    char c = 0;
    while (!feof(input)) {
        c = fgetc(input);
        if (c == '\n') line_n++;
    }
    fseek(input, 0, SEEK_SET);

    grid = malloc(sizeof(char*)*line_n);
    sizes = malloc(sizeof(size_t)*line_n);

    for (int i = 0; i < line_n; i++) {
        grid[i] = NULL;
        sizes[i] = 0;
        sizes[i] = getline(&grid[i], &sizes[i], input);
    }

    for (int i = 0; i < line_n; i++) {
        sanitize(grid[i], sizes[i]);
    }

    int sum = 0;
    for (int i = 0; i < line_n; i++) {
        sum += get_sum_line(grid[i], sizes[i], i, line_n);
    }

    printf("sum %d\n", sum);

    fclose(input);
    return 0;
}
