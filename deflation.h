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

void embed_asset_folder(const char* folder, const char* output_file);

#ifdef DEFLATION_IMPLEMENTATION

# define INFO(...) \
  printf(__VA_ARGS__); \
  printf("\n")

# ifdef DEFLATION_VERBOSE
#  define DEBUG(...) \
   printf(__VA_ARGS__); \
   printf("\n");
# else
#  define DEBUG(...)
# endif

static const char** supported_formats
    = { ".png", ".mpg", ".comps", ".obj", ".mtl", ".wav", ".mp3", ".ogg", ".pal", ".fs", ".vs", ".wav", ".ttf", ".mpeg" };

void deflate_folder(const char* folder, const char* output_file)
{
    DEBUG("Embedded folder %s to %s", folder, output_file);
}

#endif

#endif
