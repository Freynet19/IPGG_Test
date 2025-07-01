#include "MainWindow.h"
#include "ui_MainWindow.h"  // build (run uic code generator) first
#include "PlotController.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    pc.init(ui->customPlot);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_btnLoadPlot1_clicked() {
    loadPlot(PlotIdx::FIRST);
}

void MainWindow::on_btnLoadPlot2_clicked() {
    loadPlot(PlotIdx::SECOND);
}

void MainWindow::on_btnTogglePlots_clicked() {
    pc.togglePlots();
}

void MainWindow::loadPlot(PlotIdx plotIdx) {
    QString fileName = QFileDialog::getOpenFileName(nullptr,
        "Load Plot from File", "./", "");
    if (fileName.isEmpty()) return;

    try {
        auto coordListPair = pc.loadCoordsFromFile(fileName);
        pc.setPlot(coordListPair, plotIdx);
        pc.redrawPlots();
        QMessageBox::information(nullptr, "Success", QString(
            "Plot %1 loaded successfully").arg(static_cast<int>(plotIdx)));
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(nullptr, "Error", e.what());
    }
}
