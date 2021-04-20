import guinsoodb
import numpy

class TestBlob(object):
    def test_blob(self, guinsoodb_cursor):
        guinsoodb_cursor.execute("SELECT BLOB 'hello'")
        results = guinsoodb_cursor.fetchall()
        assert results[0][0] == b'hello'

        guinsoodb_cursor.execute("SELECT BLOB 'hello' AS a")
        results = guinsoodb_cursor.fetchnumpy()
        assert results['a'] == numpy.array([b'hello'], dtype=numpy.object)
