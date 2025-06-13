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
    tr.reconstruct(filename,pedestal,outname);
    return 1;
}
void TreeReader::reconstruct(string filename,string pedestal,string outname){
    TCanvas *c=new TCanvas("c","c",4000,4000);
    SetRawTreeBranchAdress(filename);

    ROOT::RDataFrame df_pedestal("ChnLevel",TString(pedestal));
    map<int,double> m_pedestal;
    df_pedestal.Foreach([&](vector<int> CellID,vector<double> PedHighMean){
        for(int i=0;i<CellID.size();i++){
            m_pedestal[CellID.at(i)]=PedHighMean.at(i);
        }
    },{"CellID","PedHighMean"});
    struct Calib_Hit{
        vector<int> CellID;
        vector<double> Hit_Energy;
        vector<double> Hit_X;
        vector<double> Hit_Y;
        vector<double> Hit_Z;
        double Total_Energy=0;
    };
    double *position;
    (*df).Define("Calib_Hit",[&](vector<int> CellID,vector<int> HitTag,vector<double> HG_Charge){
        Calib_Hit calib_hit;
        double energy=0;
        for (int i = 0; i < CellID.size(); i++) {
            if (HitTag.at(i) == 1) {
                int layer=CellID.at(i)/1e5;
                int chip=CellID.at(i)/10000%10;
                int channel=CellID.at(i)%100;
                position=EBUdecode(layer,chip,channel);
                energy=(HG_Charge.at(i)-m_pedestal[CellID.at(i)])/300 * 0.305;
                calib_hit.CellID.push_back(CellID.at(i));
                calib_hit.Hit_Energy.push_back(energy);
                calib_hit.Hit_X.push_back(*(position));
                calib_hit.Hit_Y.push_back(*(position+1));
                calib_hit.Hit_Z.push_back(*(position+2));
                calib_hit.Total_Energy+=energy;
            }
        }
        return calib_hit;
    },{"CellID","HitTag","HG_Charge"})
    .Redefine("CellID",[](struct Calib_Hit calib_hit){
        return calib_hit.CellID;
    },{"Calib_Hit"})
    .Define("Hit_Energy",[](struct Calib_Hit calib_hit){
        return calib_hit.Hit_Energy;
    },{"Calib_Hit"})
    .Define("Hit_X",[](struct Calib_Hit calib_hit){
        return calib_hit.Hit_X;
    },{"Calib_Hit"})
    .Define("Hit_Y",[](struct Calib_Hit calib_hit){
        return calib_hit.Hit_Y;
    },{"Calib_Hit"})
    .Define("Hit_Z",[](struct Calib_Hit calib_hit){
        return calib_hit.Hit_Z;
    },{"Calib_Hit"})
    .Define("Total_Energy",[](struct Calib_Hit calib_hit){
        return calib_hit.Total_Energy;
    },{"Calib_Hit"})
    .Snapshot<vector<int>,vector<double>,vector<double>,vector<double>,vector<double>,double>("Calib_Hit",TString(outname),{"CellID","Hit_Energy","Hit_X","Hit_Y","Hit_Z","Total_Energy"});
}