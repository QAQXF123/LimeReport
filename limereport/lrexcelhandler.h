#ifndef LREXCELHANDLER_H
#define LREXCELHANDLER_H

#include "lrabstractlayout.h"
#include "lrbanddesignintf.h"
#include "lrpagedesignintf.h"
#include "lrreportengine.h"
#include "lrtextitem.h"
#include "xlsxdocument.h"

namespace LimeReport {

using namespace QXlsx;
//ExcelHandler begin
class ExcelHandler {

public:

    ExcelHandler(bool isSingleHeader = false);

    struct XInfo {
        double x;
        double width;
    };

    struct YInfo {
        double y;
        double height;
    };

    // 单元格信息
    struct CellInfo {
        CellInfo() {
            ti = nullptr;
            firstCol = -1;
            lastCol = -1;
            firstRow = -1;
            lastRow = -1;
        }

        int firstCol;
        int lastCol;
        int firstRow;
        int lastRow;

        YInfo yInfo;
        XInfo xInfo;
        QString text;
        TextItem* ti;
    };

    void exportPageToExcel(PageItemDesignIntf::Ptr page, Document& doc, int& startRow, const QString& sheetName = "Sheet1");

private:

    const bool m_isSingleHeader;
    bool m_isPrintDataBand{false};
    const double m_mmPerY = 4; // y轴换算比例
    const double m_mmPerX = 18; // x轴换算比例
    const double m_precision = 8.0; // 误差范围

    QVector<XInfo> m_xInfos;



    // 获取所有单元格相信
    void getAllCells(BaseDesignIntf* base, QVector<CellInfo>& cells, int parentX = 0, int parentY = 0);

    // 解析单元格坐标 根据坐标选择单页格位置
    QVector<QVector<CellInfo>> getCellRows(const QVector<CellInfo>& cells);


    // 判断是否为合计栏  
    bool isTotalLine(BaseDesignIntf* base);

    // isFixed 表示固定的格式较复杂需要手动处理
    void handleDesign(BaseDesignIntf* base, Document& doc, int& startRow, bool isFixed = false);

    // 格式处理
    void setFmt(TextItem* ti, Format& srcFmt);
    void setAlignment(Qt::Alignment alignment, Format& fmt);
    void setBorderLine(const BaseDesignIntf::BorderLines& lines, Format& fmt);
    void setFont(const TextItem* ti, Format& fmt);
    bool handleMergeBorder(Document& dst, const QXlsx::CellRange& dstMergedRange, const Format& srcFmt); // 处理合并后的边框

    // 单元格合并处理
    template <typename T>
    std::pair<int, int> findIndexGeneric(
        const QVector<T>& infos,
        double begin,
        double offset,
        std::function<double(const T&)> getStart,
        std::function<double(const T&)> getLength);

    std::pair<int, int> findColIndex(const QVector<XInfo>& infos, double begin, double offset);
    std::pair<int, int> findRowIndex(const QVector<YInfo>& infos, double begin, double offset);


    // 行划分处理函数
    // 通过每一个单独的x获取行信息
    void getXInfoByAllXInfo(const QList<BaseDesignIntf*>& pageChildren);
    // 通过最多列行获取行信息
    void getXInfoByMostCol(const QList<BaseDesignIntf*>& pageChildren);
    // 通过数据行获取行信息
    void getXInfoByDataBand(const QList<BaseDesignIntf*>& pageChildren);


    //  单表头处理
    bool replacePageCntWhenSingleHeader(const QString& in, QString& out);
};
//ExcelHandler end

} // namespace
#endif // LREXCELHANDLER_H
