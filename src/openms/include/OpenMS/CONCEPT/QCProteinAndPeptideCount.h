#ifndef OPENMS_CONCEPT_QCProteinAndPeptideCount_H
#define OPENMS_CONCEPT_QCProteinAndPeptideCount_H
#include <OpenMS/FORMAT/CsvFile.h>
#include <OpenMS/FORMAT/MzTabFile.h>
#include <OpenMS/FORMAT/MzTab.h>
#include <vector>

class OPENMS_DLLAPI QCProteinAndPeptideCount
{
std::vector<std::pair<OpenMS::String,OpenMS::CsvFile>> CFile;
  public:
    QCProteinAndPeptideCount(std::vector<std::pair<OpenMS::String,OpenMS::CsvFile>> files):
      CFile(files)
      {
      }
    ~QCProteinAndPeptideCount();
    int ProtAndPepCount_( OpenMS::MzTabFile&, OpenMS::String );//MetricMap&,MetricMap&) const;
};
#endif
