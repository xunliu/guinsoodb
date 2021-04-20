import traceback

class TestNull(object):

    def test_fetchone_null(self, guinsoodb_cursor):
        guinsoodb_cursor.execute("CREATE TABLE atable (Value int)")
        guinsoodb_cursor.execute("INSERT INTO atable VALUES (1)")
        guinsoodb_cursor.execute("SELECT * FROM atable")
        assert(guinsoodb_cursor.fetchone()[0] == 1)
        assert(guinsoodb_cursor.fetchone() is None)
        
        
       