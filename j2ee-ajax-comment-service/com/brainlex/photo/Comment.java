package com.brainlex.photo;
import java.util.*;
import java.lang.*;
import java.io.*;

public class Comment implements Serializable {
	private String mangle(String s) {
		String ns = s.replaceAll("/</", "&lt;").replaceAll("/>/", "&gt;");
		return ns;
	}
	public Comment(String nname, String nwebsite, String ncomment) {
		try {
			map = new TreeMap();
			map.put("timestamp", ""+System.currentTimeMillis());
			map.put("name", mangle(nname));
			map.put("website", mangle(nwebsite));
			map.put("comment", mangle(ncomment));
		}
		catch (Exception e) {
			// no op
		}
	}
	public Map getMap() {
		return map;
	}
	Map map;
}

