#include "TROOT.h"
#include "TSystem.h"

AliAnalysisTaskSE* AddTaskFemtoGranma(bool isMC, TString CentEst = "kInt7",
                                      bool DCAPlots = false) {
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  if (!mgr) {
    printf("No analysis manager to connect to!\n");
    return nullptr;
  }
  // just to see if all went well, check if the input event handler has been
  // connected
  if (!mgr->GetInputEventHandler()) {
    printf("This task requires an input event handler!\n");
    return nullptr;
  }

  AliFemtoDreamEventCuts *evtCuts = AliFemtoDreamEventCuts::StandardCutsRun2();
  evtCuts->CleanUpMult(false, false, false, true);
  evtCuts->SetMultVsCentPlots(true);

  AliAnalysisTaskGrandma *task = new AliAnalysisTaskGrandma("myFirstTask",
                                                            isMC);
  task->SetTrackBufferSize(2000);
  task->SetEventCuts(evtCuts);
  AliFemtoDreamTrackCuts *TrackCuts = AliFemtoDreamTrackCuts::PrimProtonCuts(
      isMC, DCAPlots, false, false);
  TrackCuts->SetCutCharge(1);
//wanna change something? Do it like this: TrackCuts->SetPtRange(0.3, 4.05);

  task->SetTrackCuts(TrackCuts);
  AliFemtoDreamTrackCuts *AntiTrackCuts =
      AliFemtoDreamTrackCuts::PrimProtonCuts(isMC, DCAPlots, false, false);
  AntiTrackCuts->SetCutCharge(-1);
  task->SetAntiTrackCuts(AntiTrackCuts);

  AliFemtoDreamCollConfig *config = new AliFemtoDreamCollConfig("Femto",
                                                                "Femto");
  std::vector<int> PDGParticles;
  PDGParticles.push_back(2212);
  PDGParticles.push_back(2212);
//  PDGParticles.push_back(3122);
//  PDGParticles.push_back(3122);
//  PDGParticles.push_back(3312);
//  PDGParticles.push_back(3312);
  config->SetPDGCodes(PDGParticles);

  std::vector<float> ZVtxBins;
  ZVtxBins.push_back(-10);
  ZVtxBins.push_back(-8);
  ZVtxBins.push_back(-6);
  ZVtxBins.push_back(-4);
  ZVtxBins.push_back(-2);
  ZVtxBins.push_back(0);
  ZVtxBins.push_back(2);
  ZVtxBins.push_back(4);
  ZVtxBins.push_back(6);
  ZVtxBins.push_back(8);
  ZVtxBins.push_back(10);
  config->SetZBins(ZVtxBins);

  std::vector<int> MultBins;
  MultBins.push_back(0);
  MultBins.push_back(4);
  MultBins.push_back(8);
  MultBins.push_back(12);
  MultBins.push_back(16);
  MultBins.push_back(20);
  MultBins.push_back(24);
  MultBins.push_back(28);
  MultBins.push_back(32);
  MultBins.push_back(36);
  MultBins.push_back(40);
  MultBins.push_back(44);
  MultBins.push_back(48);
  MultBins.push_back(52);
  MultBins.push_back(56);
  MultBins.push_back(60);
  MultBins.push_back(64);
  MultBins.push_back(68);
  MultBins.push_back(72);
  MultBins.push_back(76);
  MultBins.push_back(80);
  MultBins.push_back(84);
  MultBins.push_back(88);
  MultBins.push_back(92);
  MultBins.push_back(96);
  MultBins.push_back(100);
  config->SetMultBins(MultBins);

  config->SetMultBinning(false);
  config->SetCentBinning(false);
  config->SetkTBinning(false);
  config->SetmTBinning(false);

  std::vector<float> centBins;
  centBins.push_back(20);
  centBins.push_back(40);
  centBins.push_back(90);
  config->SetCentBins(centBins);
  config->SetkTCentralityBinning(false);

  config->SetInvMassPairs(false);

  if (isMC) {
    config->SetMomentumResolution(false);//kstar true vs. kstar reco
  } else {
    std::cout
        << "You are trying to request the Momentum Resolution without MC Info; fix it wont work! \n";
  }
  if (isMC) {
    config->SetPhiEtaBinnign(false);  // check for track splitting/merging at various TPC Radii
  } else {
    std::cout
        << "You are trying to request the Eta Phi Plots without MC Info; fix it wont work! \n";
  }
//  if (DeltaEtaDeltaPhiCut) {
//    config->SetDeltaEtaMax(0.01);
//    config->SetDeltaPhiMax(0.01);
//  }
  config->SetdPhidEtaPlots(false);  // warsaw like plots

  std::vector<int> NBins;
  NBins.push_back(750);  // p p
  NBins.push_back(750);  // p barp
//  NBins.push_back(750);  // p Lambda
//  NBins.push_back(750);  // p barLambda
//  NBins.push_back(750);  // p Xi
//  NBins.push_back(750);  // p barXi
  NBins.push_back(750);  // barp barp
//  NBins.push_back(750);  // barp Lambda
//  NBins.push_back(750);  // barp barLambda
//  NBins.push_back(750);  // barp Xi
//  NBins.push_back(750);  // barp barXi
//  NBins.push_back(750);  // Lambda Lambda
//  NBins.push_back(750);  // Lambda barLambda
//  NBins.push_back(750);  // Lambda Xi
//  NBins.push_back(750);  // Lambda barXi
//  NBins.push_back(750);  // barLambda barLambda
//  NBins.push_back(750);  // barLambda Xi
//  NBins.push_back(750);  // barLambda barXi
//  NBins.push_back(750);  // Xi Xi
//  NBins.push_back(750);  // Xi barXi
//  NBins.push_back(750);  // barXi barXi
  std::vector<float> kMin;
  kMin.push_back(0.);
  kMin.push_back(0.);
  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
//  kMin.push_back(0.);
  std::vector<float> kMax;
  kMax.push_back(3.);
  kMax.push_back(3.);
  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
//  kMax.push_back(3.);
  config->SetNBinsHist(NBins);
  config->SetMinKRel(kMin);
  config->SetMaxKRel(kMax);

  config->SetUseEventMixing(true);
  config->SetMixingDepth(10);

  config->SetUsePhiSpinning(false);
//  config->SetSpinningDepth(10);
//  config->SetUseStravinskyMethod(false);

  config->SetMinimalBookingME(false);
  config->SetMinimalBookingSample(true);

  config->SetMultiplicityEstimator(AliFemtoDreamEvent::kRef08);

  task->SetCollectionConfig(config);

  if (CentEst == "kInt7") {
    task->SelectCollisionCandidates(AliVEvent::kINT7);
    std::cout << "Added kINT7 Trigger \n";
  } else if (CentEst == "kMB") {
    task->SelectCollisionCandidates(AliVEvent::kMB);
    std::cout << "Added kMB Trigger \n";
  } else if (CentEst == "kHM") {
    task->SelectCollisionCandidates(AliVEvent::kHighMultV0);
    std::cout << "Added kHighMultV0 Trigger \n";
  } else {
    std::cout
        << "====================================================================="
        << std::endl;
    std::cout
        << "====================================================================="
        << std::endl;
    std::cout
        << "Centrality Estimator not set, fix it else your Results will be empty!"
        << std::endl;
    std::cout
        << "====================================================================="
        << std::endl;
    std::cout
        << "====================================================================="
        << std::endl;
  }
  mgr->AddTask(task);

  TString file = AliAnalysisManager::GetCommonFileName();

  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  mgr->ConnectInput(task, 0, cinput);

  AliAnalysisDataContainer *coutputEvtCuts;
  TString QAName = Form("QA");
  AliAnalysisDataContainer *coutputQA;
  coutputQA = mgr->CreateContainer(
      //@suppress("Invalid arguments") it works ffs
      QAName.Data(),
      TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), QAName.Data()));
  mgr->ConnectOutput(task, 1, coutputQA);

  TString EvtCutsName = Form("EvtCuts");
  coutputEvtCuts = mgr->CreateContainer(
      //@suppress("Invalid arguments") it works ffs
      EvtCutsName.Data(),
      TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), EvtCutsName.Data()));
  mgr->ConnectOutput(task, 2, coutputEvtCuts);

  AliAnalysisDataContainer *couputTrkCuts;
  TString TrackCutsName = Form("TrackCuts");
  couputTrkCuts = mgr->CreateContainer(
      //@suppress("Invalid arguments") it works ffs
      TrackCutsName.Data(),
      TList::Class(),
      AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), TrackCutsName.Data()));
  mgr->ConnectOutput(task, 3, couputTrkCuts);

  AliAnalysisDataContainer *coutputAntiTrkCuts;
  TString AntiTrackCutsName = Form("AntiTrackCuts");
  coutputAntiTrkCuts = mgr->CreateContainer(
      //@suppress("Invalid arguments") it works ffs
      AntiTrackCutsName.Data(),
      TList::Class(),
      AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), AntiTrackCutsName.Data()));
  mgr->ConnectOutput(task, 4, coutputAntiTrkCuts);

  AliAnalysisDataContainer *coutputResults;
  TString ResultsName = Form("Results");
  coutputResults = mgr->CreateContainer(
      //@suppress("Invalid arguments") it works ffs
      ResultsName.Data(),
      TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), ResultsName.Data()));
  mgr->ConnectOutput(task, 5, coutputResults);

  AliAnalysisDataContainer *coutputResultQA;
  TString ResultQAName = Form("ResultQA");
  coutputResultQA = mgr->CreateContainer(
      //@suppress("Invalid arguments") it works ffs
      ResultQAName.Data(),
      TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:%s", file.Data(), ResultQAName.Data()));
  mgr->ConnectOutput(task, 6, coutputResultQA);

  if (isMC) {
    AliAnalysisDataContainer *coutputTrkCutsMC;
    TString TrkCutsMCName = Form("TrkCutsMC");
    coutputTrkCutsMC = mgr->CreateContainer(
        //@suppress("Invalid arguments") it works ffs
        TrkCutsMCName.Data(),
        TList::Class(),
        AliAnalysisManager::kOutputContainer,
        Form("%s:%s", file.Data(), TrkCutsMCName.Data()));
    mgr->ConnectOutput(task, 7, coutputTrkCutsMC);

    AliAnalysisDataContainer *coutputAntiTrkCutsMC;
    TString AntiTrkCutsMCName = Form("AntiTrkCutsMC");
    coutputAntiTrkCutsMC = mgr->CreateContainer(
        //@suppress("Invalid arguments") it works ffs
        AntiTrkCutsMCName.Data(),
        TList::Class(),
        AliAnalysisManager::kOutputContainer,
        Form("%s:%s", file.Data(), AntiTrkCutsMCName.Data()));
    mgr->ConnectOutput(task, 8, coutputAntiTrkCutsMC);
  }

  return task;

}
