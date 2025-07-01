#pragma once

#include <qcustomplot.h>
#include "PlotController.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

 private slots:
    void on_btnLoadPlot1_clicked();
    void on_btnLoadPlot2_clicked();
    void on_btnTogglePlots_clicked();

 private:
    void loadPlot(PlotIdx plotIdx);

Ui::MainWindow *ui;
    PlotController pc;
};
