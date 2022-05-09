#include <stack>
#include <utility>

#include <Python.h>

#include <pybind11/pybind11.h>


template <typename value_type>
class MinStack {
public:
    void push(std::pair<int, value_type> pair) {
        inner_stack_.push(pair);
        if (inner_min_stack_.empty() ||
            pair.first <= inner_min_stack_.top().first) {
            inner_min_stack_.push(pair);
        }
    }
    void pop() {
        if (inner_stack_.size() == 0) {
            throw(pybind11::index_error("pop from empty list"));
        }
        if (inner_stack_.top().first == inner_min_stack_.top().first) {
            inner_min_stack_.pop();
        }
        inner_stack_.pop();
    }
    value_type top() {
        if (inner_stack_.size() == 0) {
            throw(pybind11::index_error("top from empty list"));
        }
        return inner_stack_.top().second;
    }
    value_type get_min() {
        if (inner_stack_.size() == 0) {
            throw(pybind11::index_error("get_min from empty list"));
        }
        return inner_min_stack_.top().second;
    }
private:
    std::stack<std::pair<int, value_type>> inner_stack_;
    std::stack<std::pair<int, value_type>> inner_min_stack_;
};


PYBIND11_MODULE(min_stack, m) {
    m.doc() = "min_stack with pybind11::object";

    pybind11::class_<MinStack<pybind11::object>>(m, "MinStack")
        .def(pybind11::init<>())
        .def("push", &MinStack<pybind11::object>::push)
        .def("pop", &MinStack<pybind11::object>::pop)
        .def("top", &MinStack<pybind11::object>::top)
        .def("get_min", &MinStack<pybind11::object>::get_min);
}
