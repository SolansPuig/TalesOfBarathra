#ifndef MATH_H
#define MATH_H

#include <stdbool.h>

void math_random_seed(void);

int math_random(int min, int max);

int math_random_dice(int rolls, int dice);

int math_random_normal(int min, int max, int n);

int math_random_normal_hi(int min, int max, int n);

int math_random_normal_lo(int min, int max, int n);

bool math_random_prob(int prob);

 #endif
