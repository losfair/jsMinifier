#include <stdio.h>

#include <iostream>
#include <fstream>

const char trimList[] = {'\n','\t','\r',' '};
const char trimAfterList[] = {';','{','}','(',')','[',']',',','.'};
const char separatorList[] = {'\"','\''};

inline void initAsFalse(bool *table, size_t length) {
	for(;length>0;length--) {
		*table = false;
		table++;
	}
}

void trimJSData(unsigned char *origData) {
	unsigned char *origData_start = origData;

	bool trimTable[256],trimAfter[256],separatorTable[256];

	initAsFalse(trimTable,256);
	initAsFalse(trimAfter,256);
	initAsFalse(separatorTable,256);

	for(int i=0;i<sizeof(trimList);i++) trimTable[trimList[i]] = true;
	for(int i=0;i<sizeof(trimAfterList);i++) trimAfter[trimAfterList[i]] = true;
	for(int i=0;i<sizeof(separatorList);i++) separatorTable[separatorList[i]] = true;

	char currentSeparator = 0;

	bool trimNow = false;

	while(*origData!='\0') {
		trimNow = false;

		if(*origData == '\\') {
				if(*(origData+1) == '\0') break;
				origData += 2;
				continue;
		}

		if(currentSeparator) {
			if(*origData == currentSeparator) currentSeparator = 0;
			origData++;
			continue;
		}

		if(separatorTable[*origData]) {
			currentSeparator = *origData;
			origData++;
			continue;
		}

		if(*(origData+1) != '\0' && *origData == '/' && *(origData+1) == '/') {
			while(*origData != '\0' && *origData != '\n') {
				*origData = 0;
				origData++;
			}
			if(*origData == '\0') break;
			else {
				origData--;
				trimNow = true;
			}
		}

		if(trimAfter[*origData] || trimNow) {
			origData++;
			while(*origData != '\0' && trimTable[*origData]) {
				*origData = 0;
				origData++;
			}
			continue;
		}

		origData++;
	}
}

int main(int argc, char *argv[]) {
	if(argc!=2) {
		printf("Bad usage\n");
		return -1;
	}

	std::ifstream inFile(argv[1],std::ios::binary);
	if(!inFile.is_open()) {
		printf("Unable to open input file");
		return -2;
	}

	inFile.seekg(0,std::ios::end);
	size_t size = inFile.tellg();

	inFile.seekg(0,std::ios::beg);
	
	unsigned char *data = new unsigned char [size+1];
	inFile.read((char*)data,size);
	data[size] = '\0';

	inFile.close();

	trimJSData(data);

	for(int i=0;i<size;i++) {
		if(data[i]!=0) putchar(data[i]);
	}
	putchar('\n');

	delete[] data;

	return 0;
}
