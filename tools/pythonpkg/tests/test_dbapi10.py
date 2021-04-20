# cursor description

class TestCursorDescription(object):
    def test_description(self, guinsoodb_cursor):
        description = guinsoodb_cursor.execute("SELECT * FROM integers").description
        assert description == [('i', None, None, None, None, None, None)]
