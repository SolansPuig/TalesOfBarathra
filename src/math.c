#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "math.h"

void math_random_seed(void) {
    srand(time(NULL));
}

int math_random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int math_random_dice(int rolls, int dice) {
    int sum = 0;
    while(rolls-- > 0) {
        sum += math_random(1, dice);
    }

    return sum;
}

int math_random_normal(int min, int max, int n) {
    int sum = 0;
    int m = n;
    while(m-- > 0) {
        sum += math_random(min, max);
    }
    return sum/n;
}

int math_random_normal_hi(int min, int max, int n) {
    int r;
    do {
        r = math_random_normal(min, max*2, n);
    } while (r < max);

    return r - max;
}

int math_random_normal_lo(int min, int max, int n) {
    int r = 0;
    do {
        r = math_random_normal(min, max*2, n);
    } while (r > max);

    return r;
}

bool math_random_prob(int prob) {
    int r = math_random(0, 100);
    if (r < prob) return true;
    else return false;
}
