#ifndef _DIVINE_
#define _DIVINE_

#include <sstream>
#include <fstream>
#include <pthread.h>

const int BUFFER_SIZE = 10*1024;
const int MB = 1048576;
const int KB = 1024;
const int GB = 1073741824;

using namespace std;

pthread_mutex_t mutPct = PTHREAD_MUTEX_INITIALIZER;

struct output {
	float percentage;
	string mainFileName;
	string status;
} op;

class divine {
	string filename;
	long int partsize;
	string toc;
	string outputs;
public:
	void setFile(string name);
	void calcSize(string input);
	void bufRW(fstream in, fstream out, long int bufSize, char *buf, long int totalBytes); //Write the func
	void kfs(string strFN, string strFS);
	void getToc(string file);
	void kfm(string strTOC);
	void output();
	char *bufRead(string file, long int bufsize);
};

void divine::output() {
	cout<<outputs<<endl;
}

void divine::setFile(string name) {
	filename = name;
}

void divine::calcSize(string input) {
	stringstream s1;
	char c;
	s1<<input;
	s1>>partsize>>c;
	if(c=='k' || c=='K') {
		partsize *= KB;
	} else if(c=='m' || c=='M') {
		partsize *= MB;
	} else if(c=='g' || c=='G') {
		partsize *= GB;
	}
}

void divine::kfs(string strFN, string strFS) {
	// -------------------------------
	pthread_mutex_lock(&mutPct);
	op.percentage = 0.0;
	op.status.clear();
	pthread_mutex_unlock(&mutPct);
	// -------------------------------
	long int fsize;
	this->setFile(strFN);
	this->calcSize(strFS);
	stringstream sfename, inst;
	fstream in(filename.c_str(), ios::binary|ios::in|ios::ate);
	if(in.is_open()) {
		// ---------------------------
		pthread_mutex_lock(&mutPct);
		op.mainFileName = filename;
		pthread_mutex_unlock(&mutPct);
		// ---------------------------
		fsize = in.tellg();
		in.seekg(0, ios::beg);
		long int r = fsize, p;
		int i = 0;
		char * buffer = new char[BUFFER_SIZE+1];
		fstream out;
		inst<<filename<<";";
		int len = filename.length();
		while(partsize <= r) {
				i++;
				sfename<<filename<<".part"<<i;
				p = partsize;
				out.open(sfename.str().c_str(), ios::out|ios::binary);
				if(out.is_open()) {
					//-----------------------------------
					pthread_mutex_lock(&mutPct);
					op.status = "Writing "+sfename.str();
					pthread_mutex_unlock(&mutPct);
					//-----------------------------------
					while(p>=BUFFER_SIZE) {
						in.read(buffer, BUFFER_SIZE);
						out.write(buffer, BUFFER_SIZE);
						p -= BUFFER_SIZE;
						//--------------------------------------------
						pthread_mutex_lock(&mutPct);
						op.percentage += (float)BUFFER_SIZE/fsize*100;
						pthread_mutex_unlock(&mutPct);
						//--------------------------------------------
					}
					if(p>0) {
						in.read(buffer, p);
						out.write(buffer, p);
						//-------------------------------------
						pthread_mutex_lock(&mutPct);
						op.percentage += (float)p/fsize*100;
						pthread_mutex_unlock(&mutPct);
						//-------------------------------------
					}
				} else {
				    outputs += "Cannot open the file \""+sfename.str()+"\"\n";
				}
				out.close();
				inst<<sfename.str().c_str()<<";";
				sfename.str("");
				r -= partsize;
		}
		if(r>0) {
				i++;
				sfename<<filename<<".part"<<i;
				out.open(sfename.str().c_str(), ios::out|ios::binary);
				if(out.is_open()) {
					pthread_mutex_lock(&mutPct);
					op.status = "Writing "+sfename.str();
					pthread_mutex_unlock(&mutPct);
					p=r;
					while(p>=BUFFER_SIZE) {
						in.read(buffer, BUFFER_SIZE);
						out.write(buffer, BUFFER_SIZE);
						p -= BUFFER_SIZE;
						pthread_mutex_lock(&mutPct);
						op.percentage += (float)BUFFER_SIZE/fsize*100;
						pthread_mutex_unlock(&mutPct);
					}
					if(p>0) {
						in.read(buffer, p);
						out.write(buffer, p);
						pthread_mutex_lock(&mutPct);
						op.percentage += (float)p/fsize*100;
						pthread_mutex_unlock(&mutPct);
					}
				} else {
				    outputs += "Cannot open the file \"" + sfename.str() + "\"\n";
				}
				out.close();
				inst<<sfename.str().c_str()<<';';
				sfename.str("");
		}
		pthread_mutex_lock(&mutPct);
		op.percentage = 100;
		pthread_mutex_unlock(&mutPct);
		delete[] buffer;
	} else {
	    outputs += "Cannot open the file \"" + filename + "\"\n";
	}
	in.close();
	string instname = filename + ".toc";
	ofstream toc_out(instname.c_str(), ios::trunc);
	if(toc_out.is_open()) {
		toc_out<<inst.str().c_str();
	} else {
	    outputs += "Cannot open the file \"" + instname + "\"\n";
	}
	toc_out.close();
}

