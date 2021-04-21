package org.guinsoodb;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.sql.SQLException;

public class GuinsooDBDatabase {

	protected String url;
	protected boolean read_only;

	public GuinsooDBDatabase(String url, boolean read_only) throws SQLException {
		if (!url.startsWith("jdbc:guinsoodb")) {
			throw new SQLException("GuinsooDB JDBC URL needs to start with 'jdbc:guinsoodb:'");
		}
		this.url = url;
		String db_dir = url.replaceFirst("^jdbc:guinsoodb:", "").trim();
		if (db_dir.length() == 0) {
			db_dir = ":memory:";
		}
		this.read_only = read_only;
		db_ref = GuinsooDBNative.guinsoodb_jdbc_startup(db_dir.getBytes(StandardCharsets.UTF_8), read_only);
	}

	public void shutdown() {
		try {
			finalize();
		} catch (Throwable e) {
		}
	}

	protected synchronized void finalize() throws Throwable {
		if (db_ref != null) {
			GuinsooDBNative.guinsoodb_jdbc_shutdown(db_ref);
			db_ref = null;
		}
	}

	protected ByteBuffer db_ref;

}
