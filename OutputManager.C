#include <stdio.h> 
#include <time.h> 
#include <string>
#include <iostream>

char output[200];
vector<string> outfiles;

string MakeOutput(int runnumber, int segment, const char *file ="")
{
  sprintf(output, "%s/%s_%s-%010d-%04d.root", file, file, gSystem->Getenv("PRODTAG"), runnumber, segment);
  char mkdircmd[100];
  sprintf(mkdircmd, "mkdir -p %s", file);
  gSystem->Exec(mkdircmd);
  outfiles.push_back(output);
  return output;
}

string MakePRDFOutput(int runnumber, int segment, const char *file ="")
{
  sprintf(output, "%s/%s_%s-%010d-%04d.PRDFF", file, file, gSystem->Getenv("PRODTAG"), runnumber, segment);
  char mkdircmd[100];
  sprintf(mkdircmd, "mkdir -p %s", file);
  gSystem->Exec(mkdircmd);
  outfiles.push_back(output);
  return output;
}

void addCommon(Fun4AllDstOutputManager *manager)
{
  manager->AddNode("EventHeader");
  manager->AddNode("Sync");
  manager->AddNode("TrigLvl1");
  manager->AddNode("PreviousEvent");
  manager->AddNode("ErtOut");
  manager->AddNode("VtxOut");
}

void DST_EVE(const int runnumber,const int segment,const char *file,const char *trgsel = 0)
{

  MakeOutput(runnumber,segment,file);
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *DSTEVE_Manager  = new Fun4AllDstOutputManager(file,output);

  if (trgsel)
    {
      DSTEVE_Manager->AddEventSelector(trgsel);
    }
  addCommon(DSTEVE_Manager);
  DSTEVE_Manager->AddNode("BbcOut");
  DSTEVE_Manager->AddNode("BbcRaw");
  DSTEVE_Manager->AddNode("ZdcOut");
  DSTEVE_Manager->AddNode("ZdcRaw");
  DSTEVE_Manager->AddNode("SmdOut");
  DSTEVE_Manager->AddNode("PHGlobal");
  DSTEVE_Manager->AddNode("PHGlobal_CENTRAL");
//  DSTEVE_Manager->AddNode("RpSumXYObject");

  se->registerOutputManager(DSTEVE_Manager);

}

void DST_SVX(const int runnumber, const int segment,const char *file,const char *trgsel = 0)
{

  MakeOutput(runnumber,segment,file);
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *OutManager  = new Fun4AllDstOutputManager(file,output);

  if (trgsel)
    {
      OutManager->AddEventSelector(trgsel);
    }
  addCommon(OutManager);
  OutManager->AddNode("SvxEventInfo");
  OutManager->AddNode("SvxHit_VarArray");
  OutManager->AddNode("SvxTrack_VarArray");
  OutManager->AddNode("SvxCentralTrack_VarArray");
  OutManager->AddNode("SvxCentralTrackBG_VarArray");

//  OutManager->AddNode("SvxClusterList");
//  OutManager->AddNode("SvxSegmentList");
//  OutManager->AddNode("SvxCentralTrackList");
//  OutManager->AddNode("SvxCentralTrackBackList");

  se->registerOutputManager(OutManager);
}


