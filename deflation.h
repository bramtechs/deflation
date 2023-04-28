// Valid build parameters
// - DEFLATION_IMPLEMENTATION
// - DEFLATION_VERBOSE

#ifndef DEFLATION_H
#define DEFLATION_H

#define DEFLATION_VERBOSE

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
# include <dirent.h>
# include <string.h>
# include <sys/stat.h>

# define SUCCESS 1
# define FAILURE 0

# define INFO(...) \
  printf(__VA_ARGS__); \
  printf("\n")

# define ERR(...) \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n")

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

typedef struct FilePathList {
    char** files;
    size_t count;
    size_t capacity;
} FilePathList;

static const char* supported_formats[]
    = { ".png", ".mpg", ".comps", ".obj", ".mtl", ".wav", ".mp3", ".ogg", ".pal", ".fs", ".vs", ".wav", ".ttf", ".mpeg" };

void filepathlist_append(FilePathList* list, const char* folder)
{
    if (list->files == NULL) {
        list->capacity = 2;
        list->files = malloc(list->capacity);
    }

    if (list->count == list->capacity) {
        list->capacity *= 2;
    }

    list->files[list->count++] = strdup(folder);
}

void filepathlist_free(FilePathList* list)
{
    for (size_t i = 0; i < list->count; i++) {
        free(list->files[i]);
    }
    free(list->files);
    list->files = NULL;
}

void crawl_folder(FilePathList* list, const char* folder)
{
    // crawl through folder
    DIR* dir = opendir(folder);

    if (dir == NULL) {
        return;
    }

    for (;;) {
        struct dirent* entry = readdir(dir);
        if (entry == NULL) {
            break;
        }

        const char* name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        crawl_folder(list, name);
        filepathlist_append(list, name);
    };

    closedir(dir);
}

int deflate_folder(const char* folder, const char* output_file)
{
    FilePathList list = { 0 };
    crawl_folder(&list, folder);

    if (list.count == 0) {
        ERR("Deflation could not open input directory %s.", folder);
        return FAILURE;
    }

    for (size_t i = 0; i < list.count; i++) {
        const char* file = list.files[i];
        INFO(file);
    }

    return SUCCESS;
}

#endif

#endif
