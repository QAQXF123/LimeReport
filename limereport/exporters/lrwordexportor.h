#ifndef LRWORDEXPORTOR_H
#define LRWORDEXPORTOR_H

#include <QObject>
#include "lrexporterintf.h"

// add by hwf
namespace LimeReport {
class ReportEnginePrivate;

class WordExportor : public QObject, public ReportExporterInterface
{
    Q_OBJECT
public:
    explicit WordExportor(ReportEnginePrivate *parent = NULL);
    // ReportExporterInterface interface
    virtual bool exportPages(ReportPages pages,
                             const QString &fileName,
                             const QMap<QString, QVariant> &params) override;
    virtual QString exporterName() override { return "Word"; }
    virtual QString exporterFileExt() override { return "docx"; }
    virtual QString hint() override { return tr("Export to word"); }

private:
    ReportEnginePrivate *m_reportEngine;
};

} // namespace LimeReport

#endif // LRWORDEXPORTOR_H
