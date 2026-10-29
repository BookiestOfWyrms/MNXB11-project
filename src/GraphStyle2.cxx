#include "GraphStyle2.h"
#include <TStyle.h>
#include <TPad.h>

void GraphStyle2 (TH1* graph){
    gStyle>SetOptStat(0);
    gStyle->SetOptTitle(0);

    gStyle->SetPadTopMargin(0.05); 
    gStyle->SetPadRightMargin(0.06);
    gStyle->SetPadBottomMargin(0.15);
    gPad->SetLeftMargin(0.08);

    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->CenterTitle();

    graph->SetTitleSize(0.04, "x");
    graph->SetTitleSize(0.04, "y"); 
    graph->GetYaxis()->SetTitleOffset(0.6);

}