void CNT_Compact(const int runnumber,const int segment,const char *file,const char *trgsel = NULL)
{

  MakeOutput(runnumber,segment,file);
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *CNTCOMPACT_Manager  = new Fun4AllDstOutputManager(file,output);

  if (trgsel)
    {
      CNTCOMPACT_Manager->AddEventSelector(trgsel);
    }

  addCommon(CNTCOMPACT_Manager);
  CNTCOMPACT_Manager->AddNode("PHGlobal");
  CNTCOMPACT_Manager->AddNode("PHGlobal_CENTRAL");
  //CNTCOMPACT_Manager->AddNode("RpSumXYObject");
  CNTCOMPACT_Manager->AddNode("DchHit_VarArray");
  CNTCOMPACT_Manager->AddNode("EmcHit_VarArray");
  CNTCOMPACT_Manager->AddNode("Pc1Hit_VarArray");
  CNTCOMPACT_Manager->AddNode("Pc2Hit_VarArray");
  CNTCOMPACT_Manager->AddNode("Pc3Hit_VarArray");
  CNTCOMPACT_Manager->AddNode("TofeHit_VarArray");
  CNTCOMPACT_Manager->AddNode("TofwHit_VarArray");
  CNTCOMPACT_Manager->AddNode("CrkHit_VarArray");
  CNTCOMPACT_Manager->AddNode("AccHit_VarArray");
  CNTCOMPACT_Manager->AddNode("CglTrackHits_VarArray");
  CNTCOMPACT_Manager->AddNode("CglTrackBackHits_VarArray");
  CNTCOMPACT_Manager->AddNode("TrackProjection_VarArray");
  CNTCOMPACT_Manager->AddNode("TrackLineProjection_VarArray");
  CNTCOMPACT_Manager->AddNode("TrackPathLength_VarArray");
  CNTCOMPACT_Manager->AddNode("emcHitContainer");
  CNTCOMPACT_Manager->AddNode("TFvtxCompactTrk"); 
  se->registerOutputManager(CNTCOMPACT_Manager);

}

void JPSI_IOManager(const int runnumber,const int segment,const char *file,const char *trgsel = 0)
{
  MakeOutput(runnumber,segment,file);

  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *MWG_Manager  = new Fun4AllDstOutputManager(file,output);

  if (trgsel)
    {
      MWG_Manager->AddEventSelector(trgsel);
    }
  MWG_Manager->AddNode("PHMuoTracksOO");
  MWG_Manager->AddNode("PHGlobal");
  MWG_Manager->AddNode("PHGlobal_MUON");


     MWG_Manager->AddNode("SingleMuonContainer");
     MWG_Manager->AddNode("DiMuonContainer");

  addCommon(MWG_Manager);
  se->registerOutputManager(MWG_Manager);
}

void MWG_IOManager(const int runnumber,const int segment,const char *file,const char *trgsel = 0)
{
  MakeOutput(runnumber,segment,file);

  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *MWG_Manager  = new Fun4AllDstOutputManager(file,output);

  if (trgsel)
    {
      MWG_Manager->AddEventSelector(trgsel);
    }
  addCommon(MWG_Manager);
	
	// Important group
  MWG_Manager->AddNode("PHMuoTracksOO");
  MWG_Manager->AddNode("PHGlobal");
  MWG_Manager->AddNode("PHGlobal_MUON");

  MWG_Manager->AddNode("PHMuoTracksAdc");
  MWG_Manager->AddNode("TMuiPseudoLL1");
	
  MWG_Manager->AddNode("MutrgHeaderArray");// was in run11pp500
  MWG_Manager->AddNode("MutrgHitArray");// was in run11pp500
  MWG_Manager->AddNode("MutrgTrkArray");// was in run11pp500
	
	// Important
  MWG_Manager->AddNode("TMuiRoadO");
  MWG_Manager->AddNode("TMutTrk");
  MWG_Manager->AddNode("TMutTrkMap");
  MWG_Manager->AddNode("TRpcHit");

  MWG_Manager->AddNode("TRpcHodoHit"); // Small
  // MWG_Manager->AddNode("TRpcClus"); // Can be reconstructed
  // MWG_Manager->AddNode("TRpcRoad"); // Can be reconstructed

  // MWG_Manager->AddNode("TRpcCoord"); // Can be reconstructed
  MWG_Manager->AddNode("TRpcHitMap");
  MWG_Manager->AddNode("TRpcHodoHitMap");
  // MWG_Manager->AddNode("TRpcCoordMap"); // Can be reconstructed
  // MWG_Manager->AddNode("TRpcTrkMap");  // The big one!

  MWG_Manager->AddNode("TFvtxCompactTrk");
  se->registerOutputManager(MWG_Manager);
}


