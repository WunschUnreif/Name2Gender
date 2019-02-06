#include <iostream>
#include <map>
#include <algorithm>
#include <cctype>
#include <fstream>

using namespace std;

map<unsigned short, int> maleCharCount;
map<unsigned short, int> femaCharCount;

int maleNum = 0, femaNum = 0;

bool genderIsMale(char s) {
    return (unsigned char)(s) == 208;
}

void processOneLine(const string &nameAndGender) {
    int commaPos = nameAndGender.find(',');

    string name = nameAndGender.substr(0, nameAndGender.find(','));
    bool ismale = genderIsMale(nameAndGender[commaPos + 2]);

    if(ismale) {
        for(auto it = name.begin(); it != name.end(); ) {
            unsigned short curr = *it;
            ++it;
            if(it != name.end()) { curr = (curr << 8) + *it; ++it; }

            maleCharCount[curr]++;
            maleNum++;
        }
    } else {
        for(auto it = name.begin(); it != name.end(); ) {
            unsigned short curr = *it;
            ++it;
            if(it != name.end()) { curr = (curr << 8) + *it; ++it; }

            femaCharCount[curr]++;
            femaNum++;
        }
    }
}

void outputResult() {
    ofstream maleFile("male.txt");
    maleFile << maleNum << endl;
    for(auto it = maleCharCount.begin(); it != maleCharCount.end(); ++it) {
        maleFile << (it->first) << ' ' << it->second << endl;
    }

    ofstream femaFile("female.txt");
    femaFile << femaNum << endl;
    for(auto it = femaCharCount.begin(); it != femaCharCount.end(); ++it) {
        femaFile << (it->first) << ' ' << it->second << endl;
    }

    maleFile.close();
    femaFile.close();
}

void learn() {
    string nameAndGender;
    ifstream datas("name.csv");
    while(datas >> nameAndGender){
        processOneLine(nameAndGender);
    }
    datas.close();
}

bool predict(const string & name) {
    double probMale = maleNum / double(maleNum + femaNum);
    for(auto it = name.begin(); it != name.end(); ) {
        unsigned short c = *it;
        ++it;
        if(it != name.end()) {c = (c << 8) + *it; ++it;}

        if(maleCharCount[c] != 0 && femaCharCount[c] != 0) {
            probMale *= (maleCharCount[c] / double(maleNum)) / ((maleCharCount[c] + femaCharCount[c]) / double(maleNum + femaNum));
        }
    }
    cout << "Male:   " << probMale << endl;
    
    double probFemale = femaNum / double(maleNum + femaNum);
    for(auto it = name.begin(); it != name.end(); ) {
        unsigned short c = *it;
        ++it;
        if(it != name.end()) {c = (c << 8) + *it; ++it;}

        if(femaCharCount[c] != 0 && femaCharCount[c] != 0) {
            probFemale *= (femaCharCount[c] / double(femaNum)) / ((maleCharCount[c] + femaCharCount[c]) / double(maleNum + femaNum));
        }
    }
    cout << "Female: " << probFemale << endl;
}

int main() {
    learn();
    outputResult();

    cout << "Please Input Names:" << endl;

    string nameInput;
    while(cin >> nameInput) {
        predict(nameInput);
    }

    return 0;
}

