#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THnSparse.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TRandom2.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliMultSelection.h"
#include "AliInputEventHandler.h"
#include "AliESDInputHandler.h"
#include "AliESDtrackCuts.h"
#include "AliESDtrack.h"
#include "AliESDpid.h"
#include "AliESDVertex.h"
#include "AliESDEvent.h"
#include "AliESDv0.h"
#include "AliESDVertex.h"
#include "AliVertexerTracks.h"
#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliMCVertex.h"
#include "AliStack.h"
#include "TPDGCode.h"
#include "AliAnalysisTaskHypTritEventTree.h"
#include "AliReducedHypTritEvent.h"

using namespace std;

ClassImp(AliAnalysisTaskHypTritEventTree)

// Default Constructor
AliAnalysisTaskHypTritEventTree::AliAnalysisTaskHypTritEventTree()
  :AliAnalysisTaskSE("AliAnalysisTaskHypTritEventTree"),
  fEvent(0),
  fInputHandler(0),
  fTrackCutsV0(0),
  fPID(0),
  fMCtrue(0),
  fHistdEdx(0),
  fHistdEdxProton(0),
  fHistdEdxDeuteron(0),
  fHistdEdxTriton(0),
  fHistdEdxHelium3(0),
  fHistdEdxHypTriton(0),
  fHistdEdxHypTritonAnti(0),
  fHistInvMassHypTriton(0),
  fHistCentrality(0),
  fHistTrigger(0),
  fHistNumEvents(0),
  fTree(0),
  fTreeMCGen(0),
  fReducedEvent(0),
  fReducedEventMCGen(0),
  fPosVx(0),
  fPosVy(0),
  fPosVz(0),
  fMCGenRec(),
  fMCGenRecArray(),
  fEtaCorrection(kFALSE),
  fMultiplicityCorrection(kFALSE),
  fHistogramList(NULL) {
}

// Constructor
AliAnalysisTaskHypTritEventTree::AliAnalysisTaskHypTritEventTree(const char *name)
  :AliAnalysisTaskSE(name),
  fEvent(0),
  fInputHandler(0),
  fTrackCutsV0(0),
  fPID(0),
  fMCtrue(0),
  fHistdEdx(0),
  fHistdEdxProton(0),
  fHistdEdxDeuteron(0),
  fHistdEdxTriton(0),
  fHistdEdxHelium3(0),
  fHistdEdxHypTriton(0),
  fHistdEdxHypTritonAnti(0),
  fHistInvMassHypTriton(0),
  fHistCentrality(0),
  fHistTrigger(0),
  fHistNumEvents(0),
  fTree(0),
  fTreeMCGen(0),
  fReducedEvent(0),
  fReducedEventMCGen(0),
  fPosVx(0),
  fPosVy(0),
  fPosVz(0),
  fMCGenRec(),
  fMCGenRecArray(),
  fEtaCorrection(kFALSE),
  fMultiplicityCorrection(kFALSE),
  fHistogramList(NULL) {
  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());
  DefineOutput(2, TTree::Class());
  DefineOutput(3, TTree::Class());
  fTrackCutsV0 = new AliESDtrackCuts("AlitrackCutsV0", "AlitrackCutsV0");
  fTrackCutsV0->SetEtaRange(-0.9,0.9);
  fTrackCutsV0->SetAcceptKinkDaughters(kFALSE);
  fTrackCutsV0->SetRequireTPCRefit(kTRUE);
  fTrackCutsV0->SetMaxChi2PerClusterTPC(5);
  fTrackCutsV0->SetMinNClustersTPC(60);
  fMCtrue = kTRUE;
  for (int i = 0; i < 40; i++) {
    fMCGenRec[i] = 0;
  }
}

// Destructor
AliAnalysisTaskHypTritEventTree::~AliAnalysisTaskHypTritEventTree() {
  if (fTrackCutsV0) delete fTrackCutsV0;
}

