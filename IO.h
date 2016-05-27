#ifndef _IO_H_
#define _IO_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <time.h>
#include <ctime>

using namespace std;

struct PrSM{
	string title;
	int scan;
	double precur_mass;
	int charge;
	double fdr;
	string sequence;
	double theory_mass;
	double diff_Da;
	double score;
	double evalue;
	string ptm;
	string specific;
	string ac;
	string pos_info;
	string label_type;
	bool isDecoy;
	int missing;
	static int decnt;
	PrSM(){
		title = "";
		scan = 0;
		precur_mass = 0;
		charge = 0;
		fdr = 0;
		sequence = "";
		theory_mass = 0;
		diff_Da = 0;
		score = 0;
		evalue = 0;
		ptm = "";
		specific = "";
		ac = "";
		pos_info = "";
		label_type = "";
		isDecoy = true;
		missing = 0;
	}
	~PrSM(){ 
		++ decnt;
		//cout << decnt << "destructor prsm..." << endl;
	}
	friend ifstream& operator>> (ifstream&, PrSM&);
	friend ofstream& operator<< (ofstream&, const PrSM&);
};

struct testObject0{
	double a;
	int b;
	bool c;
	char s[10];
	testObject0(){}
	testObject0(double _a, int _b, bool _c, char _s[]){
		a = _a;
		b = _b;
		c = _c;
		strcpy(s,_s);
	}
	~testObject0(){
		cout << "deconstructor testObject0..." << endl;
	}
};
struct testObject1{
	double a;
	int b;
	bool c;
	string s;
	testObject1(){}
	testObject1(double _a, int _b, bool _c, string _s){
		a = _a;
		b = _b;
		c = _c;
		s = _s;
	}
	~testObject1(){
		cout << "deconstructor testObject1..." << endl;
	}
};


class IO{
public:
	void testIO();

	void fscanfTest(string path, vector<PrSM>& prsms);
	void fscanfTest1(string path, vector<PrSM>& prsms);
	void fscanfTest2(string path, vector<PrSM>& prsms);
	void fprintfTest(string path, vector<PrSM>& prsms);
	void ifstreamTest(string path, vector<PrSM>& prsms);
	void ofstreamTest(string path, vector<PrSM>& prsms);
	void freadSscanfTest(string path, vector<PrSM>& prsms);
	void fwriteSprintfTest(string path, vector<PrSM>& prsms);
	void freadTest(string path, vector<PrSM>& prsms);
	string fwriteTest(string path, vector<PrSM>& prsms);

private:
	static int find_first_of(char *s, char ch, int pos);

};



#endif