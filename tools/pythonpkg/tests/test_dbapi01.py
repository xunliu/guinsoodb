# multiple result sets

import numpy


class TestMultipleResultSets(object):
    def test_regular_selection(self, guinsoodb_cursor):
        guinsoodb_cursor.execute('SELECT * FROM integers')
        guinsoodb_cursor.execute('SELECT * FROM integers')
        result = guinsoodb_cursor.fetchall()
        assert result == [(0,), (1,), (2,), (3,), (4,), (5,), (6,), (7,), (8,), (9,), (None,)], "Incorrect result returned"

    def test_numpy_selection(self, guinsoodb_cursor):
        guinsoodb_cursor.execute('SELECT * FROM integers')
        guinsoodb_cursor.execute('SELECT * FROM integers')
        result = guinsoodb_cursor.fetchnumpy()
        expected = numpy.ma.masked_array(numpy.arange(11), mask=([False]*10 + [True]))

        numpy.testing.assert_array_equal(result['i'], expected)
