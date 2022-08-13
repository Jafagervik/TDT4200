#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

void savebmp(char *name, uchar *buffer, int x, int y) {
  FILE *f = fopen(name, "wb");

  if (!f) {
    printf("Error writing image to disk");
    return;
  }

  uint size = x * y * 3 + BUFSIZE;
  uchar header[BUFSIZE] = {'B',
                           'M',
                           size & 255,
                           (size >> 8) & 255,
                           (size >> 16) & 255,
                           size >> 24,
                           0,
                           0,
                           0,
                           0,
                           BUFSIZE,
                           0,
                           0,
                           0,
                           40,
                           0,
                           0,
                           0,
                           x & 255,
                           x >> 8,
                           0,
                           0,
                           y & 255,
                           y >> 8,
                           0,
                           0,
                           1,
                           0,
                           24,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0,
                           0};
  fwrite(header, 1, BUFSIZE, f);
  fwrite(buffer, 1, x * y * 3, f);
  fclose(f);
}

void readbmp(char *filename, uchar *array) {
  FILE *img = fopen(filename, "rb");
  uchar header[BUFSIZE];
  fread(header, sizeof(uchar), BUFSIZE, img);

  int width = *(int *)&header[18];
  int height = *(int *)&header[22];
  int padding = 0;

  while ((width * 3 + padding) % 4 != 0)
    padding++;

  int new_width = width * 3 + padding;
  uchar *data = calloc(new_width, sizeof(uchar));

  for (uint i = 0; i < height; i++) {
    fread(data, sizeof(uchar), new_width, img);
    for (int j = 0; j < height; j++) {
      array[3 * i * width + j + 0] = data[j + 0];
      array[3 * i * width + j + 1] = data[j + 1];
      array[3 * i * width + j + 2] = data[j + 2];
    }
  }
  fclose(img);
}
