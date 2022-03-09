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

// Source: https://youtu.be/ZPk8HuyrKXU
// Compile: c++ --std=c++1z type_erasure_example_in_cpp.cc
// !!The result may not be executable in MacOS!!
#include <iostream>
#include <memory>

struct AliceOrBob {
  template <typename T>
  AliceOrBob(T&& value) {
    *this = value;
  }

  template <typename T>
  AliceOrBob& operator=(T&& value) {
    m_value.reset(new Model<T>(value));
  }

  void say() const { m_value->say(); }

 private:
  struct Concept {
    virtual ~Concept() {}
    virtual void say() const = 0;
  };
  template <typename T>
  struct Model : Concept {
    Model(T const& value) : m_val(value) {}

    void say() const override { m_val.say(); }

    T m_val;
  };

 private:
  std::unique_ptr<Concept> m_value;
};

struct Alice {
  void say() const { std::cout << "alice\n"; }
};

struct Bob {
  void say() const { std::cout << "bob\n"; }
};

int main() {
  AliceOrBob aliceOrBob{Alice()};
  aliceOrBob.say();  // alice
  aliceOrBob = Bob();
  aliceOrBob.say();  // bob
}
