#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

void Fun4AllMPCEX( int nEvents=20,const char* inputfile="FVTXFILTERED_run16_online_muon-0000445513-0329.PRDFF",const char* filtered_prdf = "FVTXFILTERED_run14AuAu.PRDFF",const char* mwgfile = "MWG.root" )
{  // Tell root to not to start signal handling but crash
  
  for (int i = 0; i < kMAXSIGNALS; i++)
    {
      gSystem->IgnoreSignal((ESignals)i);
    }
  
  // set the production name
//  std::string prodtag(gSystem->Getenv("PRODTAG"));
//  std::string launch(gSystem->Getenv("PRODROOT"));
//  launch = launch + "/launch";


  gSystem->Exec("/bin/env");

  std::cout << "Processing file: " << inputfile << std::endl;
  char ifile[strlen(inputfile)+1]; // + 1 for the \0 which marks the end of string
  strcpy(ifile,inputfile);
  strtok(ifile,"-");
  int runnumber = atoi(strtok(0,"-"));
  int segnumber = atoi(strtok(strtok(0,"-"),"."));
  
  // Loading libraries
  gSystem->Load("libfun4all.so");
  gSystem->Load("libfun4allfuncs.so");
  gSystem->Load("libMpcExReco");
  gSystem->Load("libmpc.so");
  gSystem->Load("libbbc.so");
  gSystem->Load("libzdc.so");
    
//  gSystem->Load("libcompactCNT.so");
//  gSystem->Load("libspin.so");
//  gSystem->Load("libSvxDstQA.so");

  //IO manager
  gROOT->ProcessLine(".L rawdatacheck.C");
  gROOT->ProcessLine(".L OutputManager.C");
  // gROOT->ProcessLine(".L QA.C");
  // gROOT->ProcessLine(".L TrigSelect.C");

  SetCvsTag();

  Fun4AllServer* se= Fun4AllServer::instance();
  se->Verbosity(0);


  // RecoConsts setup
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("MPC_RECO_MODE",0x3);  
  rc->set_FloatFlag("EASTMAXSAG", -0.017);
  rc->set_FloatFlag("WESTMAXSAG", -0.017);


  rc->Print();

  // Print database readouts
  // May have to use local dead channel file .. don't know yet?
  TMutDatabaseCntrl::print();

  //muon timeserver instance
  PHTimeServer* timeserv = PHTimeServer::get();

  //Define and configure all subsystems
  //Make and register the Raw Data Checker
  RawDataCheck *raw = rawdatacheck();
  
  HeadReco *head = new HeadReco();
  // add the rawdatacheck pointer so a list of
  // packets is added to the EventHeader
  head->SetRawDataCheck(raw);   
  se->registerSubsystem(head);

  SubsysReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  SubsysReco *trig    = new TrigReco();
  se->registerSubsystem(trig);

  SubsysReco *peve    = new PreviousEventReco();
  se->registerSubsystem(peve);

  SubsysReco *mpc     = new MpcReco();
  se->registerSubsystem(mpc);

  SubsysReco *mpcex = new MpcExCompactReco();
  se->registerSubsystem(mpcex);

  BbcReco *bbc        = new BbcReco();
  bbc->setBbcVtxError( 2.0 );
  se->registerSubsystem(bbc);

  SubsysReco *vtx     = new VtxReco();
  se->registerSubsystem(vtx);

  SubsysReco *zdc     = new ZdcReco();
  se->registerSubsystem(zdc);

  SubsysReco *global  = new GlobalReco();
  se->registerSubsystem(global);

  SubsysReco *global_central  = new GlobalReco_central();
  se->registerSubsystem(global_central);


  ////////////////////////////////// 
  // Accounting
  ////////////////////////////////// 
  SubsysReco *trigacc = new TriggerAccounting();
  se->registerSubsystem(trigacc);
  
  SubsysReco *outacc = new OutputAccounting();
  se->registerSubsystem(outacc);

  ///////////////////////////////////////////
  /// Trigger dicing now in loaded TrigSelect.C macro
  ///////////////////////////////////////////
  // TrigSelect();


  TrigSelect *ppg      = new TrigSelect("PPG");      // This will veto ppg triggers
  ppg->AddVetoTrigger("PPG(Laser)");
  ppg->AddVetoTrigger("PPG(Pedestal)");
  ppg->AddVetoTrigger("PPG(Test Pulse)");
  ppg->SetReturnCode("ABORT");
  se->registerSubsystem(ppg);

  //Minbias
  TrigSelect *minbias = new TrigSelect("MB");
  minbias->AddTrigger("BBCLL1(>1 tubes)");
  minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx");
  minbias->AddTrigger("BBCLL1(>1 tubes) novertex");
//  minbias->AddTrigger("Noise");
  se->registerSubsystem(minbias);


  //MPC
/*  
  TrigSelect *mpctrig = new TrigSelect("MPC");
  mpctrig->AddTrigger("MPC_N_B");
  mpctrig->AddTrigger("MPC_S_B");
  mpctrig->AddTrigger("MPC_N_C&ERT_2x2");
  mpctrig->AddTrigger("MPC_S_C&ERT_2x2");
  mpctrig->AddTrigger("MPC_N_C&ERTLL1_2x2");
  mpctrig->AddTrigger("MPC_S_C&ERTLL1_2x2");
  mpctrig->AddTrigger("MPC_N_C&MPC_N_C");
  mpctrig->AddTrigger("MPC_S_C&MPC_S_C");
  mpctrig->AddTrigger("MPC_N_C&MPC_S_C");
  mpctrig->AddTrigger("MPC_N_S_A");
*/  


  std::cout << "registering Fun4AllDstOutputManager" << std::endl;
  DST_MPC(runnumber, segnumber,"DST_MPC");
  DST_MPCEX(runnumber, segnumber,"DST_MPCEX");
  DST_EVE(runnumber, segnumber,"DST_EVE");


  gSystem->Exec("ps -o sid,ppid,pid,user,comm,vsize,rssize,time");

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(inputfile);
  se->registerInputManager(in);
  se->run(nEvents);
  se->End();
  //  PrintTrigSelect();

  int evts = se->PrdfEvents();
  std::cout << "Total Events:  " << evts << std::endl;
//  ostringstream sqls;
//  std::string sql;
//  sqls << launch << "/recoDB_update.pl ";
//  sqls << "\" nevents=" << evts;
//  sqls << " where runnumber=" << runnumber;
//  sqls << " and sequence=" << segnumber << " and prodtag =\'" << prodtag << "\'\"";
//  sql  = sqls.str();
//  sqls.str("");
//  cout << "executing " << sql << endl;
//  gSystem->Exec(sql.c_str());

  // create list with files to export
  FileSummary();

  timeserv->print_stat();

  delete se;
  //delete all singletons to make valgrind happy :)
  delete timeserv;
//  TMutParameterDB &mutdb = TMutParameterDB::get();
//  delete &mutdb;

  gSystem->Exec("ps -o sid,ppid,pid,user,comm,vsize,rssize,time");

  cout<<"Fun4All successfully completed "<<endl;
  gSystem->Exit(0);
}
