// IOTest.cpp : Defines the entry point for the console application.
//

#include "IO.h"

void StringTest(){
	string* ls1 = new string[3];
	ls1[0] = "a";
	ls1[1] = "bb";
	ls1[2] = "ccc";
	for(int i=0; i<3; ++i){
		cout << ls1[i] << endl;
	}
	delete[] ls1;

    vector<string> vs;
    vs.push_back("1");
    vs.push_back("22");
    vs.push_back("333lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll");
    FILE *fp = fopen("string.txt","wb");
	fwrite(&vs[0],sizeof(vs[0]),vs.size(),fp);
	fclose(fp);
	fp = fopen("string.txt","rb");
	string *ls = new string[3];
	fread(ls,sizeof(string),3,fp);
	for(int i=0; i<3; ++i){
		cout << ls[i] << endl;
	}
	//delete[] ls;
	fclose(fp);
	cout << "---------------------------------------------------" << endl;
}

int main(int argc, char* argv[])
{
	
	//StringTest();
	cout << "sizeof(string): " << sizeof(string) << endl;
	IO io;
	// "D:\\search_task_20160328143341.spectra"
	string inputpath = "ptop1.spectra";
	if (argc > 1){
		inputpath = argv[1];
	}
	vector<PrSM> prsms;
	//io.testIO();

	io.fscanfTest(inputpath, prsms);
	prsms.clear();
	io.fscanfTest1(inputpath, prsms);
	prsms.clear();
	io.fscanfTest2(inputpath, prsms);
	io.fprintfTest(inputpath, prsms);
	prsms.clear();
	io.ifstreamTest(inputpath, prsms);
	io.ofstreamTest(inputpath, prsms);
	//string path2 = io.fwriteTest(inputpath, prsms);
	//prsms.clear();
	//io.freadTest(path2, prsms);
	io.fwriteSprintfTest(inputpath, prsms);
	prsms.clear();
	io.freadSscanfTest(inputpath, prsms);
	return 0;
}

