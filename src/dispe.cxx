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
    SetRawTreeBranchAdress(filename,"MC_Truth");
    TH3D *h_display1 = new TH3D("display1", "display1", 320, 0, 320*3, 5,-113.5,113.5,42,-111.3,111.3);
    TH3D *h_display2 = new TH3D("display2", "display2", 320, 0, 320*3,42,-111.3,111.3, 5,-113.5,113.5);
    TH3D *h_display1_noise = new TH3D("display1_noise", "display1_noise", 320, 0, 320*3, 5,-113.5,113.5,42,-111.3,111.3);
    TH3D *h_display2_noise = new TH3D("display2_noise", "display2_noise", 320, 0, 320*3,42,-111.3,111.3, 5,-113.5,113.5);
    TGraph2D *gr=new TGraph2D();
    gr->SetLineColor(kRed);
    // _noise->GetXaxis()->SetRangeUser(0, 800);
    // h_display1->GetYaxis()->SetRangeUser(-120,120);
    // h_display1->GetZaxis()->SetRangeUser(-120,120);
    TCanvas *c = new TCanvas("c", "c", 48, 130, 800, 500);
    gStyle->SetOptStat(0);
    int n = stoi(nentries);
    bool flag = 1;
    string tmp = "";
    double *position;
    while (flag) {
        h_display1->Reset();
        h_display2->Reset();
        h_display1_noise->Reset();
        h_display2_noise->Reset();
        int hitno=0;
        int hitno2=0;
        (*df).Range(n,n+1,1).Foreach([&](vector<int> CellID,vector<double> Hit_Energy,vector<double> Hit_X,vector<double> Hit_Y,vector<double> Hit_Z){
            for (int i = 0; i < CellID.size(); i++) {
                    double e=Hit_Energy.at(i);
                    // if(e<0.1)continue;
                    hitno++;
                    // e = log(e + 1) + 10;
                    int layer=CellID.at(i)/1e5;
                    int chip=CellID.at(i)/10000%10;
                    int channel=CellID.at(i)%100;
                    // position=EBUdecode(layer,chip,channel);
                    if(layer%2==0){
                        h_display1->Fill(Hit_Z.at(i), Hit_X.at(i),Hit_Y.at(i),e);
                        // if(abs(distance_x.at(i))<30&&abs(distance_y.at(i))<5){
                        //     hitno2++;
                        //     h_display1_noise->Fill(*(position+2), *position,*(position+1),e);
                        // }
                    }
                    else{
                        h_display2->Fill(Hit_Z.at(i), Hit_X.at(i),Hit_Y.at(i),e);
                        // if(abs(distance_x.at(i))<5&&abs(distance_y.at(i))<30){
                        //     hitno2++;
                        //     h_display2_noise->Fill(*(position+2), *position,*(position+1),e);
                        // }
                    }
            }
            h_display1->SetTitle("entries_" + TString(nentries)+TString::Format("  hitno: %d",hitno));
            h_display1_noise->SetTitle("entries_" + TString(nentries)+TString::Format("  hitno: %d",hitno2));
        },{"CellID","Hit_Energy","Hit_X","Hit_Y","Hit_Z"});
        c->Clear();
        // c->Divide(1,2);
        // c->cd(1);
        h_display1->Draw("box2");
        h_display2->Draw("same box2");
        // c->Update();
        // c->Modified();
        
        // c2->Clear();
        // c->cd(2);
        // h_display1_noise->Draw("box2");
        // h_display2_noise->Draw("same box2");
        // gr->Draw("LINE same");
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