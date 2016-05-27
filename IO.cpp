#include "IO.h"
#include "BasicTools.h"
using namespace std;

const int LINE_LEN = 20000;
const int MAX_COUNT = 20000;
const int BUFFER_SIZE = 200 * 1024 * 1024;

ifstream& operator>> (ifstream& ifs, PrSM& prsm){
	string tmp;
	ifs >> prsm.title >> prsm.scan >> prsm.precur_mass >> prsm.charge
		>> prsm.fdr >> prsm.sequence >> prsm.theory_mass >> prsm.diff_Da
		>> prsm.score >> prsm.evalue >> prsm.ptm >> prsm.specific
		>> prsm.ac >> prsm.pos_info >> prsm.label_type >> tmp
		>> prsm.missing;
	prsm.isDecoy = (tmp == "Decoy" ? true : false);
	return ifs;
}

ofstream& operator<< (ofstream& ofs, const PrSM& prsm){
	ofs << prsm.title << "\t" << prsm.scan << "\t" << prsm.precur_mass << "\t"
		<< prsm.charge << "\t" << prsm.fdr << "\t" << prsm.sequence << "\t"
		<< prsm.theory_mass << "\t" << prsm.diff_Da << "\t" << prsm.score << "\t"
		<< prsm.evalue << "\t" << prsm.ptm << "\t" << prsm.specific << "\t"
		<< prsm.ac << "\t" << prsm.pos_info << "\t" << prsm.label_type << "\t"
		<< (prsm.isDecoy ? "Decoy" : "Target") << "\t" << prsm.missing << endl;
	return ofs;
}



void IO::fscanfTest1(string path, vector<PrSM>& prsms){
	printf("*** fgets + sscanf ***\n");
	time_t start = clock();
	FILE *fp = fopen(path.c_str(), "r");
	if (fp == NULL){
		printf("Error: Can't open file %s\n", path.c_str());
		return;
	}
	char* line = new char[LINE_LEN];
	char* title= new char[200];
	char* seq = new char[10000];
	char* ptm = new char[2048];
	char* speci = new char[100];
	char* ac = new char[200];
	char* pos = new char[100];
	char* label = new char[100];
	char* isdecoy = new char[10];
	while (fgets(line,2048,fp)){
		if (strlen(line) == 0)  continue;
		PrSM prsm;		 
		sscanf(line,"%s\t%d\t%lf\t%d\t%lf\t%s\t%lf\t%lf\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%d",
			title, &prsm.scan, &prsm.precur_mass, &prsm.charge, &prsm.fdr, seq, &prsm.theory_mass, &prsm.diff_Da,
			&prsm.score, &prsm.evalue, ptm, speci, ac, pos, label, isdecoy, &prsm.missing);
		prsm.title = title;
		prsm.sequence = seq;
		prsm.ptm = ptm;
		prsm.specific = speci;
		prsm.ac = ac;
		prsm.pos_info = pos;
		prsm.label_type = label;
		prsm.isDecoy = (strcmp(isdecoy, "Decoy") == 0 ? true : false);
		prsms.push_back(prsm);
	}
	delete[] line;
	delete[] title;
	delete[] seq;
	delete[] ptm;
	delete[] speci;
	delete[] pos;
	delete[] label;
	delete[] isdecoy;
	fclose(fp);
	printf("PrSMs: %d\n", prsms.size());
	time_t end = clock();
	printf("Time used: %d\n\n", (end - start));
}

