// Valid build parameters
// - DEFLATION_IMPLEMENTATION
// - DEFLATION_VERBOSE

#ifndef DEFLATION_H
#define DEFLATION_H
#define DEFLATION

#define DEFLATION_VERBOSE

#include <stdint.h>
#define MAX_PATH_LEN 256

typedef struct RawAsset {
    char path[MAX_PATH_LEN];
    int64_t size;
    char* data;
} RawAsset;

DEFLATION int deflate_folder(const char* folder, const char* output_file);  // NOTE: Deflates the given folder into an output package.
                                                                            // Always recursive for convenience, returns 1 on success

#ifdef DEFLATION_IMPLEMENTATION
# include <stdio.h>
# include <stdbool.h>
# include <dirent.h>
# include <string.h>
# include <assert.h>
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

# define CACHE_MAX_SIZE (10240)
typedef struct FileBuffer {
    size_t cache_size;
    char* cache;
    FILE* file;
} FileBuffer;

static const char* supported_formats[]
    = { ".png", ".mpg", ".comps", ".obj", ".mtl", ".wav", ".mp3", ".ogg", ".pal", ".fs", ".vs", ".wav", ".ttf", ".mpeg" };

void filepathlist_append(FilePathList* list, const char* folder)
{
    if (list->files == NULL) {
        list->capacity = 500;
        list->files = malloc(list->capacity * sizeof(const char*));
    }

    if (list->count == list->capacity) {
        list->capacity *= 2;
        list->files = realloc(list->files, list->capacity * sizeof(const char*));
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

        // append folder with filename
        char* merged_path = malloc(strlen(folder) + 1 + strlen(name) + 1);
        sprintf(merged_path, "%s/%s", folder, name);

        crawl_folder(list, merged_path);
        filepathlist_append(list, merged_path);
        free(merged_path);
    };

    closedir(dir);
}

static void filebuffer_flush(FileBuffer* buffer)
{
    assert(buffer->cache_size < CACHE_MAX_SIZE);
    fwrite(buffer->cache, buffer->cache_size, 1, buffer->file);
    DEBUG("Flushed filebuffer (%zu bytes)",buffer->cache_size);
    buffer->cache_size = 0;
}

static FileBuffer* filebuffer_open(const char* file)
{
    FileBuffer* buffer = malloc(sizeof(FileBuffer));
    buffer->cache_size = 0;
    buffer->cache = malloc(CACHE_MAX_SIZE);
    buffer->file = fopen(file, "wb");
    return buffer;
}

static void filebuffer_close(FileBuffer* buffer)
{
    if (buffer->cache_size > 0) {
        filebuffer_flush(buffer);
    }
    free(buffer->cache);
    free(buffer);
    buffer = NULL;
}

static void filebuffer_append(FileBuffer* buffer, void* data, size_t length)
{
    if (length >= CACHE_MAX_SIZE) {
        filebuffer_flush(buffer);
        DEBUG("File too big for cache. writing directly...");
        fwrite(data, length, 1, buffer->file);
        return;
    }

    if (buffer->cache_size + length >= CACHE_MAX_SIZE) {
        filebuffer_flush(buffer);
    }

    char* offset = buffer->cache + buffer->cache_size;
    memcpy(offset, data, length);
    buffer->cache_size += length;
}

static RawAsset rawasset_load(const char* name)
{
    RawAsset asset;
    strcpy(asset.path, name); // TODO: unsafe 

    FILE* file = fopen(name,"rb");
    if (file == NULL) {
        FATAL("Failed to read file %s", name);
    }

    fseek(file, 0, SEEK_END);
    asset.size = (int64_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    // load file content into memory
    asset.data = malloc(asset.size);

    int read = fread(asset.data, 1, asset.size, file);
    if ((int64_t) read != asset.size){
        FATAL("Failed to read some parts of file %s (%d/%zu)", name, read, asset.size);
    }

    return asset;
}

static void rawasset_free(RawAsset* asset)
{
    free(asset->data);
    asset->data = NULL;
}

static void rawasset_write(FileBuffer* buffer, RawAsset asset)
{
     filebuffer_append(buffer, (void*) asset.path, MAX_PATH_LEN);
     filebuffer_append(buffer, (void*) &asset.size, sizeof(int64_t));
     filebuffer_append(buffer, (void*) asset.data, asset.size);
}

static void create_pack(FilePathList list, const char* output_file)
{
    FileBuffer* buffer = filebuffer_open(output_file);

    // write header
    int64_t amount = list.count;
    filebuffer_append(buffer, &amount, sizeof(int64_t));

    // write every item
    for (size_t i = 0; i < list.count; i++) {
        const char* file = list.files[i];
        RawAsset asset = rawasset_load(file);
        rawasset_write(buffer, asset);
        DEBUG("Wrote %s into %s", file, output_file);
        rawasset_free(&asset);
    }

    filebuffer_close(buffer);
}

DEFLATION int deflate_folder(const char* folder, const char* output_file)
{
    FilePathList list = { 0 };
    crawl_folder(&list, folder);

    if (list.count == 0) {
        ERR("Deflation could not open input directory %s", folder);
        return FAILURE;
    }

    create_pack(list, output_file);

    return SUCCESS;
}

#endif

#endif
