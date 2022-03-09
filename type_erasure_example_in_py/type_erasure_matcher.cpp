// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>

#include <Python.h>

#define TYPE_ERASURE_MATCHER_MODULE_NAME "type_erasure_matcher"
#define TYPE_ERASURE_MATCHER_MODULE_DOC  "A type erasure matcher implemented in C."

namespace matcher {

// Defines the name and context for the type erased matcher object.
inline const char MATCHER_NAME[] = "::namespace::matcher";
inline void *MATCHER_CONTEXT = malloc(1);

static PyObject *CreateMatcherCapsule(void *vptr) {
    // A helper function to create matcher capsule.
    PyObject *capsule = PyCapsule_New(vptr, MATCHER_NAME, nullptr);
    PyCapsule_SetContext(capsule, MATCHER_CONTEXT);
    return capsule;
}

struct Matcher {
public:
    virtual bool match(std::string a, std::string b) const = 0;
    virtual ~Matcher() = default;
    static const Matcher& EXACTLY;
    static const Matcher& PARTIALLY;
};

// Put header and implementation together in the same file.
// Above part is the header, below part is the implementation.

struct Exactly : public Matcher {
    bool match(std::string a, std::string b) const override {
        return a == b;
    }
};

struct Partially : public Matcher {
    bool match(std::string a, std::string b) const override {
        // Returns true if a contains b or vice versa.
        return a.find(b) != std::string::npos or b.find(a) != std::string::npos;
    }
};

static Exactly ExactlyObject;
const Matcher& Matcher::EXACTLY(ExactlyObject);
static Partially PartiallyObject;
const Matcher& Matcher::PARTIALLY(PartiallyObject);

}  // namespace matcher


static PyObject* get_matcher_exactly(PyObject* self, PyObject *unused) {
    void *vptr = const_cast<void *>(static_cast<const void *>(&(matcher::Matcher::EXACTLY)));
    return matcher::CreateMatcherCapsule(vptr);
}


static PyObject* get_matcher_partially(PyObject* self, PyObject *unused) {
    void *vptr = const_cast<void *>(static_cast<const void *>(&(matcher::Matcher::PARTIALLY)));
    return matcher::CreateMatcherCapsule(vptr);
}


static PyObject* is_matcher(PyObject* self, PyObject *args) {
    PyObject* pymatcher;
    if (!PyArg_ParseTuple(args, "O", &pymatcher)) {
        return NULL;
    }
    return PyCapsule_IsValid(pymatcher, matcher::MATCHER_NAME) != 0 &&
        PyCapsule_GetContext(pymatcher) == matcher::MATCHER_CONTEXT ?
        Py_True : Py_False;
}


static PyObject* match(PyObject* self, PyObject* args) {
    char *a, *b;
    PyObject* pymatcher;
    if (!PyArg_ParseTuple(args, "ssO", &a, &b, &pymatcher)) {
        return NULL;
    }
    void* capsule_payload_matcher = PyCapsule_GetPointer(pymatcher, matcher::MATCHER_NAME);
    matcher::Matcher* matcher = static_cast<matcher::Matcher*>(capsule_payload_matcher);
  
    std::string sa(a);
    std::string sb(b);
    return matcher->match(sa, sb) ? Py_True : Py_False;
}


static PyMethodDef type_erasure_matcher_methods[] = {
    {
        "match", match, METH_VARARGS,
        "Matches two strings with the matcher."
    },
    {
        "get_matcher_exactly", get_matcher_exactly, METH_NOARGS,
        "Gets the EXACTLY matcher."
    },
    {
        "get_matcher_partially", get_matcher_partially, METH_NOARGS,
        "Gets the PARTIALLY matcher."
    },
    {
        "is_matcher", is_matcher, METH_VARARGS,
        "Checks if the matcher is valid."
    },
    { NULL, NULL, 0, NULL}
};


static struct PyModuleDef type_erasure_matcher_def = {
    PyModuleDef_HEAD_INIT,
    TYPE_ERASURE_MATCHER_MODULE_NAME,
    TYPE_ERASURE_MATCHER_MODULE_DOC,
    -1,
    type_erasure_matcher_methods    
};


PyMODINIT_FUNC PyInit_type_erasure_matcher(void) {
    Py_Initialize();
    return PyModule_Create(&type_erasure_matcher_def);
}