void IO::fscanfTest2(string path, vector<PrSM>& prsms){
	printf("*** fgets + split ***\n");
	time_t start = clock();
	FILE *fp = fopen(path.c_str(), "r");
	if (fp == NULL){
		printf("Error: Can't open file %s\n", path.c_str());
		return;
	}
	char* line = new char[LINE_LEN];
	while (fgets(line, LINE_LEN, fp)){
		vector<string> tmpv;
		CStringProcess::Split(line, "\t", tmpv);
		if (tmpv.size() < 17){
			printf("Error: invalid line %s\n", line);
			continue;
		}
		PrSM prsm;
		prsm.title = tmpv[0];
		prsm.scan = atoi(tmpv[1].c_str());
		prsm.precur_mass = atof(tmpv[2].c_str());
		prsm.charge = atoi(tmpv[3].c_str());
		prsm.fdr = atof(tmpv[4].c_str());
		prsm.sequence = tmpv[5];
		prsm.theory_mass = atof(tmpv[6].c_str());
		prsm.diff_Da = atof(tmpv[7].c_str());
		prsm.score = atof(tmpv[8].c_str());
		prsm.evalue = atof(tmpv[9].c_str());
		prsm.ptm = tmpv[10];
		prsm.specific = tmpv[11];
		prsm.ac = tmpv[12];
		prsm.pos_info = tmpv[13];
		prsm.label_type = tmpv[14];
		prsm.isDecoy = (tmpv[15] == "Decoy") ? true : false;
		prsm.missing = atoi(tmpv[16].c_str());
		prsms.push_back(prsm);
	}
	delete[] line;
	fclose(fp);
	printf("PrSMs: %d\n", prsms.size());
	time_t end = clock();
	printf("Time used: %d\n\n", (end - start));
}

void IO::fscanfTest(string path, vector<PrSM>& prsms){
	printf("*** fscanf ***\n");
	time_t start = clock();
	FILE *fp = fopen(path.c_str(), "r");
	if (fp == NULL){
		printf("Error: Can't open file %s\n", path.c_str());
		return;
	}
	char* line = new char[20000];
	char* title = new char[200];
	char* seq = new char[10000];
	char* ptm = new char[2048];
	char* speci = new char[100];
	char* ac = new char[200];
	char* pos = new char[100];
	char* label = new char[100];
	char* isdecoy = new char[10];
	while (!feof(fp)){
		PrSM prsm;
		fscanf(fp, "%s\t%d\t%lf\t%d\t%lf\t%s\t%lf\t%lf\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%d",
			title, &prsm.scan, &prsm.precur_mass, &prsm.charge, &prsm.fdr, seq, &prsm.theory_mass, &prsm.diff_Da,
			&prsm.score, &prsm.evalue, ptm, speci, ac, pos, label, isdecoy, &prsm.missing);
		if (strlen(title) > 0){
			prsm.title = title;
			prsm.sequence = seq;
			prsm.ptm = ptm;
			prsm.specific = speci;
			prsm.ac = ac;
			prsm.pos_info = pos;
			prsm.label_type = label;
			prsm.isDecoy = (strcmp(isdecoy, "Decoy") == 0 ? true : false);
			prsms.push_back(prsm);
		}
	}
	delete[] line;
	delete[] title;
	delete[] seq;
	delete[] ptm;
	delete[] speci;
	delete[] pos;
	delete[] label;
	delete[] isdecoy;
	fclose(fp);
	printf("PrSMs: %d\n", prsms.size());
	time_t end = clock();
	printf("Time used: %d\n\n", (end - start));

}
void IO::fprintfTest(string path, vector<PrSM>& prsms){
	path = "fprintfTest.txt";
	printf("*** fprintf ***\n");
	time_t start = clock();
	FILE *fp = fopen(path.c_str(), "w");
	if (fp == NULL){
		printf("Can't create file %s\n", path.c_str());
		return;
	}
	for (int i = 0; i < prsms.size(); ++i){
		fprintf(fp,"%s\t%d\t%lf\t%d\t%lf\t%s\t%lf\t%lf\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n",prsms[i].title.c_str(),prsms[i].scan,
			prsms[i].precur_mass, prsms[i].charge, prsms[i].fdr, prsms[i].sequence.c_str(), prsms[i].theory_mass, prsms[i].diff_Da,
			prsms[i].score, prsms[i].evalue, prsms[i].ptm.c_str(),prsms[i].specific.c_str(), prsms[i].pos_info.c_str(),
			prsms[i].ac.c_str(), prsms[i].label_type.c_str(), prsms[i].isDecoy?"Decoy":"Target", prsms[i].missing);
	}
	fclose(fp);
	time_t end = clock();
	printf("Time used: %d\n\n", (end - start));
}

