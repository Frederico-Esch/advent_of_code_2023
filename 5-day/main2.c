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

struct range_t {
    uint64_t begin;
    uint64_t end;
};

struct range_t seeds[1024] = {0};
size_t seeds_size = 0;

struct range_t soils[1024] = {0};
size_t soils_size = 0;

struct range_t ferts[1024] = {0};
size_t ferts_size = 0;

struct range_t waters[1024] = {0};
size_t waters_size = 0;

struct range_t lights[1024] = {0};
size_t lights_size;

struct range_t temps[1024] = {0};
size_t temps_size = 0;

struct range_t humis[1024] = {0};
size_t humis_size = 0;

struct range_t locations[1024] = {0};
size_t locations_size = 0;

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

void map_from_to(size_t map_start, size_t map_size, struct range_t* value, struct range_t* src, size_t* src_size, struct range_t* dest, size_t* dest_size) {

    char found = 0;
    for (size_t i = map_start; i < map_size; i++) {
        //printf("map begin: %ld, end: %ld - seed %ld %ld\n", maps[i].begin, maps[i].end, value->begin, value->end);
        //if (value->end < maps[i].begin || value->begin >= maps[i].end) { //no inter
        //    continue;
        //} else
        if (value->begin >= maps[i].begin && value->end < maps[i].end) { //inside
            dest[(*dest_size)++] = (struct range_t){
                .begin = value->begin + maps[i].delta,
                .end = value->end + maps[i].delta
            };
            found = 1;
            break;
        }
        else if (value->begin < maps[i].begin && value->end >= maps[i].end) { //outside
            src[(*src_size)++] = (struct range_t) { //upper bound
                .begin = value->begin,
                .end = maps[i].begin-1
            };
            src[(*src_size)++] = (struct range_t) { //lower bound
                .begin = maps[i].end,
                .end = value->end
            };

            value->begin = maps[i].begin;
            value->end = maps[i].end -1;

            dest[(*dest_size)++] = (struct range_t) {
                .begin = value->begin + maps[i].delta,
                .end = value->end + maps[i].delta
            };

            found = 1;
            break;
        }
        else if (value->begin < maps[i].begin && value->end >= maps[i].begin) {

            src[(*src_size)++] = (struct range_t) {
                .begin = value->begin,
                .end = maps[i].begin-1
            };

            value->begin = maps[i].begin;

            dest[(*dest_size)++] = (struct range_t) {
                .begin = value->begin + maps[i].delta,
                .end = value->end + maps[i].delta
            };

            found = 1;
            break;
        }
        else if (value->end >= maps[i].end && value->begin < maps[i].end){

            src[(*src_size)++] = (struct range_t) {
                .begin = maps[i].end,
                .end = value->end
            };

            value->end = maps[i].end-1;

            dest[(*dest_size)++] = (struct range_t) {
                .begin = value->begin + maps[i].delta,
                .end = value->end + maps[i].delta
            };

            found = 1;
            break;
        }
    }
    if (!found) {
        dest[(*dest_size)++] = (struct range_t){
            .begin = value->begin,
            .end = value->end
        };
    }
    if ((*dest_size > 1023) || (*src_size > 1023)) {
        puts("BUFFER OVERFLOW");
        exit(0);
    }
}

void get_seeds(char* line, size_t size) {
    line = strtok(line, ":");
    line = strtok(NULL, ":");

    char* num = strtok(line, " ");
    while (num != NULL) {
        if (seeds[seeds_size].begin == 0)
            seeds[seeds_size].begin = strtoul(num, NULL, 10);
        else {
            seeds[seeds_size].end = seeds[seeds_size].begin + strtoul(num, NULL, 10) - 1;
            seeds_size++;
        }

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
        //printf("SEED %ld %ld\n", seeds[i].begin, seeds[i].end);
        map_from_to(0, s2s_size, &seeds[i], seeds, &seeds_size, soils, &soils_size);
    }
    for (int i = 0; i < soils_size; i++) {
        map_from_to(s2s_size, s2f_size, &soils[i], soils, &soils_size, ferts, &ferts_size);
    }
    for (int i = 0; i < ferts_size; i++) {
        map_from_to(s2f_size, f2w_size, &ferts[i], ferts, &ferts_size, waters, &waters_size);
    }

    for (int i = 0; i < waters_size; i++) {
        map_from_to(f2w_size, w2l_size, &waters[i], waters, &waters_size, lights, &lights_size);
    }
    for (int i = 0; i < lights_size; i++) {
        map_from_to(w2l_size, l2t_size, &lights[i], lights, &lights_size, temps, &temps_size);
    }
    for (int i = 0; i < temps_size; i++) {
        map_from_to(l2t_size, t2h_size, &temps[i], temps, &temps_size, humis, &humis_size);
    }
    for (int i = 0; i < humis_size; i++) {
        map_from_to(t2h_size, h2l_size, &humis[i], humis, &humis_size, locations, &locations_size);
    }


    for (int i = 0; i < locations_size; i++) {
        //printf("%ld - %ld\n", locations[i].begin, locations[i].end);
        min = MIN(min, locations[i].begin);
    }
    //find min in locations
    //printf("seed %ld -> location: %ld\n", seeds[i], location);
    //min = MIN(min, location);

    printf("MIN LOCATION: %ld\n", min);

    fclose(input);

    return 0;
}
