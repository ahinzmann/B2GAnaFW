#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"


#include<vector>
#include <TMath.h>


using namespace reco;
using namespace edm;
using namespace std;

typedef std::vector<pat::Jet> PatJetCollection;

class PileupJetIdUserData : public edm::EDProducer {
public:
  PileupJetIdUserData( const edm::ParameterSet & );   
  
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::EDGetTokenT<std::vector<pat::Jet> >     jetToken_;
  
  InputTag jLabel_, pileupJetIdToken_; 
  
};

PileupJetIdUserData::PileupJetIdUserData(const edm::ParameterSet& iConfig) :
  jLabel_             (iConfig.getParameter<edm::InputTag>("jetLabel")),
  pileupJetIdToken_             (iConfig.getParameter<edm::InputTag>("pileupJetId"))
{
   produces<vector<pat::Jet> >();
}
  
  
void PileupJetIdUserData::produce( edm::Event& iEvent, const edm::EventSetup& iSetup) {


  edm::Handle<std::vector<pat::Jet> > jetHandle;
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jLabel_, jetHandle);
  iEvent.getByLabel(jLabel_, jets);
  auto_ptr<std::vector<pat::Jet> > jetColl( new std::vector<pat::Jet> (*jetHandle) );

  edm::Handle<edm::ValueMap<StoredPileupJetIdentifier>> pileupJetIdHandle; 
  iEvent.getByLabel(pileupJetIdToken_, pileupJetIdHandle);

  for (size_t i = 0; i< jetColl->size(); i++){
    pat::Jet & jet = (*jetColl)[i];
    edm::RefToBase<pat::Jet> jetRef(edm::Ref<std::vector<pat::Jet> >(jetHandle,i));
    if(pileupJetIdHandle.isValid()) {
      jet.addUserFloat("pileupJetIdRMS", (*pileupJetIdHandle)[jetRef].RMS());
      jet.addUserFloat("pileupJetIdbeta", (*pileupJetIdHandle)[jetRef].beta());
      jet.addUserFloat("pileupJetIdbetaClassic", (*pileupJetIdHandle)[jetRef].betaClassic());
      jet.addUserFloat("pileupJetIdbetaStar", (*pileupJetIdHandle)[jetRef].betaStar());
      jet.addUserFloat("pileupJetIdbetaStarClassic", (*pileupJetIdHandle)[jetRef].betaStarClassic());
      jet.addUserFloat("pileupJetIddR2Mean", (*pileupJetIdHandle)[jetRef].dR2Mean());
      jet.addUserFloat("pileupJetIddRMean", (*pileupJetIdHandle)[jetRef].dRMean());
      jet.addUserFloat("pileupJetIddZ", (*pileupJetIdHandle)[jetRef].dZ());
      jet.addUserFloat("pileupJetIdfrac01", (*pileupJetIdHandle)[jetRef].frac01());
      jet.addUserFloat("pileupJetIdfrac02", (*pileupJetIdHandle)[jetRef].frac02());
      jet.addUserFloat("pileupJetIdfrac03", (*pileupJetIdHandle)[jetRef].frac03());
      jet.addUserFloat("pileupJetIdfrac04", (*pileupJetIdHandle)[jetRef].frac04());
      jet.addUserFloat("pileupJetIdjetR", (*pileupJetIdHandle)[jetRef].jetR());
      jet.addUserFloat("pileupJetIdjetRchg", (*pileupJetIdHandle)[jetRef].jetRchg());
      jet.addUserFloat("pileupJetIdmajW", (*pileupJetIdHandle)[jetRef].majW());
      jet.addUserFloat("pileupJetIdminW", (*pileupJetIdHandle)[jetRef].minW());
      jet.addUserFloat("pileupJetIdnCharged", (*pileupJetIdHandle)[jetRef].nCharged());
      jet.addUserFloat("pileupJetIdnNeutrals", (*pileupJetIdHandle)[jetRef].nNeutrals());
      jet.addUserFloat("pileupJetIdnParticles", (*pileupJetIdHandle)[jetRef].nParticles());
      jet.addUserFloat("pileupJetIdptD", (*pileupJetIdHandle)[jetRef].ptD());
      jet.addUserFloat("pileupJetIdpull", (*pileupJetIdHandle)[jetRef].pull());
    }
  }


  iEvent.put( jetColl );

}



#include "FWCore/Framework/interface/MakerMacros.h"


DEFINE_FWK_MODULE(PileupJetIdUserData);
