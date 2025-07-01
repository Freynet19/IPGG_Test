#pragma once

#include <QVector>

typedef struct {
    QVector<double> x, y;
    bool isLower;
} SubPlot;

using PlotPair = QPair<QList<SubPlot>, QList<SubPlot>>;

class PlotSplitter {
 public:
    PlotSplitter(const QVector<double>& x1, const QVector<double>& y1,
        const QVector<double>& x2, const QVector<double>& y2);
    PlotPair split();

 private:
    void addIntersectionIfExists(
        double x1a, double y1a, double x1b, double y1b,
        double x2c, double y2c, double x2d, double y2d);
    static double yInterpolation(
        const QVector<double>& xVec, const QVector<double>& yVec, double x);
    void findIntersections();
    QList<SubPlot> splitSinglePlot(
        const QVector<double>& xThisList, const QVector<double>& yThisList,
        const QVector<double>& xOtherList, const QVector<double>& yOtherList);

    const QVector<double> &x1, &y1, &x2, &y2;
    QVector<double> xIntList, yIntList;
};
