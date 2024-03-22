// Group 14 Info 
// Cuellar Sanchez Edgardo
// Faek Ilias
// Messast Haïtham
// Nehili Adel

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


void threshold_c(unsigned char* image, unsigned char* modified, unsigned char threshold, long size) {

	for (long i = 0; i < size; i++) {

		if (image[i] < threshold) {
			modified[i] = 0;
		} else {
			modified[i] = 255;
		}	 			
	}
}


void threshold_smid(unsigned char* image, unsigned char* modified, unsigned char threshold, int size) {

	unsigned char* pos  = (unsigned char *) (modified);
	unsigned char* mask = (unsigned char *)malloc(16*sizeof(unsigned char));
	size_t counter      = size;

	if (mask == NULL) {
		printf ("Out of memory!\n");
		exit (1);
	}
	
	memset(mask, threshold, 16*sizeof(unsigned char));
	memcpy(modified, image, size); 

	__asm__(

        // loading values
		"mov %[pos],     %%rax;" 
        "mov %[mask],    %%rsi;" 
        "mov %[counter], %%rcx;"
        "movdqu (%%rsi), %%xmm0;"

        "loopp:" 
        "movdqu (%%rax), %%xmm7;"

        // determine 0 or 255
        "pminub  %%xmm0, %%xmm7;"  
        "pcmpeqb %%xmm0, %%xmm7;" 
         
        // Move result to memory
        "movdqu  %%xmm7, (%%rax);" 

        "add $16, %%rax;"
        "sub $16, %%rcx;"
        "jnz loopp;"

	 	:                                                              //out
        : [pos] "m"(modified), [mask] "m"(mask), [counter]"m"(counter) //inputs
        : "xmm0", "xmm1", "rcx", "rax"                                 //clobbers
    );
	
	// to avoid memory leaks
	free(mask);
}


void write_out(char *file_name, char *extension, unsigned char *dst, int size) {
    // Extract file name without extension
    char *file_base = strrchr(file_name, '/');
    if (file_base == NULL)
        file_base = file_name;
    else
        file_base++;

    // Generate a unique filename
    char out_file[256];
    snprintf(out_file, sizeof(out_file), "./output/%s_%d%s", file_base, getpid(), extension);

    // Write to the output file
    FILE *fp1 = fopen(out_file, "wb");
    if (fp1 != NULL) {
        fwrite(dst, sizeof(unsigned char), size, fp1);
        fclose(fp1);
    } else {
        printf("Can't open specified file: %s\n", out_file);
        exit(EXIT_FAILURE);
    }
}


long size_of_file(char *file_name) {

	FILE *fp1 = fopen(file_name, "rb");

	if (fp1 == NULL) {
        printf("failed to fopen %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    if (fseek(fp1, 0, SEEK_END) == -1) {
        printf("failed to fseek %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    long size = ftell(fp1);

    if (size == -1) {
        printf("failed to ftell %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    if (fclose(fp1) != 0){
        printf("failed to fclose %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    return size;
}

void run_image(char *file_name){
	long size = size_of_file(file_name);
	int  threshold = 127;

    // allocate memory
	unsigned char* src = (unsigned char *) malloc (size*sizeof(unsigned char));
	unsigned char* dst = (unsigned char *) malloc (size*sizeof(unsigned char));

	// check if enough memory
	if (src == NULL || dst == NULL) {
		printf ("Out of memory!");
		exit (1);
	}
 
    // open file
	FILE *fp1 = fopen(file_name,"rb");

	if (fp1 != NULL) { // read only if file opened
		fread(src, sizeof(unsigned char), size, fp1);
		fclose(fp1);}
	else {
		printf("Can't open specified file!");
		exit(1);
	}

	//clock_t start, end;
   
	// threshold processing in C
	char *extension = "out_C.raw";

	//start = clock();
    threshold_c(src, dst, threshold, size);
    //end = clock();

    //float time = (float)(end - start)/CLOCKS_PER_SEC;
    //printf("Time spent: %f for program in c\n", time);

    write_out(file_name, extension, dst, size);

    // threshold processing in SMId
    extension = "out_SIMD.raw";

    //start = clock();
    threshold_smid(src, dst, threshold, size);
    //end= clock();

    //float time_c = (float)(end - start)/CLOCKS_PER_SEC;
    //printf("Time spent: %f for program in SIMD\n", time_c);

    write_out(file_name, extension, dst, size);

    // to avoid memory leaks
	free(src); 
	free(dst);
}

void run_all_images_from_dir(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("Processing image: %s\n", entry->d_name);
            char file_path[256];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);
            run_image(file_path);
        }
    }

    closedir(dir);
}

int main() {

	clock_t start, end;
    start = clock();

	run_all_images_from_dir("./input");	

	end = clock();
    float time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("All time spent: %f\n", time);

	return EXIT_SUCCESS;
}	