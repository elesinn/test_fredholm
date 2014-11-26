#include "somevalues.h"
#include "ui_somevalues.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
SomeValues::SomeValues(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SomeValues)
{
    ui->setupUi(this);
}

SomeValues::~SomeValues()
{
    delete ui;
}

void vectorToItemToModel(QVector<double> vector,int column,QStandardItemModel *model,QStandardItem *item)
{
    QString tmp;
    for(int i=0;i<vector.size();i++)
    {
        tmp=QString::number(vector[i]);
        item = new QStandardItem(tmp);
        model->setItem(i,column,item);
    }
}

void SomeValues::setValue(QVector<double> s,QVector<double> z0, QVector<double> newZ)
{
    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

    QStringList horizontalHeader;
    horizontalHeader.append("s");
    horizontalHeader.append("Z0(s)");
    horizontalHeader.append("Z*(s)");        
    model->setHorizontalHeaderLabels(horizontalHeader);    
    vectorToItemToModel(s,0,model,item);
    vectorToItemToModel(z0,1,model,item);
    vectorToItemToModel(newZ,2,model,item);
    ui->tableView->setModel(model);
}

void SomeValues::saveAsTXT(QString filename)
{

    QFile f(filename);

    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;
        for( int r = 0; r < ui->tableView->verticalHeader()->count(); ++r )
        {
            strList.clear();
            for( int c =0; c < ui->tableView->horizontalHeader()->count(); ++c )
            {
                strList <<ui->tableView->model()->data(ui->tableView->model()->index(r, c), Qt::DisplayRole).toString();
            }
            ts << strList.join( "\t" )+"\n";
        }
        f.close();
    }
}

void SomeValues::on_pushButton_clicked()
{
    QString str = QFileDialog::getSaveFileName(0, "Save as", "", "");
    saveAsTXT(str);
}
