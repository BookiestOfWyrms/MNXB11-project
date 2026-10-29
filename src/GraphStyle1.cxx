#include "GraphStyle1.h"
#include <TPad.h>


void GraphStyle1(TH1D* hist){  
  hist->SetStats(false);

  hist->GetXaxis()->SetTitleSize(0.04);
  hist->GetYaxis()->SetTitleSize(0.04);
  hist->GetXaxis()->CenterTitle();
  hist->GetYaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleOffset(1.2);

  hist->GetXaxis()->SetLabelSize(0.035);
  hist->GetYaxis()->SetLabelSize(0.035);

  gPad->SetTopMargin(0.05);  // Change the margins to fit our new sizes
  gPad->SetRightMargin(0.06);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.12);
}

void GraphStyle1(TGraphErrors* hist){

  hist->GetXaxis()->SetTitleSize(0.04);
  hist->GetYaxis()->SetTitleSize(0.04);
  hist->GetXaxis()->CenterTitle();
  hist->GetYaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleOffset(1.2);

  hist->GetXaxis()->SetLabelSize(0.035);
  hist->GetYaxis()->SetLabelSize(0.035);

  gPad->SetTopMargin(0.05);  // Change the margins to fit our new sizes
  gPad->SetRightMargin(0.06);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.12);
}

