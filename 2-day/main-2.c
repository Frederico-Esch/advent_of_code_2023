#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (a) > (b) ? (a) : (b)

struct Set_t {
    int r;
    int g;
    int b;
};

struct Game_t {
    int id;
    struct Set_t sets[1024];
    int size;
};

void tokenize_set(char* set_str, struct Set_t* set) {
    char* numstr = strtok(set_str, " ");
    while (numstr) {
        int num = atoi(numstr);
        char* color = strtok(NULL, ",");
        //printf("COLOR: %s = %d\n", color, num);
        if (!strcmp("blue", color)) {
            set->b = num;
        }
        else if (!strcmp("red", color)) {
            set->r = num;
        }
        else {
            set->g = num;
        }
        numstr = strtok(NULL, " ");
    };
}

struct Set_t find_minimum_set(struct Game_t game) {

    struct Set_t minimum = {0};

    for (int i = 0; i < game.size; i++) {
        minimum.r = MAX(game.sets[i].r, minimum.r);
        minimum.g = MAX(game.sets[i].g, minimum.g);
        minimum.b = MAX(game.sets[i].b, minimum.b);
    }

    return minimum;
}

int validate_game(struct Game_t game) {

    for (int i = 0; i < game.size; i++) {
        if(game.sets[i].r > 12 || game.sets[i].g > 13 || game.sets[i].b > 14) {
            return 0;
        }
    }

    return 1;
}

int main() {

    FILE* input = fopen("../input.txt", "r");

    char* line = NULL;
    int sum = 0;
    do {
        size_t size = 0;
        line = NULL;
        size = getline(&line, &size, input);
        if (feof(input)) break;

        struct Game_t game = {0};
        sscanf(line, "Game %d", &game.id);
        char* content = strstr(line, ": ") + 2;
        printf("CONTENT %s", content);

        //char* first_set = strtok(content, ";");
        //char* second_set = strtok(NULL, ";") + 1;
        //char* third_set = strtok(NULL, ";") + 1;

        ////printf("1st set: %s\n", first_set);
        ////printf("2nd set: %s\n", second_set);
        ////printf("3rd set: %s\n", third_set);

        //tokenize_set(first_set, &game.sets[0]);
        //tokenize_set(second_set, &game.sets[1]);
        //tokenize_set(third_set, &game.sets[2]);

        char* set_str[1024] = {0};
        set_str[0] = strtok(content, "\n;");
        int index = 0;
        while (set_str[index]) {
            printf("%s\n", set_str[index]);
            index++;
            set_str[index] = strtok(NULL, "\n;");
        }
        game.size = index;

        for (int i = 0; i < game.size; i++) {
            tokenize_set(set_str[i], &game.sets[i]);
        }

        printf("GAME %d:\n", game.id);
        for (int i = 0; i < game.size; i++) {
            printf(" R: %d G: %d B: %d\n", game.sets[i].r, game.sets[i].g, game.sets[i].b);
        }

        struct Set_t minimum = find_minimum_set(game);
        int power_of_set = minimum.r * minimum.g * minimum.b;

        sum += power_of_set;

        puts("");

        free(line);
    } while (1);

    printf("SUM %d\n", sum);
    fclose(input);

    return 0;
}
