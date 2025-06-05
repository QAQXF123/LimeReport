#include "lrexcelhandler.h"
namespace LimeReport {

ExcelHandler::ExcelHandler(bool isSingleHeader)
    : m_isSingleHeader(isSingleHeader) {
}

void ExcelHandler::exportPageToExcel(PageItemDesignIntf::Ptr page, Document& doc, int& startRow, const QString& sheetName) {
    // qDebug() << "new handler begin 2";
    PageItemDesignIntf* pageItem = page.get();
    QList<BaseDesignIntf*> pageChildren = pageItem->childBaseItems();

    if (m_xInfos.empty()) {
        getXInfoByAllXInfo(pageChildren);
    }

    //  getXInfoByMostCol(pageChildren);

    std::sort(m_xInfos.begin(), m_xInfos.end(), [&](const XInfo& a, const XInfo& b) {
        return a.x < b.x;
    });

    for (int i = 0; i < m_xInfos.size(); i++) {
        double mmWidth = m_xInfos[i].width;
        double excelColWidth = mmWidth / m_mmPerX;
        doc.setColumnWidth(i + 1, excelColWidth);
    }

    for (BaseDesignIntf* base : pageChildren) {
        // qDebug() << "base ObjectName:" << base->objectName() << "class name:" << base->metaObject()->className();
        auto* band = dynamic_cast<BandDesignIntf*>(base);
        if (!band) {
            // qDebug() << "is not band";
            continue;
        }

        if (dynamic_cast<DataBandDesignIntf*>(band)) { // 是数据行
            m_isPrintDataBand = true;
            handleDesign(base, doc, startRow);
        } else {

            // 单表头的打印
            // 第一种情况  第一页并且没有打印过数据行  认为是表头  可以打印
            // 第二种情况   为单行并且出现 合计 小计等关键词 判定为合计行 可以打印
            if (!m_isSingleHeader || (startRow == 0 && !m_isPrintDataBand) || isTotalLine(base)) {
                handleDesign(base, doc, startRow, true);
            }
        }
    }
    // qDebug() << "new handler end 2";
}

void ExcelHandler::getAllCells(BaseDesignIntf* base, QVector<CellInfo>& cells, int parentX, int parentY) {

    QList<BaseDesignIntf*> bandChildren = base->childBaseItems();

    for (auto c : bandChildren) {
        auto ti = dynamic_cast<TextItem*>(c);
        auto cName = c->objectName();
        if (ti) {
            QRectF geo = ti->geometry();

            CellInfo cell;
            cell.ti = ti;
            cell.text = ti->content();
            cell.xInfo.x = geo.x() + parentX;
            cell.xInfo.width = geo.width();
            cell.yInfo.y = geo.y() + parentY;
            cell.yInfo.height = geo.height();
            cells.push_back(cell);
        }

        auto layout = dynamic_cast<AbstractLayout*>(c);
        if (layout) {
            auto layoutBase = static_cast<BaseDesignIntf*>(layout);
            getAllCells(layoutBase, cells, parentX + layout->geometry().x(), parentY + layout->geometry().y());
        }
    }

    std::sort(cells.begin(), cells.end(), [&](const CellInfo& a, const CellInfo& b) {
        return a.yInfo.y < b.yInfo.y || (a.yInfo.y == b.yInfo.y && a.xInfo.x < b.xInfo.x);
    });
}


// isFixed 表示固定的格式
void ExcelHandler::handleDesign(BaseDesignIntf* base, Document& doc, int& startRow, bool isFixed) {

    QVector<CellInfo> cells;
    getAllCells(base, cells);

    // 获取单元格信息

    auto cellRows = getCellRows(cells);

    // 获取每行的高度 用行里最矮的列作为行的高度
    QVector<YInfo> yInfos(cellRows.size());
    for (int i = 0; i < cellRows.size(); i++) {

        double min_height = cellRows[i][0].yInfo.height;
        double min_y = cellRows[i][0].yInfo.y;
        for (int j = 1; j < cellRows[i].size(); j++) {
            min_height = std::min(min_height, cellRows[i][j].yInfo.height);
            min_y = std::min(min_y, cellRows[i][j].yInfo.y);
        }
        yInfos[i].y = min_y;
        yInfos[i].height = min_height;
    }

    for (int i = 0; i < yInfos.size(); i++) {
        // qDebug() << "set row height" << startRow + i + 1 << "," << yInfos[i].y / m_mmPerY;
        doc.setRowHeight(startRow + i + 1, yInfos[i].height / m_mmPerY);
    }

    // 按行处理单元格
    for (int i = 0; i < cellRows.size(); i++) {
        // qDebug() << "row begin" << i << " begin";

        auto& cellRow = cellRows[i];
        for (int j = 0; j < cellRow.size(); j++) {

            auto& cell = cellRow[j];
            Format fmt;

            setFmt(cell.ti, fmt);

            // 获取单元格
            auto [firstCol, lastCol] = findColIndex(m_xInfos, cell.xInfo.x, cell.xInfo.width);
            cell.firstCol = firstCol;
            cell.lastCol = lastCol;

            auto [firstRow, lastRow] = findRowIndex(yInfos, cell.yInfo.y, cell.yInfo.height);
            cell.firstRow = firstRow;
            cell.lastRow = lastRow;

            // 转换为excel需要的行号

            cell.firstRow += startRow;
            cell.lastRow += startRow;

            // 合并
            if (cell.firstCol != cell.lastCol || cell.firstRow != cell.lastRow) {
                CellRange cellRange(cell.firstRow + 1, cell.firstCol + 1, cell.lastRow + 1, cell.lastCol + 1);
                doc.mergeCells(cellRange, fmt);
            }

            // qDebug() << "cell.xInfo.x" << cell.xInfo.x << "," << "cell.xInfo.width" << cell.xInfo.width;
            // qDebug() << "cell.yInfo.y" << cell.yInfo.y << "," << "cell.yInfo.height" << cell.yInfo.height;

            // qDebug() << "write" << cell.firstRow + 1 << "," << cell.firstCol + 1 << "," << cell.text << "begin";
            // qDebug() << cell.firstCol << "," << cell.lastCol << "," << cell.firstRow << "," << cell.lastRow;

            if (m_isSingleHeader && startRow == 0 && isFixed) {
                QString replacePageCnt;
                if (replacePageCntWhenSingleHeader(cell.text, replacePageCnt)) {
                    cell.text = replacePageCnt;
                }
            }

            doc.write(cell.firstRow + 1, cell.firstCol + 1, cell.text, fmt);
            // qDebug() << "end";
        }
        // qDebug() << "row " << i << "ok";
    }

    startRow += cellRows.size();
}

void ExcelHandler::setFmt(TextItem* ti, Format& srcFmt) {

    setFont(ti, srcFmt);
    setBorderLine(ti->borderLines(), srcFmt);
    setAlignment(ti->alignment(), srcFmt);

    auto& dstFmt = srcFmt;

    dstFmt.setFontColor(Qt::black);
    dstFmt.setTextWrap(true); // 自动换行
    dstFmt.setLeftBorderStyle(srcFmt.leftBorderStyle());
    dstFmt.setLeftBorderColor(srcFmt.leftBorderColor());
    dstFmt.setRightBorderStyle(srcFmt.rightBorderStyle());
    dstFmt.setRightBorderColor(srcFmt.rightBorderColor());
    dstFmt.setTopBorderStyle(srcFmt.topBorderStyle());
    dstFmt.setTopBorderColor(srcFmt.topBorderColor());
    dstFmt.setBottomBorderStyle(srcFmt.bottomBorderStyle());
    dstFmt.setBottomBorderColor(srcFmt.bottomBorderColor());
}

void ExcelHandler::setFont(const TextItem* ti, Format& fmt) {
    QFont fnt = ti->font();
    QColor fcol = ti->fontColor();
    QColor bgcol = ti->backgroundColor();
    fmt.setFontName(fnt.family());
    fmt.setFontSize(fnt.pointSize());
    fmt.setFontBold(fnt.bold());
    fmt.setFontItalic(fnt.italic());
    fmt.setFontColor(fcol);
    fmt.setPatternBackgroundColor(bgcol);
}

void ExcelHandler::setBorderLine(const BaseDesignIntf::BorderLines& lines, Format& fmt) {
    if (lines & BaseDesignIntf::LeftLine) {
        fmt.setLeftBorderStyle(Format::BorderThin);
    }
    if (lines & BaseDesignIntf::TopLine) {
        fmt.setTopBorderStyle(Format::BorderThin);
    }
    if (lines & BaseDesignIntf::RightLine) {
        fmt.setRightBorderStyle(Format::BorderThin);
    }
    if (lines & BaseDesignIntf::BottomLine) {
        fmt.setBottomBorderStyle(Format::BorderThin);
    }
}

void ExcelHandler::setAlignment(Qt::Alignment alignment, Format& fmt) {
    // 水平对齐
    if (alignment & Qt::AlignLeft) {
        fmt.setHorizontalAlignment(Format::AlignLeft);
    } else if (alignment & Qt::AlignHCenter) {
        fmt.setHorizontalAlignment(Format::AlignHCenter);
    } else if (alignment & Qt::AlignRight) {
        fmt.setHorizontalAlignment(Format::AlignRight);
    } else if (alignment & Qt::AlignJustify) {
        fmt.setHorizontalAlignment(Format::AlignHJustify);
    }

    // 垂直对齐
    if (alignment & Qt::AlignTop) {
        fmt.setVerticalAlignment(Format::AlignTop);
    } else if (alignment & Qt::AlignVCenter) {
        fmt.setVerticalAlignment(Format::AlignVCenter);
    } else if (alignment & Qt::AlignBottom) {
        fmt.setVerticalAlignment(Format::AlignBottom);
    }
}

// 处理合并后的边框
bool ExcelHandler::handleMergeBorder(Document& dst, const QXlsx::CellRange& dstMergedRange, const Format& srcFmt) {

    // 3. 为合并区域内的其他单元格设置边框
    for (int r = dstMergedRange.firstRow(); r <= dstMergedRange.lastRow(); ++r) {
        for (int c = dstMergedRange.firstColumn(); c <= dstMergedRange.lastColumn(); ++c) {
            // 跳过已处理的左上角单元格
            if (r == dstMergedRange.firstRow() && c == dstMergedRange.firstColumn())
                continue;

            Format borderFmt;

            // 设置左边框（最左侧单元格或原格式有左边框）
            if (c == dstMergedRange.firstColumn() || srcFmt.leftBorderStyle() != Format::BorderNone) {
                borderFmt.setLeftBorderStyle(srcFmt.leftBorderStyle());
                borderFmt.setLeftBorderColor(srcFmt.leftBorderColor());
            }

            // 设置右边框（最右侧单元格或原格式有右边框）
            if (c == dstMergedRange.lastColumn() || srcFmt.rightBorderStyle() != Format::BorderNone) {
                borderFmt.setRightBorderStyle(srcFmt.rightBorderStyle());
                borderFmt.setRightBorderColor(srcFmt.rightBorderColor());
            }

            // 设置上边框（最上方单元格或原格式有上边框）
            if (r == dstMergedRange.firstRow() || srcFmt.topBorderStyle() != Format::BorderNone) {
                borderFmt.setTopBorderStyle(srcFmt.topBorderStyle());
                borderFmt.setTopBorderColor(srcFmt.topBorderColor());
            }

            // 设置下边框（最下方单元格或原格式有下边框）
            if (r == dstMergedRange.lastRow() || srcFmt.bottomBorderStyle() != Format::BorderNone) {
                borderFmt.setBottomBorderStyle(srcFmt.bottomBorderStyle());
                borderFmt.setBottomBorderColor(srcFmt.bottomBorderColor());
            }

            if (!dst.write(r, c, QVariant(), borderFmt)) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
std::pair<int, int> ExcelHandler::findIndexGeneric(
    const QVector<T>& infos,
    double begin,
    double offset,
    std::function<double(const T&)> getStart,
    std::function<double(const T&)> getLength) {
    int n = infos.size();
    double end = begin + offset;

    int firstIdx = 0;
    int lastIdx = 0;

    for (int i = 0; i < n; ++i) {
        double start = getStart(infos[i]);
        double length = getLength(infos[i]);
        double mid = start + length / 2;
        double finish = start + length;

        if (begin >= start && begin <= finish) {
            firstIdx = i;
            if (begin > mid) {
                firstIdx++;
            }
        }

        if (end >= start) {
            // if (end >= start && end <= finish) {
            lastIdx = i;
            if (end < mid) {
                lastIdx--;
            }
        }
    }

    if (firstIdx < 0)
        firstIdx = 0;
    if (lastIdx >= n)
        lastIdx = n - 1;
    if (lastIdx < firstIdx)
        lastIdx = firstIdx;

    return { firstIdx, lastIdx };
}

std::pair<int, int> ExcelHandler::findColIndex(const QVector<XInfo>& infos, double begin, double offset) {
    return findIndexGeneric<XInfo>(infos, begin, offset, [](const XInfo& info) { return info.x; }, [](const XInfo& info) { return info.width; });
}

std::pair<int, int> ExcelHandler::findRowIndex(const QVector<YInfo>& infos, double begin, double offset) {
    return findIndexGeneric<YInfo>(infos, begin, offset, [](const YInfo& info) { return info.y; }, [](const YInfo& info) { return info.height; });
}

// 通过每一个单独的x获取行信息
void ExcelHandler::getXInfoByAllXInfo(const QList<BaseDesignIntf*>& pageChildren) {

    QVector<XInfo> allXInfos;

    // 遍历每一个单元格
    for (BaseDesignIntf* base : pageChildren) {

        QVector<CellInfo> cells;
        getAllCells(base, cells);
        for (auto& cell : cells) {
            allXInfos.push_back(cell.xInfo);
        }
    }
    std::sort(allXInfos.begin(), allXInfos.end(), [&](const XInfo& a, const XInfo& b) {
        return a.x < b.x || (a.x == b.x && a.width < b.width);
    });

    QVector<XInfo> xInfos { allXInfos[0] };
    for (int i = 1; i < allXInfos.size(); i++) {
        if (std::abs(xInfos.back().x - allXInfos[i].x) > 30) {
            xInfos.push_back(allXInfos[i]);
        }
    }
    for (int i = 0; i < xInfos.size() - 1; i++) {
        xInfos[i].width = xInfos[i + 1].x - xInfos[i].x;
    }
    m_xInfos = xInfos;
    // qDebug() << "get info size " << m_xInfos.size();
}

// 通过最多列行获取行信息
void ExcelHandler::getXInfoByMostCol(const QList<BaseDesignIntf*>& pageChildren) {
    QVector<XInfo> mostXInfos;

    // 遍历每一个单元格
    for (BaseDesignIntf* base : pageChildren) {

        QVector<XInfo> tempXInfos;
        QVector<CellInfo> cells;
        getAllCells(base, cells);

        // 获取单元格信息

        std::sort(cells.begin(), cells.end(), [&](const CellInfo& a, const CellInfo& b) {
            return a.yInfo.y < b.yInfo.y || (a.yInfo.y == b.yInfo.y && a.xInfo.x < b.xInfo.x);
        });

        QVector<CellInfo> cellRow;
        QVector<QVector<CellInfo>> cellRows;

        // 把单元格按行划分
        for (const auto& cell : cells) {
            if (cellRow.empty()) {
                cellRow.push_back(cell);
            } else {
                // 判断在不在同一行 误差处理
                if (std::abs(cellRow.back().yInfo.y - cell.yInfo.y) > m_precision) {
                    cellRows.push_back(cellRow);
                    cellRow.clear();
                    cellRow.push_back(cell);
                } else {
                    cellRow.push_back(cell);
                }
            }
        }
        if (!cellRow.empty()) {
            cellRows.push_back(cellRow);
        }

        // 找到最多列的行
        int maxColIndex = -1;
        int maxColCnt = 0;
        for (int i = 0; i < cellRows.size(); i++) {
            if (cellRows[i].size() > maxColCnt) {
                maxColCnt = cellRows[i].size();
                maxColIndex = i;
            }
        }

        cellRow = cellRows[maxColIndex];
        for (int i = 0; i < cellRow.size(); i++) {
            tempXInfos.push_back(XInfo { cellRow[i].xInfo.x, cellRow[i].xInfo.width });
        }

        if (tempXInfos.size() > mostXInfos.size()) {
            mostXInfos = tempXInfos;
        }
    }
    m_xInfos = mostXInfos;
}

// 通过数据行获取行信息
void ExcelHandler::getXInfoByDataBand(const QList<BaseDesignIntf*>& pageChildren) {

    for (BaseDesignIntf* base : pageChildren) {
        auto band = dynamic_cast<DataBandDesignIntf*>(base);
        if (band == nullptr)
            continue;
        // 用数据行来决定列数 先这样处理
        QList<BaseDesignIntf*> bandChildren = band->childBaseItems();
        for (BaseDesignIntf* bci : bandChildren) {
            auto* ti = dynamic_cast<TextItem*>(bci);
            if (ti == nullptr)
                continue;
            QRectF geo = ti->boundingRect().translated(ti->pos());
            m_xInfos.push_back(XInfo { geo.x(), geo.width() });
        }
        break;
    }
}

bool ExcelHandler::replacePageCntWhenSingleHeader(const QString& in, QString& out) {
    QString matchRule = "第页共页";
    if (in.isEmpty() || in[0] != matchRule[0]) {
        return false;
    }
    int j = 0;

    for (int i = 0; i < in.size(); i++) {
        if (in[i] == matchRule[j]) {
            j++;
        }
    }

    if (j == matchRule.size()) {
        out = "第  1  页  共 1 页";
        return true;
    }

    return false;
}


// 判断是否是合计行
bool ExcelHandler::isTotalLine(BaseDesignIntf* base) {
    QVector<CellInfo> cells;
    getAllCells(base, cells);

    // 获取单元格信息

    auto cellRows = getCellRows(cells);

    if (cellRows.size() != 1) {
        return false;
    }
    auto& cellRow = cellRows[0];

    static QSet<QString> totalKeyWords { "合计", /*"小计"*/ };


    for (const auto& cell : cellRow) {
        for (const auto& word : totalKeyWords) {
            if (cell.text.contains(word)) {
                return true;
            }
        }        
    }
    

    return false;
}



QVector<QVector<CellInfo>> ExcelHandler::getCellRows(const QVector<CellInfo>& cells) {
        QVector<CellInfo> cellRow;
        QVector<QVector<CellInfo>> cellRows;

        // 把单元格按行划分
        for (const auto& cell : cells) {
            if (cellRow.empty()) {
                cellRow.push_back(cell);
            } else {
                // 判断在不在同一行 误差处理
                if (std::abs(cellRow.back().yInfo.y - cell.yInfo.y) > m_precision) {
                    cellRows.push_back(cellRow);
                    cellRow.clear();
                    cellRow.push_back(cell);
                } else {
                    cellRow.push_back(cell);
                }
            }
        }
        if (!cellRow.empty()) {
            cellRows.push_back(cellRow);
        }
        return cellRows;
    }

} // namespace
