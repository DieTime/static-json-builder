#include <stdlib.h>
#include <stdio.h>
#include <static-json-builder.h>

int main(void)
{
    Json json = JsonObject(                /*  {                                 */
        JsonProp("items", JsonArray(       /*     "items": [                     */
            JsonNull(),                    /*         null,                      */
            JsonBool(true),                /*         true,                      */
            JsonInt(1),                    /*         1,                         */
            JsonString("hello")            /*         "hello",                   */
        ))                                 /*      ],                            */
    );                                     /*  },                                */

    char *string = json_stringify(json);   /*  {"items":[null,true,1,"hello"]}  */
    printf("%s\n", string);

    free(string);
    return 0;
}
