#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX(x, y) (x) > (y) ? (x) : (y)

struct Card {
    int biggest_card;
    bool my_nums[100];
    bool win_nums[100];
};

void get_nums(bool nums[100], char* numstr, int* biggest) {
    char* c_num = strtok(numstr, " ");

    while (c_num != NULL) {
        int num = atoi(c_num);
        nums[num] = true;
        if (biggest) {
            *biggest = MAX(*biggest, num+1);
        }

        c_num = strtok(NULL, " ");
    }
}

int main() {

    FILE* input = fopen("../input.txt", "r");


    int total = 0;
    for(int i = 0;; i++) {
        char* line = NULL;
        size_t size = 0;
        size = getline(&line, &size, input);
        if (size == 0) { free(line); break; }
        if (feof(input)) {free(line); break; }

        char* nums = line;
        for (;*nums != ':'; nums = &nums[1]);
        nums++;

        char* my_nums  = strtok(nums, "|");
        char* win_nums = strtok(NULL, "|");

        struct Card card = {0};
        get_nums(card.my_nums, my_nums, &card.biggest_card);
        get_nums(card.win_nums, win_nums, NULL);

        int result = 0;
        for (int i = 0; i < card.biggest_card; i++) {
            if (card.my_nums[i] && card.win_nums[i]) {
                //printf("%d\n", i);
                result = result == 0 ? 1 : result*2;
            }
        }

        total += result;

        free(line);
    }

    printf("%d\n", total);

    fclose(input);
    return 0;
}
