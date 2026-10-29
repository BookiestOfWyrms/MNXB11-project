#include "GraphStyle.h"
#include <TStyle.h>


void GraphStyle(){  
  gStyle->SetOptStat(0);  // Get rid of the stat box
  gStyle->SetOptTitle(0);  // Get rid of the title (use legends)

  gStyle->SetLabelSize(0.035, "x");// Customize size axes labels
  gStyle->SetLabelSize(0.035, "y");
  gStyle->SetTitleSize(0.04, "x");  // Use bigger text on the axes
  gStyle->SetTitleSize(0.04, "y");
  gStyle->SetTitleOffset(1.6,"x");     // Change distance axes and title

  gStyle->SetPadTopMargin(0.05);  // Change the margins to fit our new sizes
  gStyle->SetPadRightMargin(0.06);
  gStyle->SetPadBottomMargin(0.14);
  gStyle->SetPadLeftMargin(0.12);
};
