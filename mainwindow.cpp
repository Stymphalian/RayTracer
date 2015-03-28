#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionSave_As,SIGNAL(triggered(bool)),this,SLOT(save_as(bool)));
    connect(ui->actionBegin_Ray_Trace,SIGNAL(triggered(bool)),this,SLOT(ray_trace(bool)));
    ui->widget->setModel(&model);
    //ui->widget->setProgressBar(ui->progressBar);
    ui->widget_2->setModel(&model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if( event->key() == Qt::Key_M){
        ui->widget->makeImage();
    } else if( event->key() == Qt::Key_Escape){
        close();
    }else{
        ui->widget_2->moveCam(event);
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::save_as(bool value)
{
    //if(value == false){return;}
    QString fileBuf;

    qDebug() << " saving image\n";
    fileBuf = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "/home/untitled.png",
                               tr("Images (*.png *.xpm *.jpg)"));
     if (fileBuf=="") return;   // cancelled
   // QMessageBox::information( this, "Qtglskel: Opened image file ", fileBuf, QMessageBox::Ok );
     qDebug()<< "calling save in glwidget "<< fileBuf << "\n";
    ui->widget->saveImage(fileBuf);
}

void MainWindow::ray_trace(bool)
{
    ui->widget->makeImage();
}
