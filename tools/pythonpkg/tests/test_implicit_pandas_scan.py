# simple DB API testcase

import guinsoodb
import numpy
import pandas

global_df = pandas.DataFrame([{"COL1": "val1", "CoL2": 1.05},{"COL1": "val4", "CoL2": 17}])

class TestImplicitPandasScan(object):
    def test_local_pandas_scan(self, guinsoodb_cursor):
        con = guinsoodb.connect()
        df = pandas.DataFrame([{"COL1": "val1", "CoL2": 1.05},{"COL1": "val3", "CoL2": 17}])
        r1 = con.execute('select * from df').fetchdf()
        assert r1["COL1"][0] == "val1"
        assert r1["COL1"][1] == "val3"
        assert r1["CoL2"][0] == 1.05
        assert r1["CoL2"][1] == 17

    def test_global_pandas_scan(self, guinsoodb_cursor):
        con = guinsoodb.connect()
        r1 = con.execute('select * from global_df').fetchdf()
        assert r1["COL1"][0] == "val1"
        assert r1["COL1"][1] == "val4"
        assert r1["CoL2"][0] == 1.05
        assert r1["CoL2"][1] == 17

