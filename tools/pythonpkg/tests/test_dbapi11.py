# cursor description

import guinsoodb
import tempfile
import os

def check_exception(f):
    had_exception = False
    try:
        f()
    except:
        had_exception = True
    assert(had_exception)

class TestReadOnly(object):
    def test_readonly(self, guinsoodb_cursor):
        fd, db = tempfile.mkstemp()
        os.close(fd)
        os.remove(db)

        # this is forbidden
        check_exception(lambda :guinsoodb.connect(":memory:", True))

        con_rw = guinsoodb.connect(db, False)
        con_rw.cursor().execute("create table a (i integer)")
        con_rw.cursor().execute("insert into a values (42)")
        con_rw.close()

        con_ro = guinsoodb.connect(db, True)
        con_ro.cursor().execute("select * from a").fetchall()
        check_exception(lambda : con_ro.execute("delete from a"))
        con_ro.close()

        con_rw = guinsoodb.connect(db, False)
        con_rw.cursor().execute("drop table a")
        con_rw.close()
