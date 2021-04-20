package org.guinsoodb;

import java.nio.ByteBuffer;

public class GuinsooDBVector {
	
	public GuinsooDBVector(String guinsoodb_type, int length,  boolean[] nullmask) {
		super();
		this.guinsoodb_type = guinsoodb_type;
		this.length = length;
		this.nullmask = nullmask;
	}
	protected String guinsoodb_type;
	protected int length;
	protected boolean[] nullmask;
	protected ByteBuffer constlen_data = null;
	protected Object[] varlen_data = null;

}
