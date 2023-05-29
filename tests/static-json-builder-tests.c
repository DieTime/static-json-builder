#include <munit.h>
#include <static-json-builder.h>

#define MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data) \
    do {                                             \
        (void) (params);                             \
        (void) (data);                               \
    } while (0)

#define MUNIT_SIMPLE_TEST_CASE(name, function) \
    { (char*) (name), (function), NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }

#define MUNIT_SIMPLE_TEST_SUITE(name, test_cases) \
    { (char*) (name), (test_cases), NULL, 1, MUNIT_SUITE_OPTION_NONE }

static MunitResult json_null(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonNull();
    munit_assert(json->type == JSON_VALUE_TYPE_NULL);

    return MUNIT_OK;
}

static MunitResult json_bool_false(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonBool(false);

    munit_assert(json->type == JSON_VALUE_TYPE_BOOL);
    munit_assert(json->as.boolean == false);

    return MUNIT_OK;
}

static MunitResult json_bool_true(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonBool(true);

    munit_assert(json->type == JSON_VALUE_TYPE_BOOL);
    munit_assert(json->as.boolean == true);

    return MUNIT_OK;
}

static MunitResult json_int_negative(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonInt(-12345678);

    munit_assert(json->type == JSON_VALUE_TYPE_INT);
    munit_assert_int64(json->as.integer, ==, -12345678);

    return MUNIT_OK;
}

static MunitResult json_int_zero(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonInt(0);

    munit_assert(json->type == JSON_VALUE_TYPE_INT);
    munit_assert_int64(json->as.integer, ==, 0);

    return MUNIT_OK;
}

static MunitResult json_int_positive(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonInt(12345678);

    munit_assert(json->type == JSON_VALUE_TYPE_INT);
    munit_assert_int64(json->as.integer, ==, 12345678);

    return MUNIT_OK;
}

static MunitResult json_float_negative(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonFloat(-1234.5678);

    munit_assert(json->type == JSON_VALUE_TYPE_FLOAT);
    munit_assert_float(json->as.floating, ==, -1234.5678);

    return MUNIT_OK;
}

static MunitResult json_float_zero(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonFloat(0);

    munit_assert(json->type == JSON_VALUE_TYPE_FLOAT);
    munit_assert_float(json->as.floating, ==, 0);

    return MUNIT_OK;
}

static MunitResult json_float_positive(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonFloat(1234.5678);

    munit_assert(json->type == JSON_VALUE_TYPE_FLOAT);
    munit_assert_float(json->as.floating, ==, 1234.5678);

    return MUNIT_OK;
}

static MunitResult json_string_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonString("");

    munit_assert(json->type == JSON_VALUE_TYPE_STRING);
    munit_assert_string_equal(json->as.string, "");

    return MUNIT_OK;
}

static MunitResult json_string_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonString("Hello, world!");

    munit_assert(json->type == JSON_VALUE_TYPE_STRING);
    munit_assert_string_equal(json->as.string, "Hello, world!");

    return MUNIT_OK;
}

static MunitResult json_array_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonArray();

    munit_assert(json->type == JSON_VALUE_TYPE_ARRAY);
    munit_assert_size(json->as.array->size, ==, 0);

    return MUNIT_OK;
}

static MunitResult json_array_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonArray(
        JsonNull(),
        JsonInt(1),
        JsonFloat(1.1),
        JsonString("string"),
        JsonArray(),
        JsonObject()
    );

    munit_assert(json->type == JSON_VALUE_TYPE_ARRAY);
    munit_assert_size(json->as.array->size, ==, 6);

    munit_assert(json->as.array->entries[0]->type == JSON_VALUE_TYPE_NULL);

    munit_assert(json->as.array->entries[1]->type == JSON_VALUE_TYPE_INT);
    munit_assert_int64(json->as.array->entries[1]->as.integer, ==, 1);

    munit_assert(json->as.array->entries[2]->type == JSON_VALUE_TYPE_FLOAT);
    munit_assert_float(json->as.array->entries[2]->as.floating, ==, 1.1);

    munit_assert(json->as.array->entries[3]->type == JSON_VALUE_TYPE_STRING);
    munit_assert_string_equal(json->as.array->entries[3]->as.string, "string");

    munit_assert(json->as.array->entries[4]->type == JSON_VALUE_TYPE_ARRAY);
    munit_assert_size(json->as.array->entries[4]->as.array->size, ==, 0);

    munit_assert(json->as.array->entries[5]->type == JSON_VALUE_TYPE_OBJECT);
    munit_assert_size(json->as.array->entries[5]->as.object->size, ==, 0);

    return MUNIT_OK;
}

static MunitResult json_object_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonObject();

    munit_assert(json->type == JSON_VALUE_TYPE_OBJECT);
    munit_assert_size(json->as.object->size, ==, 0);

    return MUNIT_OK;
}