void divine::getToc(string file) {
	toc.assign("");
	ifstream tocfile(file.c_str());
	if(tocfile.is_open()) {
		getline(tocfile, toc);
	} else {
		outputs += "Cannot open the file \"" + file + "\".";
	}
}

void divine::kfm(string strTOC) {
	long int donebytes = 0;
	pthread_mutex_lock(&mutPct);
	op.percentage = 0;
	op.status.clear();
	pthread_mutex_unlock(&mutPct);
	this->getToc(strTOC);
	int emark = toc.find_first_of(';', 0), smark=0;
	string fname = toc.substr(smark, emark-smark);
	ofstream out(fname.c_str(), ios::binary);
	if(out.is_open()) {
		string pn;
		ifstream pin;
		char * data_block = new char[BUFFER_SIZE+1];
		data_block[BUFFER_SIZE] = '\0';
		if(data_block) {
			long int psize, r;
			while(emark<toc.find_last_of(';')) {
				smark = emark+1;
				emark = toc.find_first_of(';', smark);
				pn = toc.substr(smark, emark-smark);
				pin.open(pn.c_str(), ios::binary|ios::ate);
				if(pin.is_open()) {
					pthread_mutex_lock(&mutPct);
					op.status = "Main File: "+fname+"\nPart Name: "+pn;
					pthread_mutex_unlock(&mutPct);
					psize = pin.tellg();
					pin.seekg(0, ios::beg);
					// USE BUFFERED METHOD
					if(psize > BUFFER_SIZE) {
						r = psize;
						while(r>=BUFFER_SIZE) {
							pin.read(data_block, BUFFER_SIZE);
							data_block[BUFFER_SIZE] = '\0';
							out.write(data_block, BUFFER_SIZE);
							r -= BUFFER_SIZE;
							donebytes += BUFFER_SIZE;
							pthread_mutex_lock(&mutPct);
							op.percentage += (donebytes/psize*100);
							pthread_mutex_unlock(&mutPct);
						}
						if(r>0) {
							pin.read(data_block, r);
							data_block[r] = '\0';
							out.write(data_block, r);
							donebytes += r;
							pthread_mutex_lock(&mutPct);
							op.percentage += (donebytes/psize*100);
							pthread_mutex_unlock(&mutPct);
						}
					// USE NORMAL METHOD
					} else {
						pin.read(data_block, psize);
						out.write(data_block, psize);
						pthread_mutex_lock(&mutPct);
						op.percentage = 100;
						pthread_mutex_unlock(&mutPct);
					}
					pthread_mutex_lock(&mutPct);
					op.percentage = 100;
					pthread_mutex_unlock(&mutPct);
					pin.close();
				} else {
					outputs += "Cannot open the file \"" + pn + "\".\n";
					break;
				}
			}
			delete[] data_block;
		} else {
			outputs += "Cannot allocate memory for buffer.\n";
		}
	} else {
		out.close();
		outputs += "Cannot open the file \"" + fname + "\".\n";
	}
}

char *divine::bufRead(string file, long int bufsize) {
	fstream orig(file.c_str(), ios::out|ios::binary|ios::ate);
	if(orig.is_open()) {
		char *buffer = new char[bufsize+1];
		if(!buffer) return NULL;
		long int fsize = orig.tellg(), r = fsize;
		orig.seekg(0, ios::beg);
		if(r>bufsize) {
			while(r >= bufsize) {
				orig.read(buffer, bufsize);
				//
			}
		}
	}
}
#endif
