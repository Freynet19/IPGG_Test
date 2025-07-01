#include "PlotController.h"
#include <QFileDialog>
#include <QMessageBox>
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

void PlotController::loadPlotFromFile(PlotIdx idx) {
    QString fileName = QFileDialog::getOpenFileName(
            nullptr, "Load Plot from File", "./", "");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error",
            "Failed to open file.");
        return;
    }

    QVector<double> x, y;
    QTextStream in(&file);
    while (!in.atEnd()) {
        parseLineToCoords(in.readLine(), x, y);
    }
    if (x.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
            "No plot data has been parsed from file.");
        return;
    }

    if (idx == PlotIdx::FIRST) {
        x1 = x;
        y1 = y;
    } else {
        x2 = x;
        y2 = y;
    }

    setSubPlots();
    plot->rescaleAxes();
    setPens();
    plot->replot();

    QMessageBox::information(nullptr, "Success",
        QString("Plot %1 loaded successfully").arg(static_cast<int>(idx)));
}

void PlotController::togglePlots() {
    if (bottomIdx == PlotIdx::SECOND) bottomIdx = PlotIdx::FIRST;
    else bottomIdx = PlotIdx::SECOND;
    setPens();
    plot->replot();
}

void PlotController::parseLineToCoords(
    const QString& line, QVector<double>& x, QVector<double>& y) {
    if (line.isEmpty()) return;

    QStringList parts = line.split("\t");
    if (parts.size() != 2) {
        qDebug() << "Invalid line: " << line;
        return;
    }

    bool xIsOk, yIsOk;
    double newX = parts[0].toDouble(&xIsOk);
    double newY = parts[1].toDouble(&yIsOk);

    if (!xIsOk || !yIsOk) {
        if (!xIsOk) qDebug() << "Failed to parse x value: " << parts[0];
        if (!yIsOk) qDebug() << "Failed to parse y value: " << parts[1];
        return;
    }

    x.append(newX);
    y.append(newY);
}

void PlotController::setSubPlots() {
    for (const auto &sp : firstPlot) plot->removeGraph(sp.graph);
    for (const auto &sp : secondPlot) plot->removeGraph(sp.graph);
    firstPlot.clear();
    secondPlot.clear();

    PlotSplitter ps(x1, y1, x2, y2);
    auto plotPair = ps.split();
    firstPlot = plotPair.first;
    secondPlot = plotPair.second;

    for (auto &sp : firstPlot) {
        sp.graph = plot->addGraph();
        sp.graph->setData(sp.x, sp.y);
    }
    for (auto &sp : secondPlot) {
        sp.graph = plot->addGraph();
        sp.graph->setData(sp.x, sp.y);
    }
}

void PlotController::setPens() {
    if (bottomIdx == PlotIdx::SECOND) {
        for (auto &sp : secondPlot) {
            if (sp.isLower) sp.graph->setPen(penBottom2);
            else sp.graph->setPen(penTop2);
        }
        for (auto &sp : firstPlot) {
            sp.graph->setPen(penTop1);
        }
    } else {
        for (auto &sp : firstPlot) {
            if (sp.isLower) sp.graph->setPen(penBottom1);
            else sp.graph->setPen(penTop1);
        }
        for (auto &sp : secondPlot) {
            sp.graph->setPen(penTop2);
        }
    }
}
