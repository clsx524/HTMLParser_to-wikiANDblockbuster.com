#include "Parse_model.h"

string MOV_TV::getName(){
        return this->name;
}

int MOV_TV::getYear(){
        return this->year;
}

string MOV_TV::getContent(){
        return this->content;
}

MOV_TV_Linker MOV_TV::getNext(){
    return this->next;
}

string MOV_TV::getDirector(){
    return this->director;
}

string MOV_TV::getChannel(){
    return this->channel;
}

int MOV_TV::getLength(){
    return this->length;
}

string MOV_TV::getWikiContent(){
    return this->wikicontent;
}

string MOV_TV::getCast(){
    return this->cast;
}

string MOV_TV::getImageURL(){
    return this->imageurl;
}

void MOV_TV::setNext(MOV_TV_Linker Node){
    this->next = Node; 
}

void MOV_TV::setName(string name){
    this->name = name;
}

void MOV_TV::setYear(int year){
    this->year = year;
}

void MOV_TV::setChannel(string channel){
    this->channel = channel;
}

void MOV_TV::setContent(string content){
    this->content = content;
}

void MOV_TV::addDirector(string name){
    string x = this->getDirector();
    x = x + name;
    this->director = name;
}

void MOV_TV::clearDirector(){
    this->director = "";
}


void MOV_TV::setLength(int length){
    this->length = length;
}

void MOV_TV::addWikiContent(string wiki){
    string x = this->getWikiContent();
    x = x + wiki;
    this->wikicontent = x;
}

void MOV_TV::addCast(string cast){
    string x = this->getCast();
    x = x + cast;
    this->cast = x;
}

void MOV_TV::clearCast(){
    this->cast = "";
}

void MOV_TV::clearWikiContent(){
    this->wikicontent = "";
}

void MOV_TV::setImageURL(string url){
    this->imageurl = url;
}

/*//////////////////////////////////////////////////////////////////
Implementation: Basic Parse elements
Content: 
1. Html_Parse: Parse url and get the html file back
2. Image_Parse: Parse url and get the image file back
3. Image_Saver: Write Function for saving image
///////////////////////////////////////////////////////////////////*/

int Curl_Parse::Html_Parse(string name, string url){
    string agent = "Hello blockbuster!/This is a user!";

    int flag = 0;
    CURL *curl;  
    CURLcode result; 
    FILE *my_file;
    curl = curl_easy_init();
    if(curl){
        flag = 1;
        my_file = fopen(name.c_str(), "wb");
        // Set up curl
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        //set default fwrite
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL); 
        //write the data to the file
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, my_file);
        //show the verbose information of programming
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //show the parsing progress
        //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        //wikipedia query criteria
        curl_easy_setopt(curl, CURLOPT_USERAGENT, agent.c_str());

        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        if (result == CURLE_OK)
            cout << "Success"<<endl;

        curl_global_cleanup(); 
        fclose(my_file);
    }   

    return flag;
}

size_t Image_Saver(void* ptr, size_t size, size_t nmemb, void* stream){
    return fwrite(ptr, size, nmemb, (FILE*)stream);
}

void Curl_Parse::Image_Parse(string name, string url){
    string filename = "[Image]" + name + ".jpg";
    string agent = "Hello Blockbuster!/1.0";

    CURL *curl;  
    CURLcode result; 
    FILE *my_file;
    curl = curl_easy_init();
    if(curl){
        my_file = fopen(filename.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Image_Saver); 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, my_file);

        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (result == CURLE_OK)
            cout << "Get Image Success"<<endl;

        curl_global_cleanup(); 
        fclose(my_file);
    }   
}


/*//////////////////////////////////////////////////////////////////
Implementation: Basic Parse elements
Content:
String manipulation: 
1. split: Parse url and get the html file back
2. Simple_Line: Parse url and get the image file back
3. Wiki_Lookup_Format: Write Function for saving image
4. Wiki_Rev_Lookup_Format: 
5. NumberToString: 
///////////////////////////////////////////////////////////////////*/

vector<string> WebParser::split(const string& line) {
    int j = 0, k = 0;
    vector<string> arg;
    for (vector<string>::size_type i = 0; i != line.length(); ++i)
    {
        if (line.at(i) == ' ')
        {
            arg.push_back(line.substr(j,k));
            j = i+1;
            k = 0;
        } else if (i == line.length()-1) {
            arg.push_back(line.substr(j));
        } else {
            k++;
        }
    }
    return arg;
}

