#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <dirent.h>
#include <libgen.h>
#include <regex.h>

#include <iostream>
using std::cerr;
using std::ios;
using std::endl;
using std::cout;
using std::ostream;

#include <sstream>
using std::ostringstream;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <iconv.h>
#include <langinfo.h>

// Options
const char* defaultXSLT = "http://www.brainlex.com/XSLT/Directory.xsl";
const char* XSLT;
const char* defaultEncoding = "UTF-8";
const char* encoding;
bool	hasPattern;
bool	dirMatching;
regex_t	regex;
bool	recurse;
bool	wantMIME;
const char* requestedMIME;

// Version Info
static const char* versionInfo = "1.0 (Daredevil)";

static void freeAll() {
	if (XSLT != defaultXSLT)
		free((char*)XSLT);
	if (encoding != defaultEncoding)
		free((char*)encoding);
	if (hasPattern)
		regfree(&regex);
	if (requestedMIME)
		free((char*)requestedMIME);
}

static void convertToTargetEncoding(string& s) {
	iconv_t iconv_cd;
	setlocale(LC_ALL, "");
	char* from_encoding = nl_langinfo(CODESET);
	size_t input_length = s.length()+1;
	size_t output_length = input_length * MB_LEN_MAX;
	char* ouput_buf = new char[output_length];
	char* output = ouput_buf;
	char* input_buf = strdup(s.c_str());
	char* input  = input_buf;
	iconv_cd = iconv_open(encoding, from_encoding);
	if (iconv_cd == (iconv_t)(-1)) {
		free(input_buf);
		delete[] ouput_buf;
		return;
	}
	size_t done = iconv(iconv_cd, &input, &input_length, &output, &output_length);
	iconv_close(iconv_cd);
	if (done == (size_t) -1 || input_length) {
		free(input_buf);
		delete[] ouput_buf;
		return;
	}
	s = ouput_buf;
	free(input_buf);
	delete[] ouput_buf;
	// replace & by &amp;
	int pos=0;
	while ((pos=s.find("&", pos)) != s.npos) {
		s.replace(pos, 1, "&amp;");
		++pos;
	}
}

struct temporary {
	temporary() {
		ostringstream tmpName;
		tmpName << "/tmp/.nsl-" << static_cast<int>(getpid());
		name = tmpName.str();
	}
	~temporary() {
		struct stat buf;
		if (stat(name.c_str(), &buf)==0) {
			remove(name.c_str());
		}
	}
	bool fillWithCommandOutput(const string& path) {
		ostringstream command;
		command << path << " >" << name << " 2>/dev/null";
		return (system(command.str().c_str()) != -1);
	}
	string	name;
};