static MunitResult json_object_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonObject(
        JsonProp("Null",   JsonNull()),
        JsonProp("Int",    JsonInt(1)),
        JsonProp("Float",  JsonFloat(1.1)),
        JsonProp("String", JsonString("string")),
        JsonProp("Array",  JsonArray()),
        JsonProp("Object", JsonObject()),
    );

    munit_assert(json->type == JSON_VALUE_TYPE_OBJECT);
    munit_assert_size(json->as.array->size, ==, 6);

    munit_assert_string_equal(json->as.object->props[0]->key, "Null");
    munit_assert(json->as.object->props[0]->entry->type == JSON_VALUE_TYPE_NULL);

    munit_assert_string_equal(json->as.object->props[1]->key, "Int");
    munit_assert(json->as.object->props[1]->entry->type == JSON_VALUE_TYPE_INT);
    munit_assert_int64(json->as.object->props[1]->entry->as.integer, ==, 1);

    munit_assert_string_equal(json->as.object->props[2]->key, "Float");
    munit_assert(json->as.object->props[2]->entry->type == JSON_VALUE_TYPE_FLOAT);
    munit_assert_float(json->as.object->props[2]->entry->as.floating, ==, 1.1);

    munit_assert_string_equal(json->as.object->props[3]->key, "String");
    munit_assert(json->as.object->props[3]->entry->type == JSON_VALUE_TYPE_STRING);
    munit_assert_string_equal(json->as.object->props[3]->entry->as.string, "string");

    munit_assert_string_equal(json->as.object->props[4]->key, "Array");
    munit_assert(json->as.object->props[4]->entry->type == JSON_VALUE_TYPE_ARRAY);
    munit_assert_size(json->as.object->props[4]->entry->as.array->size, ==, 0);

    munit_assert_string_equal(json->as.object->props[5]->key, "Object");
    munit_assert(json->as.object->props[5]->entry->type == JSON_VALUE_TYPE_OBJECT);
    munit_assert_size(json->as.object->props[5]->entry->as.object->size, ==, 0);

    return MUNIT_OK;
}

static MunitResult json_stringify_null(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonNull();
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "null");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_bool_false(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonBool(false);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "false");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_bool_true(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonBool(true);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "true");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_int_negative(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonInt(-12345678);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "-12345678");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_int_zero(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonInt(0);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "0");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_int_positive(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonInt(12345678);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "12345678");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_float_negative(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonFloat(-1234.567800);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "-1234.567800");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_float_zero(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonFloat(0.000000);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "0.000000");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_float_positive(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonFloat(1234.567800);
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "1234.567800");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_string_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonString("");
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "\"\"");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_string_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonString("string");
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "\"string\"");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_array_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonArray();
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "[]");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_array_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonArray(
        JsonNull(),
        JsonInt(1),
        JsonFloat(1.100000),
        JsonString("string"),
        JsonArray(),
        JsonObject()
    );
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "[null,1,1.100000,\"string\",[],{}]");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_object_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonObject();
    char *string = json_stringify(json);

    munit_assert_string_equal(string, "{}");
    free(string);

    return MUNIT_OK;
}

static MunitResult json_stringify_object_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonObject(
        JsonProp("Null",   JsonNull()),
        JsonProp("Int",    JsonInt(1)),
        JsonProp("Float",  JsonFloat(1.100000)),
        JsonProp("String", JsonString("string")),
        JsonProp("Array",  JsonArray()),
        JsonProp("Object", JsonObject()),
    );
    char *string = json_stringify(json);

    munit_assert_string_equal(string,
        "{"
            "\"Null\":null,"
            "\"Int\":1,"
            "\"Float\":1.100000,"
            "\"String\":\"string\","
            "\"Array\":[],"
            "\"Object\":{}"
        "}"
    );
    free(string);

    return MUNIT_OK;
}

/* ---------------------------------- */

