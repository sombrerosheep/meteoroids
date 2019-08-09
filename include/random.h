#ifndef RANDOM_H
#define RANDOM_H

void random_init(int seed);
int random_get();
float random_getf();
int random_get_between(int min, int max);
float random_getf_between(float min, float max);

#endif
