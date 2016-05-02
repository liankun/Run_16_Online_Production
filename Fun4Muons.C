#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

void Fun4Muons( int nEvents=20, 
		      const char* inputfile="FVTXFILTERED_run16_online_muon-0000445513-0329.PRDFF", 
		      const char* filtered_prdf = "FVTXFILTERED_run14AuAu.PRDFF", 
		      const char* mwgfile = "MWG.root" )
{  // Tell root to not to start signal handling but crash
  for (int i = 0; i < kMAXSIGNALS; i++)
    {
      gSystem->IgnoreSignal((ESignals)i);
    }
  // set the production name
  std::string prodtag(gSystem->Getenv("PRODTAG"));
  std::string launch(gSystem->Getenv("PRODROOT"));
  launch = launch + "/launch";


  gSystem->Exec("/bin/env");

  std::cout << "Processing file: " << inputfile << std::endl;
  char ifile[strlen(inputfile)+1]; // + 1 for the \0 which marks the end of string
  strcpy(ifile,inputfile);
  strtok(ifile,"-");
  int runnumber = atoi(strtok(0,"-"));
  int segnumber = atoi(strtok(strtok(0,"-"),"."));
  // Loading libraries
  gSystem->Load("libfun4all");
  gSystem->Load("libmutoo_subsysreco" );
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("liblvl2");
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("librecal.so");
  gSystem->Load( "librpc_subsysreco" );
  gSystem->Load("libmutrg");
  gSystem->Load("libMWGOO");
  gSystem->Load("librecal");
  gSystem->Load("libSvxDstQA.so");
  gSystem->Load("libpicodst_object.so");


  //IO manager
  gROOT->ProcessLine(".L rawdatacheck.C");
  gROOT->ProcessLine(".L OutputManager.C");
  // gROOT->ProcessLine(".L QA.C");
  // gROOT->ProcessLine(".L TrigSelect.C");

  SetCvsTag();

  Fun4AllServer* se= Fun4AllServer::instance();
  se->Verbosity(0);

  // get pointer to raw data checker to pass to reco modules
  //  RawDataCheck *raw = rawdatacheck();

  // RecoConsts setup
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("PRINT_MUTOO_PARAMETERS",0);

  //************
  rc->set_IntFlag( "MUTOO_ERRORSTATS", 0 ); // to stop TMutErrorStats
  // use # of strips with hard-coded number
  MutrgPar::IS_READ_NSTRIP_DB = true;
  rc->set_IntFlag( "RpcGeomType", 3 );//NEEDED for run 12+ RPC geometry...
  //***********

  rc->set_IntFlag("SVXACTIVE", true);
  rc->set_IntFlag( "RpcGeomType", 3 );//NEEDED for run 12+ RPC geometry...

  rc->Print();

  TMutExtVtx::get().set_verbosity( MUTOO::NONE );

  // Print database readouts
  // May have to use local dead channel file .. don't know yet?
  TMutDatabaseCntrl::print();

  //muon timeserver instance
  PHTimeServer* timeserv = PHTimeServer::get();

 //Define and configure all subsystems
  HeadReco *head = new HeadReco();
  //  head->SetRawDataCheck(raw); // add the rawdatacheck pointer so a list of 
                              // bad packets is added to the EventHeader
  se->registerSubsystem(head);

  SubsysReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  SubsysReco *trig    = new TrigReco();
  se->registerSubsystem(trig);

  SubsysReco *peve    = new PreviousEventReco();
  se->registerSubsystem(peve);

  BbcReco *bbc        = new BbcReco();
  bbc->setBbcVtxError( 0.5 );
  se->registerSubsystem(bbc);

  SubsysReco *zdc     = new ZdcReco();
  se->registerSubsystem(zdc);

  SubsysReco *t0      = new T0Reco();
  se->registerSubsystem(t0);

  SubsysReco *vtx     = new VtxReco();
  se->registerSubsystem(vtx);

  ////////////////////////////////// 
  // Accounting
  ////////////////////////////////// 
  SubsysReco *trigacc = new TriggerAccounting();
  se->registerSubsystem(trigacc);

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

  // remove events with Z vertex > 30cm where tracks reach MuTr with no absorber 
  MuonTrigFilter * filter_zvertex = new MuonTrigFilter("ZVERTEX", 
						       MuonTrigFilter::Z_VERTEX, MuonTrigFilter::ABORT_EVENT);
  filter_zvertex->set_z_vertex_window(-30, 30);
  se->registerSubsystem(filter_zvertex);

  SubsysReco *outacc = new OutputAccounting();
  se->registerSubsystem(outacc);

  SubsysReco *unpack = new MuonUnpackPRDF();
  se->registerSubsystem(unpack);

  MuiooReco *muioo  = new MuiooReco();
  muioo->set_max_occupancy_per_arm(1000);
  se->registerSubsystem(muioo);

  SubsysReco *mutoo  = new MuonDev();
  se->registerSubsystem(mutoo);

  SubsysReco *global = new GlobalReco();
  se->registerSubsystem(global);

  SubsysReco *global_muons = new GlobalReco_muons();
  se->registerSubsystem(global_muons);

  SubsysReco *mwg = new MWGOOReco(new MWGInclusiveNanoCutsv2()); //muon nDST module
  se->registerSubsystem(mwg);



    mFillSingleMuonContainer* msngl = new mFillSingleMuonContainer();
    se->registerSubsystem(msngl);

    mFillDiMuonContainer* mdi = new mFillDiMuonContainer(false, 2.0,false); // 1st parameter: with mixed even?,  2nd parameter = Z vertex resolution
    se->registerSubsystem(mdi);
    mdi->set_bbcz_cut(15);
    mdi->set_mass_cut(1.5);
    mdi->set_is_pp(true);  // avoid mess up with centrality in online productions

    std::cout << "registering Fun4AllDstOutputManager" << std::endl;

//  PRDF_IOManager(runnumber,segnumber,"FVTXFILTERED","mFillDiMuonContainer");

  JPSI_IOManager(runnumber,segnumber,"DIMUONMWG","");



  gSystem->Exec("ps -o sid,ppid,pid,user,comm,vsize,rssize,time");

  // DeathToMemoryHogs * dt = new DeathToMemoryHogs();
  // dt->event_frequency(100); // check every hundred events
  // dt->SaveHisto();  // save data in histogram
  // se->registerSubsystem(dt);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(inputfile);
  se->registerInputManager(in);

  se->run(nEvents);

  se->End();
  //  PrintTrigSelect();

  int evts = se->PrdfEvents();
  std::cout << "Total Events:  " << evts << std::endl;
  ostringstream sqls;
  std::string sql;
  sqls << launch << "/recoDB_update.pl ";
  sqls << "\" nevents=" << evts;
  sqls << " where runnumber=" << runnumber;
  sqls << " and sequence=" << segnumber << " and prodtag =\'" << prodtag << "\'\"";
  sql  = sqls.str();
  sqls.str("");
  cout << "executing " << sql << endl;
  gSystem->Exec(sql.c_str());

  // create list with files to export
  FileSummary();

  timeserv->print_stat();

  delete se;
  //delete all singletons to make valgrind happy :)
  delete timeserv;
  TMutParameterDB &mutdb = TMutParameterDB::get();
  delete &mutdb;

  gSystem->Exec("ps -o sid,ppid,pid,user,comm,vsize,rssize,time");

  cout<<"Fun4All successfully completed "<<endl;
  gSystem->Exit(0);

}
