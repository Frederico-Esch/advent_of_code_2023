#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define IS_NUM(c) ((c) >= '0' && (c) <= '9')

#define SECOND

char* name_numbers[] = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

size_t translate_number(const char* start, int* number) {
    size_t size = strlen(start);

    for (size_t i = 0; i < size; i++) {

        for (size_t n = 0; n < 10; n++) {
            if ( (size-i) >= strlen(name_numbers[n])) {
                bool found = true;
                for (size_t c = 0; c < strlen(name_numbers[n]); c++) {
                    if (start[i + c] != name_numbers[n][c]) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    *number = (int)n;
                    return i;
                }
            }
        }

    }

    *number = -1;
    return (size_t)(-1);
}

size_t find_first_numeric(const char* line, size_t size, int* number) {

    *number = -1;
    for (size_t i = 0; i < size; i++) {
        if (IS_NUM(line[i])) {
            *number = (int)(line[i] - '0');
            return i;
        }
    }

    return (size_t)-1;
}

size_t find_first_number(const char* line, size_t size, int* number) {

    size_t pos = translate_number(line, number);

    return pos;
}

size_t find_last_numeric(const char* line, size_t size, int* number) {

    size_t pos = (size_t)-1;
    *number = -1;
    for (size_t i = 0; i < size; i++) {
        if (IS_NUM(line[i])) {
            *number = (int)(line[i] - '0');
            pos = i;
        }
    }

    return pos;
}

size_t find_last_number(const char* line, size_t size, int* number) {


    size_t pos = (size_t)-1;
    *number = (int)-1;
    for (size_t i = 0; i < size; i++) {
        size_t _pos; int num;
        _pos = translate_number(line + i, &num);
        if (num > -1) {
            i += strlen(name_numbers[num]) - 2;
            pos = _pos+i;
            *number = num;
        }
        else break;
    }

    return pos;
}


int main() {

#ifdef FIRST

    FILE* input = fopen("./input.txt", "r");
    int result = 0;


    int first = 0; bool found_first = false;
    int second = 0;
    char c = '\0';

    do {
        do {
            c = fgetc(input);
            if (c >= '0' && c <= '9') {
                if (!found_first) {
                    first = c - '0';
                    found_first = true;
                }
                second = c - '0';
            }
        } while (c != '\n' && !feof(input));
        if (feof(input)) break;

        //printf("%d %d\n", first, second);
        found_first = false;
        result += first*10 + second;
    } while (!feof(input));

    printf("%d\n", result);

#else
#ifdef SECOND

    FILE* input = fopen("./input.txt", "r");
    size_t sum;
    int debug = 0;
    do {
        char* line = NULL;
        size_t size = 0;
        size = getline(&line,  &size, input);
        if (feof(input)) { free(line); break; }

        size_t pos[4] = {0};
        int nums[] = {-1, -1, -1, -1};
        pos[0] = find_first_numeric(line, size, &nums[0]);
        pos[1] = find_first_number(line, size, &nums[1]);

        pos[2] = find_last_numeric(line, size, &nums[2]);
        pos[3] = find_last_number(line, size, &nums[3]);

        int first;
        int last;

        if (nums[0] > -1 && nums[1] > -1) {
            if (pos[0] < pos[1]) first = nums[0];
            else first = nums[1];
        }
        else if (nums[0] > -1) first = nums[0];
        else first = nums[1];

        if (nums[2] > -1 && nums[3] > -1) {
            if (pos[2] < pos[3]) last = nums[3];
            else last = nums[2];
        }
        else if (nums[2] > -1) last = nums[2];
        else last = nums[3];

        //puts("");
        //printf("%zu\n", size);
        //printf("NUM: %d POS: %zu NUM: %d POS: %zu\n", nums[0], pos[0], nums[1], pos[1]);
        //printf("NUM: %d POS: %zu NUM: %d POS: %zu\n", nums[2], pos[2], nums[3], pos[3]);
        sum += first*10 + last;
        printf("%d\n", first*10 + last);

        free(line);
        debug++;
    } while(!feof(input));

    fclose(input);
    printf("SUM: %zu\n", sum);

#endif
#endif
    return 0;
}
