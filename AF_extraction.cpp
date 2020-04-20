#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int main(){
    map<string, string> rsID2base;
    {
        ifstream ifs("CAP.txt");

        if(ifs.fail()){
            cerr << "CAP.txt error!" << endl;
            return -1;
        }

        string title;
        getline(ifs, title);

        string rsID, base;

        while(ifs >> rsID >> base){
            rsID2base[rsID] = base;
        }

        ifs.close();
    }

    {
        ifstream ifs("rs_chr.txt");

        if(ifs.fail()){
            cerr << "error!" << endl;
            return -1;
        }

        string s;
        while(getline(ifs, s)){
            if(s[0] != '#') break;
        }

        ofstream ofs("AF_chr.txt");

        ofs << "rsID AF EAS AMR AFR EUR SAS" << endl;

        string rsID, base, info;
        while(ifs >> rsID >> base >> info){
            if(rsID.size() < 3) continue;
            rsID = rsID.substr(2);
            auto itr = rsID2base.find(rsID);

            if(itr != rsID2base.end()){
                int sz = info.size();

                int AFs = -1, AFe = -1;
                for(int i=3; i<sz; i++){
                    string pre = info.substr(i - 3, 3);
                    if(pre == "AF="){
                        if(AFs == -1) AFs = i;
                    }

                    if(info[i] == ';'){
                        if(AFs != -1 && AFe == -1) AFe = i;
                    }

                    if(AFe != -1) break;
                }

                int EASs = -1, EASe = -1;
                int AMRs = -1, AMRe = -1;
                int AFRs = -1, AFRe = -1;
                int EURs = -1, EURe = -1;
                int SASs = -1, SASe = -1;
                for(int i=4; i<sz; i++){
                    string pre = info.substr(i - 4, 4);
                    if(pre == "_AF="){
                        if(EASs == -1) EASs = i;
                        else if(AMRs == -1) AMRs = i;
                        else if(AFRs == -1) AFRs = i;
                        else if(EURs == -1) EURs = i;
                        else if(SASs == -1) SASs = i;
                    }

                    if(info[i] == ';'){
                        if(EASs != -1 && EASe == -1) EASe = i;
                        else if(AMRs != -1 && AMRe == -1) AMRe = i;
                        else if(AFRs != -1 && AFRe == -1) AFRe = i;
                        else if(EURs != -1 && EURe == -1) EURe = i;
                        else if(SASs != -1 && SASe == -1) SASe = i;
                    }

                    if(SASe != -1) break;
                }

                double AF = stod(info.substr(AFs, AFe - AFs));
                double EAS = stod(info.substr(EASs, EASe - EASs));
                double AMR = stod(info.substr(AMRs, AMRe - AMRs));
                double AFR = stod(info.substr(AFRs, AFRe - AFRs));
                double EUR = stod(info.substr(EURs, EURe - EURs));
                double SAS = stod(info.substr(SASs, SASe - SASs));

                int AF25cnt = 0;
                if(rsID2base[rsID] == base){
                    // CAP の塩基がリファレンスのとき
                    AF25cnt += (0.25 < 1 - EAS);
                    AF25cnt += (0.25 < 1 - AMR);
                    AF25cnt += (0.25 < 1 - AFR);
                    AF25cnt += (0.25 < 1 - EUR);
                    AF25cnt += (0.25 < 1 - SAS);
                    if(2 <= AF25cnt){
                        ofs << rsID << " " << 1 - AF << " " << 1 - EAS << " " << 1 - AMR << " " << 1 - AFR << " " << 1 - EUR << " " << 1 - SAS << endl;
                    }
                }
                else{
                    AF25cnt += (0.25 < EAS);
                    AF25cnt += (0.25 < AMR);
                    AF25cnt += (0.25 < AFR);
                    AF25cnt += (0.25 < EUR);
                    AF25cnt += (0.25 < SAS);
                    if(2 <= AF25cnt){
                        ofs << rsID << " " << AF << " " << EAS << " " << AMR << " " << AFR << " " << EUR << " " << SAS << endl;
                    }
                }
            }
        }

        ifs.close();
        ofs.close();
    }
}