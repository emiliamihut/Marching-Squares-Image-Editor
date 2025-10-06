# Algorithm Overview

The algorithm uses functions mapped to commands read from standard input. Each function modifies the pixel matrix to achieve specific image processing steps.

## Read Function

- Reads an image in PPM format.
- Checks the correctness of input data and displays an error message if necessary.
- Utilizes dynamically allocated matrices with proper memory removal.
- Each pixel is represented by a structure: red, green, blue, and brightness (arithmetic mean of RGB).
- The `maxvalue` constant is set at 255.

## Write Function

- Outputs the pixel matrix in the required format.

## Resize Function

- Enlarges the image by replicating each pixel.
- Expands the image four times along each axis, making it 16 times bigger in pixel count.

## Grid Function

- Creates a brightness grid by dividing the image into 4x4 cells.
- Includes an extra row and column to assist the marching squares step.
- Each grid cell stores a weighted average of neighbor corner pixels and itself, if present.
- Handles cells on the last row or column using only existing neighbors.
- Binarizes brightness at a threshold of 200 (values become either 1 or 0).

## March Function

- Analyzes the binary grid to detect contours.
- Draws contours by changing pixel values directly in the image matrix.
- Utilizes 16 fixed pattern matrices, each corresponding to one of the possible 4-bit contour configurations.

## Program Exit

- The `EXIT` command frees all dynamically allocated memory related to the image.
- Prints a confirmation message on program exit.
- Error handling or invalid input will update a flag and display a relevant error message.
