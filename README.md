<p align="center">
    <img src="assets/logo.png" width="400px" style="margin-bottom: 5px;" />
</p>

<p align="center">
    <img src="https://github.com/DieTime/static-json-builder/actions/workflows/linux.yml/badge.svg" alt="Health Check Linux" />
    <img src="https://github.com/DieTime/static-json-builder/actions/workflows/macos.yml/badge.svg" alt="Health Check MacOS" />
    <img src="https://github.com/DieTime/static-json-builder/actions/workflows/windows-mingw.yml/badge.svg" alt="Health Check Windows MinGW" />
    <img src="https://github.com/DieTime/static-json-builder/actions/workflows/windows-msvc.yml/badge.svg" alt="Health Check Windows MSVC" />
    <img src="https://img.shields.io/github/license/DieTime/c-flags?color=%34D058" alt="License type: MIT" />
    <img src="https://img.shields.io/badge/supports-single%20header-green?color=%34D058" alt="Single Header Support" />
</p>

Library for convenient initialization of JSON objects without memory allocations.

# 👨‍💻 Usage

You can see examples sources [here](examples).

```c
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
```

# 🔨 Building, testing and installing

```bash
$ meson build -Dbuildtype=release -Dtests=true
$ cd build
$ meson test
$ meson compile
$ sudo meson install
```

# 🔌 Linking

The library supports pkg-config, which makes linking easier and more convenient.

>👍 [Single header](single-header/c-flags.h) version is also supported!

```cmake
cmake_minimum_required(VERSION 3.14)
project(program)

find_package(PkgConfig)
pkg_check_modules(StaticJsonBuilder REQUIRED IMPORTED_TARGET static-json-builder)

add_executable(program example.c)

target_link_libraries(program PUBLIC PkgConfig::StaticJsonBuilder)
```
