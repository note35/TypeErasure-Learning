import sys
import unittest

from min_stack import MinStack


class TestMinStack(unittest.TestCase):

    def setUp(self):
        self.stack = MinStack()


    def test_push_pop(self):
        self.stack.push((1, '1'))
        self.assertEqual(self.stack.top(), '1')
        self.stack.push((2, '2'))
        self.assertEqual(self.stack.top(), '2')
        self.stack.push((3, '3'))
        self.assertEqual(self.stack.top(), '3')
        self.stack.pop()
        self.assertEqual(self.stack.top(), '2')
        self.stack.pop()
        self.assertEqual(self.stack.top(), '1')


    def test_get_min_without_dup(self):
        self.stack.push((2, '2'))
        self.assertEqual(self.stack.get_min(), '2')
        self.stack.push((3, '3'))
        self.assertEqual(self.stack.get_min(), '2')
        self.stack.push((1, '1'))
        self.assertEqual(self.stack.get_min(), '1')
        self.stack.pop()
        self.assertEqual(self.stack.get_min(), '2')
        self.stack.pop()
        self.assertEqual(self.stack.get_min(), '2')


    def test_get_min_with_dup(self):
        self.stack.push((2, '2a'))
        self.assertEqual(self.stack.get_min(), '2a')
        self.stack.push((2, '2b'))
        self.assertEqual(self.stack.get_min(), '2b')
        self.stack.push((1, '1'))
        self.assertEqual(self.stack.get_min(), '1')
        self.stack.pop()
        self.assertEqual(self.stack.get_min(), '2b')
        self.stack.pop()
        self.assertEqual(self.stack.get_min(), '2a')


    def test_boundary_top_pop_get_min(self):
        with self.assertRaises(IndexError):
            self.stack.top()
        with self.assertRaises(IndexError):
            self.stack.pop()
        with self.assertRaises(IndexError):
            self.stack.get_min()


    def test_reference_count_push_pop(self):
        obj = object()
        self.assertEqual(sys.getrefcount(obj), 2)
        self.stack.push((1, obj))
        self.assertEqual(sys.getrefcount(obj), 4)
        self.stack.push((2, '2'))
        self.assertEqual(sys.getrefcount(obj), 4)
        self.stack.pop()
        self.assertEqual(sys.getrefcount(obj), 4)
        self.stack.pop()
        self.assertEqual(sys.getrefcount(obj), 2)


    def test_reference_count_del(self):
        obj = object()
        local_stack = MinStack()
        self.assertEqual(sys.getrefcount(obj), 2)
        local_stack.push((1, obj))
        self.assertEqual(sys.getrefcount(obj), 4)
        del local_stack
        self.assertEqual(sys.getrefcount(obj), 2)



if __name__ == '__main__':
    unittest.main()
