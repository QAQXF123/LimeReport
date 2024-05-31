#ifndef LREXCELEXPORTOR_H
#define LREXCELEXPORTOR_H

#include <QObject>
#include "lrexporterintf.h"

// add by hwf
namespace LimeReport {
class ReportEnginePrivate;

class ExcelExportor : public QObject, public ReportExporterInterface
{
    Q_OBJECT
public:
    explicit ExcelExportor(ReportEnginePrivate *parent = NULL);
    // ReportExporterInterface interface
    virtual bool exportPages(ReportPages pages,
                             const QString &fileName,
                             const QMap<QString, QVariant> &params) override;
    virtual QString exporterName() override { return "Excel"; }
    virtual QString exporterFileExt() override { return "xlsx"; }
    virtual QString hint() override { return tr("Export to excel"); }

private:
    ReportEnginePrivate *m_reportEngine;
};

} //namespace LimeReport

#endif // LREXCELEXPORTOR_H
