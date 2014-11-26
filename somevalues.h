#ifndef SOMEVALUES_H
#define SOMEVALUES_H

#include <QDialog>
#include <QVector>
#include "ui_somevalues.h"
#include "QStandardItemModel"
#include "QStandardItem"
namespace Ui {
class SomeValues;
}

class SomeValues : public QDialog
{
    Q_OBJECT

public:
    explicit SomeValues(QWidget *parent = 0);

    void setValue(QVector<double> s,QVector<double> z0, QVector<double> newZ);
    void saveAsTXT(QString filename);

    ~SomeValues();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SomeValues *ui;
};

#endif // SOMEVALUES_H
