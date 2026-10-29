#include "GraphStyle2.h"

#include <TPad.h>

void GraphStyle2(TH1D* graph) {
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.06);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.08);

  graph->SetStats(false);
  graph->GetXaxis()->SetTitleSize(0.045);
  graph->GetYaxis()->SetTitleSize(0.045);
  graph->GetXaxis()->SetTitleOffset(1.6);
  graph->GetYaxis()->SetTitleOffset(0.7);
  graph->GetXaxis()->SetLabelSize(0.035);
  graph->GetYaxis()->SetLabelSize(0.035);

  graph->GetXaxis()->CenterTitle();
  graph->GetYaxis()->CenterTitle();
}

void GraphStyle2(TGraph* graph) {
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.06);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.08);

  graph->GetXaxis()->SetTitleSize(0.045);
  graph->GetYaxis()->SetTitleSize(0.045);
  graph->GetXaxis()->SetTitleOffset(1.6);
  graph->GetYaxis()->SetTitleOffset(0.7);
  graph->GetXaxis()->SetLabelSize(0.035);
  graph->GetYaxis()->SetLabelSize(0.035);

  graph->GetXaxis()->CenterTitle();
  graph->GetYaxis()->CenterTitle();
}