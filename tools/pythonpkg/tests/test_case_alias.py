import pandas
import numpy as np
import datetime
import guinsoodb

class TestCaseAlias(object):
    def test_case_alias(self, guinsoodb_cursor):
        import pandas
        import numpy as np
        import datetime
        import guinsoodb

        con = guinsoodb.connect(':memory:')

        df = pandas.DataFrame([{"COL1": "val1", "CoL2": 1.05},{"COL1": "val3", "CoL2": 17}])

        r1 = con.from_df(df).query('df', 'select * from df').fetchdf()
        assert r1["COL1"][0] == "val1"
        assert r1["COL1"][1] == "val3"
        assert r1["CoL2"][0] == 1.05
        assert r1["CoL2"][1] == 17

        r2 = con.from_df(df).query('df', 'select COL1, COL2 from df').fetchdf()
        assert r2["COL1"][0] == "val1"
        assert r2["COL1"][1] == "val3"
        assert r2["CoL2"][0] == 1.05
        assert r2["CoL2"][1] == 17

        r3 = con.from_df(df).query('df', 'select COL1, COL2 from df ORDER BY COL1').fetchdf()
        assert r3["COL1"][0] == "val1"
        assert r3["COL1"][1] == "val3"
        assert r3["CoL2"][0] == 1.05
        assert r3["CoL2"][1] == 17

        r4 = con.from_df(df).query('df', 'select COL1, COL2 from df GROUP BY COL1, COL2 ORDER BY COL1').fetchdf()
        assert r4["COL1"][0] == "val1"
        assert r4["COL1"][1] == "val3"
        assert r4["CoL2"][0] == 1.05
        assert r4["CoL2"][1] == 17