/////////////////////////////////////////////////////////////////////////////
// Creates histograms and output containers. Called once before main loop. //
/////////////////////////////////////////////////////////////////////////////
void AliAnalysisTaskHypTritEventTree::UserCreateOutputObjects() {
  fInputHandler = dynamic_cast<AliESDInputHandler*>
    (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
  if(!fInputHandler) {
    AliError("Could not get ESD InputHandler.\n");
    return;
  }
  fPID = fInputHandler->GetESDpid();
  if (!fPID) {
    AliError("Could not get PID response.\n");
    return;
  }
  fHistdEdx = new TH2F("fHistdEdX","dE/dx;#frac{#it{p}}{z} (GeV/#it{c});TPC Signal (a.u.)",400,-4.0,4.0,500,0.0,1500);
  fHistdEdxProton = new TH2F("fHistdEdXProton","dE/dx Deuterons;#frac{#it{p}}{z} (GeV/#it{c});TPC Signal (a.u.)",400,-4.0,4.0,500,0.0,1500);
  fHistdEdxDeuteron = new TH2F("fHistdEdXDeutron","dE/dx Deuterons;#frac{#it{p}}{z} (GeV/#it{c});TPC Signal (a.u.)",400,-4.0,4.0,500,0.0,1500);
  fHistdEdxTriton = new TH2F("fHistdEdXTriton","dE/dx Triton;#frac{#it{p}}{z} (GeV/#it{c});TPC Signal (a.u.)",400,-4.0,4.0,500,0.0,1500);
  fHistdEdxHelium3 = new TH2F("fHistdEdXHelium3","dE/dx Helium3;#frac{#it{p}}{z} (GeV/#it{c});TPC Signal (a.u.)",400,-4.0,4.0,500,0.0,1500);
  fHistdEdxHypTriton = new TH2F("fHistdEdXHypTriton","dE/dx HypTriton Daughters;#frac{#it{p}}{z} (GeV/#it{c});TPC Signal (a.u.)",400,-4.0,4.0,500,0.0,1500);
  fHistdEdxHypTritonAnti = new TH2F("fHistdEdXHypTritonAnti","dE/dx HypTriton Daughters;#frac{#it{p}}{z} (GeV/#it{c});TPC Signal (a.u.)",400,-4.0,4.0,500,0.0,1500);
  fHistInvMassHypTriton = new TH1F("fHistInvMassHypTriton","Inv. Mass;Invariant Mass (GeV/#it{c}^{2})",100,2.9,3.1);
  fHistCentrality = new TH1F("fHistCentrality","Centrality; Centrality; Counts",101,-1,100);
  fHistTrigger = new TH1F("fHistTrigger","fired Triggers; Trigger; number of Events",3,0,3);
  fHistNumEvents = new TH1F("fHistNumEvents","Number of Events",2,0,2);
  fHistNumEvents->GetXaxis()->SetBinLabel(1,"before PhysSel");
  fHistNumEvents->GetXaxis()->SetBinLabel(2,"after PhysSel");
  fHistogramList = new TList();
  fHistogramList->SetOwner(kTRUE);
  fHistogramList->SetName(GetName());
  fHistogramList->Add(fHistdEdx);
  fHistogramList->Add(fHistdEdxProton);
  fHistogramList->Add(fHistdEdxDeuteron);
  fHistogramList->Add(fHistdEdxTriton);
  fHistogramList->Add(fHistdEdxHelium3);
  fHistogramList->Add(fHistdEdxHypTriton);
  fHistogramList->Add(fHistdEdxHypTritonAnti);
  fHistogramList->Add(fHistInvMassHypTriton);
  fHistogramList->Add(fHistCentrality);
  fHistogramList->Add(fHistTrigger);
  fHistogramList->Add(fHistNumEvents);
  fTree = new TTree("tree","fTree");
  fReducedEvent = new AliReducedHypTritEvent();
  fTree->Branch("event","AliReducedHypTritEvent",&fReducedEvent,32000,99);
  fTreeMCGen = new TTree("tree_mc", "fTreeMCGen");
  fReducedEventMCGen = new AliReducedHypTritEvent();
  fTreeMCGen->Branch("event","AliReducedHypTritEvent",&fReducedEventMCGen,32000,99);
  PostData(1, fHistogramList);
  PostData(2, fTree);
  PostData(3, fTreeMCGen);
}

///////////////////////////////////////////////////
// Main loop over events. Called for each event. //
///////////////////////////////////////////////////
void AliAnalysisTaskHypTritEventTree::UserExec(Option_t *) {
  // MC
  AliMCEventHandler *mcEventHandler = dynamic_cast<AliMCEventHandler*>
      (AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler());
  if (!mcEventHandler) {
    fMCtrue = kFALSE;
  }
  AliMCEvent *mcEvent = 0x0;
  AliStack *stack = 0x0;
  if (mcEventHandler) mcEvent = mcEventHandler->MCEvent();
  if (!mcEvent) {
    if (fMCtrue) return;
  }
    if (fMCtrue) {
    stack = mcEvent->Stack();
    if (!stack) return;
  }
  // Data
  fEvent = dynamic_cast<AliESDEvent*>(InputEvent());
  if (!fEvent) {
    AliError("Could not get ESD Event.\n");
    return;
  }
  if (!fPID) {
    AliError("Could not get PID response.\n");
    return;
  }
  fPID->SetUseTPCEtaCorrection(fEtaCorrection);
  fPID->SetUseTPCMultiplicityCorrection(fMultiplicityCorrection);
  for (int i = 0; i < 40; i++) {
    fMCGenRec[i] = -1;
  }
  fHistNumEvents->Fill(0);
  // Physics selection and primary vertex < 10 cm cut.
  const AliESDVertex *vertex = fEvent->GetPrimaryVertexTracks();
    if (vertex->GetNContributors() < 1) {
      vertex = fEvent->GetPrimaryVertexSPD();
      if (vertex->GetNContributors() < 1) {
        PostData(1,fHistogramList);
        return;
      }
    }
  if (TMath::Abs(vertex->GetZ()) > 10) {
    PostData(1, fHistogramList);
    return;
  }
  TriggerSelection();
  AliMultSelection *multSelection = dynamic_cast<AliMultSelection*>(fEvent->FindListObject("MultSelection"));
  Float_t centrality = -1;
  if ( multSelection ){
    centrality = multSelection->GetMultiplicityPercentile("V0M",kFALSE);
  }else{
    AliInfo("Didn't find MultSelection.\n");
  }
  Int_t runNumber = fEvent->GetRunNumber();
  if(!fMCtrue){
    if (centrality < 0.0 || centrality > 100.0 ) {
      return;
    }
  }
  fHistNumEvents->Fill(1);
  fHistCentrality->Fill(centrality);
  fReducedEvent->fCentrality = centrality;
  Double_t magneticField  = fEvent->GetMagneticField();
  Double_t xPrimaryVertex = vertex->GetX();
  Double_t yPrimaryVertex = vertex->GetY();
  Double_t zPrimaryVertex = vertex->GetZ();
  TVector3 primaryVertex(xPrimaryVertex,yPrimaryVertex,zPrimaryVertex);
  fReducedEvent->fVertexPosition = primaryVertex;
  fReducedEvent->fRunNumber = runNumber;
  Int_t mcGenRecCounter = 0;
  TClonesArray *v0Array = fReducedEvent->fV0s;
  Int_t nV0Cand = 0;
  fMCGenRecArray = new TObjArray();
  // Loops over V0s.
  for (Int_t ivertex = 0; ivertex < fEvent->GetNumberOfV0s(); ivertex++) {
    AliESDv0 *v0 = fEvent->GetV0(ivertex);
    Bool_t isreconstructed = kFALSE;
    isreconstructed = v0->GetOnFlyStatus();
    if (!isreconstructed) continue;
    Float_t dcav0 = v0->GetDcaV0Daughters();
    Float_t cosineOfPointingAngle = v0->GetV0CosineOfPointingAngle();
    Float_t decayRadius = v0->GetRr();
    Bool_t v0ChargeCorrect = kTRUE;
    AliESDtrack *trackN = fEvent->GetTrack(v0->GetIndex(0));
    AliESDtrack *trackP = fEvent->GetTrack(v0->GetIndex(1));
    // Checks charge because of bug in V0 interface.
    if (trackN->GetSign() > 0 ) {
      trackN = fEvent->GetTrack(v0->GetIndex(1));
      trackP = fEvent->GetTrack(v0->GetIndex(0));
      v0ChargeCorrect = kFALSE;
    }
    if (!fTrackCutsV0->AcceptTrack(trackN)) continue;
    if (!fTrackCutsV0->AcceptTrack(trackP)) continue;
    Double_t momentumN = trackN->GetInnerParam()->GetP();
    Double_t momentumP = trackP->GetInnerParam()->GetP();
    fHistdEdx->Fill(momentumP*trackP->GetSign(), trackP->GetTPCsignal());
    fHistdEdx->Fill(momentumN*trackN->GetSign(), trackN->GetTPCsignal());
    // Identifies particle of trackN and trackP using specific energyloss in TPC.
    // Distinguishes charge of particle.
    Bool_t pionPositive     = kFALSE;
    Bool_t pionNegative     = kFALSE;
    Bool_t protonPositive   = kFALSE;
    Bool_t protonNegative   = kFALSE;
    Bool_t deuteronPositive = kFALSE;
    Bool_t deuteronNegative = kFALSE;
    Bool_t tritonPositive   = kFALSE;
    Bool_t tritonNegative   = kFALSE;
    Bool_t helium3Positive  = kFALSE;
    Bool_t helium3Negative  = kFALSE;
    Double_t piondedxsigma = -1;
    Double_t hededxsigma = -1;
    if (TMath::Abs(fPID->NumberOfSigmasTPC(trackP, AliPID::kPion)) < 3
        && trackP->GetSign() > 0) {
      pionPositive = kTRUE;
      piondedxsigma = TMath::Abs(fPID->NumberOfSigmasTPC(trackP, AliPID::kPion));
    }
    if (TMath::Abs(fPID->NumberOfSigmasTPC(trackN, AliPID::kPion)) < 3
        && trackN->GetSign() < 0) {
      pionNegative = kTRUE;
      piondedxsigma = TMath::Abs(fPID->NumberOfSigmasTPC(trackN, AliPID::kPion));
    }
    if (TMath::Abs(fPID->NumberOfSigmasTPC(trackN, AliPID::kProton)) < 3
        && trackN->GetSign() < 0) {
      protonNegative = kTRUE;
      fHistdEdxProton->Fill(momentumN*trackN->GetSign(), trackN->GetTPCsignal());
    }
    if (TMath::Abs(fPID->NumberOfSigmasTPC(trackP, AliPID::kProton)) < 3
        && trackP->GetSign() > 0) {
      protonPositive = kTRUE;
      fHistdEdxProton->Fill(momentumP*trackP->GetSign(), trackP->GetTPCsignal());
    }
    if (TMath::Abs(fPID->NumberOfSigmasTPC(trackN, AliPID::kDeuteron)) < 3
        && trackN->GetSign() < 0) {
      deuteronNegative = kTRUE;
      fHistdEdxDeuteron->Fill(momentumN*trackN->GetSign(), trackN->GetTPCsignal());
    }
    if (TMath::Abs(fPID->NumberOfSigmasTPC(trackP, AliPID::kDeuteron)) < 3
        && trackP->GetSign() > 0) {
      deuteronPositive = kTRUE;
      fHistdEdxDeuteron->Fill(momentumP*trackP->GetSign(), trackP->GetTPCsignal());
    }
     if (TMath::Abs(fPID->NumberOfSigmasTPC(trackN, AliPID::kTriton)) < 3
         && trackN->GetSign() < 0) {
       tritonNegative = kTRUE;
       fHistdEdxTriton->Fill(momentumN*trackN->GetSign(), trackN->GetTPCsignal());
     }
     if (TMath::Abs(fPID->NumberOfSigmasTPC(trackP, AliPID::kTriton)) < 3
         && trackP->GetSign() > 0) {
       tritonPositive = kTRUE;
      fHistdEdxTriton->Fill(momentumP*trackP->GetSign(), trackP->GetTPCsignal());
     }
     if (TMath::Abs(fPID->NumberOfSigmasTPC(trackN, AliPID::kHe3)) < 4
         && trackN->GetSign() < 0) {
       helium3Negative = kTRUE;
       fHistdEdxHelium3->Fill(momentumN*trackN->GetSign(), trackN->GetTPCsignal());
       hededxsigma = TMath::Abs(fPID->NumberOfSigmasTPC(trackN, AliPID::kHe3));
     }
     if (TMath::Abs(fPID->NumberOfSigmasTPC(trackP, AliPID::kHe3)) < 4
         && trackP->GetSign() > 0) {
       helium3Positive = kTRUE;
       fHistdEdxHelium3->Fill(momentumP*trackP->GetSign(), trackP->GetTPCsignal());
       hededxsigma = TMath::Abs(fPID->NumberOfSigmasTPC(trackP, AliPID::kHe3));
     }
    // Checks if Charge of V0 was assigned correctly and sets the momenta of
    // daughter particles. Factor 2 because momentum * charge is measured.
    TVector3       momentumVector(0,0,0);
    TLorentzVector momentumPion(0,0,0,0);
    TLorentzVector momentumNucleon(0,0,0,0);
    TLorentzVector momentumMother(0,0,0,0);
    if (helium3Negative) {
      v0->GetPPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
      momentumPion.SetVect(momentumVector);
      v0->GetNPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
      momentumNucleon.SetVect(2*momentumVector);
      if (!v0ChargeCorrect) {
        v0->GetNPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
        momentumPion.SetVect(momentumVector);
        v0->GetPPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
        momentumNucleon.SetVect(2*momentumVector);
      }
    }
    if (helium3Positive) {
      v0->GetNPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
      momentumPion.SetVect(momentumVector);
      v0->GetPPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
      momentumNucleon.SetVect(2*momentumVector);
      if (!v0ChargeCorrect) {
        v0->GetPPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
        momentumPion.SetVect(momentumVector);
        v0->GetNPxPyPz(momentumVector(0), momentumVector(1), momentumVector(2));
        momentumNucleon.SetVect(2*momentumVector);
      }
    }
    // Calculates invariant Mass and momentum of (Anti-)HyperTriton using
    // TLorentzVectors. Fills information into histograms and tree.
    if ((helium3Negative && pionPositive) || (helium3Positive && pionNegative)) {
      momentumPion.SetE(TMath::Sqrt(AliPID::ParticleMass(AliPID::kPion) *
          AliPID::ParticleMass(AliPID::kPion) + momentumPion.Vect().Mag2()));
      momentumNucleon.SetE(TMath::Sqrt(AliPID::ParticleMass(AliPID::kHe3) *
          AliPID::ParticleMass(AliPID::kHe3) + momentumNucleon.Vect().Mag2()));
      Double_t hypTritInvMass = (momentumNucleon + momentumPion).M();
      Double_t hypTritPt      = (momentumNucleon + momentumPion).Pt();
      Double_t hypTritP       = (momentumNucleon + momentumPion).P();
      Double_t rapidity       = (momentumNucleon + momentumPion).Rapidity();
      AliReducedHypTritV0 *reducedV0 = dynamic_cast<AliReducedHypTritV0*>(v0Array->ConstructedAt(nV0Cand));
      nV0Cand = nV0Cand + 1;
      AliReducedHypTritTrack *reducedPi = reducedV0->Pi();
      AliReducedHypTritTrack *reducedHe = reducedV0->He();
      TVector3 secondaryVertex(v0->Xv(), v0->Yv(), v0->Zv());
      Int_t mctruth = 0;
      reducedV0->fCharge = 1;
      if (helium3Negative) reducedV0->fCharge = -1;
      reducedHe->fP = momentumNucleon;
      reducedPi->fP = momentumPion;
      reducedV0->fPosition = secondaryVertex;
      reducedPi->fDedxSigma = piondedxsigma;
      reducedHe->fDedxSigma = hededxsigma;
      if (helium3Negative) {
      reducedHe->fDedx = trackN->GetTPCsignal();
      reducedPi->fDedx = trackP->GetTPCsignal();
      reducedPi->fEta = trackP->Eta();
      reducedHe->fEta = trackN->Eta();
      reducedPi->fPhi = trackP->Phi();
      reducedHe->fPhi = trackN->Phi();
      reducedHe->fTpcNClusters = trackN->GetTPCNcls();
      reducedPi->fTpcNClusters = trackP->GetTPCNcls();
      }
      if (helium3Positive) {
      reducedHe->fDedx = trackP->GetTPCsignal();
      reducedPi->fDedx = trackN->GetTPCsignal();
      reducedPi->fEta = trackN->Eta();
      reducedHe->fEta = trackP->Eta();
      reducedPi->fPhi = trackN->Phi();
      reducedHe->fPhi = trackP->Phi();
      reducedHe->fTpcNClusters = trackP->GetTPCNcls();
      reducedPi->fTpcNClusters = trackN->GetTPCNcls();
      }
      reducedV0->fChi2 = v0->GetChi2V0();
      reducedV0->fM = hypTritInvMass;
      reducedV0->fP= hypTritP;
      reducedV0->fPt = hypTritPt;
      reducedV0->fDCAv0 = dcav0;
      reducedV0->fSigmaD0 = v0->GetSigmaD0();
      reducedV0->fCosPointingAngle = cosineOfPointingAngle;
      secondaryVertex = secondaryVertex - primaryVertex;
      Double_t decayLength = secondaryVertex.Mag() * hypTritInvMass / hypTritP;
      reducedV0->fDecayLength = decayLength;
      reducedV0->fMCTruth = mctruth;
      reducedV0->fRapidity = rapidity;
      fHistInvMassHypTriton->Fill(hypTritInvMass);
      if (fMCtrue) {
        Int_t labelP = trackP->GetLabel();
        Int_t labelN = trackN->GetLabel();
        TParticle *daughterParticleP = stack->Particle(TMath::Abs(labelP));
        TParticle *daughterParticleN = stack->Particle(TMath::Abs(labelN));
        TParticle *particleMotherP = stack->Particle(TMath::Abs(daughterParticleP->GetFirstMother()));
        TParticle *particleMotherN = stack->Particle(TMath::Abs(daughterParticleN->GetFirstMother()));
        Int_t labelMotherP = daughterParticleP->GetFirstMother();
        Int_t labelMotherN = daughterParticleN->GetFirstMother();
        if (((particleMotherN->GetPdgCode() == 1010010030 && particleMotherP->GetPdgCode() == 1010010030)   ||
            (particleMotherN->GetPdgCode() == -1010010030 && particleMotherP->GetPdgCode() == -1010010030)) &&
            (labelMotherN == labelMotherP)) {
          mctruth = 1;
          reducedV0->fMCTruth = mctruth;
          fMCGenRecArray->AddAtAndExpand(reducedV0, mcGenRecCounter);
          fMCGenRec[mcGenRecCounter] = labelMotherP;
          mcGenRecCounter++;
        }
      }
      if (helium3Positive && pionNegative) {
        fHistdEdxHypTriton->Fill(momentumP*trackP->GetSign(), trackP->GetTPCsignal());
        fHistdEdxHypTriton->Fill(momentumN*trackN->GetSign(), trackN->GetTPCsignal());
      }
      if (helium3Negative && pionPositive) {
        fHistdEdxHypTritonAnti->Fill(momentumP*trackP->GetSign(), trackP->GetTPCsignal());
        fHistdEdxHypTritonAnti->Fill(momentumN*trackN->GetSign(), trackN->GetTPCsignal());
      }
    }
  }
  fReducedEvent->fNumberV0s = (nV0Cand);
  fTree->Fill();
  fReducedEvent->ClearEvent();
  if (fMCtrue) {
    const AliMCVertex* mcVertex = (const AliMCVertex*) mcEvent->GetPrimaryVertex();
    fPosVx = mcVertex->GetX();
    fPosVy = mcVertex->GetY();
    fPosVz = mcVertex->GetZ();
    MCStackLoop(stack);
  }
  PostData(1, fHistogramList);
  PostData(2, fTree);
  PostData(3, fTreeMCGen);
}
//////////////////////////////////////////
// Called once at the end of the query. //
//////////////////////////////////////////
void AliAnalysisTaskHypTritEventTree::Terminate(const Option_t*) {
  if (!GetOutputData(0)) return;
}

//////////////////////////////////////////////////////////////////////////////
// Loops over MC stack to find Hypertritons and Antihypertritons and fills  //
// them in the histograms for MC generated particles.                       //
//////////////////////////////////////////////////////////////////////////////
void AliAnalysisTaskHypTritEventTree::MCStackLoop(AliStack *stack) {
  TClonesArray *v0Array = (TClonesArray*) fReducedEventMCGen->fV0s;
  Int_t nV0Gen = 0;
  for (Int_t istack = 0; istack < stack->GetNtrack(); istack++) {
    const TParticle *tparticleMother = stack->Particle(istack);
    Long_t pdgCodeMother = tparticleMother->GetPdgCode();
    if (pdgCodeMother == 1010010030 || pdgCodeMother == -1010010030) {
      TLorentzVector momentumDaughter1;
      TLorentzVector momentumDaughter2;
      Int_t labelSecondDaughter = tparticleMother->GetDaughter(1);
      Int_t labelFirstDaughter  = labelSecondDaughter - 1;
      TParticle *daughterparticle1 = stack->Particle(TMath::Abs(labelFirstDaughter));
      TParticle *daughterparticle2 = stack->Particle(TMath::Abs(labelSecondDaughter));
      if ((daughterparticle1->GetPdgCode() == 1000020030  /*Helium3*/     &&
          daughterparticle2->GetPdgCode() == -211)        /*PionMinus*/   ||
          (daughterparticle1->GetPdgCode() == -1000020030 /*AntiHelium3*/ &&
          daughterparticle2->GetPdgCode() == 211))        /*PionPlus*/    {
        momentumDaughter1.SetPxPyPzE(daughterparticle1->Px(), daughterparticle1->Py(),
          daughterparticle1->Pz(), daughterparticle1->Energy());
        momentumDaughter2.SetPxPyPzE(daughterparticle2->Px(), daughterparticle2->Py(),
          daughterparticle2->Pz(), daughterparticle2->Energy());
        AliReducedHypTritV0 *reducedV0 = (AliReducedHypTritV0*)v0Array->ConstructedAt(nV0Gen);
        AliReducedHypTritTrack *reducedPi = reducedV0->Pi();
        AliReducedHypTritTrack *reducedHe = reducedV0->He();
        Double_t posx = daughterparticle1->Vx();
        Double_t posy = daughterparticle1->Vy();
        Double_t posz = daughterparticle1->Vz();
        Double_t disx = posx - fPosVx;
        Double_t disy = posy - fPosVy;
        Double_t disz = posz - fPosVz;
        Double_t distance = TMath::Sqrt(disx*disx + disy*disy + disz*disz );
        reducedV0->fM = tparticleMother->GetCalcMass();
        reducedV0->fP = tparticleMother->P();
        reducedV0->fPt = tparticleMother->Pt();
        reducedV0->fDecayLength = distance * tparticleMother->GetCalcMass() / tparticleMother->P();
        reducedV0->fMCTruth = 0;
        for(Int_t ii = 0; ii < 40; ii++) {
          if (fMCGenRec[ii] == istack) {
              AliReducedHypTritV0 *mcReducedV0 = (AliReducedHypTritV0*) fMCGenRecArray->At(ii);
              AliReducedHypTritTrack *mcReducedPi = mcReducedV0->Pi();
              AliReducedHypTritTrack *mcReducedHe = mcReducedV0->He();
              reducedV0->fDecayLength = mcReducedV0->fDecayLength;
              reducedV0->fDCAv0 = mcReducedV0->fDCAv0;
              reducedV0->fCosPointingAngle = mcReducedV0->fCosPointingAngle;
              reducedPi->fP = mcReducedPi->fP;
              reducedHe->fP = mcReducedHe->fP;
              reducedPi->fEta = mcReducedPi->fEta;
              reducedHe->fEta = mcReducedHe->fEta;
              reducedPi->fTpcNClusters = mcReducedPi->fTpcNClusters;
              reducedHe->fTpcNClusters = mcReducedHe->fTpcNClusters;
              reducedV0->fRapidity = mcReducedV0->fRapidity;
              reducedV0->fM = mcReducedV0->fM;
              reducedV0->fMCTruth = mcReducedV0->fMCTruth;
            }
          }
        nV0Gen = nV0Gen +1;
        }
      }
    }
  fReducedEventMCGen->fNumberV0s = nV0Gen;
  fTreeMCGen->Fill();
  fReducedEventMCGen->ClearEvent();
  fMCGenRecArray->Clear();
}

/////////////////////////
// Selects the Trigger //
/////////////////////////
Bool_t AliAnalysisTaskHypTritEventTree::TriggerSelection() {
  fReducedEvent->fTrigger = 0;
  if ((fInputHandler->IsEventSelected() & AliVEvent::kMB)) fReducedEvent->fTrigger = 1;
  if ((fInputHandler->IsEventSelected() & AliVEvent::kCentral)) fReducedEvent->fTrigger = 2;
  if ((fInputHandler->IsEventSelected() & AliVEvent::kSemiCentral)) fReducedEvent->fTrigger =  3;
  Bool_t isTriggered = kTRUE;
  if (fReducedEvent->fTrigger == 0) isTriggered = kFALSE;
  return isTriggered;
}
