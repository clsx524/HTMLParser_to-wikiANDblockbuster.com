#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

using namespace std;

class MOV_TV{
private:
	string name;
	int year;
	int length;
	string channel;
	string director;
	string content;
	string cast;
	string wikicontent;
	string imageurl;
	MOV_TV* next;

public:
    MOV_TV():name(""), year(0), length(0), channel(""), director(""), content(""), cast(""), wikicontent(""), imageurl(""), next(NULL){}
    MOV_TV* getNext();
    string getName();
    int getYear();
    string getContent();
    int getLength();
    string getChannel();
	string getDirector();
	string getCast();
	string getWikiContent();
	string getImageURL();
	void setNext(MOV_TV* Node);
	void setName(string name);
	void setYear(int year);
	void setContent(string content);
	void setChannel(string channel);
	void setLength(int length);
	void addDirector(string name);
	void clearDirector();
	void addWikiContent(string wiki);
	void addCast(string cast);
	void clearCast();
	void clearWikiContent();
	void setImageURL(string url);
};

typedef MOV_TV* MOV_TV_Linker;

size_t Image_Saver(void* ptr, size_t size, size_t nmemb, void* stream);

// Basic parser
class Curl_Parse{
public:
	int Html_Parse(string name, string url);
	void Image_Parse(string name, string url);	
};

// Parser for BlockBuster.com and wikipedia
class WebParser: public Curl_Parse{
private:
	//generated movie list
	MOV_TV_Linker Head;

	//string manipulation
	vector<string> split(const string& line);
	void Simple_Line(string &line, int name);
	void ProcessName(string &name);
	void Wiki_Lookup_Format(string &line);
	void Wiki_Rev_Lookup_Format(string &line);
	bool isNum(char c);
	unsigned FindColoumn(string line, string target);
	unsigned FindComma(string line, string target);
	unsigned FindBracket(string line, string target);

	//Movie Linker
	MOV_TV_Linker getLink(string title, MOV_TV_Linker head);

	//Webpage information extract
	int URL_Title_Parse(string in_filename, ofstream &out_Pfile, ofstream &out_TIfile);
	void Info_Detail_Parse(string filename, MOV_TV_Linker ptr);
	int Wiki_URL_Title_Parse_MOV(string in_filename, string title, MOV_TV_Linker head);
	int Wiki_URL_Title_Parse_TV(string in_filename, string title, MOV_TV_Linker head);
public:
	WebParser(): Head(NULL){}
	~WebParser();

	void setHead(MOV_TV_Linker head);
	MOV_TV_Linker getHead();
	
	//Three main parser functions:
	int Main_Page_Parse(string url, ofstream &out_Pfile, ofstream &out_TIfile);
	void Detail_Page_Parse(string Pageurl, MOV_TV_Linker ptr);	
	void Wiki_Page_Parse(string title, string wiki_baseurl, MOV_TV_Linker head, bool movortv);
	//peripheral functions:
	void PrinttoFile_TV();
	void PrinttoFile_MOV();
	void PrinttoTerminal();
	void Freestruct();
	string NumberToString ( int Number );
};
