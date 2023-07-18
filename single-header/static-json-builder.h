#ifndef STATIC_JSON_BUILDER_H
#define STATIC_JSON_BUILDER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
static inline char *json_stringify(json_value_t *json);

/**
 * Serializes target json into a string and puts the result into a buffer.
 *
 * @param json The target json to be converted into a string
 * @param buffer Buffer where you want to put the string json representation
 * @note You can find out how big the allocated buffer should be with `json_stingified_size(...)` method
 */
static inline void json_stringify_into_buffer(json_value_t *json, char *buffer);

/**
 * Computes the size of the string representation of the json.
 *
 * @param json The target json for which you want to compute the size of the string representation
 * @return the size of the string representation of the target json
 */
static inline size_t json_stingified_size(json_value_t *json);

typedef void (*json_size_compute_func_t) (json_value_t *json, size_t *size);
typedef void (*json_write_func_t)        (json_value_t *json, char  **cursor);

static inline void json_size_compute_func_for_null     (json_value_t *json, size_t *size);
static inline void json_size_compute_func_for_bool     (json_value_t *json, size_t *size);
static inline void json_size_compute_func_for_int      (json_value_t *json, size_t *size);
static inline void json_size_compute_func_for_floating (json_value_t *json, size_t *size);
static inline void json_size_compute_func_for_string   (json_value_t *json, size_t *size);
static inline void json_size_compute_func_for_array    (json_value_t *json, size_t *size);
static inline void json_size_compute_func_for_object   (json_value_t *json, size_t *size);

static const json_size_compute_func_t json_size_compute_func_by_type[JSON_VALUE_TYPE_MAX] = {
    [JSON_VALUE_TYPE_NULL]   = json_size_compute_func_for_null,
    [JSON_VALUE_TYPE_BOOL]   = json_size_compute_func_for_bool,
    [JSON_VALUE_TYPE_INT]    = json_size_compute_func_for_int,
    [JSON_VALUE_TYPE_FLOAT]  = json_size_compute_func_for_floating,
    [JSON_VALUE_TYPE_STRING] = json_size_compute_func_for_string,
    [JSON_VALUE_TYPE_ARRAY]  = json_size_compute_func_for_array,
    [JSON_VALUE_TYPE_OBJECT] = json_size_compute_func_for_object,
};

static inline void json_write_func_for_null     (json_value_t *json, char **cursor);
static inline void json_write_func_for_bool     (json_value_t *json, char **cursor);
static inline void json_write_func_for_int      (json_value_t *json, char **cursor);
static inline void json_write_func_for_floating (json_value_t *json, char **cursor);
static inline void json_write_func_for_string   (json_value_t *json, char **cursor);
static inline void json_write_func_for_array    (json_value_t *json, char **cursor);
static inline void json_write_func_for_object   (json_value_t *json, char **cursor);

static const json_write_func_t json_write_func_by_type[JSON_VALUE_TYPE_MAX] = {
    [JSON_VALUE_TYPE_NULL]   = json_write_func_for_null,
    [JSON_VALUE_TYPE_BOOL]   = json_write_func_for_bool,
    [JSON_VALUE_TYPE_INT]    = json_write_func_for_int,
    [JSON_VALUE_TYPE_FLOAT]  = json_write_func_for_floating,
    [JSON_VALUE_TYPE_STRING] = json_write_func_for_string,
    [JSON_VALUE_TYPE_ARRAY]  = json_write_func_for_array,
    [JSON_VALUE_TYPE_OBJECT] = json_write_func_for_object,
};

static inline size_t json_size_compute(json_value_t *json)
{
    size_t size = 0;
    json_size_compute_func_by_type[json->type](json, &size);
    return size;
}

static inline size_t json_write(json_value_t *json, char *buffer)
{
    char *cursor = buffer;
    json_write_func_by_type[json->type](json, &cursor);
    return (size_t) (cursor - buffer);
}

static inline void json_size_compute_func_for_null(json_value_t *json,  size_t *size)
{
    (void) json;
    *size += strlen("null");
}

static inline void json_size_compute_func_for_bool(json_value_t *json, size_t *size)
{
    *size += json->as.boolean ? strlen("true") : strlen("false");
}

static inline void json_size_compute_func_for_int(json_value_t *json, size_t *size)
{
    *size += snprintf(NULL, 0, "%" PRId64, json->as.integer);
}

static inline void json_size_compute_func_for_floating(json_value_t *json, size_t *size)
{
    *size += snprintf(NULL, 0, "%f", json->as.floating);
}

static inline void json_size_compute_func_for_string(json_value_t *json, size_t *size)
{
    *size += strlen("\"") + strlen(json->as.string) + strlen("\"");
}

static inline void json_size_compute_func_for_array(json_value_t *json, size_t *size)
{
    *size += strlen("[");

    for (size_t i = 0; i < json->as.array->size; i++) {
        if (i != 0) {
            *size += strlen(",");
        }

        *size += json_size_compute(json->as.array->entries[i]);
    }

    *size += strlen("]");
}

static inline void json_size_compute_func_for_object(json_value_t *json, size_t *size)
{
    *size += strlen("{");

    for (size_t i = 0; i < json->as.object->size; i++) {
        json_prop_t *property = json->as.object->props[i];

        if (i != 0) {
            *size += strlen(",");
        }

        *size += strlen("\"") + strlen(property->key) + strlen("\":");
        *size += json_size_compute(property->entry);
    }

    *size += strlen("}");
}

static inline void json_write_func_for_null(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%s", "null");
}

static inline void json_write_func_for_bool(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%s", json->as.boolean ? "true" : "false");
}

static inline void json_write_func_for_int(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%" PRId64, json->as.integer);
}

static inline void json_write_func_for_floating(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%f", json->as.floating);
}

static inline void json_write_func_for_string(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "\"%s\"", json->as.string);
}

static inline void json_write_func_for_array(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, 2, "%c", '[');

    for (size_t i = 0; i < json->as.array->size; i++) {
        json_value_t *entry = json->as.array->entries[i];

        if (i != 0) {
            *cursor += snprintf(*cursor, 2, "%c", ',');
        }

        *cursor += json_write(entry, *cursor);
    }

    *cursor += snprintf(*cursor, 2, "%c", ']');
}

static inline void json_write_func_for_object(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, 2, "%c", '{');

    for (size_t i = 0; i < json->as.object->size; i++) {
        json_prop_t *property = json->as.object->props[i];

        if (i != 0) {
            *cursor += snprintf(*cursor, 2, "%c", ',');
        }

        *cursor += snprintf(*cursor, strlen(property->key) + 4, "\"%s\":", property->key);
        *cursor += json_write(property->entry, *cursor);
    }

    *cursor += snprintf(*cursor, 2, "%c", '}');
}

static inline char *json_stringify(json_value_t *json)
{
    assert(json && "attempt to stringify json but json is a null pointer");

    size_t length = json_size_compute(json);
    char  *buffer = calloc(length + 1, sizeof(char));

    if (buffer == NULL) {
        return NULL;
    }

    size_t written = json_write(json, buffer);
    assert(length == written && "not all data was written to the buffer");

    return buffer;
}

static inline void json_stringify_into_buffer(json_value_t *json, char *buffer)
{
    assert(json   && "attempt to write json into buffer but json is a null pointer");
    assert(buffer && "attempt to write json into buffer but buffer is a null pointer");

    json_write(json, buffer);
}

static inline size_t json_stingified_size(json_value_t *json)
{
    assert(json && "attempt to get the json string size but json is a null pointer");

    return json_size_compute(json) + 1;
}

#endif /* STATIC_JSON_BUILDER_H */