struct node {
	string		path;
	node() {
		char* cwd = getcwd(0L, 0);
		path = cwd;
		free(cwd);
	}
	node(const string& _path) {
		path = _path;
	}
	node(const node& copy) {
		path = copy.path;
	}
	static void tabulate(int t, ostream& out) {
		while(t--) out << "\t";
	}
	void encodePath(string& s) const {
		s = path;
		convertToTargetEncoding(s);
	}
	void basename(string& s) const {
		char* copy = strdup(path.c_str());
		const char* c_s = ::basename(copy);
		s = c_s;
		free(copy);
		convertToTargetEncoding(s);
	}
	void dump(int tabs, ostream& out) {
		string bname, bpath;
		basename(bname);
		encodePath(bpath);
		struct stat64 buf;
		if (lstat64(path.c_str(), &buf)==0) {
			int t = type(static_cast<unsigned long long>(buf.st_mode));
			switch(t) {
			case Directory:
				{
				ostringstream content;
				
				if (tabs==0) {
					content	<< "<?xml version=\"1.0\" encoding=\"" << encoding << "\" standalone=\"yes\" ?>" << endl
						<< "<?xml-stylesheet type=\"text/xsl\" href=\"" << XSLT << "\"?>" << endl
						<< "<!DOCTYPE Directory SYSTEM \"http://www.brainlex.com/DTD/Directory.dtd\">" << endl
						<< "<Directory"
						<< " path=\"" << bpath << "\"";
				}
				else {
					tabulate(tabs, content);
					content	<< "<Directory";
				}
				content	<< " name=\"" << bname << "\""
					<< ">" << endl;
				
				int length = content.str().length();

				DIR* dir = opendir(path.c_str());
				if (dir) {
					dirent* entry = 0L;
					while (entry=readdir(dir)) {
						node child(entry->d_name);
						if ((child.path == ".") || (child.path == ".."))
							continue;
						string cname = child.path;
						child.path = path + "/" + cname;
					
						bool traverse = true;
						int  ctype = child.type();
						if (ctype==Directory)
							traverse = recurse;
						else if (hasPattern) {
							int err = regexec(&regex, cname.c_str(), 0, 0L, 0);
							traverse = (err==0);
						}
						if (traverse)
							child.dump(tabs+1, content);
					}
					closedir(dir);
				}
				// FUTURE: else { mark permission has denied. }

				bool empty = (length == content.str().length());
				if (empty) {
					if (hasPattern && dirMatching) {
						string base(path.substr(1+path.find_last_of("/")));
						int err = regexec(&regex, base.c_str(), 0, 0L, 0);
						if (err == 0) {
							string contentStr = content.str();
							contentStr = contentStr.substr(0, contentStr.find_last_of(">"));
							out << contentStr << " />" << endl;
						}
					}
					// else: output nothing
				}
				else {
					out << content.str();
					tabulate(tabs, out);
					out	<< "</Directory>" << endl;
				}
				}
				return;
			case CharacterDevice:
			case BlockDevice:
				{
				tabulate(tabs, out);
				out	<< "<BlockDevice"
					<< " name=\"" << bname << "\""
					<< " />" << endl;
				}
				return;
			case File:
				{
				string MIMEType("");
				if (wantMIME || requestedMIME) {
					temporary tmpObject;
					ostringstream command;
					command << "file -ib \"" << path.c_str() << "\"";
					if (tmpObject.fillWithCommandOutput(command.str())) {
						ifstream is(tmpObject.name.c_str());
						getline(is, MIMEType);
						// remove comments
						MIMEType = MIMEType.substr(0, MIMEType.find(','));
						MIMEType = MIMEType.substr(0, MIMEType.find('\t'));
					}
					if (requestedMIME) {
						string reqStr(requestedMIME);
						// is requestedMIME a superset of MIMEType ?
						bool ok = false;
						ok  = (MIMEType.length()>reqStr.length());
						ok &= (reqStr.compare(MIMEType.substr(0, reqStr.length()))==0);
						ok &= (	(MIMEType.c_str()[reqStr.length()] == ';') ||
							(MIMEType.c_str()[reqStr.length()-1] == '/'));
						// or a perfect match?
						ok |= (reqStr.compare(MIMEType)==0);
						if (!ok)
							MIMEType = "";
					}
				}
				if ((!requestedMIME) || ( requestedMIME && (MIMEType.length()>0) )) {
					tabulate(tabs, out);
					out	<< "<File"
						<< " name=\"" << bname << "\""
						<< " size=\"" << static_cast<unsigned long long>(buf.st_size) << "\"";
					if (MIMEType.length()>0 && wantMIME)
						out << " type=\"" << MIMEType << "\"";

					out	<< " />" << endl;
				}
				}
				return;
			case Link:
				{
				tabulate(tabs, out);
				out	<< "<Link"
					<< " name=\"" << bname << "\""
					<< " />" << endl;
				}
				return;
			case Socket:
				{
				tabulate(tabs, out);
				out	<< "<Socket"
					<< " name=\"" << bname << "\""
					<< " />" << endl;
				}
				return;
			}
		}
		cerr << "Error stating \"" << path << "\"" << endl;
	}
private:
	int	type() {
		struct stat64 buf;
		if (lstat64(path.c_str(), &buf)==0)
			return type(buf.st_mode);
		return Unknown;
	}
	int	type(unsigned long long mode) {
		if (S_ISLNK(mode))
			return Link;
		else if (S_ISDIR(mode))
			return Directory;
		else if (S_ISCHR(mode))
			return CharacterDevice;
		else if (S_ISBLK(mode))
			return BlockDevice;
		else if (S_ISFIFO(mode))
			return Fifo;
		else if (S_ISREG(mode))
			return File;
		else return Socket;
	}
	enum EType {
		Directory,
		CharacterDevice,
		BlockDevice,
		File,
		Fifo,
		Link,
		Socket,
		Unknown
	};
};

