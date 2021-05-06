#include <iostream>
#include <iomanip>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>

void particle_map(const char* direct,const char* output, int Npart_min, int Npart_max, int pid,  float angelMax, float angleMin)
{
  cout << endl << endl << "Map of regions from which the identified hadrons come..." << pid<< endl;
  cout << "Processing events from directory: " << direct << endl;

  // Borders of pt and y
  const double ptMin = 0.4;
  const double ptMax =  0.6;
  const double yMin = 0.1;
  const double yMax = 0.5;
  const int nBins = 8;

  // Adding root files from directory into a tree
  TString dirname = direct;
  TChain *tree = new TChain("treefin");
  TSystemDirectory dir ("rootfiles",dirname);
  TList *files = dir.GetListOfFiles();
  TSystemFile *file;
  TIter next(files);
  while ((file=(TSystemFile*)next()))
  {
    TString fname = file->GetName();
    if(strstr(fname,".root"))
      tree->Add(dirname+fname);
  }

  // Maximum number of particles
  const int NP = 60000;
  Float_t px [NP], py[NP], pz[NP],x[NP],y[NP],z[NP],E[NP];
  int id[NP];

  Int_t npart, Nparticipants;

  int nevents = tree->GetEntries();
  cout << "Total number of events in directory: " << nevents << endl;
  int centrality_events = 0;
  tree->SetBranchAddress("px",&px[0]);
  tree->SetBranchAddress("py",&py[0]);
  tree->SetBranchAddress("pz",&pz[0]);
  tree->SetBranchAddress("x",&x[0]);
  tree->SetBranchAddress("y",&y[0]);
  tree->SetBranchAddress("z",&z[0]);
  tree->SetBranchAddress("E",&E[0]);
  tree->SetBranchAddress("id",&id[0]);
  tree->SetBranchAddress("npart",&npart);
  tree->SetBranchAddress("Nparticipants",&Nparticipants);
  cout << angelMax << "\t"<< angleMin<<endl;

  ofstream fout;
  fout.open(output, ofstream::app);

  for(int iev=0; iev<nevents; iev++)
  {
    if ((iev)%((int)nevents/20) == 0) cout << (int)100*iev/nevents << "%" << endl;
    tree->GetEntry(iev);
    if (Nparticipants > Npart_min && Nparticipants < Npart_max)
    {
      centrality_events++;
      for(int i=0; i<npart; i++)
      {
        //const float pabs = sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]);
        const float pt = sqrt(px[i]*px[i]+py[i]*py[i]);
        const float phi = atan2(py[i],px[i]);
        //const float eta = 0.5*log((pabs+pz[i])/(pabs-pz[i]));
        const float rap = 0.5*log((E[i]+pz[i])/(E[i]-pz[i]));

        if (yMin<rap && rap<yMax && pt<ptMax && pt>ptMin && id[i]==pid && angelMax > phi && angleMin < phi) {
          float r_x=x[i];
          float r_y=y[i];
          fout << r_x<<"\t"<<r_y<<endl;
          r_x=0;
          r_y=0;

        }
      }
    }
  }


}