void MuTRnDST_IOManager(const int runnumber,const int segment,const char *file,const char *trgsel = 0)
{
  MakeOutput(runnumber,segment,file);

  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *MuTRnDST_Manager  = new Fun4AllDstOutputManager(file,output);

  if (trgsel)
  {
    MuTRnDST_Manager->AddEventSelector(trgsel);
  }
  addCommon(MuTRnDST_Manager);
  //  MuTRnDST_Manager->AddNode("SpinDataEventOut");
  MuTRnDST_Manager->AddNode("BbcOut");
  MuTRnDST_Manager->AddNode("PHMuoTracksOO");
  MuTRnDST_Manager->AddNode("PHGlobal");
  MuTRnDST_Manager->AddNode("PHGlobal_MUON");
  MuTRnDST_Manager->AddNode("TMuiPseudoLL1");
  MuTRnDST_Manager->AddNode("RpSumXYObject");
  se->registerOutputManager(MuTRnDST_Manager);
}

char* muid_eff_IOManager(const int runnumber,const int segment)
{
  MakeOutput(runnumber,segment,"MUID_EFF");
  return output;
}

char* pDST_IOManager(const int runnumber,const int segment)
{
  MakeOutput(runnumber,segment,"pDST");
  return output; // output char filled within MakeOutput function.
}

std::string LPanaOut(const int runnumber,const int segment, const char *file)
{
  std::string out = MakeOutput(runnumber,segment,file);
  cout << "full fname: " << out << endl;
  return out;
}


void QA_IOManager(const int runnumber,const int segment)
{
  MakeOutput(runnumber,segment,"qaRoot");
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllHistoManager *hm = se->getHistoManager("QA");
  if(hm) hm->dumpHistos(output);
}

void SVXQA_IOManager(const int runnumber,const int segment)
{
  MakeOutput(runnumber,segment,"SVXQA");
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllHistoManager *hm = se->getHistoManager("SVXQA");
  if(hm) hm->dumpHistos(output);
}

void PRDF_IOManager(const int runnumber,const int segment, const char *file, char *trgsel=0, char *trgsel2=0)
{
  MakePRDFOutput(runnumber,segment,file);
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllEventOutputManager *prdf_manager       = new Fun4AllEventOutputManager(file, output);

  if (trgsel)
    {
      prdf_manager->AddEventSelector(trgsel);
    }
  if (trgsel2)
    {
      prdf_manager->AddEventSelector(trgsel2);
    }
  se->registerOutputManager(prdf_manager);
}

void DST_MPC(const int runnumber,const int segment,const char *file, char *trgsel=0)
{

  MakeOutput(runnumber,segment,file);
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *DST_MPC_Manager  = new Fun4AllDstOutputManager(file,output);

  if (trgsel)
    {
      DST_MPC_Manager->AddEventSelector(trgsel);
    }

  addCommon(DST_MPC_Manager);
  DST_MPC_Manager->AddNode("PHGlobal");
  DST_MPC_Manager->AddNode("MpcRaw2");

  se->registerOutputManager(DST_MPC_Manager);
}

void DST_MPCEX(const int runnumber,const int segment,const char *file, char *trgsel=0){
  MakeOutput(runnumber,segment,file);
  Fun4AllServer *se = Fun4AllServer::instance();
  Fun4AllDstOutputManager *DST_MPCEX_Manager  = new Fun4AllDstOutputManager(file,output);
  if(trgsel){
    DST_MPCEX_Manager->AddEventSelector(trgsel);
  }

  addCommon(DST_MPCEX_Manager);
  DST_MPCEX_Manager->AddNode("MpcExRawHit");
  DST_MPCEX_Manager->AddNode("MpcExEventHeader");
  se->registerOutputManager(DST_MPCEX_Manager);
}

void FileSummary(const char *file = "FileSummary.txt")
{
  ofstream fp_out;
  fp_out.open(file, ios::app);
  for (unsigned int i=0; i<outfiles.size(); i++)
    {
      fp_out << gSystem->Getenv("PRODTAG") << " " << outfiles[i] << endl;
    }
  fp_out.close();
}
