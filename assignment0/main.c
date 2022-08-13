#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 2560
#define HEIGHT 2048
// One pixel has 3 color channel, each consisting of 24 bits
#define TOT_SIZE (WIDTH * HEIGHT * 3)

// since I will be looping a lot, im constructing a macro for loops
#define loop(i) for (uint i = 0; i < TOT_SIZE; i++)

void revert_colors(uchar *image) {
  loop(i) { image[i] = 255 - image[i]; }
}

void grayscale(uchar *image) {
  loop(i) { image[i] = (uint)(image[i] / 2); }
}

// Go through image and either turn on or off color
void contrast(uchar *image) {
  // Loop through every pixel
  for (uint i = 0; i < TOT_SIZE; i += 3) {

    // Loop through every color value
    for (uint j = 0; j < 3; j++)
      image[i + j] = 255 - image[i + j] > 127 ? 255 : 0;
  }
}

void scale(uchar *image, uchar *new_image, uint scale_factor) {

  const int row_length = WIDTH * 3 * scale_factor;

  for (int k = 0; k < 3; k++) {
    int pixelValue = image[k];

    new_image[k] = pixelValue;
    new_image[3 + k] = pixelValue;
    new_image[row_length + k] = pixelValue;
    new_image[3 + row_length + k] = pixelValue;
  }

  for (int i = 3; i < TOT_SIZE; i += 3) {
    int rowNumber = i / (WIDTH * 3);
    int rows = rowNumber * row_length;
    int scaledXIndex = i * 2;

    for (int k = 0; k < 3; k++) {
      int pixelValue = image[i + k];

      new_image[scaledXIndex + rows + k] = pixelValue;
      new_image[3 + scaledXIndex + rows + k] = pixelValue;
      new_image[scaledXIndex + rows + row_length + k] = pixelValue;
      new_image[3 + scaledXIndex + rows + row_length + k] = pixelValue;
    }
  }
}

int main() {
  uchar *image = calloc(TOT_SIZE, 1);
  readbmp("before.bmp", image);

  revert_colors(image);
  grayscale(image);

  const uint scale_factor = 3;
  // Scale up equally in both directions
  uchar *new_image = calloc(TOT_SIZE * 2 * scale_factor, 1);

  // Scale up image 3 times
  scale(image, new_image, scale_factor);

  savebmp("after.bmp", new_image, WIDTH * scale_factor, HEIGHT * scale_factor);
  free(image);
  free(new_image);
  return 0;
}
