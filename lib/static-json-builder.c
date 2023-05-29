#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "static-json-builder.h"

typedef void (*json_size_compute_func_t) (json_value_t *json, size_t *size);
typedef void (*json_write_func_t)        (json_value_t *json, char  **cursor);

static void json_size_compute_func_for_null     (json_value_t *json, size_t *size);
static void json_size_compute_func_for_bool     (json_value_t *json, size_t *size);
static void json_size_compute_func_for_int      (json_value_t *json, size_t *size);
static void json_size_compute_func_for_floating (json_value_t *json, size_t *size);
static void json_size_compute_func_for_string   (json_value_t *json, size_t *size);
static void json_size_compute_func_for_array    (json_value_t *json, size_t *size);
static void json_size_compute_func_for_object   (json_value_t *json, size_t *size);

static const json_size_compute_func_t json_size_compute_func_by_type[JSON_VALUE_TYPE_MAX] = {
    [JSON_VALUE_TYPE_NULL]   = json_size_compute_func_for_null,
    [JSON_VALUE_TYPE_BOOL]   = json_size_compute_func_for_bool,
    [JSON_VALUE_TYPE_INT]    = json_size_compute_func_for_int,
    [JSON_VALUE_TYPE_FLOAT]  = json_size_compute_func_for_floating,
    [JSON_VALUE_TYPE_STRING] = json_size_compute_func_for_string,
    [JSON_VALUE_TYPE_ARRAY]  = json_size_compute_func_for_array,
    [JSON_VALUE_TYPE_OBJECT] = json_size_compute_func_for_object,
};

static void json_write_func_for_null     (json_value_t *json, char **cursor);
static void json_write_func_for_bool     (json_value_t *json, char **cursor);
static void json_write_func_for_int      (json_value_t *json, char **cursor);
static void json_write_func_for_floating (json_value_t *json, char **cursor);
static void json_write_func_for_string   (json_value_t *json, char **cursor);
static void json_write_func_for_array    (json_value_t *json, char **cursor);
static void json_write_func_for_object   (json_value_t *json, char **cursor);

static const json_write_func_t json_write_func_by_type[JSON_VALUE_TYPE_MAX] = {
    [JSON_VALUE_TYPE_NULL]   = json_write_func_for_null,
    [JSON_VALUE_TYPE_BOOL]   = json_write_func_for_bool,
    [JSON_VALUE_TYPE_INT]    = json_write_func_for_int,
    [JSON_VALUE_TYPE_FLOAT]  = json_write_func_for_floating,
    [JSON_VALUE_TYPE_STRING] = json_write_func_for_string,
    [JSON_VALUE_TYPE_ARRAY]  = json_write_func_for_array,
    [JSON_VALUE_TYPE_OBJECT] = json_write_func_for_object,
};

static size_t json_size_compute(json_value_t *json)
{
    size_t size = 0;
    json_size_compute_func_by_type[json->type](json, &size);
    return size;
}

static size_t json_write(json_value_t *json, char *buffer)
{
    char *cursor = buffer;
    json_write_func_by_type[json->type](json, &cursor);
    return (size_t) (cursor - buffer);
}

static void json_size_compute_func_for_null(json_value_t *json,  size_t *size)
{
    (void) json;
    *size += strlen("null");
}

static void json_size_compute_func_for_bool(json_value_t *json, size_t *size)
{
    *size += json->as.boolean ? strlen("true") : strlen("false");
}

static void json_size_compute_func_for_int(json_value_t *json, size_t *size)
{
    *size += snprintf(NULL, 0, "%" PRId64, json->as.integer);
}

static void json_size_compute_func_for_floating(json_value_t *json, size_t *size)
{
    *size += snprintf(NULL, 0, "%f", json->as.floating);
}

static void json_size_compute_func_for_string(json_value_t *json, size_t *size)
{
    *size += strlen("\"") + strlen(json->as.string) + strlen("\"");
}

static void json_size_compute_func_for_array(json_value_t *json, size_t *size)
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

static void json_size_compute_func_for_object(json_value_t *json, size_t *size)
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

static void json_write_func_for_null(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%s", "null");
}

static void json_write_func_for_bool(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%s", json->as.boolean ? "true" : "false");
}

static void json_write_func_for_int(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%" PRId64, json->as.integer);
}

static void json_write_func_for_floating(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "%f", json->as.floating);
}

static void json_write_func_for_string(json_value_t *json, char **cursor)
{
    *cursor += snprintf(*cursor, json_size_compute(json) + 1, "\"%s\"", json->as.string);
}

static void json_write_func_for_array(json_value_t *json, char **cursor)
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

static void json_write_func_for_object(json_value_t *json, char **cursor)
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

char *json_stringify(json_value_t *json)
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

void json_stringify_into_buffer(json_value_t *json, char *buffer)
{
    assert(json   && "attempt to write json into buffer but json is a null pointer");
    assert(buffer && "attempt to write json into buffer but buffer is a null pointer");

    json_write(json, buffer);
}

size_t json_stingified_size(json_value_t *json)
{
    assert(json && "attempt to get the json string size but json is a null pointer");

    return json_size_compute(json) + 1;
}
