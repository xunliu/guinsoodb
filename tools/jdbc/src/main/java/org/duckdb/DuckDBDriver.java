package org.guinsoodb;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.DriverPropertyInfo;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.util.Properties;
import java.util.logging.Logger;

public class GuinsooDBDriver implements java.sql.Driver {

	static final String GUINSOODB_READONLY_PROPERTY = "guinsoodb.read_only";

	static {
		try {
			DriverManager.registerDriver(new GuinsooDBDriver());
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	public Connection connect(String url, Properties info) throws SQLException {
		if (!acceptsURL(url)) {
			return null;
		}
		boolean read_only = false;
		if (info != null) {
			String prop_val = info.getProperty(GUINSOODB_READONLY_PROPERTY);
			if (prop_val != null) {
				String prop_clean = prop_val.trim().toLowerCase();
				read_only = prop_clean.equals("1") || prop_clean.equals("true") || prop_clean.equals("yes");
			}
		}
		GuinsooDBDatabase db = new GuinsooDBDatabase(url, read_only);
		return new GuinsooDBConnection(db);
	}

	public boolean acceptsURL(String url) throws SQLException {
		return url.startsWith("jdbc:guinsoodb:");
	}

	public DriverPropertyInfo[] getPropertyInfo(String url, Properties info) throws SQLException {
		DriverPropertyInfo[] ret = {};
		return ret; // no properties
	}

	public int getMajorVersion() {
		return 1;
	}

	public int getMinorVersion() {
		return 0;
	}

	public boolean jdbcCompliant() {
		return true; // of course!
	}

	public Logger getParentLogger() throws SQLFeatureNotSupportedException {
		throw new SQLFeatureNotSupportedException("no logger");
	}

}
