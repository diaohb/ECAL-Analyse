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
    string outname;
    if(argc==2){
        outname=filename.substr(0,filename.rfind("."))+"_analyse.root";
    }
    else{
        outname=argv[2];
    }
    TreeReader tr;
    tr.analyse(filename,outname);
    return 1;
}
void TreeReader::analyse(string filename,string outname){
    TCanvas *c=new TCanvas("c","c",4000,4000);
    SetRawTreeBranchAdress(filename,"Calib_Hit");
    struct Analyse{
        vector<double> layer_energy;
        vector<int> layer;
        vector<int> layer_hit;
        double Total_Energy=0;
        int hitno=0;
        int hitlayer=0;
    };
    int layer_position[32]={0,1,2,3,4,5,6,7,8,9,31,31,31,31,18,19,31,31,31,31,31,31,10,11,12,13,31,31,14,15,16,17};
    (*df).Define("analyse",[&](vector<int> CellID,vector<double> Hit_X,vector<double> Hit_Y,vector<double> Hit_Z,vector<double> Hit_Energy){
        Analyse analyse;
        for(int i=0;i<32;i++){
            analyse.layer.push_back(i);
            analyse.layer_energy.push_back(0);
            analyse.layer_hit.push_back(0);
        }
        double energy=0;

        for (int i = 0; i < CellID.size(); i++) {
            int layer=CellID.at(i)/1e5;
            layer=layer_position[layer];
            analyse.layer_energy[layer]+=Hit_Energy.at(i);
            analyse.Total_Energy+=Hit_Energy.at(i);
            analyse.layer_hit[layer]++;
            analyse.hitno++;
        }
        for(int i=0;i<32;i++){
            if(analyse.layer_hit[i]>0){
                analyse.hitlayer++;
            }
        }
        return analyse;
    },{"CellID","Hit_X","Hit_Y","Hit_Z","Hit_Energy"})
    .Define("Layer",[](struct Analyse analyse){
        return analyse.layer;
    },{"analyse"})
    .Define("Layer_Energy",[](struct Analyse analyse){
        return analyse.layer_energy;
    },{"analyse"})
    .Redefine("Total_Energy",[](struct Analyse analyse){
        return analyse.Total_Energy;
    },{"analyse"})
    .Define("Hitno",[](struct Analyse analyse){
        return analyse.hitno;
    },{"analyse"})
    .Define("Hitlayer",[](struct Analyse analyse){
        return analyse.hitlayer;
    },{"analyse"})
    .Define("Layer_Hitno",[](struct Analyse analyse){
        return analyse.layer_hit;
    },{"analyse"})
    .Snapshot<vector<int>,vector<int>,vector<double>,int,int,double>("analyse",TString(outname),{"Layer","Layer_Hitno","Layer_Energy","Hitno","Hitlayer","Total_Energy"});
}