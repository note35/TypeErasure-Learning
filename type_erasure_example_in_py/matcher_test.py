# Copyright 2022 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import unittest

import type_erasure_matcher as matcher


class TestTypeErasureMatcherMethods(unittest.TestCase):

  def setUp(self):
    self.exactly_matcher = matcher.get_matcher_exactly()
    self.partially_matcher = matcher.get_matcher_partially()

  def test_is_matcher(self):
    self.assertFalse(matcher.is_matcher(123))
    self.assertTrue(matcher.is_matcher(self.exactly_matcher))
    self.assertTrue(matcher.is_matcher(self.partially_matcher))

  def test_match_by_matcher_exactly(self):
    self.assertFalse(matcher.match('apple', 'banana', self.exactly_matcher))
    self.assertTrue(matcher.match('apple', 'apple', self.exactly_matcher))
    self.assertFalse(matcher.match('apple', 'applepie', self.exactly_matcher))

  def test_match_by_matcher_partially(self):
    self.assertFalse(matcher.match('apple', 'banana', self.partially_matcher))
    self.assertTrue(matcher.match('apple', 'apple', self.partially_matcher))
    self.assertTrue(matcher.match('apple', 'applepie', self.partially_matcher))

  def test_matcher_reference_count(self):
    first_count = sys.getrefcount(self.exactly_matcher)
    matcher.match('apple', 'banana', self.exactly_matcher)
    second_count = sys.getrefcount(self.exactly_matcher)

    self.assertTrue(first_count == second_count)


if __name__ == '__main__':
  unittest.main()
