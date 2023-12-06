#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MIN(x, y) (x) < (y) ? (x) : (y)

struct map_entry_t {
    int64_t begin;
    int64_t end;
    int64_t delta;
};

uint64_t seeds[256] = {0};
size_t seeds_size = 0;

struct map_entry_t maps[256] = {0};
size_t s2s_size = 0;
size_t s2f_size = 0;
size_t f2w_size = 0;
size_t w2l_size = 0;
size_t l2t_size = 0;
size_t t2h_size = 0;
size_t h2l_size = 0;

void print_maps() {
    puts("Seed to soil:");
    for (int i = 0; i < s2s_size; i++) {
        printf("\t%ld %ld %ld\n", maps[i].begin, maps[i].end, maps[i].delta);
    }

    puts("");
    puts("Soil to fertilizer:");
    for (int i = s2s_size; i < s2f_size; i++) {
        printf("\t%ld %ld %ld\n", maps[i].begin, maps[i].end, maps[i].delta);
    }

    puts("");
    puts("Fertilizer to water:");
    for (int i = s2f_size; i < f2w_size; i++) {
        printf("\t%ld %ld %ld\n", maps[i].begin, maps[i].end, maps[i].delta);
    }

    puts("");
    puts("Water to light:");
    for (int i = f2w_size; i < w2l_size; i++) {
        printf("\t%ld %ld %ld\n", maps[i].begin, maps[i].end, maps[i].delta);
    }

    puts("");
    puts("Light to temp:");
    for (int i = w2l_size; i < l2t_size; i++) {
        printf("\t%ld %ld %ld\n", maps[i].begin, maps[i].end, maps[i].delta);
    }

    puts("");
    puts("Temp to humi:");
    for (int i = l2t_size; i < t2h_size; i++) {
        printf("\t%ld %ld %ld\n", maps[i].begin, maps[i].end, maps[i].delta);
    }

    puts("");
    puts("Temp to humi:");
    for (int i = l2t_size; i < t2h_size; i++) {
        printf("\t%ld %ld %ld\n", maps[i].begin, maps[i].end, maps[i].delta);
    }
}

uint64_t map_from_to(size_t map_start, size_t map_size, uint64_t value) {

    for (size_t i = map_start; i < map_size; i++) {
        //printf("map begin: %ld, end: %ld - seed %ld\n", maps[i].begin, maps[i].end, value);
        if (maps[i].begin <= value && maps[i].end > value) {
            value = value + maps[i].delta;
            break;
        }
    }

    return value;
}

void get_seeds(char* line, size_t size) {
    line = strtok(line, ":");
    line = strtok(NULL, ":");

    char* num = strtok(line, " ");
    while (num != NULL) {
        seeds[seeds_size++] = strtoul(num, NULL, 10);

        num = strtok(NULL, " ");
    }
}

struct map_entry_t get_map_entry(char* line, size_t size) {
    struct map_entry_t map_entry = {0};

    char* dst_str = strtok(line, " ");
    char* src_str = strtok(NULL, " ");
    char* rng_str = strtok(NULL, " ");

    uint64_t dst = strtoul(dst_str, NULL, 10);
    uint64_t src = strtoul(src_str, NULL, 10);
    uint64_t range = strtoul(rng_str, NULL, 10);

    map_entry.begin = src;
    map_entry.end = src + range;
    map_entry.delta = dst - src;

    return map_entry;
}

void parse_map(size_t start, size_t* map_size, FILE* input) {
    char* line = NULL; size_t size = 0;
    size = getline(&line, &size, input);
    free(line); line = NULL; size = 0;

    size = getline(&line, &size, input);

    *map_size = start;
    while (size > 1 && !feof(input)) {

        maps[(*map_size)++] = get_map_entry(line, size);

        free(line); line = NULL, size = 0;
        size = getline(&line, &size, input);
    }

    free(line);
}

int main() {

    FILE* input = fopen("../input.txt", "r");

    {
        char* line = NULL; size_t size = 0;
        size = getline(&line, &size, input);
        get_seeds(line, size);
        free(line);
    }

    { char* line = NULL; size_t size = 0; getline(&line, &size, input); free(line); } //empty line

    parse_map(0, &s2s_size, input); //seed to soil

    parse_map(s2s_size, &s2f_size, input);

    parse_map(s2f_size, &f2w_size, input);

    parse_map(f2w_size, &w2l_size, input);

    parse_map(w2l_size, &l2t_size, input);

    parse_map(l2t_size, &t2h_size, input);

    parse_map(t2h_size, &h2l_size, input);

    uint64_t min = UINT64_MAX;
    for (int i = 0; i < seeds_size; i++) {
        uint64_t soil     = map_from_to(0, s2s_size, seeds[i]);
        uint64_t fert     = map_from_to(s2s_size, s2f_size, soil);
        uint64_t water    = map_from_to(s2f_size, f2w_size, fert);
        uint64_t light    = map_from_to(f2w_size, w2l_size, water);
        uint64_t temp     = map_from_to(w2l_size, l2t_size, light);
        uint64_t humi     = map_from_to(l2t_size, t2h_size, temp);
        uint64_t location = map_from_to(t2h_size, h2l_size, humi);
        //printf("seed %ld soil %ld fert %ld water %ld light %ld temp %ld humi %ld location: %ld\n",
        //    seeds[i], soil, fert,
        //    water, light, temp,
        //    humi, location
        //);
        printf("seed %ld -> location: %ld\n", seeds[i], location);
        min = MIN(min, location);
    }

    printf("MIN LOCATION: %ld\n", min);

    fclose(input);

    return 0;
}
