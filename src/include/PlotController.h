#pragma once

#include <qcustomplot.h>
#include "PlotSplitter.h"

enum class PlotIdx { FIRST = 1, SECOND = 2 };

class PlotController {
 public:
    void init(QCustomPlot *customPlot);
    void loadPlotFromFile(PlotIdx idx);
    void togglePlots();

 private:
    static void parseLineToCoords(
        const QString& line, QVector<double>& x, QVector<double>& y);
    void setSubPlots();
    void setPensAndReplot();

    QCustomPlot *plot = nullptr;
    PlotIdx bottomIdx = PlotIdx::SECOND;

    QPen penTop1, penBottom1, penTop2, penBottom2;
    const QColor PEN_FIRST_COLOR = Qt::red, PEN_SECOND_COLOR = Qt::green;
    static constexpr double PEN_TOP_WIDTH = 2, PEN_BOTTOM_WIDTH = 1;
    static constexpr Qt::PenStyle
        PEN_TOP_STYLE = Qt::SolidLine, PEN_BOTTOM_STYLE = Qt::DashDotLine;

    QVector<double> x1, y1, x2, y2;
    QList<SubPlot> firstPlot, secondPlot;
};
