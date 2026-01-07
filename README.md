# Separate chaining (ASCII diagram)

This diagram demonstrates separate chaining for a hash table: an array of buckets where each bucket holds a linked list of entries to handle collisions.

```
Array (size 8)
+----+-------------------------------+
|  0 | NULL                          |
|  1 | [42] -> [90] -> NULL          |
|  2 | NULL                          |
|  3 | [17] -> NULL                  |
|  4 | [33] -> [77] -> [21] -> NULL  |
|  5 | NULL                          |
|  6 | [10] -> NULL                  |
|  7 | NULL                          |
+----+-------------------------------+
```

Explanation
- The hash function maps a key to an index in the array (e.g. `index = key % 8`).
- When multiple keys map to the same index, they are stored in a linked list at that bucket — this is separate chaining.
- Lookups, insertions, and deletions for a particular bucket require traversing that bucket's list; average time depends on load factor (number of elements / number of buckets).

Example
- Insert 33: if `33 % 8 == 1`, place `33` at the head (or tail) of bucket 1's list: bucket[1] becomes `[33] -> [42] -> [90] -> NULL` (depending on insertion policy).

Tips
- Keep load factor low (resize the array when necessary) to maintain good performance.
- Alternatively, use open addressing which stores all entries in the array itself rather than lists.
