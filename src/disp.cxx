#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH3D.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGraph2D.h"
#include <iostream>
#include <string>
#include <vector>
#include "TreeReader.h"
#include "EBUdecode.h"
using namespace std;
double MIP_E = 0.461;
int main(int argc, char *argv[]) {
    string filename=argv[1];
    string nentries;
    if(argc==2){
        nentries="1";
    }
    else{
        nentries=argv[2];
    }
    TApplication theApp("app", &argc, argv);
    TreeReader tr;
    tr.disp(filename,nentries);
    return 1;
}
void TreeReader::disp(string filename,string nentries){
    SetRawTreeBranchAdress(filename);
    TH3D *h_display1 = new TH3D("display1", "display1", 320, 0, 320, 5,-113.5,113.5,42,-111.3,111.3);
    TH3D *h_display2 = new TH3D("display2", "display2", 320, 0, 320,42,-111.3,111.3, 5,-113.5,113.5);
    // _noise->GetXaxis()->SetRangeUser(0, 800);
    // h_display1->GetYaxis()->SetRangeUser(-120,120);
    // h_display1->GetZaxis()->SetRangeUser(-120,120);
    TCanvas *c = new TCanvas("c", "c", 48, 130, 800, 500);
    // TCanvas *c2 = new TCanvas("c2", "c2", 48, 130, 800, 500);
    gStyle->SetOptStat(0);
    int n = stoi(nentries);
    bool flag = 1;
    string tmp = "";
    double *position;
    while (flag) {
        h_display1->Reset();
        h_display2->Reset();
        int hitno=0;
        int hitno2=0;
        (*df).Range(n,n+1,1).Foreach([&](vector<int> &HitTag,vector<int> &CellID,vector<double> &HG_Charge){
            for (int i = 0; i < CellID.size(); i++) {
                if (HitTag.at(i) == 1) {
                    double e = HG_Charge.at(i);
                    if(e<550)continue;
                    hitno++;
                    // e = log(e + 1) + 10;
                    int layer=CellID.at(i)/1e5;
                    int chip=CellID.at(i)/10000%10;
                    int channel=CellID.at(i)%100;
                    position=EBUdecode(layer,chip,channel);
                    if(layer%2==0){
                        h_display1->Fill(*(position+2), *position,*(position+1),e);
                    }
                    else{
                        h_display2->Fill(*(position+2), *position,*(position+1),e);
                    }
                }
            }
            h_display1->SetTitle("entries_" + TString(nentries)+TString::Format("  hitno: %d",hitno));
        },{"HitTag","CellID","HG_Charge"});
        c->Clear();
        h_display1->Draw("box2");
        h_display2->Draw("same box2");
        c->Update();
        c->Modified();
        
        do {
            cout << "next display entry no('q' for quit):" << endl;
            cin >> tmp;
            if (tmp == "q") {
                flag = 0;
                break;
            } else {
                try {
                    n = stoi(tmp);
                } catch (std::invalid_argument) {
                    cout << "wrong input!!!" << endl;
                }
            }
        } while (tmp == nentries);
        nentries = tmp;
        // cout << nentries << endl;
    }
}