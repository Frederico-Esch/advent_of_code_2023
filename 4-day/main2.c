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


    int cards[256] = { 0 };
    for (int i = 0; i < 256; i++) cards[i] = 1;
    int c_card;
    for(c_card = 0;; c_card++) {
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

        int match = 0;
        printf("%d of CARD %d\n", cards[c_card], c_card);
        for (int i = 0; i < card.biggest_card; i++) {
            if (card.my_nums[i] && card.win_nums[i]) {
                cards[c_card + (++match)] += cards[c_card];
                printf("%d de %d\n", cards[c_card], c_card+match);
            }
        }

        free(line);
    }

    int total = 0;
    for (int i = 0; i < c_card; i++) total += cards[i];
    printf("%d\n", total);

    fclose(input);
    return 0;
}
