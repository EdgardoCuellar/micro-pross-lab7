#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define THRESHOLD 1 // Define the threshold value for pixel conversion.

// Function prototypes.
void transform_img(const char *dir_path);
void process_image(const char *image_path);

// Main function: the entry point of the program.
int main() {
    clock_t start = clock(); // Record the start time of the program.

    const char *dir_path = "./input"; // Define the directory path of input images.
    transform_img(dir_path); // Process the images in the specified directory.

    clock_t end = clock(); // Record the end time of the program.
    // Calculate and print the total time spent in processing.
    printf("Time spent: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    return EXIT_SUCCESS; // Indicate successful execution.
}

// Processes all the images found in the given directory.
void transform_img(const char *dir_path) {
    DIR *dir = opendir(dir_path); // Attempt to open the specified directory.
    if (!dir) { // Check if directory opening failed.
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    printf("Processing images in %s\n", dir_path);
    struct dirent *entry;

    // Iterate over each entry in the directory.
    while ((entry = readdir(dir)) != NULL) {
        // Process only regular files, ignoring directories.
        if (entry->d_type == DT_REG) {
            char image_path[256];
            // Construct the full path for each image file.
            snprintf(image_path, sizeof(image_path), "%s/%s", dir_path, entry->d_name);
            process_image(image_path); // Process the image.
        }
    }

    closedir(dir); // Close the directory once done.
}

// Processes a single image: reads it, applies thresholding, and writes the result.
void process_image(const char *image_path) {
    FILE *input_file = fopen(image_path, "rb"); // Open the input file for reading in binary mode.
    if (!input_file) { // Check if the file opening failed.
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Prepare the output file name by removing the extension and appending '_out_C.raw'.
    char base_name[256];
    strcpy(base_name, strrchr(image_path, '/') + 1); // Extract the file name from the path.
    char *dot = strrchr(base_name, '.');
    if (dot) *dot = '\0'; // Remove the extension, if present.

    char output_path[256];
    // Create the output file path using the base name.
    snprintf(output_path, sizeof(output_path), "./output/%s_out_C.raw", base_name);

    FILE *output_file = fopen(output_path, "wb"); // Open the output file for writing in binary mode.
    if (!output_file) { // Check if the file opening failed.
        perror("Error opening output file");
        fclose(input_file); // Close the input file before exiting.
        exit(EXIT_FAILURE);
    }

    clock_t start_time = clock(); // Start the timer for pixel processing.

    int pixel_value;
    // Read each pixel from the input file and write the processed pixel to the output file.
    while (fread(&pixel_value, sizeof(pixel_value), 1, input_file) == 1) {
        // Apply thresholding to the pixel value.
        pixel_value = (pixel_value < THRESHOLD) ? 0 : 255;

        if (fwrite(&pixel_value, sizeof(pixel_value), 1, output_file) != 1) {
            perror("Error writing to output file");
            fclose(input_file);
            fclose(output_file);
            exit(EXIT_FAILURE);
        }
    }

    clock_t end_time = clock(); // Stop the timer after processing all pixels.
    // Calculate and print the time taken for processing the pixels of this image.
    printf("Time for processing pixels of %s: %f seconds\n", image_path, (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Check for errors in reading from the input file.
    if (ferror(input_file)) {
        perror("Error reading from input file");
        fclose(input_file);
        fclose(output_file);
        exit(EXIT_FAILURE);
    }

    // Close both input and output files.
    fclose(input_file);
    fclose(output_file);
}
