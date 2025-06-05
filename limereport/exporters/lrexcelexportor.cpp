#include "lrexcelexportor.h"
#include "lrexportersfactory.h"
#include "lrreportengine_p.h"
#include <QMessageBox>

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
        QString outFileName = fileName;

        QFileInfo fileInfo(outFileName);
        if (fileInfo.suffix().isEmpty()) {
            outFileName += ".xlsx";
        }
    

        bool isSingleHeader = params["isSingleHeader"].toBool();
        bool isSingle =  params["isSingle"].toBool();

        QString excelSheetName = params["excelSheetName"].toString();

        // 默认就用文件
        if (excelSheetName.isEmpty()) {
            excelSheetName = QFileInfo(outFileName).baseName();
        }

        qDebug() << "isSingle, " << isSingle  << ", outFileName" <<  outFileName;
 
        if (isSingle && QFile::exists(outFileName)) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, 
                                        tr("文件已经存在"),
                                        tr("文件已经存在，是否覆盖？"),
                                        QMessageBox::Yes | QMessageBox::No);
            
            if (reply == QMessageBox::No) {
                return false; // User chose not to overwrite
            }

            if (!QFile::remove(outFileName)) {
                QMessageBox::warning(nullptr, 
                                   tr("错误"), 
                                   tr("移除文件失败 请跟换目录导出"));
                return false;
            }

        }

        if (!pages.isEmpty()) {
            m_reportEngine->printPagesExcel(pages, outFileName, excelSheetName, isSingleHeader);
        }
        return true;
    }
    return false;
}

} // namespace LimeReport
