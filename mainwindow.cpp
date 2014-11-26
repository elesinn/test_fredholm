#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "somevalues.h"
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <iostream>
#include <cmath>
#include <QVector>
#include <stdlib.h>
#include <iomanip>
#include <gradient_descent.h>
#include "QStandardItemModel"
#include "QStandardItem"
#include "qwt_plot_curve.h"
#include <QFile>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    d = new Dialog();
    values = new SomeValues();
    ui->Qwt_Widget->hide();
    addPlot();
    addPlotGrid();
    setZ();
}

void MainWindow::addPlot()
{
    ui->Qwt_Widget->setTitle( QString("Curve") );
    ui->Qwt_Widget->setCanvasBackground( Qt::white );
    // Параметры осей координат
    ui->Qwt_Widget->setAxisTitle(QwtPlot::yLeft, "Y");
    ui->Qwt_Widget->setAxisTitle(QwtPlot::xBottom, "X");
    ui->Qwt_Widget->insertLegend( new QwtLegend() );

}
//FILE *file;
//void exportFile(QVector<double> s,QVector<double> z0, QVector<double> newZ)
//{

//    file = fopen("values.txt", "r");
//    fprintf(file,"%f\t %f\t %f\n",s[0],z0[0],newZ[0]);
//}

void MainWindow::addPlotGrid()
{

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QPen( Qt::gray, 2 ));
    grid->attach( ui->Qwt_Widget );
}

void MainWindow::setZ()
{
    kz z1;
    curv1=new QwtPlotCurve(QString("Z0(s)"));
    curv1->setRenderHint(QwtPlotItem::RenderAntialiased);
    curv1->setPen(QPen(Qt::blue));
    double a = d->getA();
    double b = d->getB();

    double h1 = (b - a) / snum;
    QVector<double> s(snum);
    for (int i = 0; i<snum; i++){
        s[i] = a + i*h1;
    }

    double q;
    QVector<double> z(xnum);
    for (int i = 0; i<xnum; i++){
        q = a + i*h1;
        z[i] = z1(q);
    }


    curv1->setSamples(s,z);
    curv1->attach(ui->Qwt_Widget);
}

void MainWindow::setPlot(int redColor,int blueColor)
{       

    curv2= new QwtPlotCurve();

    curv2->setPen(QPen(QColor(redColor,blueColor,0)));
    kxs k;
    kz z1;

    double a = d->getA();
    double b = d->getB();    

    double h1 = (b - a) / snum;
    QVector<double> lambda(xnum);
    QVector<double> s(snum);
    for (int i = 0; i<snum; i++){
        s[i] = a + i*h1;
    }
    QVector<double> res;
    double q;
    QVector<double> z(xnum);
    for (int i = 0; i<xnum; i++){
        q = a + i*h1;
        z[i] = z1(q);
    }

    QVector<double> u(xnum);
    QVector<double> u_delta(xnum);
    QVector<double> h_x(snum);
    for (int i = 0; i<snum; i++){
        h_x[i] = rand();
    }

    double h_x_norm = norm(a, b, h_x);

    QVector<double> h(snum);
    double delta = d->getDelta(); // возмущение u
    double delta1 = delta;// возмущение A и A*
    for (int i = 0; i<snum; i++){
        h[i] = h_x[i] * delta / h_x_norm;
    }
    QVector<double> hhh(snum);
    for (int i = 0; i<snum; i++){
        hhh[i] = h_x[i] * delta1 / h_x_norm;
    }
    u = A(k, a, b, s, z,hhh);
    for (int i = 0; i<xnum; i++){
        u_delta[i] = u[i] + h[i];
    }
    //double alpha=d->getAlpha();

    res = GradDown(lambda, s, a, b, u_delta,hhh);
    QVector<double> newz(xnum);

    newz = mult_numb(A_link(k, a, b, res, s,hhh), -0.5, xnum);
// cout << endl;
//    for (int i = 0; i<xnum; i++){
//        cout << newz[i] << endl;
//    }

    QVector<double> new1(xnum);
    new1 = sub( z, newz, snum);
    double nev = norm(a, b, new1);
   // cout << "alpha= " << alpha << "  " << "Nev = " << nev << "  " <<"  delta= " << delta <<endl;
    QString nevText=QString::number(nev);
    ui->label_nev->setText(nevText);
    QString deltaString=QString::number(delta);
 //   QString alphaString=QString::number(alpha);

 //   curv2->setTitle(QString(QString::fromUtf8("Z*(s), α= ")+alphaString+QString::fromUtf8(", δ= ")+deltaString));

    //exportFile(s,z,newz);
    curv2->setSamples(s,newz);
    curv2->attach(ui->Qwt_Widget);
    values->setValue(s,z,newz);

}





MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_action_2_triggered()
//{
//    close();
//}

void MainWindow::on_action_triggered()
{    
  d->show();  
}
int rColor=255;
int bColor=0;
void MainWindow::on_pushButton_clicked()
{

    setPlot(rColor,bColor);
    rColor-=10;
    //bColor+=10;
    ui->Qwt_Widget->replot();
    ui->Qwt_Widget->show();
}

void MainWindow::on_actionValues_triggered()
{
   values->show();
}