void IO::ifstreamTest(string path, vector<PrSM>& prsms){
	cout << ("*** ifstream ***") << endl;
	time_t start = clock();
	ifstream ifs(path.c_str());
	ifs.sync_with_stdio(false);
	if (ifs){
		while (!ifs.eof()){
			PrSM prsm;
			ifs >> prsm;
			prsms.push_back(prsm);
		}
	}
	else{
		cout << "error: unable to open input file: " << path << endl;
		return;
	}
	ifs.sync_with_stdio(true);
	ifs.close();
	cout << "PrSMs: " << prsms.size() << endl;
	time_t end = clock();
	cout << ("Time used: ") << (end - start) << endl << endl;
	//std::cout.sync_with_stdio(true);
}

void IO::ofstreamTest(string path, vector<PrSM>& prsms){
	path = "ofstreamTest.txt";
	cout << ("*** ofstream ***") << endl;
	time_t start = clock();
	ofstream ofs(path.c_str());
	ofs.sync_with_stdio(false);
	if (ofs){
		for (int i = 0; i < prsms.size(); ++i){
			ofs << prsms[i];
		}
	}
	else{
		cout << "error: unable to open input file: " << path << endl;
		return;
	}
	ofs.sync_with_stdio(true);
	ofs.close();
	time_t end = clock();
	cout << ("Time used: ") << (end - start) << endl << endl;
	
}

int IO::find_first_of(char *s, char ch, int pos)
{
	int len = strlen(s);
	for (int i = pos; i < len; ++i){
		if (s[i] == ch){
			return i;
		}
	}
	return -1;
}

void IO::freadSscanfTest(string path, vector<PrSM>& prsms){
	printf("*** fread + sscanf ***\n");
	time_t start = clock();
	FILE *fp = fopen(path.c_str(), "rb");
	if (fp == NULL){
		printf("Error: Can't open file %s\n", path.c_str());
		return;
	}
	char* buf = new char[BUFFER_SIZE];
	char* title = new char[200];
	char* seq = new char[10000];
	char* ptm = new char[2048];
	char* speci = new char[100];
	char* ac = new char[200];
	char* pos = new char[100];
	char* label = new char[100];
	char* isdecoy = new char[10];
	while (1){
		int len = fread(buf,sizeof(char),BUFFER_SIZE,fp);	
		long count = 0;
		while (buf[len - 1] != '\n'){
			--len;
			++count;
		}
		buf[len-1] = '\0';
		fseek(fp,0L-count,SEEK_CUR);
		int p = 0;
		while (p < len){
			PrSM prsm;
			int ret = sscanf(buf+p, "%s\t%d\t%lf\t%d\t%lf\t%s\t%lf\t%lf\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%d",
				title, &prsm.scan, &prsm.precur_mass, &prsm.charge, &prsm.fdr, seq, &prsm.theory_mass, &prsm.diff_Da,
				&prsm.score, &prsm.evalue, ptm, speci, ac, pos, label, isdecoy, &prsm.missing);
			if (ret == 17){
				prsm.title = title;
				prsm.sequence = seq;
				prsm.ptm = ptm;
				prsm.specific = speci;
				prsm.ac = ac;
				prsm.pos_info = pos;
				prsm.label_type = label;
				prsm.isDecoy = (strcmp(isdecoy, "Decoy") == 0 ? true : false);
				prsms.push_back(prsm);
			}
			else{
				printf("Error: Invalid line...\n");
				if (ret > 0){
					printf("%s...\n", title);
				}
			}
			int idx = find_first_of(buf, '\n', p);
			if (idx == -1)  break;
			p = idx + 1;
		}
		if (len < BUFFER_SIZE){
			break;
		}
	}
	delete[] title;
	delete[] seq;
	delete[] ptm;
	delete[] speci;
	delete[] pos;
	delete[] label;
	delete[] isdecoy;
	delete[] buf;
	fclose(fp);
	printf("PrSMs: %d\n", prsms.size());
	time_t end = clock();
	printf("Time used: %d\n\n", (end - start));
}

