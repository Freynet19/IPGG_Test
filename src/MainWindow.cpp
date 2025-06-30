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
    pc.loadPlotFromFile(PlotIdx::FIRST);
}

void MainWindow::on_btnLoadPlot2_clicked() {
    pc.loadPlotFromFile(PlotIdx::SECOND);
}

void MainWindow::on_btnTogglePlots_clicked() {
    pc.togglePlots();
}