static bool canonicalize(string& s) {
#	ifdef PATH_MAX
		char* canonalized = new char[PATH_MAX];
#	else
		int path_max = pathconf (s.c_str(), _PC_PATH_MAX);
		if (path_max <= 0)
			path_max = 4096;
		char* canonalized = new char[path_max];
#	endif
	char* r = realpath(s.c_str(), canonalized);
	if (r)
		s = r;
	delete[] canonalized;
	return (r!=0L);
}

static void printUsage(ostream& out) {
	out	<< "XMLS version " << versionInfo << endl
		<< "http://deepmind.brainlex.com/xmls/" << endl << endl
		<< "Usage: xmls"
		<< " [-V]"
		<< " [-H]"
		<< " [-X../MyCustomXSLT.xsl]"
		<< " [-Ecodeset]"
		<< " [-R]"
		<< " [-M]"
		<< " [-PmyRegex]" << endl << "           "
		<< " [-Tapplication/x-example]"
		<< " [/some/path]" << endl
		<< "where:" << endl << endl
		<< "\t-V, --version" << endl
		<< "                               Display version string." << endl
		<< "\t-H, --help" << endl
		<< "                               Display this short help." << endl
		<< "\t-X..., --xsl=..." << endl
		<< "                               Use a custom XSL transform." << endl
		<< "                               default: " << defaultXSLT << endl
		<< "\t-E..., --encoding=..." << endl
		<< "                               Set the target encoding." << endl
		<< "                               default: " << defaultEncoding << endl
		<< "\t-R, --recurse" << endl
		<< "                               Traverse directories recursively." << endl
		<< "                               default; No" << endl
		<< "\t-M, --mime" << endl
		<< "                               Ouput MIME type (slower)." << endl
		<< "                               default: No" << endl
		<< "\t-P..., --pattern=..." << endl
		<< "                               Report only files that match. see regex(7)" << endl
		<< "                               default: Report everything" << endl
		<< "\t-T..., --type=..." << endl
		<< "                               Report only files whose MIME type match." << endl
		<< "\t-F, --files" << endl
		<< "                               Apply pattern to files only (not to directories)" << endl
		<< "                               default: No" << endl
		<< "\t/some/path" << endl
		<< "                               Directory to list." << endl
		<< "                               default: Current directory" << endl;
}

static void badArg(const string& arg) {
	cerr << "*** Bad argument: " << arg << endl;
	printUsage(cerr);
	freeAll();
	exit(1);
}

