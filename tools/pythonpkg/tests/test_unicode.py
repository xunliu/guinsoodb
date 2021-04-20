#!/usr/bin/env python
# -*- coding: utf-8 -*-

import guinsoodb
import pandas as pd
import sys

class TestUnicode(object):
    def test_unicode_pandas_scan(self, guinsoodb_cursor):
        if sys.version_info.major < 3:
            return
        con = guinsoodb.connect(database=':memory:', read_only=False)
        test_df = pd.DataFrame.from_dict({"i":[1, 2, 3], "j":["a", "c", u"ë"]})
        con.register('test_df_view', test_df)
        con.execute('SELECT i, j, LENGTH(j) FROM test_df_view').fetchall()
