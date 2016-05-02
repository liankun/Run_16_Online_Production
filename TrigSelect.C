#include <vector>
vector<string> trgsel;
vector<string>::const_iterator striter;

void TrigSelect()
{
  gSystem->Load("libfun4all.so");
  gSystem->Load("libfun4allfuncs.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  string trgselname = "MU";
  TrigSelect *mutrig = new TrigSelect(trgselname);
  trgsel.push_back(trgselname);
  mutrig->AddTrigger( "MUIDLL1_N1D&BBCLL1" );//9999999
  mutrig->AddTrigger( "MUIDLL1_S1D&BBCLL1" );//9999999
  mutrig->AddTrigger( "MUIDLL1_N2D&BBCLL1" );
  mutrig->AddTrigger( "MUIDLL1_S2D&BBCLL1" );
  mutrig->AddTrigger( "MUIDLL1_N1D&S1D&BBCLL1" );
  mutrig->AddTrigger( "MUON_SG1_N&BBCLL1" );//9999999
  mutrig->AddTrigger( "MUON_SG1_S&BBCLL1" );//9999999
  mutrig->AddTrigger( "MUON_SG3_N&BBCLL1" );//9999999
  mutrig->AddTrigger( "MUON_SG3_S&BBCLL1" );//9999999
  mutrig->AddTrigger( "MUON_SG1_N&MUIDLL1_N1D&BBCLL1narrow" );//9999999
  mutrig->AddTrigger( "MUON_SG1_S&MUIDLL1_S1D&BBCLL1narrow" );//9999999
  mutrig->AddTrigger( "MUON_SG3_N&MUIDLL1_N1D&BBCLL1narrow" );//9999999
  mutrig->AddTrigger( "MUON_SG3_S&MUIDLL1_S1D&BBCLL1narrow" );//9999999

  trgselname = "MB";
  TrigSelect *minbias = new TrigSelect(trgselname);
  trgsel.push_back(trgselname);
  minbias->AddTrigger("BBCLL1(>1 tubes)");
  minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx");
  minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx Copy A");
  minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx Copy B");
  minbias->AddTrigger("BBCLL1(>1 tubes) novertex");
  minbias->AddTrigger("ZDCLL1narrow");
  minbias->AddTrigger("ZDCLL1wide");
  minbias->AddTrigger("ZDCNS");

  trgselname = "ERT";
  TrigSelect *erttrig = new TrigSelect(trgselname);
  trgsel.push_back(trgselname);
  erttrig->AddTrigger("ERT_4x4b");
  erttrig->AddTrigger("ERT_4x4c&BBCLL1");//9999999
  erttrig->AddTrigger("ERT_4x4a&BBCLL1");
  erttrig->AddTrigger("ERTLL1_E&BBCLL1 narrowvtx");
  erttrig->AddTrigger("ERTLL1E_2x2");//99999999
  erttrig->AddTrigger("ERTLL1W_2x2");//99999999

  TrigSelect *ppg      = new TrigSelect("PPG");      // This will veto ppg triggers
  ppg->AddVetoTrigger("PPG(Laser)");
  ppg->AddVetoTrigger("PPG(Pedestal)");
  ppg->AddVetoTrigger("PPG(Test Pulse)");
  ppg->SetReturnCode("ABORT");

// This will take all triggers from the previous trigselect modules
// and will only save events which were triggered by other triggers
// It will save the event even if it was also triggered by a previously
// selected trigger
  trgselname = "OT";
  TrigSelect *others = new TrigSelect(trgselname); 
  trgsel.push_back(trgselname);
  others->AddNoSaveTrigger(erttrig);
  others->AddNoSaveTrigger(minbias);
  others->AddNoSaveTrigger(mutrig);
  se->registerSubsystem(ppg);
  se->registerSubsystem(erttrig);
  se->registerSubsystem(mutrig);
  se->registerSubsystem(minbias);
  se->registerSubsystem(others);   
}

void PrintTrigSelect()
{
  
  Fun4AllServer *se = Fun4AllServer::instance();
  for (int i = 0; i < trgsel.size(); i++)
    {
      TrigSelect *trg = (TrigSelect *) se->getSubsysReco(trgsel[i].c_str());
      trg->Print();
      cout << endl;
    }
}