string WebParser::NumberToString(int number)
{
   stringstream ss; 
   ss << number; 
   return ss.str(); 
}

void WebParser::Simple_Line(string &line, int name){
    std::string::size_type found, gap;
    while(1){
        if((found = line.find("<")) != std::string::npos){
	        gap = line.find(">") - found + 1;
            line.erase(found, gap);
        }else if((found = line.find("[")) != std::string::npos){
	       if(line.find("]") != std::string::npos){
	           gap = line.find("]") - found + 1;
                line.erase(found, gap);
	        }else
	           line.erase(found,1);
            }else if((found=line.find("&amp;")) != std::string::npos){
                line.erase(found,5);
                line.insert(found,"&");
            }else if((found=line.find("\t")) != std::string::npos){
                line.erase(found,1);
            }else if((found=line.find("&#039;")) != std::string::npos){ 
                line.erase(found,6);
                line.insert(found,"'");
            }else if((found=line.find("Vs.")) != std::string::npos){
                line.erase(found,3);
	            line.insert(found,"vs.");
	        }else if((found=line.find("/")) != std::string::npos && name){
	            line.erase(found,1);
	        }else if((found=line.find("~")) != std::string::npos){
                line.erase(found,line.size()-found+1);
            }else if((found=line.find("&eacute;")) != std::string::npos){
	            line.erase(found, 8);
	            line.insert(found,"e");
	        }else if((found=line.find("\u00E9")) != std::string::npos){
	            line.erase(found,2);
	            line.insert(found,"e");
	        }else if((found=line.find("&aacute;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"a");                
            }else if((found=line.find("\u00E1")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"a"); 
            }else if((found=line.find("&iacute;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"i");                
            }else if((found=line.find("\u00ED")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"i");   
            }else if((found=line.find("&ouml;")) != std::string::npos){
                line.erase(found, 6);
                line.insert(found,"o");                
            }else if((found=line.find("\u00F6")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"o"); 
            }else if((found=line.find("&auml;")) != std::string::npos){
                line.erase(found, 6);
                line.insert(found,"a");                
            }else if((found=line.find("\u00E4")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"a"); 
            }else if((found=line.find("&quot;")) != std::string::npos){
                line.erase(found,6);
                line.insert(found,"\""); 
            }else if((found=line.find("&#034;")) != std::string::npos){
                line.erase(found,6);
                line.insert(found,"\"");  
            }else if((found=line.find("&uuml;")) != std::string::npos){
                line.erase(found, 6);
                line.insert(found,"u");                
            }else if((found=line.find("\u00FC")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"u");         
            }else if((found=line.find("&ntilde;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"u");                
            }else if((found=line.find("\u00F1")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"u");      
            }else if((found=line.find("&ograve;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"o");                
            }else if((found=line.find("\u00F2")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"o");          
            }else if((found=line.find("&agrave;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"a");                
            }else if((found=line.find("\u00E0")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"a");        
            }else if((found=line.find("&ugrave;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"u");                
            }else if((found=line.find("\u00F9")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"u"); 
            }else if((found=line.find("&frac12;")) != std::string::npos){
                line.erase(found, 8);              
            }else if((found=line.find("\u00BD")) != std::string::npos){
                line.erase(found,2);  
            }else if((found=line.find("&deg;")) != std::string::npos){
                line.erase(found, 5);
                line.insert(found,"degree");                              
            }else if((found=line.find("\u00B0")) != std::string::npos){
                line.erase(found,2); 
                line.insert(found,"degree");
            }else if((found=line.find("&Eacute;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"E");                
            }else if((found=line.find("\u00C9")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"E");  
            }else if((found=line.find("&uacute;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"u");                
            }else if((found=line.find("\u00FA")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"u");  
            }else if((found=line.find("&oacute;")) != std::string::npos){
                line.erase(found, 8);
                line.insert(found,"o");                
            }else if((found=line.find("\u00F3")) != std::string::npos){
                line.erase(found,2);
                line.insert(found,"o");                
            }else
                break;
    }
}
void WebParser::Wiki_Lookup_Format(string &line){
    //replace all the " " to "_" for wiki lookup
    std::string::size_type found;
    while(1)
        if((found = line.find(" ")) != std::string::npos)
            replace(line.begin(),line.end(),' ','_');
        else
            break;
}

void WebParser::Wiki_Rev_Lookup_Format(string &line){
    //replace all the " " to "_" for wiki lookup
    std::string::size_type found;
    while(1)
        if((found = line.find("_")) != std::string::npos)
            replace(line.begin(),line.end(),'_',' ');
        else
            break;
}

bool WebParser::isNum(char c){
    if(c >= '0' && c <= '9')
        return true;
    return false;
}

int WebParser::URL_Title_Parse(string in_filename, ofstream &out_Pfile, ofstream &out_TIfile){

    int max = 0;
    string line, line2;
    vector<string> arg;
    std::string::size_type infosta, infoend;

    ifstream inputfile;
    inputfile.open(in_filename.c_str());

    if(!inputfile.is_open()){
        cout<< "The file " << in_filename << " fails to be opened!" <<endl;
        return 0;
    }

    while(1){
        if(inputfile.eof())
            break;
        else{
            getline(inputfile, line);
        }

        arg = split(line);

        if(line.find("<a title=") != std::string::npos && line.find("<div class=") != std::string::npos && line.find("<div class=\"main sizeBBoxart") != std::string::npos ){
            line2 = line;
            //process the title
            infosta = line.find("<a title=\"");
            line.erase(0, infosta+10);
            infoend = line.find("\" ");
            line.erase(infoend, line.length());
            Simple_Line(line,1);   
            Wiki_Lookup_Format(line);
            if(line.c_str()){
                out_TIfile<<line<<endl;
//                cout<<"TI: "<<line<<endl;
            }
            //process the webpage
            infosta = line2.find(" href=\"");
            line2.erase(0, infosta+7);
            infoend = line2.find("\" ");
            line2.erase(infoend, line2.length()); 
            line2 = "http://www.blockbuster.com" + line2; 
            Simple_Line(line2,0); 
            if(line.c_str()){
                out_Pfile<<line2<<endl;
//                cout<<"WP: "<<line2<<endl;
            }            
        }
        if(line.find("DISPLAY OPTIONS") != std::string::npos){
	        Simple_Line(line,1);
	        infosta = line.find("DISPLAY OPTIONS");;
            string linenum = line.substr(infosta-2,2).c_str();
            if(! isNum(linenum.at(0)))
                linenum = linenum.substr(1,1);
            max = atoi(linenum.c_str());
	   }
    }

    return max;
}

MOV_TV_Linker WebParser::getLink(string title, MOV_TV_Linker head){
    MOV_TV_Linker ptr = head->getNext();
    while(ptr != NULL && ptr->getName() != title){
        ptr = ptr->getNext();
    }
    if(ptr == NULL)
        return NULL;
    else
        return ptr;    
}


unsigned WebParser::FindColoumn(string line, string target){
  unsigned found = line.find(target.c_str());
  while(found != 0){
    if(line.at(found) == ':')
      break;
    found--;
  }
  return found;
}


unsigned WebParser::FindComma(string line, string target){
  unsigned found = line.find(target.c_str());
  while(found != 0){
    if(line.at(found) == ',')
      break;
    found--;
  }
  return found;
}


unsigned WebParser::FindBracket(string line, string target){
  unsigned found = line.find(target.c_str());
  while(found != 0){
    if(line.at(found) == '[')
      break;
    found--;
  }
  return found;
}

void WebParser::ProcessName(string &name){
  if(name.find("_Season") != std::string::npos && name.find(":") != std::string::npos){
    if(FindColoumn(name, "_Season") == 0)
      name.erase(name.find(":"), name.size()-name.find(":"));
    else
      name.erase(FindColoumn(name, "_Season"), name.size()-FindColoumn(name, "_Season"));
  }
    if(name.find("_Series") != std::string::npos && name.find(":") != std::string::npos){
    if(FindColoumn(name, "_Series") == 0)
      name.erase(name.find(":"), name.size()-name.find(":"));
    else
      name.erase(FindColoumn(name, "_Series"), name.size()-FindColoumn(name, "_Series"));
  }
  if(name.find("_Set") != std::string::npos && name.find(":") != std::string::npos){
    if(FindColoumn(name, "_Set") == 0)
      name.erase(name.find(":"), name.size()-name.find(":"));
    else
      name.erase(FindColoumn(name, "_Set"), name.size()-FindColoumn(name, "_Set"));
  }
  if(name.find("_Episode") != std::string::npos && name.find(":") != std::string::npos){
    if(FindColoumn(name, "_Episode") == 0)
      name.erase(name.find(":"), name.size()-name.find(":"));
    else
      name.erase(FindColoumn(name, "_Episode"), name.size()-FindColoumn(name, "_Episode"));
  }
  if(name.find("_Operation") != std::string::npos && name.find(":") != std::string::npos){
    if(FindColoumn(name, "_Operation") == 0)
      name.erase(name.find(":"), name.size()-name.find(":"));
    else
      name.erase(FindColoumn(name, "_Operation"), name.size()-FindColoumn(name, "_Operation"));
  }
  if(name.find("_Volume") != std::string::npos && name.find(":") != std::string::npos){
    if(FindColoumn(name, "_Volume") == 0)
      name.erase(name.find(":"), name.size()-name.find(":"));
    else
      name.erase(FindColoumn(name, "_Volume"), name.size()-FindColoumn(name, "_Volume"));
  }
  if(name.find("Vol.") != std::string::npos && name.find(",") != std::string::npos){
        if(FindComma(name, "Vol.") == 0)
            name.erase(name.find(","), name.size()-name.find(","));
        else
            name.erase(FindColoumn(name, "Vol."), name.size()-FindColoumn(name, "Vol."));
  }
  if(name.find(" [TV Series]") != std::string::npos){
    name.erase(name.find(" [TV Series]"), name.size()-name.find(" [TV Series]"));
  }
  if(name.find("Discs]") != std::string::npos){
    name.erase(FindBracket(name, "Discs]"), name.size()-FindBracket(name, "Discs]"));
  }

    return;
}



void WebParser::Info_Detail_Parse(string filename, MOV_TV_Linker ptr){
    
    MOV_TV_Linker NewNode = new MOV_TV;
    ptr->setNext(NewNode);
    string line; 
    int nextflag = 0;
    vector<string> arg;
    std::string::size_type found;

    ifstream inputfile;
    inputfile.open(filename.c_str()); 

    if(!inputfile.is_open()){
        cout<< "The file " << filename << " fails to be opened!" <<endl;
        return;
    }

    while(1){
        if(inputfile.eof())
            break;
        else{
            getline(inputfile, line);
        }

        arg = split(line);

        if((found = line.find("<h1>")) != std::string::npos){
            Simple_Line(line,1);
            Wiki_Lookup_Format(line);
            NewNode->setName(line);
            nextflag = 1;
        }else if(nextflag){
            found = line.find("(");
            int year = atoi(line.substr(found+1,found+5).c_str());
            NewNode->setYear(year);
            nextflag = 0;
        }else if(line.find("<h2>Synopsis of") != std::string::npos){
            found = line.find("<p");
            line.erase(0,found+3);
            found = line.find("Theatrical Feature Running Time:");
            NewNode->setLength(atoi(line.substr(line.find("<dd>")+4, line.find("&nbsp;")-line.find("<dd>")-4).c_str()));
            Simple_Line(line,1);
            NewNode->setContent(line);   
        }else if((found = line.find("<dt>Director(s):")) != std::string::npos){
      line.erase(0,found);
            found = line.find("<a href=");
            line.erase(0,found);
            found = line.find(">");
            line.erase(0,found+1);
            found = line.find("</a>");
            line.erase(found,line.size()-found+1);
            NewNode->addDirector(line);
        }else if(line.find("Theatrical Feature Running Time:") != std::string::npos){
            found = line.find("Theatrical Feature Running Time:");
            NewNode->setLength(atoi(line.substr(line.find("<dd>")+4, line.find("&nbsp;")-line.find("<dd>")-4).c_str()));

        }else if(line.find("<meta name=\"og:image\"") != std::string::npos){
            line.erase(0,line.find("<meta name=\"og:image\""));
            found = line.find("http://");
            line = line.substr(found, line.find("\"/>")-found);
            NewNode->setImageURL(line);
        }
    }
}

int WebParser::Wiki_URL_Title_Parse_MOV(string in_filename, string title, MOV_TV_Linker head){
    int MatchFlag = 0, castflag = 0; //flag for return value
    string line, line2;
    string title_file = "[file]" + title + ".txt"; 
    vector<string> arg;

    MOV_TV_Linker ptr = getLink(title, head);

    ifstream inputfile;
    inputfile.open(in_filename.c_str());

    if(!inputfile.is_open()){
        cout<< "The file " << in_filename << " fails to be opened!" <<endl;
        return 0;
    }

    while(1){
        if(inputfile.eof()){
            if( MatchFlag == 0 ){
                ptr->clearCast();
                ptr->clearWikiContent();
            }
            return MatchFlag;
        }else{
            getline(inputfile, line);
        }

        arg = split(line);

        if(line.find("<p") == 0 && castflag == 0){
            Simple_Line(line,1);   
            if(line.compare("Other reasons this message may be displayed:")){
                line = line + "\n";
                ptr->addWikiContent(line);
            }
        }

        if((line.find("Cast") != std::string::npos || line.find("cast") != std::string::npos) && line.find("<h") != std::string::npos){
	        if(atoi(line.substr(line.find("section=")+8,1).c_str())<5){
	           castflag = 1;
                getline(inputfile,line);
	            int counter = 0;
                while(line.find("<h") == std::string::npos){
	                if(line.find("<ul>") != std::string::npos)
		              counter++;
	                else if(line.find("</ul>") != std::string::npos)
		              counter--;
			        else{
	                    if(counter > 0 && line.find("<li>") != std::string::npos){
                            Simple_Line(line,1);
                            if(line.find(" as") != std::string::npos)
		                      line.erase(line.find(" as"), line.size()-line.find(" as")+1);
		                    else if(line.find(":") != std::string::npos)
		                      line.erase(line.find(":"), line.size()-line.find(":")+1);
		                    else if(line.find("...") != std::string::npos)
		                      line.erase(line.find("..."), line.size()-line.find("...")+1);
                            line = line + ",";
                            ptr->addCast(line);
                        }
			        }
		            getline(inputfile,line);
                }
	        }
        }

        if((line.find("Plot") != std::string::npos || line.find("Cast") != std::string::npos) && line.find("<h") !=  std::string::npos){
            MatchFlag = 1;
        }
    }
}


int WebParser::Wiki_URL_Title_Parse_TV(string in_filename, string title, MOV_TV_Linker head){
    int ContentFlag = 0, MatchFlag = 0, sectioncounter = 3; //flag for return value
    string line, line2;
    string title_file = "[file]" + title + ".txt"; 
    vector<string> arg;

    MOV_TV_Linker ptr = getLink(title, head);

    ifstream inputfile;
    inputfile.open(in_filename.c_str());

    if(!inputfile.is_open()){
        cout<< "The file " << in_filename << " fails to be opened!" <<endl;
        return 0;
    }

    while(1){
        ContentFlag = 0;
        if(inputfile.eof()){
            if( MatchFlag == 0 ){
                ptr->clearCast();
                ptr->clearWikiContent();
            }
            return MatchFlag;
        }else{
            getline(inputfile, line);
        }

        arg = split(line);

        if(line.find("<tr") == 0){
            while(line.find("</tr>") == std::string::npos){
                Simple_Line(line,1);   

                switch(ContentFlag){
                  case 1: 
                      line = line + ",";
                      ptr->addDirector(line);break;
                  case 2: 
                      line = line + ",";
                      ptr->addCast(line);break;
                  case 3: 
                      ptr->setChannel(line);break;
                   default: ;
                }

                if(line.find("Created by") != std::string::npos || line.find("Developed by") != std::string::npos)
                    ContentFlag = 1;// the director
                if(line.find("Starring") != std::string::npos)
                    ContentFlag = 2;// the stars
                if(line.find("Original channel") != std::string::npos)
                    ContentFlag = 3;// the channels

                getline(inputfile, line);               
            }

        }

        if(line.find("<p") == 0 && sectioncounter){
            Simple_Line(line,1);
            ptr->addWikiContent(line);
        }

        if(line.find("<h2>") != std::string::npos && sectioncounter){
            sectioncounter--;
            if(line.find("Production") != std::string::npos)
                sectioncounter = 0;
        }

        if((line.find("Production") != std::string::npos || line.find("Plot") != std::string::npos) && line.find("<h") != std::string::npos){
            MatchFlag = 1;
        }
    }
}

int WebParser::Main_Page_Parse(string url, ofstream &out_Pfile, ofstream &out_TIfile){
    string filename = "output.txt";
    int max;
    if(Html_Parse(filename, url)){
        max = URL_Title_Parse(filename, out_Pfile, out_TIfile);
        remove(filename.c_str());  
    }
    return max;
}

void WebParser::Wiki_Page_Parse(string title, string wiki_baseurl, MOV_TV_Linker head, bool movortv){
    string filename = title + "_temp.txt"; 
    Wiki_Lookup_Format(title);
    int yearint = getLink(title, head)->getYear();
    int judge;
    if(yearint == 0)
        judge = 2;
    else 
        judge = 3;
    string year = NumberToString(yearint);
    string wikiurl1, wikiurl2, wikiurl3;


    if(movortv){
        wikiurl1= wiki_baseurl + title;
        wikiurl2= wiki_baseurl + title + "_(film)";
        wikiurl3= wiki_baseurl + title + "_(" + year + "_film)"; 
    }else{
        string lookup_title = title;
        ProcessName(lookup_title); 
        wikiurl1= wiki_baseurl + lookup_title;
        wikiurl2= wiki_baseurl + lookup_title + "_(TV_series)";
        wikiurl3= wiki_baseurl + lookup_title + "_(" + year + "_TV_series)";   
    }
    string wiki_url[3];

    if(judge == 2){
        wiki_url[0] = wikiurl2; wiki_url[1] = wikiurl1; wiki_url[2] = wikiurl3;
    }else{
        wiki_url[0] = wikiurl2; wiki_url[1] = wikiurl3; wiki_url[2] = wikiurl1;}


    for(int i = 0; i < judge ; i++){
        if(Html_Parse(filename, wiki_url[i])){
            //Extract the info
            if(movortv){
                if(Wiki_URL_Title_Parse_MOV(filename, title, head)){
    	            remove(filename.c_str());  
                    break;
                }
            }else{
                if(Wiki_URL_Title_Parse_TV(filename, title, head)){
                    remove(filename.c_str());  
                    break;
                }  
            }              
            remove(filename.c_str());  
        }
    }
}

void WebParser::Detail_Page_Parse(string Pageurl, MOV_TV_Linker ptr){
    string filename = "detailed.txt";
    if(Html_Parse(filename, Pageurl)){
        //Extract the info
        Info_Detail_Parse(filename, ptr);
        remove(filename.c_str());  
    }   
}

void WebParser::PrinttoTerminal(){
    MOV_TV_Linker ptr = Head->getNext();
    ofstream out_file("Movieblock_detailed.txt");   
    while(ptr != NULL){
      string name = ptr->getName();
      Wiki_Lookup_Format(name);
      string Content = ptr->getContent();
      int year = ptr->getYear();
        out_file<<"Movie: "<< name <<endl;
        out_file<<"Year: "<< year <<endl;
        out_file<<"Content: "<< Content <<endl;
        out_file<<endl;
        ptr = ptr->getNext();
    }
}


void WebParser::Freestruct(){
    MOV_TV_Linker ptr = Head;
    while(ptr != NULL){
        MOV_TV_Linker temp = ptr;
        ptr = ptr -> getNext();
        delete temp;
    }
}



void WebParser::PrinttoFile_MOV(){
    MOV_TV_Linker ptr = Head->getNext();
    int counter = 0;
    while(ptr != NULL){
        cout<< counter++ << "-";
        string filename = "[Movie]"+ ptr->getName() + ".txt";
        ofstream outfile(filename.c_str());
        outfile << ptr->getName() << endl;
        outfile << "Released Year: "<< ptr->getYear() << endl;
        outfile << "Running time: " << ptr->getLength() << endl;
        outfile << "Director: " << ptr->getDirector() << endl;
        outfile << "Cast: " << ptr->getCast() << endl;
        outfile << "Content: " << ptr->getContent() << endl;
        outfile << "Wiki info: " <<endl;
        outfile << ptr->getWikiContent() <<endl;
        Image_Parse(ptr->getName(), ptr->getImageURL());
	    ptr = ptr->getNext();
    }
}

void WebParser::PrinttoFile_TV(){
    MOV_TV_Linker ptr = Head->getNext();
    int counter = 0;
    while(ptr != NULL){
        cout<< counter++ << "-" << ptr->getName()<< "-";
        string filename = "[TV series]"+ ptr->getName() + ".txt";
        ofstream outfile(filename.c_str());
        outfile << ptr->getName() << endl;
        outfile << "Released Year: "<< ptr->getYear() << endl;
        outfile << "Running time: " << ptr->getLength() << endl;
        outfile << "Director: " << ptr->getDirector() << endl;
        outfile << "Original Channel: " << ptr->getChannel() << endl;
        outfile << "Cast: " << ptr->getCast() << endl;
        outfile << "Content: " << ptr->getContent() << endl;
        outfile << "Wiki info: " <<endl;
        outfile << ptr->getWikiContent() <<endl;
        Image_Parse(ptr->getName(), ptr->getImageURL());
        ptr = ptr->getNext();
    }
}


WebParser::~WebParser(){
   Freestruct(); 
}

void WebParser::setHead(MOV_TV_Linker head){
    this->Head = head;
}

MOV_TV_Linker WebParser::getHead(){
    return this->Head;
}

int main(int argc, char* argv[])
{
	//input need to be pages for blockbuster.com
	//e.g:   ./Parse_model M http://www.blockbuster.com/browse/movieGenres/actionAdventure
	//e.g:   ./Parse_model T http://www.blockbuster.com/browse/television/tvActionAdventure
        if(argc != 3){
            cout << "The number of input argument is wrong";
            return 1;
        }
        string url = argv[2];
        string choose = argv[1];
        bool movortv;
        if(choose == "M")
            movortv = true;
        else if(choose == "T")
            movortv = false;
        else{
            cout << "The second argument is not T or M";
            return 1;            
        }

        //1. Get the MOV_TV title information from blockbuster websites
        ofstream out_Pfile, out_TIfile;     
        out_Pfile.open("WebPageResult.txt");
        out_TIfile.open("TitleResult.txt");

        WebParser* Parser = new WebParser;

        cout<<"working on Page 1"<<endl;
        url = url + "?pg.1.page=1&pg.1.pageSize=25";
	    int maxvalue = Parser->Main_Page_Parse(url, out_Pfile, out_TIfile);

	    for(int i = 2; i <= maxvalue; i++){      
            cout<<"working on Page "<< i <<endl;
            string num = Parser->NumberToString(i);
            string urlx = "page=" + num + "&pg.1.pageSize=25";
            url.replace(url.find("page="),url.find("page=")+6,urlx.c_str());
            Parser->Main_Page_Parse(url, out_Pfile, out_TIfile);
	    } 

        //2. Get the detailed list information from blockbuster websites for each film and generate structure
        ifstream in_Pfile("WebPageResult.txt");
        string Pageurl;
        cout << "Getting blockbuster webpage and form structure started!" << endl;

        MOV_TV_Linker ptr = new MOV_TV;
        Parser->setHead(ptr);
        int count = 0;
        while(getline(in_Pfile, Pageurl)){
            Parser->Detail_Page_Parse(Pageurl, ptr);
            cout<< ++count <<"-getting details in local:"<<ptr->getNext()->getName()<<endl;
            ptr = ptr->getNext();	  
        }

        //3. Query to wiki and use different url to get the film information
        //three different situation: simple name/ name + (film) / name + (year film)
        ifstream in_TIfile("TitleResult.txt");
        string title;
        cout << "Getting info from wiki start!" << endl;
        int count2 = 1;
        while(getline(in_TIfile, title)){
           cout<< count2++ << " out of " << count << " /working on:"<<title<<endl;
	       Parser->Wiki_Page_Parse(title, "http://en.wikipedia.org/wiki/", Parser->getHead(), movortv);
    }
    if(movortv)
        Parser->PrinttoFile_MOV(); 
    else 
        Parser->PrinttoFile_TV();

    delete Parser;
   
    return 0;
}
