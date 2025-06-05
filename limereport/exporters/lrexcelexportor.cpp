#include "lrexcelexportor.h"
#include "lrexportersfactory.h"
#include "lrreportengine_p.h"

namespace {

LimeReport::ReportExporterInterface* createExcelExporter(LimeReport::ReportEnginePrivate* parent) {
    return new LimeReport::ExcelExportor(parent);
}

bool VARIABLE_IS_NOT_USED registred = LimeReport::ExportersFactory::instance().registerCreator(
    "Excel",
    LimeReport::ExporterAttribs(QObject::tr("Export to excel"), "ExcelExportor"),
    createExcelExporter);

} // namespace

namespace LimeReport {

ExcelExportor::ExcelExportor(ReportEnginePrivate* parent)
    : QObject(parent)
    , m_reportEngine(parent) { }

bool ExcelExportor::exportPages(ReportPages pages,
    const QString& fileName,
    const QMap<QString, QVariant>& params) {
    Q_UNUSED(params);
    if (!fileName.isEmpty()) {

        bool isSingleHeader = params["isSingleHeader"].toBool();

        QString excelSheetName = params["excelSheetName"].toString();

        // 默认就用文件
        if (excelSheetName.isEmpty()) {
            excelSheetName = QFileInfo(fileName).baseName();
        }

        if (!pages.isEmpty()) {
            m_reportEngine->printPagesExcel(pages, fileName, excelSheetName, isSingleHeader);
        }
        return true;
    }
    return false;
}

} // namespace LimeReport
