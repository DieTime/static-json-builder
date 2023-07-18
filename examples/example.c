#include <stdlib.h>
#include <stdio.h>
#include <static-json-builder.h>

int main(void)
{
    Json json = JsonObject(                /*  {                  */
        JsonProp("items", JsonArray(       /*     "items": [      */
            JsonNull(),                    /*         null,       */
            JsonBool(true),                /*         true,       */
            JsonInt(1),                    /*         1,          */
            JsonString("hello")            /*         "hello",    */
        ))                                 /*      ],             */
    );                                     /*  },                 */

    char *string = NULL;
    char *buffer = malloc(json_stingified_size(json));

    string = json_stringify(json);
    json_stringify_into_buffer(json, buffer);

    printf("%s\n", string);  /*  {"items":[null,true,1,"hello"]}  */
    printf("%s\n", buffer);  /*  {"items":[null,true,1,"hello"]}  */

    free(string);
    free(buffer);

    return 0;
}
