#ifndef STATIC_JSON_BUILDER_H
#define STATIC_JSON_BUILDER_H

#if defined(STATIC_JSON_BUILDER_USE_SHARED_LIBRARY)
    #if defined(STATIC_JSON_BUILDER_BUILD_SHARED_LIBRARY)
        #if defined(_MSC_VER)
            #define STATIC_JSON_BUILDER_EXPORT extern __declspec(dllexport)
        #elif defined(__GNUC__)
            #define STATIC_JSON_BUILDER_EXPORT __attribute__((visibility("default")))
        #else
            #define STATIC_JSON_BUILDER_EXPORT
        #endif
    #else
        #if defined(_MSC_VER)
            #define STATIC_JSON_BUILDER_EXPORT extern __declspec(dllimport)
        #else
            #define STATIC_JSON_BUILDER_EXPORT
        #endif
    #endif
#else
    #define STATIC_JSON_BUILDER_EXPORT
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct json_prop_t;
struct json_object_t;
struct json_array_t;
struct json_value_t;

typedef struct json_prop_t   json_prop_t;
typedef struct json_object_t json_object_t;
typedef struct json_array_t  json_array_t;
typedef struct json_value_t  json_value_t;
typedef        json_value_t* Json;

typedef enum json_value_type_t
{
    JSON_VALUE_TYPE_NULL = 0,
    JSON_VALUE_TYPE_BOOL,
    JSON_VALUE_TYPE_INT,
    JSON_VALUE_TYPE_FLOAT,
    JSON_VALUE_TYPE_STRING,
    JSON_VALUE_TYPE_ARRAY,
    JSON_VALUE_TYPE_OBJECT,
    JSON_VALUE_TYPE_MAX,
} json_value_type_t;

struct json_object_t
{
    size_t        size;
    json_prop_t **props;
};

struct json_array_t
{
    size_t         size;
    json_value_t **entries;
};

struct json_value_t
{
    json_value_type_t type;
    union
    {
        bool           boolean;
        int64_t        integer;
        double         floating;
        const char    *string;
        json_object_t *object;
        json_array_t  *array;
    } as;
};

struct json_prop_t
{
    const char   *key;
    json_value_t *entry;
};

#define JsonNull() (                  \
    &(json_value_t) {                 \
        .type = JSON_VALUE_TYPE_NULL, \
    }                                 \
)

#define JsonBool(b) (                 \
    &(json_value_t) {                 \
        .type = JSON_VALUE_TYPE_BOOL, \
        .as.boolean = (b),            \
    }                                 \
)

#define JsonInt(i) (                  \
    &(json_value_t) {                 \
        .type = JSON_VALUE_TYPE_INT,  \
        .as.integer = (i),            \
    }                                 \
)

#define JsonFloat(f) (                 \
    &(json_value_t) {                  \
        .type = JSON_VALUE_TYPE_FLOAT, \
        .as.floating = (f),            \
    }                                  \
)

#define JsonString(s) (                  \
    &(json_value_t) {                    \
        .type = JSON_VALUE_TYPE_STRING,  \
        .as.string = (const char *) (s), \
    }                                    \
)

#define JsonProp(k,e) (            \
    &(json_prop_t) {               \
        .key = (const char *) (k), \
        .entry = (e),              \
    }                              \
)

/*
 Use __VA_ARGS__ length hack to avoid pedantic error
    - warning: ISO C forbids empty initializer braces
*/

#define TYPED_VA_ARGS_LENGTH(T, ...) (             \
    sizeof((T[]){ 0, __VA_ARGS__ }) / sizeof(T) - 1 \
)

#define JsonArray(...) (                                                                     \
    &(json_value_t) {                                                                        \
        .type = JSON_VALUE_TYPE_ARRAY,                                                       \
        .as.array = &(json_array_t) {                                                        \
            .size    = TYPED_VA_ARGS_LENGTH(json_value_t *, __VA_ARGS__),                    \
            .entries = TYPED_VA_ARGS_LENGTH(json_value_t *, __VA_ARGS__)                     \
                            ? (json_value_t **) (json_value_t *[]) { NULL, __VA_ARGS__ } + 1 \
                            : (NULL),                                                        \
        }                                                                                    \
    }                                                                                        \
)

#define JsonObject(...) (                                                                  \
    &(json_value_t) {                                                                      \
        .type = JSON_VALUE_TYPE_OBJECT,                                                    \
        .as.object = &(json_object_t) {                                                    \
            .size  = TYPED_VA_ARGS_LENGTH(json_prop_t *, __VA_ARGS__),                     \
            .props = TYPED_VA_ARGS_LENGTH(json_prop_t *, __VA_ARGS__)                      \
                            ? (json_prop_t **) (json_prop_t *[]) { NULL, __VA_ARGS__ } + 1 \
                            : (NULL),                                                      \
        }                                                                                  \
    }                                                                                      \
)

/**
 * Serializes target json into a string.
 *
 * @param json The target json to be converted into a string
 * @return String representation of the target json or NULL on string allocation error
 * @note You need to release the string allocated by this method
 */
STATIC_JSON_BUILDER_EXPORT
char *json_stringify(json_value_t *json);

/**
 * Serializes target json into a string and puts the result into a buffer.
 *
 * @param json The target json to be converted into a string
 * @param buffer Buffer where you want to put the string json representation
 * @note You can find out how big the allocated buffer should be with `json_stingified_size(...)` method
 */
STATIC_JSON_BUILDER_EXPORT
void json_stringify_into_buffer(json_value_t *json, char *buffer);

/**
 * Computes the size of the string representation of the json.
 *
 * @param json The target json for which you want to compute the size of the string representation
 * @return the size of the string representation of the target json
 */
STATIC_JSON_BUILDER_EXPORT
size_t json_stingified_size(json_value_t *json);

#endif /* STATIC_JSON_BUILDER_H */
