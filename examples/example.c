#include <stdlib.h>
#include <stdio.h>
#include <static-json-builder.h>

int main(void)
{
    Json json = JsonObject(                /*     {                                               */
        JsonProp("entries", JsonArray(     /*        "entries": [                                 */
            JsonNull(),                    /*            null,                                    */
            JsonBool(true),                /*            true,                                    */
            JsonInt(1),                    /*            1,                                       */
            JsonFloat(1.1),                /*            1.000000,                                */
            JsonString("string")           /*            "string",                                */
        ))                                 /*         ],                                          */
    );                                     /*     },                                              */

    char *string = json_stringify(json);   /*     {"entries":[null,true,1,1.000000,"string"]}     */
    printf("%s\n", string);

    free(string);
    return 0;
}
