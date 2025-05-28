#include "lrexcelexportor.h"
#include "lrexportersfactory.h"
#include "lrreportengine_p.h"

namespace {

LimeReport::ReportExporterInterface *createExcelExporter(LimeReport::ReportEnginePrivate *parent)
{
    return new LimeReport::ExcelExportor(parent);
}

bool VARIABLE_IS_NOT_USED registred = LimeReport::ExportersFactory::instance().registerCreator(
    "Excel",
    LimeReport::ExporterAttribs(QObject::tr("Export to excel"), "ExcelExportor"),
    createExcelExporter);

} // namespace

namespace LimeReport {

ExcelExportor::ExcelExportor(ReportEnginePrivate *parent)
    : QObject(parent)
    , m_reportEngine(parent)
{}

bool ExcelExportor::exportPages(ReportPages pages,
                                const QString &fileName,
                                const QMap<QString, QVariant> &params)
{
    Q_UNUSED(params);
    if (!fileName.isEmpty()){
    
        bool isSingleHeader = params["isSingleHeader"].toBool();
        if (!pages.isEmpty()){
            m_reportEngine->printPagesExcel(pages, fileName, isSingleHeader);
        }
        // m_reportEngine->emitPrintedToPDF(fileName);
        return true;
    }
    return false;
}

} // namespace LimeReport
