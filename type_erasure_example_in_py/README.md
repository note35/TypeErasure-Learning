# Type erasure string matcher

This string matcher is an example that aims to explain how to erase type in
Python but keep the type in C.

## Problem statement

1.  You need to develop a string matcher to compare if a string matches to
    another string or not with different matching business magic.
2.  Your code is implemented in C module with Python interface for the client.
3.  You need to hide the complicated business logic and unnecessary state in
    Python.

## API

To solve above problems, this example implements with following APIs:

-   `def get_matcher_exactly() -> object`
    -   Gets the C-implemented exactly matcher
-   `def get_matcher_partially() -> object`
    -   Gets the C-implemented partially matcher
-   `def is_matcher(matcher: object) -> bool`
    -   Checks if the object is C-implemented matcher
-   `def match(s1: str, s2: str, matcher: object) -> bool`
    -   Checks if the given 2 string (s1, s2) matches based on the given
        matcher's match logic in C

While C keeps the type of the `matcher`, it can be erased in Python by
[Capsule](https://docs.python.org/3/c-api/capsule.html) because `matcher` is
unused in Python.

## Steps to play with the example

1.  Setup a virtual environment

```
python3 -m venv py3
```

1.  Build the CAPI extension and test the result.

*(This step is covered by `compile_and_test.sh`)*

```
./py3/bin/python3 setup.py build_ext --inplace
./py3/bin/python3 -m unittest matcher_test.py
```

1.  Run Python with the CAPI module

```
./py3/bin/python3

>>> import type_erasure_matcher as matcher
```
