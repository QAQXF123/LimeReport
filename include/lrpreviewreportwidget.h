#ifndef LRPREVIEWREPORTWIDGET_H
#define LRPREVIEWREPORTWIDGET_H

#include "lrglobal.h"
#include "lrpreparedpagesintf.h"
#include <QApplication>
#include <QPrinter>
#include <QTimer>
#include <QWidget>

namespace LimeReport {

namespace Ui {
    class PreviewReportWidget;
}

class PreviewReportWidgetPrivate;
class ReportEnginePrivate;
class ReportEngine;
class PageDesignIntf;

class LIMEREPORT_EXPORT PreviewReportWidget : public QWidget {
    Q_OBJECT
    friend class ReportEnginePrivate;
    friend class PreviewReportWindow;
    friend class PreviewReportWidgetPrivate;

public:
    explicit PreviewReportWidget(ReportEngine* report, QWidget* parent = 0);
    ~PreviewReportWidget();
    QList<QString> aviableExporters();
    bool exportReport(QString exporterName);
    bool exportReport(QString exporterName, QMap<QString, QVariant>& params);
    ScaleType scaleType() const;
    int scalePercent() const;
    void setScaleType(const ScaleType& scaleType, int percent = 0);
    void setPreviewPageBackgroundColor(QColor color);
    QColor previewPageBackgroundColor();
    QPrinter* defaultPrinter() const;
    void setDefaultPrinter(QPrinter* defaultPrinter);
    void startInsertTextItem();
    void activateItemSelectionMode();
    void deleteSelectedItems();
    void activateCurrentPage();
    void resize(ScaleType scaleType, int percent = 0);

public slots:
    void refreshPages();
    void zoomIn();
    void zoomOut();

    void firstPage();
    void priorPage();
    void nextPage();
    void lastPage();

    void print();
    QString printToPDF(const QString& fileName = "");
    QString printToExcel(const QString& fileName = "", bool isSingle = false);
    void pageNavigatorChanged(int value);
    void loadFromFile(QString& fileName); // add by hwf
    void saveToFile();
    void setScalePercent(int percent);
    void fitWidth();
    void fitPage();

protected:
    void resizeEvent(QResizeEvent*);
signals:
    void pageChanged(int page);
    void scalePercentChanged(int percent);
    void pagesSet(int pageCount);
    void itemInserted(LimeReport::PageDesignIntf* report, QPointF pos, const QString& ItemType);
    void onSave(bool& saved, LimeReport::IPreparedPages* pages);
private slots:
    void slotSliderMoved(int value);
    void reportEngineDestroyed(QObject* object);
    void slotZoomed(double);
    void resizeDone();

private:
    void initPreview();
    void setErrorsMesagesVisible(bool visible);
    void setErrorMessages(const QStringList& value);
    void emitPageSet();

private:
    Ui::PreviewReportWidget* ui;
    PreviewReportWidgetPrivate* d_ptr;
    ScaleType m_scaleType;
    int m_scalePercent;
    QTimer m_resizeTimer;
    QColor m_previewPageBackgroundColor;
    QPrinter* m_defaultPrinter;
    void printPages(QPrinter* printer);
    bool m_scaleChanging;
    QString m_preExportDir { QApplication::applicationDirPath() };
};

} // namespace LimeReport
#endif // LRPREVIEWREPORTWIDGET_H