void IO::fwriteSprintfTest(string path, vector<PrSM>& prsms){
	path = "fwriteSprintfTest.txt";
	printf("*** fwrite + sprinf ***\n");
	time_t start = clock();
	FILE *fp = fopen(path.c_str(), "wb");
	if (fp == NULL){
		printf("Error: Can't open file %s\n", path.c_str());
		return;
	}
	char* buf = new char[BUFFER_SIZE];
	int len = 0;
	for (int i = 0; i < prsms.size(); ++i){
		len += sprintf(buf+len, "%s\t%d\t%lf\t%d\t%lf\t%s\t%lf\t%lf\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n", prsms[i].title.c_str(), prsms[i].scan,
			prsms[i].precur_mass, prsms[i].charge, prsms[i].fdr, prsms[i].sequence.c_str(), prsms[i].theory_mass, prsms[i].diff_Da,
			prsms[i].score, prsms[i].evalue, prsms[i].ptm.c_str(), prsms[i].specific.c_str(), prsms[i].pos_info.c_str(),
			prsms[i].ac.c_str(), prsms[i].label_type.c_str(), prsms[i].isDecoy ? "Decoy" : "Target", prsms[i].missing);
		if (len + LINE_LEN > BUFFER_SIZE){
			fwrite(buf, sizeof(char), len, fp);
			len = 0;
		}
	}
	if (len){
		fwrite(buf, sizeof(char), len, fp);
	}
	delete[] buf;
	fclose(fp);
	time_t end = clock();
	printf("Time used: %d\n\n", (end - start));
}

int PrSM::decnt = 0;
void IO::freadTest(string path, vector<PrSM>& prsms){
	try{
		printf("*** fread ***\n");
		time_t start = clock();
		FILE *fp = fopen(path.c_str(), "rb");
		if (fp == NULL){
			printf("Error: Can't open file %s\n", path.c_str());
			return;
		}
		PrSM *tmp = new PrSM[MAX_COUNT];
		while (1){
			int num = fread(tmp, sizeof(PrSM), MAX_COUNT, fp);
			for(int i=0; i<num; ++i){
				prsms.push_back(tmp[i]);
			}
			if (num < MAX_COUNT){
				break;
			}
		}
		//delete[] tmp;
		fclose(fp);
		printf("PrSMs: %d\n", prsms.size());
		time_t end = clock();
		printf("Time used: %d\n\n", (end - start));
	}catch(exception exe){
		printf("Exception: %s\n",exe.what());
	}
}

string IO::fwriteTest(string path, vector<PrSM>& prsms){
	path = "fwriteTest.txt";
	printf("*** fwrite ***\n");
	time_t start = clock();
	FILE *fp = fopen(path.c_str(), "wb");
	if (fp == NULL){
		printf("Error: Can't open file %s\n", path.c_str());
		return path;
	}
 	fwrite(&prsms[0],sizeof(prsms[0]),prsms.size(),fp);
	fclose(fp);
	time_t end = clock();
	printf("Time used: %d\n\n", (end - start));
	return path;
}

void IO::testIO()
{
	FILE *fp = fopen("test0.txt","wb");
	vector<testObject0> vc;	
	vc.push_back(testObject0(0,0,false,"0"));
	vc.push_back(testObject0(1,1,true, "11"));
	vc.push_back(testObject0(2,2,false, "222"));
	fwrite(&vc[0],sizeof(testObject0),vc.size(),fp);
	fclose(fp);
	fp = fopen("test0.txt","rb");
	testObject0 *list0 = new testObject0[3];
	fread(list0,sizeof(testObject0),3,fp);
	for(int i=0; i<3; ++i){
		cout << list0[i].a << ", " << list0[i].b << ", " << list0[i].c << ", " << list0[i].s << endl;
	}
	delete[] list0;
	fclose(fp);
	cout << "---------------------------------------------------------" << endl;
	fp = fopen("test1.txt","wb");
	vector<testObject1> vt;	
	vt.push_back(testObject1(0,0,false,"a"));
	vt.push_back(testObject1(1,1,true, "bb"));
	vt.push_back(testObject1(2,2,false, "ccc"));
	fwrite(&vt[0],sizeof(testObject1),vt.size(),fp);
	fclose(fp);
	fp = fopen("test1.txt","rb");
	testObject1 *list1 = new testObject1[3];
	fread(list1,sizeof(testObject1),3,fp);
	for(int i=0; i<3; ++i){
		cout << list1[i].a << ", " << list1[i].b << ", " << list1[i].c << ", " << list1[i].s << endl;
	}
	//delete[] list1;
	fclose(fp);
}
