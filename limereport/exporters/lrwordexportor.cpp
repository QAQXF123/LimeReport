#include "lrwordexportor.h"
#include "lrexportersfactory.h"
#include "lrreportengine_p.h"

namespace {

LimeReport::ReportExporterInterface *createWordExporter(LimeReport::ReportEnginePrivate *parent)
{
    return new LimeReport::WordExportor(parent);
}

bool VARIABLE_IS_NOT_USED registred = LimeReport::ExportersFactory::instance().registerCreator(
    "Excel",
    LimeReport::ExporterAttribs(QObject::tr("Export to word"), "WordExportor"),
    createWordExporter);

} // namespace

namespace LimeReport {

WordExportor::WordExportor(ReportEnginePrivate *parent)
    : QObject(parent)
    , m_reportEngine(parent)
{}

bool WordExportor::exportPages(ReportPages pages,
                               const QString &fileName,
                               const QMap<QString, QVariant> &params)
{
    Q_UNUSED(params);
    if (!fileName.isEmpty()) {
        //
        return true;
    }
    return false;
}

} // namespace LimeReport
