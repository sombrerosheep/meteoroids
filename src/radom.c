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
  int i, d;
  float rem;
  float num;

  i = random_get_between((int)min, (int)max);
  d = random_get_between(0, 9999);

  rem = (float)d * 0.0001f;
  num = (float)i + rem;

  return num;
}