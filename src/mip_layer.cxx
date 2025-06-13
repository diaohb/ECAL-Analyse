#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TTree.h"
#include <iostream>
#include <string>
#include <vector>
#include "TreeReader.h"
#include "EBUdecode.h"
#include <map>
using namespace std;
double MIP_E = 0.461;
int main(int argc, char *argv[]) {
    string filename=argv[1];
    string pedestal=argv[2];
    string outname;
    if(argc==3){
        outname=filename.substr(0,filename.rfind("."))+"_mip.root";
    }
    else{
        outname=argv[3];
    }
    TreeReader tr;
    tr.mip_layer(filename,pedestal,outname);
    return 1;
}
void TreeReader::mip_layer(string filename,string pedestal,string outname){
    TCanvas *c=new TCanvas("c","c",4000,4000);
    SetRawTreeBranchAdress(filename);
    fout=TFile::Open(TString(outname),"RECREATE");
    const int calib_layer=2;
    int layer_hit[32]={0};
    map<int,TH1D*> h_mip;
    for(int layer=0;layer<32;layer++){
        for(int chip=0;chip<6;chip++){
            for(int channel=0;channel<36;channel++){
                int cellid=layer*1e5+chip*1e4+channel;
                h_mip[cellid]=new TH1D(TString::Format("mip_%d",cellid),TString::Format("mip_%d",cellid),1000,0,2000);
            }
        }
    }
    ROOT::RDataFrame df_pedestal("ChnLevel",TString(pedestal));
    map<int,double> m_pedestal;
    df_pedestal.Foreach([&](vector<int> CellID,vector<double> PedHighMean){
        for(int i=0;i<CellID.size();i++){
            m_pedestal[CellID.at(i)]=PedHighMean.at(i);
        }
    },{"CellID","PedHighMean"});
    (*df).Foreach([&](vector<int> &CellID,vector<int> &HitTag,vector<double> HG_Charge){
        fill(layer_hit,layer_hit+32,0);
        for (int i = 0; i < CellID.size(); i++) {
            if (HitTag.at(i) == 1) {
                int layer=CellID.at(i)/1e5;
                int chip=CellID.at(i)/10000%10;
                int channel=CellID.at(i)%100;
                // position=EBUdecode(layer,chip,channel);
                layer_hit[layer]++;
            }
        }
        int mip_flag=0;
        for(int layer=0;layer<5;layer++){
            if(layer_hit[layer]>1)
                mip_flag=1;
        }
        if(mip_flag==0){
            for (int i = 0; i < CellID.size(); i++) {
                if (HitTag.at(i) == 1) {
                    int layer=CellID.at(i)/1e5;
                    // if(layer<2){
                        h_mip[CellID.at(i)]->Fill(HG_Charge.at(i));//-m_pedestal[CellID.at(i)]);
                    // }
                }
            }
        }
    },{"CellID","HitTag","HG_Charge"});
    fout->mkdir("histogram");
    fout->cd("histogram");
    for(int layer=0;layer<32;layer++){
        fout->mkdir(TString::Format("histogram/layer%d",layer));
        for(int chip=0;chip<6;chip++){
            c->Clear();
            c->Divide(6,6);
            fout->mkdir(TString::Format("histogram/layer%d/chip%d",layer,chip));
            fout->cd(TString::Format("histogram/layer%d/chip%d",layer,chip));
            for(int channel=0;channel<36;channel++){
                int cellid=layer*1e5+chip*1e4+channel;
                h_mip[cellid]->Write();
                c->cd(channel+1);
                h_mip[cellid]->Draw();
            }
            c->Write();
        }
    }
    fout->Close();
}