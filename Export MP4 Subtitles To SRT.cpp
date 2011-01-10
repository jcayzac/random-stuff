#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <math.h>
#include <unistd.h>
using namespace std;

static string addDuration(const string& HHMMSSmmm, int scale, int duration) {
	long HH=(HHMMSSmmm[0]-'0')*10 + (HHMMSSmmm[1]-'0');
	long MM=(HHMMSSmmm[3]-'0')*10 + (HHMMSSmmm[4]-'0');
	long SS=(HHMMSSmmm[6]-'0')*10 + (HHMMSSmmm[7]-'0');
	long mmm=(HHMMSSmmm[9]-'0')*100 + (HHMMSSmmm[10]-'0')*10 + (HHMMSSmmm[11]-'0');
	double t1 = (double)HH*3600.0 + (double)MM*60.0 + (double)SS + (double)mmm*0.001;
	double t2 = t1 + (double)duration/(double)scale;
	double HH2 = floor(t2/3600.0);
	t2 -= HH2*3600.0;
	double MM2 = floor(t2/60.0);
	t2 -= MM2*60.0;
	double SS2 = floor(t2);
	t2 -= SS2;
	double mmm2 = t2*1000.0;
	char res[13];
	res[0]='0' + (long)HH2 / 10;
	res[1]='0' + (long)HH2 % 10;
	res[2]=':';
	res[3]='0' + (long)MM2 / 10;
	res[4]='0' + (long)MM2 % 10;
	res[5]=':';
	res[6]='0' + (long)SS2 / 10;
	res[7]='0' + (long)SS2 % 10;
	res[8]='.';
	res[9]='0' + (long)mmm2 / 100;
	long mm2=(long)mmm2 % 100;
	res[10]='0' + (long)mm2 / 10;
	res[11]='0' + (long)mm2 % 10;
	res[12]=0;
	return string(res);
}

void process_track(const string& path, int track_num, const string& language) {
	stringstream ss;
	ss << path.substr(0,path.find_last_of('.')) << "." << language << ".srt";
	string srt(ss.str());
	
	cout << "Exporting track #" << track_num << " to \"" << srt << "\"…\n";
	
	ss.str("");
	ss << "mp4extract -t " << track_num << " \"" << path << "\" >/dev/null 2>&1";
	system(ss.str().c_str());
	
	ss.str("");
	ss << path << ".t" << track_num;
	string tx(ss.str());
	
	vector<string> subs;
	ifstream input(tx.c_str(), ios::in|ios::binary|ios::ate);
	if (input.is_open()) {
		int size(input.tellg());
		unsigned char data[4];
		vector<char> s;
		int offset(0);
		int id(0);
		while(offset < size) {
			input.seekg(offset, ios::beg);
			input.read((char*)data, 4);
			int sz((data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3]);
			s.resize(sz+1);
			input.seekg(offset+4, ios::beg);
			input.read(&s[0], sz);
			s[sz]=0;
			++id;
			subs.push_back(&s[0]);
			offset += 4+sz;
		}
		input.close();
	}
	unlink(tx.c_str());
	
	ss.str("");
	ss << language << (int)getpid();
	string track_dump(ss.str());
	

	ss.str("");
	ss << "mp4trackdump -t " << track_num << " \"" << path << "\" >\"" << track_dump << "\" 2>/dev/null";
	system(ss.str().c_str());
	
	ifstream in(track_dump.c_str(), ios::in);
	ofstream out(srt.c_str(), ios::trunc);
	if (in.is_open() && out.is_open()) {
		string line;
		int id(0), scale(1000);
		{
			getline(in, line, '\n');
			stringstream header(line);
			header >> skipws;
			string s;
			while (header >> s) { if (s=="timescale") break; }
			header >> scale;
		}
		while(getline(in, line, '\n')) {
			// sampleId   2269, size     2 duration      126 time  6856686 01:54:16.686 S
			stringstream l(line);
			l >> skipws;
			string x;
			int sampleId, size, duration, time;
			string start;
			l >> x >> sampleId >> x >> x >> size >> x >> duration >> x >> time >> start;
			if (size==2) continue;
			string end = addDuration(start, scale, duration);
			out << (id+1) << "\n" << start << " --> " << end << "\n" << subs[id] << "\n";
			++id;
		}
		in.close();
		out.close();
	}
	
	unlink(track_dump.c_str());
}

void process_file(const string& path) {
	stringstream ss;
	ss << "tracklist." << (int) getpid() << ".txt";
	string tracklist(ss.str());

	ss.str("");
	// replace 'gsed' with 'sed' on systems where 'sed' is GNU sed
	ss << "mp4track --list \"" << path << "\" | gsed ':a;N;$!ba;s/\\n/|/g;s/\\(.\\) */\\1/g;s/|track/\\ntrack/g' >" << tracklist;
	system(ss.str().c_str());

	ifstream tracks(tracklist.c_str(), ios::in);
	if (tracks.is_open()) {
		string tmp;
		int current_track=-1;
		bool is_sbtl(false);
		bool has_lang(false);
		string language("Unknown");
		while(getline(tracks, tmp, '\n')) {
			stringstream track_desc; track_desc << tmp;
			string tmp2;
			while(getline(track_desc, tmp2, '|')) {
				if (tmp2.compare(0, 6, "track[")==0) {
					stringstream trackid_desc;
					string zzz;
					trackid_desc << tmp2;
					getline(trackid_desc, zzz, '=');
					trackid_desc >> current_track;
					is_sbtl = false;
					has_lang= false;
				}
				else if (tmp2.compare("type=(sbtl)")==0) {
					is_sbtl=true;
				}
				else if (tmp2.compare(0, 9, "language=")==0) {
					stringstream lang_desc; lang_desc << tmp2;
					getline(lang_desc, language, '=');
					getline(lang_desc, language, '=');
					has_lang=true;
				}
				//
				if (is_sbtl && has_lang) {
					process_track(path, current_track, language);
					is_sbtl=false;
					has_lang=false;
				}
			}
		}
		tracks.close();
	}
}

int main(int argc, const char* argv[]) {
	if (argc<2) {
		cerr << "Usage: " << argv[0] << " <file1.mp4> [file2.mp4 [file3.mp4 […]]]\n";
		return 1;
	}

	int arg(0);
	while(++arg!=argc) {
		cout << "* Processing \"" << argv[arg] << "\"…\n";
		process_file(argv[arg]);
	}
	return 0;
}

