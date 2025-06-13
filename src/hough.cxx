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
using namespace std;
double MIP_E = 0.461;
int main(int argc, char *argv[]) {
    string filename=argv[1];
    string outname;
    if(argc==2){
        outname=filename.substr(0,filename.rfind("."))+"_hough.root";
    }
    else{
        outname=argv[2];
    }
    TApplication theApp("app", &argc, argv);
    TreeReader tr;
    tr.hough(filename,outname);
    return 1;
}
void TreeReader::hough(string filename,string outname){
    SetRawTreeBranchAdress(filename);
    TCanvas* c=new TCanvas();
    double pi=3.1415926;
    int steps=100;
    TH2D* h_houghx=new TH2D("h_houghx","h_houghx",steps,-pi/100.,pi/100.,steps,-110,110);
    TH2D* h_houghy=new TH2D("h_houghy","h_houghy",steps,-pi/100.,pi/100.,steps,-110,110);
    double *position;
    int layer=0,chip=0,channel=0;
    double x=0,y=0,z=0,theta=0,rho=0;
    double e=0;
    struct Track{
        vector<double> distance_x;
        vector<double> distance_y;
        double rhox,rhoy;
        double thetax,thetay;
    };
    auto df1=(*df)
    .Define("hitno",[](vector<int> CellID,vector<int> HitTag){
        int hitno=0;
        for(int i=0;i<HitTag.size();i++){
            hitno+=bool(HitTag.at(i));
        }
        return hitno;
    },{"CellID","HitTag"})
    .Define("hitlayer",[](vector<int> CellID,vector<int> HitTag){
        int hitno[32]={0};
        for(int i=0;i<HitTag.size();i++){
            int layer=CellID.at(i)/1e5;
            if(HitTag.at(i)==1){
                hitno[layer]=1;
            }
        }
        int hitlayer=0;
        for(int i=0;i<32;i++){
            hitlayer+=hitno[i];
        }
        return hitlayer;
    },{"CellID","HitTag"});
    cout<<"hitno hitlayer done"<<endl;
    // .Range(25,26,1)
    auto df2=df1.Define("track",[&](vector<int> CellID,vector<int> HitTag,vector<double> HG_Charge){
        h_houghx->Reset();
        h_houghy->Reset();
        for (int i = 0; i < CellID.size(); i++) {
            if (HitTag.at(i) == 1) {
                layer=CellID.at(i)/1e5;
                chip=CellID.at(i)/10000%10;
                channel=CellID.at(i)%100;
                position=EBUdecode(layer,chip,channel);
                x=*(position);
                y=*(position+1);
                z=*(position+2);
                e=HG_Charge.at(i);
                for(int eff=0;eff<9;eff++){
                    if(layer%2==0){
                        x+=(eff-4)*45.4/9;
                    }
                    else{
                        y+=(eff-4)*45.4/9;
                    }
                    for(int k=0;k<steps;k++){
                        theta=h_houghx->GetXaxis()->GetBinCenter(k+1);
                        rho = x * cos(theta) + z * sin(theta);
                        h_houghx->Fill(theta,rho,e);
                        rho = y * cos(theta) + z * sin(theta);
                        h_houghy->Fill(theta,rho,e);
                    }
                }
            }
        }
        // double max_x = h_houghx->GetMaximum();
        // double max_y = h_houghy->GetMaximum();
        
        int bin=h_houghx->GetMaximumBin();
        Int_t xBin, yBin, zBin;
        h_houghx->GetBinXYZ(bin, xBin, yBin, zBin);
        theta = h_houghx->GetXaxis()->GetBinCenter(xBin);
        rho = h_houghx->GetYaxis()->GetBinCenter(yBin);

        bin=h_houghy->GetMaximumBin();
        h_houghy->GetBinXYZ(bin, xBin, yBin, zBin);
        double thetay = h_houghy->GetXaxis()->GetBinCenter(xBin);
        double rhoy = h_houghy->GetYaxis()->GetBinCenter(yBin);
        vector<double> distance_x;
        vector<double> distance_y;
        
        Track track;
        for (int i = 0; i < CellID.size(); i++) {
            if (HitTag.at(i) == 1) {
                layer=CellID.at(i)/1e5;
                chip=CellID.at(i)/10000%10;
                channel=CellID.at(i)%100;
                position=EBUdecode(layer,chip,channel);
                x=*(position);
                y=*(position+1);
                z=*(position+2);
                double d1=x*cos(theta)+z*sin(theta)-rho;
                double d2=y*cos(thetay)+z*sin(thetay)-rhoy;
                distance_x.push_back(d1);
                distance_y.push_back(d2);
            }
            else{
                distance_x.push_back(-1000);
                distance_y.push_back(-1000);
            }
        }
        track.distance_x=distance_x;
        track.distance_y=distance_y;
        track.thetax=theta;
        track.thetay=thetay;
        track.rhox=rho;
        track.rhoy=rhoy;
        
        return track;
    },{"CellID","HitTag","HG_Charge"});
    cout<<"track done"<<endl;
    // cout<<"test "<<test<<endl;
    // cout<<h_houghx->GetEntries()<<endl;
    // h_houghx->Draw("colz");
    // c->Update();
    // c->Modified();
    // cin>>outname;
    auto df3=df2.Define("distance_x",[](struct Track t){
        return t.distance_x;
    },{"track"})
    .Define("distance_y",[](struct Track t){
        return t.distance_y;
    },{"track"})
    .Define("rhox",[](struct Track t){
        return t.rhox;
    },{"track"}).Define("thetax",[](struct Track t){
        return t.thetax;
    },{"track"})
    .Define("rhoy",[](struct Track t){
        return t.rhoy;
    },{"track"})
    .Define("thetay",[](struct Track t){
        return t.thetay;
    },{"track"});
    cout<<"fill done"<<endl;
    df3.Snapshot<vector<int>,vector<int>,vector<double>,vector<double>,vector<double>,vector<double>,int,int,double,double,double,double>("Raw_Hit",TString(outname),{"CellID","HitTag","HG_Charge","LG_Charge","distance_x","distance_y","hitlayer","hitno","rhox","thetax","rhoy","thetay"});
    cout<<"done"<<endl;
}