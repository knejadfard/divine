#include <iostream>
#include <pthread.h>
#include "divine.h"
#include <cmath>
#include <unistd.h> //for usleep function

using namespace std;

pthread_t thKfs;
int retval;

struct strInfo {
	string fname;
	string partsize;
	string listAddress;
} I1;

void *runDivine(void * arg) {
	strInfo *i1 = (strInfo*)arg;
	divine d1;
	d1.kfs(i1->fname, i1->partsize);
}

void *runDivineM(void * arg) {
        strInfo *i2 = (strInfo*)arg;
        divine d1;
        d1.kfm(i2->listAddress);
}

int main(int argc, char *argv[]) {
   string arg = argv[1];
   cout<<arg<<endl;
   if(arg == "s") {
	cout<<"File to split: ";
	string fileN;
	getline(cin, I1.fname);
	I1.partsize = "10m";
	retval = pthread_create(&thKfs, NULL, runDivine, (void*)&I1);
	cout<<endl;
	int c=0, check=0;
	pthread_mutex_lock(&mutPct);
	cout<<"Splitting File: "<<op.mainFileName<<endl;
	pthread_mutex_unlock(&mutPct);
	while(check != 1) {
		usleep(500);
		pthread_mutex_lock(&mutPct);
		cout<<"\r"<<op.status<<" --- "<<op.percentage<<" % ";
		if(op.percentage == 100) {
			check=1;
		}
		pthread_mutex_unlock(&mutPct);
			if(c==0) {
				cout<<" -   ";
				c++;
			} else if(c==1) {
				cout<<" \\   ";
				c++;
			} else if(c==2) {
				cout<<" |   ";
				c++;
			} else if(c==3) {
				cout<<" /   ";
				c = 0;
			}
		cout<<"      ";
	}
	pthread_join(thKfs, NULL);
	cout<<endl;
   } else if(arg == "m") {
	cout<<"List to merge: ";
        string fileN;
        getline(cin, I1.listAddress);
        //I1.partsize = "10m";
        retval = pthread_create(&thKfs, NULL, runDivineM, (void*)&I1);
        cout<<endl;
        int c=0, check=0;
        pthread_mutex_lock(&mutPct);
        cout<<"Splitting File: "<<op.mainFileName<<endl;
        pthread_mutex_unlock(&mutPct);
        while(check != 1) {
                usleep(500);
                pthread_mutex_lock(&mutPct);
                cout<<"\r"<<op.status<<" --- "<<op.percentage<<" % ";
                if(op.percentage == 100) {
                        check=1;
                }
                pthread_mutex_unlock(&mutPct);
                        if(c==0) {
                                cout<<" -   ";
                                c++;
                        } else if(c==1) {
                                cout<<" \\   ";
                                c++;
                        } else if(c==2) {
                                cout<<" |   ";
                                c++;
                        } else if(c==3) {
                                cout<<" /   ";
                                c = 0;
                        }
                cout<<"      ";
        }
        pthread_join(thKfs, NULL);
        cout<<endl;
   }
   return 0;
}
