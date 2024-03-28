#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define THRESHOLD 10

void transform_img(const char *dir_path);
void process_image(const char *image_path);

int main() {
    clock_t start, end;
    
    start = clock();

    const char *dir_path = "./input";
    transform_img(dir_path);

    end = clock();
    float time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time);
    
    return EXIT_SUCCESS;
}

void transform_img(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_path)) == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }
    
    printf("Processing images in %s\n", dir_path);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char image_path[256];
            snprintf(image_path, sizeof(image_path), "%s/%s", dir_path, entry->d_name);
            process_image(image_path);
        }
    }

    closedir(dir);
}

void process_image(const char *image_path) {
    FILE *input_file = fopen(image_path, "rb");
    if (input_file == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Calculate output file path
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "./output_SMID/%s", strrchr(image_path, '/') + 1);

    FILE *output_file = fopen(output_path, "wb");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }

    int pixel_value;
    clock_t start_time, end_time;

    start_time = clock();

    while (fread(&pixel_value, sizeof(pixel_value), 1, input_file) == 1) {
        // Apply thresholding
        if (pixel_value < THRESHOLD)
            pixel_value = 0;
        else
            pixel_value = 255;

        // Write the transformed pixel value to output file
        fwrite(&pixel_value, sizeof(pixel_value), 1, output_file);
    }

    end_time = clock();
    float computation_time = (float)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Computing time: %f seconds\n", computation_time);

    fclose(input_file);
    fclose(output_file);
}
