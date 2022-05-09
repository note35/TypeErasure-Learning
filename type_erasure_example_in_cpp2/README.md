# Min Stack with Pybind11 object

This [min stack](https://leetcode.com/problems/min-stack/) is an example that aims to explain how to keep the Python object reference count correct in C.

## Problem statement

Min stack is an iconic interview question. It needs to support the following functionality in O(1) time complexity:

1. `void push(int)`: add an item into the stack.
2. `int pop()`: return and remove the last added item from the stack.
3. `int top()`: return the last added item in the stack.
4. `int get_min()`: return the minimum number in the stack.

## API

Instead of storing `int` in the stack, the API is changed to store a pair of `int` and `typename`. To present the complexity of reference count behind Python object (`PyObject* -> C++ object -> PyObject`) in C++, the `typename` is `PyObject*`. 

1. `void push(pair<int, PyObject*>)`: add a key-value pair into the stack.
2. `PyObject* pop()`: return the value and remove the last added pair from the stack.
3. `PyObject* top()`: return the last added pair's value in the stack.
4. `PyObject* get_min()`: return the last added value with the minimum key in the stack.

While this spec is implement-able with the code in pure C/C-API, to make the code simple, it depends on a popular library **pybind11** to wrap the Python class from C++ code.

## Environment Setup

```
python3 -m venv py3
./py3/bin/pip3 install pybind11[global]
```

## Write and Compile C++ source code to .so

```
c++ -O3 -Wall -shared -std=c++17 -fPIC  $(./py3/bin/python3 -m pybind11 --includes) -Wl,-undefined,dynamic_lookup min_stack.cpp -o min_stack.so
```

Note that `-Wl,-undefined,dynamic_lookup` is mandatory to solve the **ld error** issue, see [github issue](https://github.com/pybind/pybind11/issues/382#issuecomment-244600572) for the details.


## Import in Python

```
./py3/bin/python3

>>> import min_stack
>>> s = min_stack.MinStack()
```

## Reference

- https://pybind11.readthedocs.io/en/latest/basics.html
