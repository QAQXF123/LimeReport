#include "lrpagecontentfooter.h"
#include "lrdesignelementsfactory.h"
#include "lrglobal.h"
#include "lrpagedesignintf.h"

const QString xmlTag = "PageContentFooter"; //页内容尾：紧跟着内容显示，在ReportFooter及PageFooter前

namespace {
LimeReport::BaseDesignIntf *createBand(QObject *owner, LimeReport::BaseDesignIntf *parent)
{
    return new LimeReport::PageContentFooter(owner, parent);
}

bool VARIABLE_IS_NOT_USED registred = LimeReport::DesignElementsFactory::instance().registerCreator(
    xmlTag,
    LimeReport::ItemAttribs(QObject::tr("PageContent Footer"), LimeReport::Const::bandTAG),
    createBand);
} // namespace

namespace LimeReport {

PageContentFooter::PageContentFooter(QObject *owner, QGraphicsItem *parent)
    : BandDesignIntf(LimeReport::BandDesignIntf::PageContentFooter, xmlTag, owner, parent)
{
    setBandTypeText(tr("PageContent Footer"));
    setMarkerColor(bandColor());
    setAutoHeight(false);
}

BaseDesignIntf *PageContentFooter::createSameTypeItem(QObject *owner, QGraphicsItem *parent)
{
    return new PageContentFooter(owner, parent);
}

QColor PageContentFooter::bandColor() const
{
    return QColor(255, 204, 0);
}

} // namespace LimeReport
