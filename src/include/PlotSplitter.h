#pragma once

#include <qcustomplot.h>

typedef struct {
    QVector<double> x, y;
    QCPGraph *graph;
    bool isLower;
} SubPlot;

class PlotSplitter {
 public:
    PlotSplitter(const QVector<double>& x1, const QVector<double>& y1,
        const QVector<double>& x2, const QVector<double>& y2,
        QList<SubPlot>& firstPlot, QList<SubPlot>& secondPlot);
    void split();

 private:
    void addIntersectionIfExists(
        double x1a, double y1a, double x1b, double y1b,
        double x2c, double y2c, double x2d, double y2d);
    static double yInterpolation(
        const QVector<double>& xVec, const QVector<double>& yVec, double x);
    void findIntersections();
    void splitSinglePlot(QList<SubPlot>& result,
        const QVector<double>& xThisList, const QVector<double>& yThisList,
        const QVector<double>& xOtherList, const QVector<double>& yOtherList);

    const QVector<double> &x1, &y1, &x2, &y2;
    QList<SubPlot> &firstPlot, &secondPlot;
    QVector<double> xIntList, yIntList;
};
