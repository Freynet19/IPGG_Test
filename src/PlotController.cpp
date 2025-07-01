#include "PlotController.h"
#include <QTextStream>

void PlotController::init(QCustomPlot* customPlot) {
    plot = customPlot;
    plot->xAxis->setLabel("X");
    plot->yAxis->setLabel("Y");
    penTop1 = QPen(PEN_FIRST_COLOR, PEN_TOP_WIDTH, PEN_TOP_STYLE);
    penBottom1 = QPen(PEN_FIRST_COLOR, PEN_BOTTOM_WIDTH, PEN_BOTTOM_STYLE);
    penTop2 = QPen(PEN_SECOND_COLOR, PEN_TOP_WIDTH, PEN_TOP_STYLE);
    penBottom2 = QPen(PEN_SECOND_COLOR, PEN_BOTTOM_WIDTH, PEN_BOTTOM_STYLE);
}

CoordListPair PlotController::loadCoordsFromFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Failed to open file.");
    }

    QVector<double> x, y;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) continue;
        try {
            auto coordPair = parseLineToCoords(line);
            x.append(coordPair.first);
            y.append(coordPair.second);
        } catch (std::runtime_error &e) {
            qDebug() << e.what();
        }
    }
    if (x.isEmpty()) {
        throw std::runtime_error("No plot data has been parsed from file.");
    }

    return qMakePair(x, y);
}

void PlotController::setPlot(const CoordListPair& clp, PlotIdx plotIdx) {
    if (plotIdx == PlotIdx::FIRST) {
        x1 = clp.first;
        y1 = clp.second;
    } else {
        x2 = clp.first;
        y2 = clp.second;
    }
}

void PlotController::redrawPlots() {
    PlotSplitter ps(x1, y1, x2, y2);
    auto plotPair = ps.split();
    firstPlot = plotPair.first;
    secondPlot = plotPair.second;

    plot->clearGraphs();
    setGraphs();
    plot->rescaleAxes();
    setPens();
    plot->replot();
}

void PlotController::togglePlots() {
    if (bottomIdx == PlotIdx::SECOND) bottomIdx = PlotIdx::FIRST;
    else bottomIdx = PlotIdx::SECOND;
    setPens();
    plot->replot();
}

CoordPair PlotController::parseLineToCoords(const QString& line) {
    QStringList parts = line.split("\t");
    if (parts.size() != 2) {
        throw std::runtime_error("Invalid line: " + line.toStdString());
    }

    bool xIsOk, yIsOk;
    double newX = parts[0].toDouble(&xIsOk);
    double newY = parts[1].toDouble(&yIsOk);

    if (!xIsOk || !yIsOk) {
        if (!xIsOk) throw std::runtime_error(
            "Failed to parse x value: " + parts[0].toStdString());
        if (!yIsOk) throw std::runtime_error(
            "Failed to parse y value: " + parts[1].toStdString());
    }

    return qMakePair(newX, newY);
}

void PlotController::setGraphs() {
    firstGraph.clear();
    secondGraph.clear();
    for (auto &sp : firstPlot) {
        firstGraph.append(plot->addGraph());
        firstGraph.last()->setData(sp.x, sp.y);
    }
    for (auto &sp : secondPlot) {
        secondGraph.append(plot->addGraph());
        secondGraph.last()->setData(sp.x, sp.y);
    }
}

void PlotController::setPens() {
    if (bottomIdx == PlotIdx::SECOND) {
        for (int i = 0; i < secondPlot.size(); ++i) {
            if (secondPlot[i].isLower) secondGraph[i]->setPen(penBottom2);
            else secondGraph[i]->setPen(penTop2);
        }
        for (int i = 0; i < firstPlot.size(); ++i) {
            firstGraph[i]->setPen(penTop1);
        }
    } else {
        for (int i = 0; i < firstPlot.size(); ++i) {
            if (firstPlot[i].isLower) firstGraph[i]->setPen(penBottom1);
            else firstGraph[i]->setPen(penTop1);
        }
        for (int i = 0; i < secondPlot.size(); ++i) {
            secondGraph[i]->setPen(penTop2);
        }
    }
}
