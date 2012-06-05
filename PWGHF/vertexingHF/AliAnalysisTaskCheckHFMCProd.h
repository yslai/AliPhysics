#ifndef ALIANALYSISTASKCHECKHFMCPROD
#define ALIANALYSISTASKCHECKHFMCPROD

/* Copyright(c) 1998-2012, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */ 

//*************************************************************************
// Class AliAnalysisTaskCheckHFMCProd
// AliAnalysisTask to check MC production at ESD+Kine level
// 
//
// Author: F. Prino, prino@to.infn.it
//          
//*************************************************************************

class TList;
class TNtuple;
class TH1F;
class TH2F;
class TTree;
class TString;
class AliESDEvent;
class AliESDfriend;
class AliStack;

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskCheckHFMCProd : public AliAnalysisTaskSE {

 public:
  
  AliAnalysisTaskCheckHFMCProd();
  virtual ~AliAnalysisTaskCheckHFMCProd();

  virtual void   UserExec(Option_t *option);
  virtual void   UserCreateOutputObjects();
  virtual void   Terminate(Option_t *option);

  Int_t CheckD0Decay(Int_t labD0, AliStack* stack) const;
  Int_t CheckDplusDecay(Int_t labDplus, AliStack* stack) const;
  Int_t CheckDsDecay(Int_t labDs, AliStack* stack) const;
  Int_t CheckDstarDecay(Int_t labDstar, AliStack* stack) const;
  Int_t CheckLcDecay(Int_t labLc, AliStack* stack) const;

  void SetReadMC(Bool_t opt) {fReadMC=opt;}
  void SetPbPb() {fPbPb=kTRUE;}
  void Setpp() {fPbPb=kFALSE;}

 private:

  AliAnalysisTaskCheckHFMCProd(const AliAnalysisTaskCheckHFMCProd &source);
  AliAnalysisTaskCheckHFMCProd& operator=(const AliAnalysisTaskCheckHFMCProd &source);
  
  TList* fOutput;          //! list of output histos
  TH1F* fHistoNEvents;     //! histo with N of events  
  
  TH1F* fHistoTracks;      //! histo with number of ESD tracks
  TH1F* fHistoSelTracks;   //! histo with number of TPC+ITS refit ESD tracks
  TH1F* fHistoTracklets;   //! histo with number of SPD tracklets
  
  TH1F* fHistoSPD3DVtxX;     //! histo with distr. of x coord. of SPD3D vertex
  TH1F* fHistoSPD3DVtxY;     //! histo with distr. of y coord. of SPD3D vertex
  TH1F* fHistoSPD3DVtxZ;     //! histo with distr. of z coord. of SPD3D vertex

  TH1F* fHistoSPDZVtxX;     //! histo with distr. of x coord. of SPDZ vertex
  TH1F* fHistoSPDZVtxY;     //! histo with distr. of y coord. of SPDZ vertex
  TH1F* fHistoSPDZVtxZ;     //! histo with distr. of z coord. of SPDZ vertex

  TH1F* fHistoTRKVtxX;     //! histo with distr. of x coord. of TRK vertex
  TH1F* fHistoTRKVtxY;     //! histo with distr. of y coord. of TRK vertex
  TH1F* fHistoTRKVtxZ;     //! histo with distr. of z coord. of TRK vertex

  TH2F* fHistoNcharmed;   //! histo of D mesons vs. dN/dy
  TH2F* fHistoNbVsNc;     //! histo of n. b quarks vs. n c. quarks

  TH2F*  fHistYPtAllDecay[5];   //! histo of y vs. pt from prompt D0, D+, D*, Ds, Lc, no selection on decay channel  
  TH2F*  fHistYPtPromptAllDecay[5];   //! histo of y vs. pt from prompt D0, D+, D*, Ds, Lc, no selection on decay channel  
  TH2F*  fHistYPtFeeddownAllDecay[5];   //! histo of y vs. pt from prompt D0, D+, D*, Ds, Lc, no selection on decay channel
  TH2F*  fHistYPtPrompt[5];   //! histo of y vs. pt from prompt D0, D+, D*, Ds, Lc
  TH2F*  fHistYPtFeeddown[5]; //! histo of y vs. pt from feeddown D0, D+, D*, Ds, Lc
  TH2F* fHistYPtD0byDecChannel[2]; //! histo of y vs. pt for D0->Kpi and D0->Kpipipi
  TH2F* fHistYPtDplusbyDecChannel[2]; //! histo of y vs. pt for D+->Kpipi and D+->K0*pi
  TH2F* fHistYPtDsbyDecChannel[2]; //! histo of y vs. pt for Ds->phipi and Ds->K0*K

  Bool_t fPbPb;
  Bool_t fReadMC;

  ClassDef(AliAnalysisTaskCheckHFMCProd,1);  
};


#endif
