# hash.h
WORK IN PROGRESS!
## TL;DR
Header Only hash table library written in C, using separate chaining, built for pedagogical purposes.
## Philosophy
This library is fragment of a broader pedagogical agenda I currently hold, where I seek to build a large project (e.g. an ML framework) using only libc and tooling that I myself create on top of it. For this reason, in any sub project, like this hash-tables library, or my recent [unit testing framework](https://github.com/SorerBOT/cunit.h), missing features, or non-optimal algorithms only problematic once they become the bottleneck of the mother-project. My aspiration, is that by doing so I will learn more about important algorithms and use naïve solutions to problems that are not focal to my scope.

## Top Features
* Header Only: file is single and ready to mingle! 😉

## How does it work?
* Collisions are handled via separate chaining:
```text
+--------+
 Index | Buckets|
       +--------+
   0   |  NULL  |
       +--------+      +-------+      +-------+
   1   |    o--------->| "Key" |----->| "Key" |----> NULL
       +--------+      |  Val  |      |  Val  |
   2   |  NULL  |      +-------+      +-------+                                   // ASCII art created by Gemini
       +--------+
   3   |    o--------->+-------+
       +--------+      | "Key" |----> NULL
   4   |  NULL  |      |  Val  |
       +--------+      +-------+
```
* Values are stored as `void` pointers.

## Usage Example
```c
#define HASH_IMPLEMENTATION
#include "../src/hash.h"

int main()
{
    int my_favorite_number = 7;
    const int* value = NULL;

    hash_table_t* table = hash_init();
    hash_set(table, "My Favorite Number", &my_favorite_number);
    value = hash_get(table, "My Favorite Number");
    printf("My favorite number is: %d\n", *value); // My favorite number is: 7

    return 0;
}

```
