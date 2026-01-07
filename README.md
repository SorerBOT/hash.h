# hash.h
WORK IN PROGRESS!
## TL;DR
Header Only hash table library written in C, using separate chaining, built for pedagogical purposes.
## Philosophy
This library is fragment of a broader pedagogical agenda I currently hold, where I seek to build a large project (e.g. an ML framework) using only libc and tooling that I myself create on top of it. For this reason, in any sub project, like this hash-tables library, or my recent [unit testing framework](https://github.com/SorerBOT/cunit.h), missing features, or non-optimal algorithms only problematic once they become the bottleneck of the mother-project. My aspiration, is that by doing so I will learn more about important algorithms and use naïve solutions to problems that are not focal to my scope.

## Top Features
* Header Only: file is single and ready to mingle! 😉

## How does it work?
* Collisions are handlded via separate chaining.
* Values are stored as `void` pointers.

## Usage Example
```c
#define HASH_IMPLEMENTATION
#include ".../hash.h"

int main()
{
    hash_table_t* table = hash_init();
    int my_favorite_number = 7;
    hash_set(table, "My Favorite Number", &x);

    const int* value = hash_get(table, "My Favorite Number");
    printf("My favorite number is: %d\n", *value);
    return 0;
}
```
