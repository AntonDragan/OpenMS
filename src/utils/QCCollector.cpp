// $Maintainer: Dragan Haberland, Leo Wurthillini
// $Authors: Dragan Haberland, Leo Wurthillini

#include <OpenMS/APPLICATIONS/TOPPBase.h>
#include <OpenMS/FILTERING/ID/IDFilter.h>
#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/FORMAT/IdXMLFile.h>
#include <OpenMS/FORMAT/FileTypes.h>
#include <OpenMS/FORMAT/FileHandler.h>
#include <OpenMS/FORMAT/FeatureXMLFile.h>
#include <OpenMS/FORMAT/ConsensusXMLFile.h>
#include <OpenMS/DATASTRUCTURES/StringListUtils.h>
//#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/FORMAT/CsvFile.h>
#include <OpenMS/CONCEPT/QCMetrics.h>
//#include <OpenMS/METADATA/DataProcessing.h>
//#include <OpenMS/FORMAT/XMLFile.h>
#include <boost/regex.hpp>

using namespace OpenMS;
using namespace std;


class OPENMS_DLLAPI QCCollector:
  public TOPPBase
{
public:
  QCCollector():
	TOPPBase("QCCollector","Will collect several mzTabs from several utils.",false)
	{
	}

protected:
	void registerOptionsAndFlags_()
	{
	  registerInputFileList_("in_ProteinQuantifier","<files>", StringList(), "Input files",false,false);
	  registerInputFileList_("in_IDMapper","<files>", StringList(), "Input files",false,false);
    registerInputFileList_("in_rawfiles_FalseDiscoveryRate","<files>", StringList(), "Input files",false,false);
	  registerInputFileList_("in_Post_FalseDiscoveryRate","<files>", StringList(), "Input files",false,false);
	  registerInputFileList_("in_FeatureLinkerUnlabeledQT","<files>", StringList(), "Input files",false,false);
	  setValidFormats_("in_ProteinQuantifier", ListUtils::create<String>("csv"));
	  setValidFormats_("in_IDMapper", ListUtils::create<String>("FeatureXML"));
    setValidFormats_("in_rawfiles_FalseDiscoveryRate", ListUtils::create<String>("MzML"));
	  setValidFormats_("in_Post_FalseDiscoveryRate", ListUtils::create<String>("IdXML"));
	  setValidFormats_("in_FeatureLinkerUnlabeledQT", ListUtils::create<String>("consensusXML"));
	  registerOutputFile_("out", "<file>", "", "Output file (mzTab)", true);
    setValidFormats_("out", ListUtils::create<String>("tsv"));
  }

  ExitCodes main_(int, const char**)
  {
    StringList ins_ProteinQuantifier = getStringList_("in_ProteinQuantifier");
    StringList ins_IDMapper = getStringList_("in_IDMapper");
    StringList ins_rawfiles_FalseDiscoveryRate = getStringList_("in_rawfiles_FalseDiscoveryRate");
    StringList ins_Post_FalseDiscoveryRate = getStringList_("in_Post_FalseDiscoveryRate");
    StringList ins_FeatureLinkerUnlabeledQT = getStringList_("in_FeatureLinkerUnlabeledQT");
    String out = getStringOption_("out");
    vector<pair<String,FeatureMap>> fvec;
    vector<pair<String,CsvFile>> cvec;
    vector <pair<String,ConsensusMap>> CMapVec;
    vector<pair<String,pair<String,String>>> ivec;
    if (ins_ProteinQuantifier.size()!=0)
		{
		  for(StringList::const_iterator it=ins_ProteinQuantifier.begin();it!=ins_ProteinQuantifier.end();++it)
			{
			  CsvFile fl(*it,'	',false,-1);
				cvec.push_back(make_pair("ProteinQuantifier",fl));
			}
    }
		else if (ins_IDMapper.size()!=0)
		{
      vector<String> frawfiles;
		  for(StringList::const_iterator it=ins_IDMapper.begin();it!=ins_IDMapper.end();++it)
			{
			  FeatureMap features;
			  FeatureXMLFile().load(*it, features);
        frawfiles.push_back(features.getMetaValue("spectra_data"));
        fvec.push_back(make_pair("IDMapper",features));
	    }
    }
    else if (ins_Post_FalseDiscoveryRate.size()!=0)
		{
      if(ins_rawfiles_FalseDiscoveryRate.size()!=ins_Post_FalseDiscoveryRate.size())
      {
        throw Exception::MissingInformation(__FILE__,__LINE__,OPENMS_PRETTY_FUNCTION,"invalid number of input rawfiles (rawfiles_FalseDiscoveryRate)");
      }
		  for(Size i=0;i<ins_Post_FalseDiscoveryRate.size();++i)
			{
			  ivec.push_back(make_pair("Post_FalseDiscoveryRate",make_pair(ins_rawfiles_FalseDiscoveryRate[i],ins_Post_FalseDiscoveryRate[i])));
		  }
	  }
    else if(ins_FeatureLinkerUnlabeledQT.size()!=0)
		{
      vector<String> crawfiles;
		  for(StringList::const_iterator it=ins_FeatureLinkerUnlabeledQT.begin();it!=ins_FeatureLinkerUnlabeledQT.end();++it)
			{
			  ConsensusMap CMap;
			  ConsensusXMLFile().load(*it,CMap);
        crawfiles.push_back(CMap.getMetaValue("spectra_data"));
			  CMapVec.push_back(make_pair("FeatureLinkerUnlabeledQT",CMap));
		  }
	  }
		Metrics metricObj(fvec,ivec,cvec,CMapVec,out);
	    metricObj.runAllMetrics();
  return EXECUTION_OK;
  }
};
int main(int argc, const char** argv)
{
  QCCollector tool;
  return tool.main(argc,argv);
}