static MunitResult json_size_null(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonNull();
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 4 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_bool_false(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonBool(false);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 5 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_bool_true(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonBool(true);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 4 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_int_negative(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonInt(-12345678);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 9 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_int_zero(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonInt(0);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 1 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_int_positive(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonInt(12345678);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 8 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_float_negative(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonFloat(-1234.567800);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 12 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_float_zero(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonFloat(0.000000);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 8 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_float_positive(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json  json   = JsonFloat(1234.567800);
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 11 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_string_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonString("");
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 2 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_string_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonString("string");
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 8 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_array_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonArray();
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 2 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_array_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonArray(
        JsonNull(),
        JsonInt(1),
        JsonFloat(1.100000),
        JsonString("string"),
        JsonArray(),
        JsonObject()
    );

    size_t size = json_stingified_size(json);
    munit_assert_size(size, ==, 32 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_object_empty(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json   json = JsonObject();
    size_t size = json_stingified_size(json);

    munit_assert_size(size, ==, 2 + 1);

    return MUNIT_OK;
}

static MunitResult json_size_object_complete(const MunitParameter params[], void *data)
{
    MUNIT_SUPPRESS_PARAMS_AND_DATA(params, data);

    Json json = JsonObject(
        JsonProp("Null",   JsonNull()),
        JsonProp("Int",    JsonInt(1)),
        JsonProp("Float",  JsonFloat(1.1)),
        JsonProp("String", JsonString("string")),
        JsonProp("Array",  JsonArray()),
        JsonProp("Object", JsonObject()),
    );

    size_t size = json_stingified_size(json);
    munit_assert_size(size, ==, 79 + 1);

    return MUNIT_OK;
}

static MunitTest tests[] = {
    MUNIT_SIMPLE_TEST_CASE("/null",                      json_null                     ),
    MUNIT_SIMPLE_TEST_CASE("/bool/false",                json_bool_false               ),
    MUNIT_SIMPLE_TEST_CASE("/bool/true",                 json_bool_true                ),
    MUNIT_SIMPLE_TEST_CASE("/int/negative",              json_int_negative             ),
    MUNIT_SIMPLE_TEST_CASE("/int/zero",                  json_int_zero                 ),
    MUNIT_SIMPLE_TEST_CASE("/int/positive",              json_int_positive             ),
    MUNIT_SIMPLE_TEST_CASE("/float/negative",            json_float_negative           ),
    MUNIT_SIMPLE_TEST_CASE("/float/zero",                json_float_zero               ),
    MUNIT_SIMPLE_TEST_CASE("/float/positive",            json_float_positive           ),
    MUNIT_SIMPLE_TEST_CASE("/string/empty",              json_string_empty             ),
    MUNIT_SIMPLE_TEST_CASE("/string/complete",           json_string_complete          ),
    MUNIT_SIMPLE_TEST_CASE("/array/empty",               json_array_empty              ),
    MUNIT_SIMPLE_TEST_CASE("/array/complete",            json_array_complete           ),
    MUNIT_SIMPLE_TEST_CASE("/object/empty",              json_object_empty             ),
    MUNIT_SIMPLE_TEST_CASE("/object/complete",           json_object_complete          ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/null",            json_stringify_null           ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/bool/false",      json_stringify_bool_false     ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/bool/true",       json_stringify_bool_true      ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/int/negative",    json_stringify_int_negative   ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/int/zero",        json_stringify_int_zero       ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/int/positive",    json_stringify_int_positive   ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/float/positive",  json_stringify_float_negative ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/float/zero",      json_stringify_float_zero     ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/float/negative",  json_stringify_float_positive ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/string/empty",    json_stringify_string_empty   ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/string/complete", json_stringify_string_complete),
    MUNIT_SIMPLE_TEST_CASE("/stringify/array/empty",     json_stringify_array_empty    ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/array/complete",  json_stringify_array_complete ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/object/empty",    json_stringify_object_empty   ),
    MUNIT_SIMPLE_TEST_CASE("/stringify/object/complete", json_stringify_object_complete),
    MUNIT_SIMPLE_TEST_CASE("/size/null",                 json_size_null                ),
    MUNIT_SIMPLE_TEST_CASE("/size/bool/false",           json_size_bool_false          ),
    MUNIT_SIMPLE_TEST_CASE("/size/bool/true",            json_size_bool_true           ),
    MUNIT_SIMPLE_TEST_CASE("/size/int/negative",         json_size_int_negative        ),
    MUNIT_SIMPLE_TEST_CASE("/size/int/zero",             json_size_int_zero            ),
    MUNIT_SIMPLE_TEST_CASE("/size/int/positive",         json_size_int_positive        ),
    MUNIT_SIMPLE_TEST_CASE("/size/float/positive",       json_size_float_negative      ),
    MUNIT_SIMPLE_TEST_CASE("/size/float/zero",           json_size_float_zero          ),
    MUNIT_SIMPLE_TEST_CASE("/size/float/negative",       json_size_float_positive      ),
    MUNIT_SIMPLE_TEST_CASE("/size/string/empty",         json_size_string_empty        ),
    MUNIT_SIMPLE_TEST_CASE("/size/string/complete",      json_size_string_complete     ),
    MUNIT_SIMPLE_TEST_CASE("/size/array/empty",          json_size_array_empty         ),
    MUNIT_SIMPLE_TEST_CASE("/size/array/complete",       json_size_array_complete      ),
    MUNIT_SIMPLE_TEST_CASE("/size/object/empty",         json_size_object_empty        ),
    MUNIT_SIMPLE_TEST_CASE("/size/object/complete",      json_size_object_complete     ),
    MUNIT_SIMPLE_TEST_CASE(NULL,                         NULL                          ),
};

static const MunitSuite suite = MUNIT_SIMPLE_TEST_SUITE("/json", tests);

int main(int argc, char* argv[])
{
    return munit_suite_main(&suite, (void*) "static-json-builder", argc, argv);
}
