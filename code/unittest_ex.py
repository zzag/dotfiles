import unittest


class SampleTestCase(unittest.TestCase):
    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_sample(self):
        self.assertTrue(True)
        self.assertFalse(False)
        self.assertEqual(10, 10)
        self.assertIn(10, [20, 10])


if __name__ == '__main__':
    unittest.main()

