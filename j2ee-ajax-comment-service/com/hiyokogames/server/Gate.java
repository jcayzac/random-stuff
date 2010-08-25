package com.hiyokogames.server;
import java.io.*;
import java.lang.*;
import nu.xom.*;
import nu.xom.xslt.*;

public class Gate extends Object {
	public Gate() { }
	private String getExceptionString(Exception e) {
		String res = "";
		res+=e.toString()+"\n";
		StackTraceElement[] stack = e.getStackTrace();
		for (int i=0; i<stack.length; ++i) {
			res+=stack[i].toString()+"\n";
		}
		return res;
	}
	public void reload() {
		try {
			XMLManager.getInstance().reload();
		}
		catch(Exception e) {
			System.err.println(getExceptionString(e));
		}
	}
	public String getUserLanguage() {
		try {
			return XMLManager.getInstance().getUserLanguage();
		}
		catch(Exception e) {
			System.err.println(getExceptionString(e));
			return null;
		}
	}
	public Document getSiteFragment(String xquery, String arg) {
		try {
			return XMLManager.getInstance().getSiteFragment(xquery,arg);
		}
		catch(Exception e) {
			System.err.println(getExceptionString(e));
			return null;
		}
	}
}
