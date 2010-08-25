package com.brainlex.photo;
import java.beans.XMLEncoder;
import java.beans.XMLDecoder;
import java.io.*;
import java.util.*;
import java.lang.*;
import java.text.DateFormat;
import uk.ltd.getahead.dwr.ExecutionContext;
import javax.servlet.http.HttpServletRequest;

public class CommentManager extends Object {
        private static String getExceptionString(Exception e) {
		String res = "";
		res+=e.toString()+"\n";
		StackTraceElement[] stack = e.getStackTrace();
		for (int i=0; i<stack.length; ++i) {
			res+=stack[i].toString()+"\n";
		}
		return res;
	}
	
	private class Saver extends TimerTask {
		CommentManager mOwner;
		public Saver(CommentManager owner) {
			mOwner = owner;
		}
		public void run() {
			if (mOwner.isDirty()) {
				try {
					ObjectOutputStream xml = new ObjectOutputStream(
					new BufferedOutputStream(
					new FileOutputStream(
						System.getProperty("com.brainlex.photo.comments", "/opt/site_comments.xml")
					)));
					xml.writeObject(mOwner.commentsMap());
					xml.close();
					mOwner.clearDirty();
				}
				catch (Exception e) {
					System.err.println(getExceptionString(e));
				}
			}
		}
	}
	static Map mComments  = null;
	static boolean mDirty = false;
	static Timer mBeat    = null;

	
	// Create a comment manager
	public CommentManager() {
		String path = System.getProperty("com.brainlex.photo.comments", "/opt/site_comments.xml");
		if (mComments==null) {
			// first try to load existing comments
			try {
				ObjectInputStream xml = new ObjectInputStream(
							new BufferedInputStream(
								new FileInputStream(path)));
				mComments = (Map) xml.readObject();
				xml.close();
			}
			catch (Exception e) {
				try {
					// no ancient history, create a new map
					mComments = new TreeMap();
				}
				catch (Exception f) {
					System.err.println(getExceptionString(f));
				}
			}
		}
		if (mBeat==null) {
			try {
				mBeat = new Timer(true);
				mBeat.schedule(new Saver(this), 1000, 3000);
			}
			catch (Exception g) {
				System.err.println(getExceptionString(g));
			}
		}
	}
	boolean isDirty() {
		return mDirty;
	}
	void clearDirty() {
		mDirty=false;
	}
	Map commentsMap() {
		return mComments;
	}
	public synchronized String getCommentsXML(String url) {
		List list = getComments(url);
		if (list!=null) {
			String s="<enveloppe>";
			s+="<lang>"+getUserLanguage()+"</lang>";
			for (int i=0; i<list.size(); ++i) {
				Comment comment = (Comment) list.get(i);
				Map map = comment.getMap();
				String date = getCommentDate((String) map.get("timestamp"));
				s+="<comment>";
				s+="<timestamp>"+(String)map.get("timestamp")+"</timestamp>";
				s+="<date>"+date+"</date>";
				s+="<author>"+(String)map.get("name")+"</author>";
				s+="<url>"+(String)map.get("website")+"</url>";
				s+="<text>"+(String)map.get("comment")+"</text>";
				s+="</comment>";
			}
			s+="</enveloppe>";
			System.err.println(s);
			return s;
		}
		return "<enveloppe><lang>"+getUserLanguage()+"</lang></enveloppe>";
	}
	private synchronized String adminQuery(String url, String query) {
		System.err.println("Admin query: '"+query+"'");
		if (query.startsWith("remove ")) {
			// remove prefix
			query=query.substring(6).trim();
			// search id
			LinkedList res = (LinkedList) mComments.get(url);
			for (int i=0; i<res.size(); ++i) {
				Comment current = (Comment) res.get(i);
				Map map = current.getMap();
				String id = (String) map.get("timestamp");
				if (query.equals(id)) {
					// remove
					res.remove(i);
					mDirty=true;
					break;
				}
			}
		}
		return getCommentsXML(url);
	}
	// List
	private synchronized List getComments(String url) {
		if (url==null) return null;
		try {
			LinkedList res = (LinkedList) mComments.get(url);
			if (res==null) {
				System.err.println("No list in there, create one!");
				res = new LinkedList();
				mComments.put(url, res);
			}
			return res;
		}
		catch(Exception e) {
			System.err.println(getExceptionString(e));
			return null;
		}
	}
	// Add new
	public synchronized String addComment2(String url, String name, String website, String comment) {
		if (url==null) return null;
		try {
			if (name.equals("admin") && website.equals("321nimda")) {
				return adminQuery(url, comment);
			}
			LinkedList comments = (LinkedList) getComments(url);
			comments.addFirst(new Comment(name, website, comment));
			mDirty=true;
		}
		catch (Exception e) {
			System.err.println(getExceptionString(e));
		}
		return getCommentsXML(url);
	}
	public synchronized String getUserLanguage() {
		HttpServletRequest req = ExecutionContext.get().getHttpServletRequest();
		String lang="en";
		if (req!=null) {
			String langs = req.getHeader("Accept-Language");
			if (langs!=null) {
				String[] languages = langs.split(",");
				for (int l=0;l<languages.length;++l) {
					if (languages[l]!=null) {
						String[] split = languages[l].split("-");
						if (split!=null) {
							if (split[0].equals("fr") || split[0].equals("ja")) {
								lang=split[0];
								return lang;
							}
						}
					}
				}
			}
		}
		return lang;
	}
	public synchronized String getCommentDate(String timestamp) {
		try {
			long t = Long.parseLong(timestamp);
			Calendar cal = new GregorianCalendar();
			cal.setTimeInMillis(t);

			Locale[] lox = DateFormat.getAvailableLocales();
			String lang = getUserLanguage();
			Locale loc = null;
			for (int i=0; i<lox.length; ++i) {
				if (lox[i].getLanguage().equals(lang)) {
					loc = lox[i];
					break;
				}
			}
			if (loc!=null) {
				return DateFormat.getDateInstance(DateFormat.LONG, loc).format(cal.getTime());
			}
			return DateFormat.getDateInstance().format(cal.getTime());
		}
		catch(Exception e) {
			System.err.println(getExceptionString(e));
			return null;
		}
	}
}

