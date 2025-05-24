#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH3D.h"
#include "TStyle.h"
#include "TTree.h"
#include <iostream>
#include <string>
#include <vector>
#include "TreeReader.h"
#include "EBUdecode.h"
using namespace std;
double MIP_E = 0.461;
int main(int argc, char *argv[]) {
    string filename=argv[1];
    string outname;
    if(argc==2){
        outname=filename.substr(0,filename.rfind("."))+"_hitmap.root";
    }
    else{
        outname=argv[2];
    }
    TreeReader tr;
    tr.hitmap(filename,outname);
    return 1;
}
void TreeReader::hitmap(string filename,string outname){
    SetRawTreeBranchAdress(filename);
    fout=TFile::Open(TString(outname),"RECREATE");
    TH2D *h[32];
    for(int i=0;i<32;i++){
        if(i%2==0)
            h[i]=new TH2D(TString::Format("layer %d",i),"hitmap",5,-113.5,113.5,42,-111.3,111.3);
        else
            h[i]=new TH2D(TString::Format("layer %d",i),"hitmap",42,-111.3,111.3,5,-113.5,113.5);
    }
    double *position;
    (*df).Foreach([&](vector<int> &CellID,vector<int> &HitTag){
        for (int i = 0; i < CellID.size(); i++) {
            if (HitTag.at(i) == 1) {
                int layer=CellID.at(i)/1e5;
                int chip=CellID.at(i)/10000%10;
                int channel=CellID.at(i)%100;
                position=EBUdecode(layer,chip,channel);
                h[layer]->Fill(*position,*(position+1));
            }
        }
    },{"CellID","HitTag"});
    fout->mkdir("histogram");
    fout->cd("histogram");
    for(int i=0;i<32;i++){
        h[i]->Write();
    }
    fout->Close();
}