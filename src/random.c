#include <random.h>
#include <stdlib.h>

void random_init(int seed) {
  srand(seed);
}

int random_get() {
  int num;

  num = rand();

  return num;
}

float random_getf() {
  int num;
  float res;

  num = rand();
  res = (float)num / 1000.f;

  return num;
}

int random_get_between(int min, int max) {
  int num;

  num = (random_get() % (max - min + 1)) + min;

  return num;
}

float random_getf_between(float min, float max) {
  float scale, num;

  scale = rand() / (float)RAND_MAX;
  num = min + scale * ( max - min );

  return num;
}