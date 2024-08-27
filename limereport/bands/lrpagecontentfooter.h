#ifndef LRPAGECONTENTFOOTER_H
#define LRPAGECONTENTFOOTER_H

#include "lrbanddesignintf.h"
#include "lrdesignelementsfactory.h"

namespace LimeReport {

class PageContentFooter : public BandDesignIntf
{
    Q_OBJECT
public:
    PageContentFooter(QObject *owner = 0, QGraphicsItem *parent = 0);
    virtual BaseDesignIntf *createSameTypeItem(QObject *owner = 0, QGraphicsItem *parent = 0);
    virtual bool isFooter() const { return true; }

protected:
    QColor bandColor() const;
};
} // namespace LimeReport

#endif // LRPAGECONTENTFOOTER_H
