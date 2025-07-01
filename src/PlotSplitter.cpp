#include "PlotSplitter.h"

PlotSplitter::PlotSplitter(const QVector<double>& x1, const QVector<double>& y1,
    const QVector<double>& x2, const QVector<double>& y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {}

void PlotSplitter::addIntersectionIfExists(
    double x1a, double y1a, double x1b, double y1b,
    double x2c, double y2c, double x2d, double y2d) {
    // segment1: A(x1a, y1a), B(x1b, y1b)
    // segment2: C(x2c, y2c), D(x2d, y2d)
    // check if their infinite lines intersect
    double denom = (y2d - y2c)*(x1b - x1a) - (x2d - x2c)*(y1b - y1a);
    if (denom == 0) return;  // AB || CD

    // parameters of intersection point for AB and CD
    // both must be in [0, 1] if segments intersect
    double parAB = ((x2d - x2c)*(y1a - y2c) - (y2d - y2c)*(x1a - x2c)) / denom;
    if (parAB < 0 || parAB > 1) return;
    double parCD = ((x1b - x1a)*(y1a - y2c) - (y1b - y1a)*(x1a - x2c)) / denom;
    if (parCD < 0 || parCD > 1) return;

    double xIntersect = x1a + parAB * (x1b - x1a);
    double yIntersect = y1a + parAB * (y1b - y1a);
    xIntList.append(xIntersect);
    yIntList.append(yIntersect);
}

double PlotSplitter::yInterpolation(
    const QVector<double>& xVec, const QVector<double>& yVec, double x) {
    if (xVec.isEmpty()) return 0;
    if (x <= xVec.first()) return yVec.first();
    if (x >= xVec.last()) return yVec.last();

    const auto it = std::ranges::lower_bound(xVec, x);
    if (it != xVec.end() && *it == x) return yVec[it - xVec.begin()];

    size_t rIdx = it - xVec.begin();
    size_t lIdx = rIdx - 1;

    double xl = xVec[lIdx], yl = yVec[lIdx];
    double xr = xVec[rIdx], yr = yVec[rIdx];
    return yl + (yr - yl) * (x - xl) / (xr - xl);
}

void PlotSplitter::findIntersections() {
    if (x1.isEmpty() || x2.isEmpty()) return;

    // intersection of plot1 and plot2 domains
    double minX = qMax(*std::ranges::min_element(x1),
                     *std::ranges::min_element(x2));
    double maxX = qMin(*std::ranges::max_element(x1),
                     *std::ranges::max_element(x2));

    // all segments of plot1
    for (int i = 1; i < x1.size(); ++i) {
        double x1a = x1[i-1], y1a = y1[i-1];
        double x1b = x1[i], y1b = y1[i];
        if (x1b < minX || x1a > maxX) continue;

        // all segments of plot2
        for (int j = 1; j < x2.size(); ++j) {
            double x2c = x2[j-1], y2c = y2[j-1];
            double x2d = x2[j], y2d = y2[j];
            if (x2d < minX || x2c > maxX) continue;

            addIntersectionIfExists(x1a, y1a, x1b, y1b, x2c, y2c, x2d, y2d);
        }
    }
}

QList<SubPlot> PlotSplitter::splitSinglePlot(
    const QVector<double>& xThisList, const QVector<double>& yThisList,
    const QVector<double>& xOtherList, const QVector<double>& yOtherList) {
    QList<SubPlot> result;
    if (xIntList.isEmpty()) {
        SubPlot sp;
        sp.x = xThisList;
        sp.y = yThisList;
        sp.isLower = false;
        result.append(sp);
        return result;
    }

    QVector<double> xSubPlotList, ySubPlotList;
    int startIdx = 0;
    for (const auto& xInt : xIntList) {
        double yInt = yInterpolation(xThisList, yThisList, xInt);
        for (int i = startIdx; i < xThisList.size(); ++i) {
            if (xThisList[i] >= xInt) {
                startIdx = i;
                break;
            }
            xSubPlotList.append(xThisList[i]);
            ySubPlotList.append(yThisList[i]);
        }
        if (xSubPlotList.isEmpty()) continue;

        double xCompare = (xInt + xSubPlotList.last()) / 2;
        xSubPlotList.append(xInt);
        ySubPlotList.append(yInt);
        SubPlot sp;
        sp.x = xSubPlotList;
        sp.y = ySubPlotList;
        double yThisComp = yInterpolation(xThisList, yThisList, xCompare);
        double yOtherComp = yInterpolation(xOtherList, yOtherList, xCompare);
        sp.isLower = yThisComp < yOtherComp;
        result.append(sp);
        xSubPlotList = QVector{xInt};
        ySubPlotList = QVector{yInt};
    }

    for (int i = startIdx; i < xThisList.size(); ++i) {
        xSubPlotList.append(xThisList[i]);
        ySubPlotList.append(yThisList[i]);
    }
    if (xSubPlotList.size() < 2) return result;

    // after the last intersection point
    SubPlot sp;
    sp.x = xSubPlotList;
    sp.y = ySubPlotList;
    double xCompare = (xSubPlotList[0] + xSubPlotList[1]) / 2;
    double yThisComp = yInterpolation(xThisList, yThisList, xCompare);
    double yOtherComp = yInterpolation(xOtherList, yOtherList, xCompare);
    sp.isLower = yThisComp < yOtherComp;
    result.append(sp);
    return result;
}

PlotPair PlotSplitter::split() {
    findIntersections();
    auto firstPlot = splitSinglePlot(x1, y1, x2, y2);
    auto secondPlot = splitSinglePlot(x2, y2, x1, y1);
    return qMakePair(firstPlot, secondPlot);
}
