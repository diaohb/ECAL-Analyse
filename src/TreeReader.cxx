#include "TreeReader.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "ROOT/RDataFrame.hxx"
void TreeReader::SetRawTreeBranchAdress(TString filename,TString treename){
    df = std::make_unique<ROOT::RDataFrame>(treename, filename);
    // df=ROOT::RDataFrame(filename,treename);
    // fin=TFile::Open(filename);
    // tin=(TTree*)fin->Get(treename);
    // tin->SetBranchAddress("Run_Num",&Run_Num);
    // tin->SetBranchAddress("Event_Time",&Event_Time);
    // tin->SetBranchAddress("CycleID",&CycleID);
    // tin->SetBranchAddress("TriggerID",&TriggerID);
    // tin->SetBranchAddress("CellID",&CellID);
    // tin->SetBranchAddress("BCID",&BCID);
    // tin->SetBranchAddress("HitTag",&HitTag);
    // tin->SetBranchAddress("GainTag",&GainTag);
    // tin->SetBranchAddress("HG_Charge",&HG_Charge);
    // tin->SetBranchAddress("LG_Charge",&LG_Charge);
    // tin->SetBranchAddress("Hit_Time",&Hit_Time);
    // tin->SetBranchAddress("Temperature",&Temperature);
}