int main(int argc, char** argv) {
	node n;
	// default settings
	XSLT		= defaultXSLT;
	encoding	= defaultEncoding;
	hasPattern	= false;
	dirMatching	= true;
	recurse		= false;
	wantMIME	= false;
	requestedMIME	= 0L;

	while (--argc) {
		string arg = argv[argc];
		int sub;
		
		sub = 0; sub = (arg.find("-X")==0)?2: (arg.find("--xsl=")==0)?6:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] == ' ') || (vrf[0] == '\0'))
				badArg(arg);

			bool ok=true;
			string xslt = arg.substr(sub);
			if (xslt.find("://") == xslt.npos)
				ok &= canonicalize(xslt);
			if (ok)
				XSLT = strdup(xslt.c_str());
			else {
				cerr << "*** Cannot find \"" << xslt << "\"" << endl;
				freeAll();
				return 1;
			}
			continue;
		}

		sub = 0; sub = (arg.find("-T")==0)?2: (arg.find("--type=")==0)?7:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] == ' ') || (vrf[0] == '\0'))
				badArg(arg);

			bool ok=true;
			string mimeType = arg.substr(sub);
			if (mimeType.find("/") == mimeType.npos) {
				cerr << "*** Malformed MIME type \"" << mimeType << "\"" << endl;
				freeAll();
				return 1;
			}

			requestedMIME = strdup(mimeType.c_str());

			continue;
		}

		sub = 0; sub = (arg.find("-E")==0)?2: (arg.find("--encoding=")==0)?11:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] == ' ') || (vrf[0] == '\0'))
				badArg(arg);

			encoding = strdup(arg.substr(sub).c_str());

			continue;
		}

		sub = 0; sub = (arg.find("-P")==0)?2: (arg.find("--pattern=")==0)?10:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] == ' ') || (vrf[0] == '\0'))
				badArg(arg);

			int err = regcomp(&regex, arg.substr(sub).c_str(), REG_NOSUB);
			hasPattern = (err==0);
			if (!hasPattern) {
				cerr << "*** Cannot construct regex from \"" << arg.substr(sub) << "\": ";
				switch(err) {
					case REG_BADRPT:	cerr << "Invalid use of repetition operators." << endl; break;
					case REG_BADBR:		cerr << "Invalid use of back reference operator." << endl; break;
					case REG_EBRACE:	cerr << "Un‐matched brace interval operators." << endl; break;
					case REG_EBRACK:	cerr << "Un‐matched bracket list operators." << endl; break;
					case REG_ERANGE:	cerr << "Invalid use of the range operator." << endl; break;
					case REG_ECTYPE:	cerr << "Unknown character class name." << endl; break;
					case REG_ECOLLATE:	cerr << "Invalid collating element." << endl; break;
					case REG_EPAREN:	cerr << "Un‐matched parenthesis group operators." << endl; break;
					case REG_ESUBREG:	cerr << "Invalid back reference to a subexpression." << endl; break;
					case REG_EEND:		cerr << "Non specific error." << endl; break;
					case REG_EESCAPE:	cerr << "Trailing backslash." << endl; break;
					case REG_BADPAT:	cerr << "Invalid use of pattern operators." << endl; break;
					case REG_ESIZE:		cerr << "Compiled regular expression too large." << endl; break;
					case REG_ESPACE:	cerr << "The regex routines ran out of memory." << endl; break;
					default:		cerr << "Unknown error " << err << "." << endl; break;
				}
				freeAll();
				return 1;
			}
			continue;
		}

		sub = 0; sub = (arg.find("-H")==0)?2: (arg.find("--help")==0)?6:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] != ' ') && (vrf[0] != '\0'))
				badArg(arg);

			printUsage(cout);
			freeAll();
			return 0;
		}

		sub = 0; sub = (arg.find("-V")==0)?2: (arg.find("--version")==0)?9:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] != ' ') && (vrf[0] != '\0'))
				badArg(arg);

			cout << versionInfo << endl;
			freeAll();
			return 0;
		}

		sub = 0; sub = (arg.find("-R")==0)?2: (arg.find("--recurse")==0)?9:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] != ' ') && (vrf[0] != '\0'))
				badArg(arg);

			recurse = true;
			continue;
		}

		sub = 0; sub = (arg.find("-F")==0)?2: (arg.find("--files")==0)?7:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] != ' ') && (vrf[0] != '\0'))
				badArg(arg);

			dirMatching = false;
			continue;
		}

		sub = 0; sub = (arg.find("-M")==0)?2: (arg.find("--mime")==0)?6:0;
		if (sub) {
			const char* vrf = arg.substr(sub).c_str();
			if ((vrf[0] != ' ') && (vrf[0] != '\0'))
				badArg(arg);

			wantMIME = true;
			continue;
		}

		if (!canonicalize(arg)) {
			cerr << "*** Cannot find \"" << arg << "\"" << endl;
			freeAll();
			return 1;
		}
		n.path = arg;
	}

	n.dump(0, cout);

	freeAll();
	return 0;
}

