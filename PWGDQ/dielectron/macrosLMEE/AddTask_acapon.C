//Names should contain a comma seperated list of cut settings
//Current options: all, electrons, TTreeCuts, V0_TPCcorr, V0_ITScorr
AliAnalysisTask *AddTask_acapon(TString outputFileName = "AnalysisResult.root", TString names ="electrons",
                                Bool_t SDDstatus = kFALSE, Bool_t getFromAlien = kFALSE,
                                Bool_t doPairing = kTRUE,  Bool_t doMixing = kTRUE,
                                Bool_t useITScorr = kFALSE, Bool_t useTPCcorr = kFALSE, Bool_t useTOFcorr = kFALSE,
																Int_t wagonNum = 0){
  
		TObjArray *arrNames = names.Tokenize(";");
		Int_t nDie = arrNames->GetEntries();
    Printf("Number of implemented cuts: %i", nDie);

    //get the current analysis manager
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr) {
        Error("AddTask_acapon", "No analysis manager found.");
        return 0;
    }

    Bool_t bESDANA=kFALSE; //Autodetect via InputHandler

    //TString configBasePath("/home/aaron/analyses/LHC16q/eeFrameworkQA/"); //Local
    TString configBasePath("$ALICE_PHYSICS/PWGDQ/dielectron/macrosLMEE/"); //AliPhysics
    TString configFile("Config_acapon.C");
    TString configLMEECutLib("LMEECutLib_acapon.C");

    //Load updated macros from private ALIEN path
    TString myConfig ="alien_cp alien:///alice/cern.ch/user/a/acapon/dielectronShizzle/Config_acapon.C .";
    TString myCutLib ="alien_cp alien:///alice/cern.ch/user/a/acapon/dielectronShizzle/LMEECutLib_acapon.C ."; 
    if (getFromAlien && (!gSystem->Exec(myConfig)) && (!gSystem->Exec(myCutLib))){

        std::cout << "Copy config from Alien" << std::endl;
        configBasePath=Form("%s/",gSystem->pwd());
    }

    TString configFilePath(configBasePath+configFile);
    TString configLMEECutLibPath(configBasePath+configLMEECutLib);

    //load dielectron configuration files
    if(!gROOT->GetListOfGlobalFunctions()->FindObject(configLMEECutLib.Data())){
        gROOT->LoadMacro(configLMEECutLibPath.Data());
    }
    if(!gROOT->GetListOfGlobalFunctions()->FindObject(configFile.Data())){
        gROOT->LoadMacro(configFilePath.Data());
    }

    // cut lib
    LMEECutLib* cutlib = new LMEECutLib(SDDstatus);

    //Do we have an MC handler?
    Bool_t hasMC=(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler()! = 0x0);

    //AOD Usage currently tested with Input handler
    if (mgr->GetInputEventHandler()->IsA()==AliAODInputHandler::Class()){
        ::Info("AddTask_acapon", "no dedicated AOD configuration"); //Prepended :: ensures resolution occurs from global namespace, not current one
    }
    else if (mgr->GetInputEventHandler()->IsA()==AliESDInputHandler::Class()){
        ::Info("AddTask_acapon","switching on ESD specific code");
        bESDANA=kTRUE;
    }

    //create task and add it to the manager
    AliAnalysisTaskMultiDielectron *task = new AliAnalysisTaskMultiDielectron(Form("DielectronTask%d", wagonNum));
		if(!task){
			::Error("AddTask_acapon", "MultiDielectron trask not created");
			return 0x0;
		}

		//Add event filter
    Int_t triggerNames = (AliVEvent::kINT7);
    task->SetEventFilter(cutlib->GetEventCuts(LMEECutLib::kAllSpecies));
    task->SelectCollisionCandidates(triggerNames);
    task->SetTriggerMask(triggerNames);
    //task->SetRejectPileup();
    // Note: event cuts are identical for all analysis 'cutDefinition's that run together!

    // Add the task to the manager
    mgr->AddTask(task);
    //add dielectron analysis with different cuts to the task
    for (Int_t i=0; i<nDie; ++i){ 
        //MB
				TString dielTaskName(arrNames->At(i)->GetName());
        AliDielectron *diel_low = Config_acapon(dielTaskName, hasMC, bESDANA, SDDstatus, doPairing, doMixing, useITScorr, useTPCcorr, useTOFcorr);
        if(!diel_low){ continue; }
        task->AddDielectron(diel_low);
        printf("successfully added AliDielectron: %s\n",diel_low->GetName());
    }//loop


    //create output container
    AliAnalysisDataContainer *coutput1 =
    mgr->CreateContainer(Form("acapon_tree_%d",wagonNum),
                         TTree::Class(),
                         AliAnalysisManager::kExchangeContainer,
                         outputFileName.Data());

    AliAnalysisDataContainer *cOutputHist1 =
    mgr->CreateContainer(Form("acapon_out_%d", wagonNum),
                         TList::Class(),
                         AliAnalysisManager::kOutputContainer,
                         outputFileName.Data());

    AliAnalysisDataContainer *cOutputHist2 =
    mgr->CreateContainer(Form("acapon_CF_%d", wagonNum),
                         TList::Class(),
                         AliAnalysisManager::kOutputContainer,
                         outputFileName.Data());

    AliAnalysisDataContainer *cOutputHist3 =
    mgr->CreateContainer(Form("acapon_EventStat_%d", wagonNum),
                         TH1D::Class(),
                         AliAnalysisManager::kOutputContainer,
                         outputFileName.Data());


    mgr->ConnectInput(task,  0, mgr->GetCommonInputContainer());
    mgr->ConnectOutput(task, 0, coutput1 );
    mgr->ConnectOutput(task, 1, cOutputHist1);
    mgr->ConnectOutput(task, 2, cOutputHist2);
    mgr->ConnectOutput(task, 3, cOutputHist3);

    return task;
}
