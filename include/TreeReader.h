#ifndef TreeReader_h
#define TreeReader_h
#include "TFile.h"
#include "TTree.h"
#include "vector"
#include "ROOT/RDataFrame.hxx"
#include "TH2D.h"
using namespace std;
class TreeReader{
    public:
    void SetRawTreeBranchAdress(TString filename,TString treename="Raw_Hit");
    void disp(string filename,string nentries);
    void hitmap(string filename,string outname);
    void hough(string filename,string outname);
    void mip_layer(string filename,string pedestal,string outname);
    void reconstruct(string filename,string pedestal,string outname);
    void analyse(string filename,string outname);

    std::unique_ptr<ROOT::RDataFrame> df;

    TFile *fin,*fout;
    TTree *tin,*tout;
    int Run_Num=0;
    int Event_Time=0;
    int CycleID=0;
    int TriggerID=0;
    vector<int> *CellID=0;
    vector<int> *BCID=0;
    vector<int> *HitTag=0;
    vector<int> *GainTag=0;
    vector<double> *HG_Charge=0;
    vector<double> *LG_Charge=0;
    vector<double> *Hit_Time=0;
    vector<vector<double>> *Temperature=0;


    private:
    
};
#endif