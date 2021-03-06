
AliAnalysisTaskSEBPlustoD0Pi *AddTaskBPlustoD0Pi(
                                TString cutsfile="",
                                TString usercomment = "doremalen",
                                Bool_t bUseMCInfo=kFALSE,
                                Int_t  nUseQuickSignalAnalysis=0,
                                Bool_t bGetCutInfo=kTRUE, 
                                Bool_t bShowRejection=kFALSE, 
                                Bool_t bShowMask=kFALSE,
                                Bool_t bUse3DHistograms=kFALSE, 
                                Bool_t bCheckBackground=kFALSE, 
                                Int_t nDebugLevel=0,
                                Int_t nUpgradeSetting=0,
                                Int_t nDegreePerRotation=3,
                                Int_t nNumberOfRotations=0,
                                Double_t fHistMassWindow=0.125
                                )
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  // gDebug = 1;
  if (!mgr) {
    ::Error("AddTaskBPlustoD0Pi", "No analysis manager to connect to.");
    return NULL;
  }  

  if(nUseQuickSignalAnalysis != 0 && !bUseMCInfo){
    ::Error("AddTaskBPlustoD0Pi", "Quick signal analysis only works for Monte Carlo.");
    return NULL;
  }  
  
  // cuts are stored in a TFile generated by makeTFile4CutsBPlustoD0Pi.C in ./macros/
  // set the cuts there!

  TFile* filecuts;
  if( cutsfile.EqualTo("") ) {
    ::Error("Input file not found : add a cut object","");
  } else {
      filecuts=TFile::Open(cutsfile.Data());
      if(!filecuts ||(filecuts && !filecuts->IsOpen())){
  ::Error("Input file not found : check your cut object","");
      }
  }

  AliRDHFCutsBPlustoD0Pi* RDHFBPlustoD0Pi=new AliRDHFCutsBPlustoD0Pi();
  RDHFBPlustoD0Pi = (AliRDHFCutsBPlustoD0Pi*)filecuts->Get("BPlustoD0PiCuts");
  if(!RDHFBPlustoD0Pi){
    cout<<"Specific AliRDHFCutsBPlustoD0Pi not found"<<endl;
    return 0;
  }


  RDHFBPlustoD0Pi->SetName("BPlustoD0PiCuts");

  RDHFBPlustoD0Pi->SetGetCutInfo(bGetCutInfo);

  //CREATE THE TASK
  printf("CREATE TASK\n");

  // create the task
  AliAnalysisTaskSEBPlustoD0Pi *task = new AliAnalysisTaskSEBPlustoD0Pi("AliAnalysisTaskSEBPlustoD0Pi",RDHFBPlustoD0Pi);

  task->SetMC(bUseMCInfo);
  task->SetQuickSignalAnalysis(nUseQuickSignalAnalysis);
  task->SetGetCutInfo(bGetCutInfo);
  task->SetDebugLevel(nDebugLevel);
  task->SetShowRejection(bShowRejection);
  task->SetShowMask(bShowMask);
  // task->SetUse3DHistograms(bUse3DHistograms);
  task->SetCheckBackground(bCheckBackground);
  task->SetUpgradeSetting(nUpgradeSetting);
  task->SetHistMassWindow(fHistMassWindow);
  task->SetDegreePerRotation(nDegreePerRotation);
  task->SetNumberOfRotations(nNumberOfRotations);

  mgr->AddTask(task);

  // Create and connect containers for input/output
  usercomment = "_" + usercomment; 

  TString outputfile = AliAnalysisManager::GetCommonFileName();

  outputfile += ":PWG3_D2H_BPlustoD0Pi";
  outputfile += usercomment;
  
  // ------ input data ------
  TString input = "input";
  input += usercomment;
  TString output1 = "chist1";
  output1 += usercomment;
  TString output2 = "cuts";
  output2 += usercomment;
  TString output3 = "D0FirstDaughter";
  output3 += usercomment;
  TString output4 = "D0SecondDaughter";
  output4 += usercomment;
  TString output5 = "BPlusPion";
  output5 += usercomment;
  TString output6 = "D0";
  output6 += usercomment;
  TString output7 = "BPlus";
  output7 += usercomment;
  TString output8 = "D0_D0Pt";
  output8 += usercomment;
  TString output9 = "BPlusMC";
  output9 += usercomment;


  AliAnalysisDataContainer *cinput0  =                    mgr->CreateContainer(input,TChain::Class(), AliAnalysisManager::kInputContainer);

 // ----- output data -----
  AliAnalysisDataContainer *coutput1 =                    mgr->CreateContainer(output1,TList::Class(),AliAnalysisManager::kOutputContainer,outputfile.Data());
  AliAnalysisDataContainer *coutputCuts =                 mgr->CreateContainer(output2,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data()); //cuts
  AliAnalysisDataContainer *coutputD0FirstDaughter =      mgr->CreateContainer(output3,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutputD0SecondDaughter =     mgr->CreateContainer(output4,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutputBPlusPion =               mgr->CreateContainer(output5,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutputD0 =                   mgr->CreateContainer(output6,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutputBPlus =                   mgr->CreateContainer(output7,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutputD0_D0Pt =              mgr->CreateContainer(output8,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());
  AliAnalysisDataContainer *coutputBPlusMC =                 mgr->CreateContainer(output9,TList::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());


  mgr->ConnectInput(task,0,mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task,1,coutput1);
  mgr->ConnectOutput(task,2,coutputCuts);
  mgr->ConnectOutput(task,3,coutputD0FirstDaughter);
  mgr->ConnectOutput(task,4,coutputD0SecondDaughter);
  mgr->ConnectOutput(task,5,coutputBPlusPion);
  mgr->ConnectOutput(task,6,coutputD0);
  mgr->ConnectOutput(task,7,coutputBPlus);
  mgr->ConnectOutput(task,8,coutputD0_D0Pt);
  mgr->ConnectOutput(task,9,coutputBPlusMC);


  return task;
}



