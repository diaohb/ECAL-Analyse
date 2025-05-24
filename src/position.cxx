#include "../include/EBUdecode.h"
int position(){
    TH2D *h1=new TH2D("h1","position",5,-113.5,113.5,42,-111.3,111.3);
    TH2D *h11=new TH2D("h11","position",5,-113.5,113.5,42,-111.3,111.3);
    TH2D *h2=new TH2D("h2","position",42,-111.3,111.3,5,-113.5,113.5);
    TH2D *h22=new TH2D("h22","position",42,-111.3,111.3,5,-113.5,113.5);
    TH2D *h1_chip=new TH2D("h1_chip","position;X(mm);Y(mm)",5,-113.5,113.5,42,-111.3,111.3);
    TH2D *h2_chip=new TH2D("h2_chip","position;X(mm);Y(mm)",42,-111.3,111.3,5,-113.5,113.5);
    double *position;
    for(int chip=0;chip<6;chip++){
        for(int channel=0;channel<36;channel++){
            position=EBUdecode(0,chip,channel);
            h1->Fill(*(position),*(position+1),channel+0.1);
            h1_chip->Fill(*(position),*(position+1),chip+0.1);
            h11->Fill(*(position),*(position+1),8);
            position=EBUdecode(1,chip,channel);
            h2->Fill(*(position),*(position+1),channel+0.1);
            h2_chip->Fill(*(position),*(position+1),chip+0.1);
            h22->Fill(*(position),*(position+1),8);
        }
    }
    // TFile* fout=TFile::Open("position.root","RECREATE");
    TCanvas* c=new TCanvas("c","c",1024*2,800*2);
    gStyle->SetOptStat(0);
    gStyle->SetPalette(kPastel);
    gStyle->SetPaintTextFormat("2.0f");
    h1_chip->GetXaxis()->CenterTitle();
    h1_chip->GetYaxis()->CenterTitle();
    h1_chip->Draw("col");
    h11->Draw("same box");
    h1->Draw("same text");
    c->Print("position.pdf(","Title:p1");
    c->Clear();
    h2_chip->GetXaxis()->CenterTitle();
    h2_chip->GetYaxis()->CenterTitle();
    h2_chip->Draw("col");
    h22->Draw("same box");
    h2->Draw("same text");
    c->Print("position.pdf)","Title:p2");
    // c->Write();
    // fout->Close();
    return 1;
}