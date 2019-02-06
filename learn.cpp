#include <iostream>
#include <map>
#include <algorithm>
#include <cctype>
#include <fstream>

using namespace std;

map<char, int> maleCharCount;
map<char, int> femaCharCount;

int maleNum = 0, femaNum = 0;

bool genderIsMale(char s) {
    return (unsigned char)(s) == (unsigned char)("男"[0]);
}

void processOneLine(const string &nameAndGender) {
    int commaPos = nameAndGender.find(',');

    string name = nameAndGender.substr(0, nameAndGender.find(','));
    bool ismale = genderIsMale(nameAndGender[commaPos + 1]);

    if(ismale) {
        for(const char& c : name) {
            maleCharCount[c]++;
            maleNum++;
        }
    } else {
        for(const char& c : name) {
            femaCharCount[c]++;
            femaNum++;
        }
    }
}

void outputResult() {
    ofstream maleFile("male.txt");
    maleFile << maleNum << endl;
    for(auto it = maleCharCount.begin(); it != maleCharCount.end(); ++it) {
        maleFile << (int)(it->first) << ' ' << it->second << endl;
    }

    ofstream femaFile("female.txt");
    femaFile << femaNum << endl;
    for(auto it = femaCharCount.begin(); it != femaCharCount.end(); ++it) {
        femaFile << (int)(it->first) << ' ' << it->second << endl;
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
    for(const char& c : name) {
        if(maleCharCount[c] != 0) {
            probMale *= (maleCharCount[c] / double(maleNum)) / ((maleCharCount[c] + femaCharCount[c]) / double(maleNum + femaNum));
        } else if(femaCharCount[c] != 0) {
            probMale *= 0.3 / (femaCharCount[c] / double(maleNum + femaNum));
        }
    }
    cout << "Male:   " << probMale << endl;
    
    double probFemale = femaNum / double(maleNum + femaNum);
    for(const char& c : name) {
        if(femaCharCount[c] != 0) {
            probFemale *= (femaCharCount[c] / double(femaNum)) / ((maleCharCount[c] + femaCharCount[c]) / double(maleNum + femaNum));
        } else if(maleCharCount[c] != 0) {
            probFemale *= 0.3 / (maleCharCount[c] / double(maleNum + femaNum));
        }
    }
    cout << "Female:   " << probFemale << endl;
}

int main() {
    learn();
    outputResult();

    string nameInput;
    while(cin >> nameInput) {
        predict(nameInput);
    }

    return 0;
}

