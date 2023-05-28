#include <stdio.h>
#include <stdlib.h>

#include "static-json-builder.h"

int main()
{
    Json json = JsonArray(
        JsonObject(
            JsonProp("1", JsonInt(4)),
            JsonProp("2", JsonInt(4)),
            JsonProp("3", JsonArray(
                JsonInt(1),
                JsonNull(),
                JsonBool(false),
                JsonString("string"),
                JsonFloat(3.14),
            )),
        )
    );

    size_t size = json_stingified_size(json);
    char string[size];

    json_stringify_into_buffer(json, string);
    printf("%s\n", string);

    return 0;
}
