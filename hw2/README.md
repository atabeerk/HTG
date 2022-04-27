# Programming assignment 2

Written in C++11.

3rd party libraries used:

- SDSL: for suffix array construction
- cereal: for serializatoin/deserialization

after executing `buildsa`, two files with same names but different extensions (`.bin` and `.sdsl`) are generated. Path to these files
(without the extensions) should be the `--index` argument of `querysa`.
