#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVector>
#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <QDialog>
#include "ui_dialog.h"
#include "dialog.h"
#include "somevalues.h"
#include "ui_somevalues.h"
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>

#include <qwt_picker_machine.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void addPlotGrid();
    void addPlot();
    void setZ();
    void setPlot(int redColor,int blueColor);
   // void exportFile(QVector(),QVector(), QVector());
    ~MainWindow();

private slots:
  //  void on_action_2_triggered();

    void on_action_triggered();

    void on_pushButton_clicked();

    void on_actionValues_triggered();

private:



    QwtPlotCurve *curv1;
    QwtPlotCurve *curv2;
   // QwtPlot *plot;

    Dialog* d;
    SomeValues *values;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
