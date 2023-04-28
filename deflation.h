// Valid build parameters
// - DEFLATION_IMPLEMENTATION
// - DEFLATION_VERBOSE

#ifndef DEFLATION_H
#define DEFLATION_H

#include <stdint.h>
#define MAX_PATH_LEN 256

typedef struct RawAsset {
    char path[MAX_PATH_LEN];
    int64_t size;
    char data[];
} RawAsset;

int deflate_folder(const char* folder, const char* output_file);  // NOTE: Deflates the given folder into an output package.
                                                                  // Always recursive for convenience, returns 1 on success

#ifdef DEFLATION_IMPLEMENTATION
# include <stdio.h>
# include <stdbool.h>
#include <sys/stat.h>

# define SUCCESS 1
# define FAILURE 0

# define INFO(...) \
  printf(__VA_ARGS__); \
  printf("\n")

# define FATAL(...) \
  fprintf(stderr, __VA_ARGS__); \
  printf("\n"); \
  exit(EXIT_FAILURE)

# ifdef DEFLATION_VERBOSE
#  define DEBUG(...) \
   printf(__VA_ARGS__); \
   printf("\n");
# else
#  define DEBUG(...)
# endif

static const char* supported_formats[]
    = { ".png", ".mpg", ".comps", ".obj", ".mtl", ".wav", ".mp3", ".ogg", ".pal", ".fs", ".vs", ".wav", ".ttf", ".mpeg" };

static bool folder_exists(const char* folder)
{
    struct stat sb;
    if (stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return true;
    }
    return false;
}

int deflate_folder(const char* folder, const char* output_file)
{
    if (!folder_exists(folder)){
        return FAILURE;
    }

    return SUCCESS;
}

#endif

#endif
