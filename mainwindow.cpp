#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utils/ObjFileReader.h"
#include "Utils/MaterialFileReader.h"
#include "Utils/jRand.h"
#include "Materials/MaterialFactory.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->sceneNum = 0;

    // initialize the rng with 400 numbers.
    jRand& jrand = jRand::getInstance();
    jrand.init(400);

    //ObjFileReader reader;
    //reader.test();
    //reader.read("test.obj",&obj_model);
    // MaterialFileReader mat_reader;
    // mat_reader.test();

    connect(ui->actionSave_As,SIGNAL(triggered(bool)),this,SLOT(save_as(bool)));
    connect(ui->actionBegin_Ray_Trace,SIGNAL(triggered(bool)),this,SLOT(ray_trace(bool)));
    connect(ui->widget,SIGNAL(timeTakenToRender(qint64)),this,SLOT(handleTimeTakenToRender(qint64)));
    ui->widget->setModel(&model);
    ui->widget_2->setModel(&model);

    uploadSettingsToGui();
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

void MainWindow::refreshMaterials(){
  MaterialFactory::getInstance().refresh();
  model.reload(sceneNum);
}

void MainWindow::uploadSettingsToGui(){
    ui->antiAliasSpinBox->setValue(model.config.numberJitterCols);
    ui->antiAliasCheckBox->setChecked(model.config.jitterSamplingEnabled);
    ui->softShadowsCheckBox->setChecked(model.config.softShadowsEnabled);
    ui->softShadowsComboBox->setCurrentIndex(sqrt(model.config.numSoftShadowSamples)-1);
    ui->maxDepthSpinBox->setValue(model.config.max_depth);
    ui->numberThreadsSpinBox->setValue(ui->widget->getMaxNumberThreads());
}

void MainWindow::handleAntiAliasFlag(int i){
    model.config.jitterSamplingEnabled = (i != 0);
}
void MainWindow::handleSoftShadowsFlag(int i){
    model.config.softShadowsEnabled = (i != 0);
}
void MainWindow::handleSoftShadowsNumber(int i){
    model.config.numSoftShadowSamples = (i+1)*(i+1);
}
void MainWindow::handleAntiAliasNumber(int i){
    model.config.numberJitterRows = i;
    model.config.numberJitterCols = i;
}
void MainWindow::handleRecursionDepth(int i){
    if( i <= 0){return;}
    model.config.max_depth = i;
}

void MainWindow::handleNumberThreads(int value){
    ui->widget->setMaxNumberThreads(value);
}

void MainWindow::handleTimeTakenToRender(qint64 value){
    ui->TimeTakenLineEdit->setText("Time Taken: " + QString::number(value) + " ms");
}

void MainWindow::handleChooseScene(int v){
    sceneNum = v;
}
void MainWindow::handleLoadScene(){
    model.reload(sceneNum);
}

/*
FIXES:
    weird perspective issue between ray-traced viewport + opengl viewport.
    Refractions are highly broken with everything but a sphere.
    Code size is enormous
    documentation.
    make sure it complies on the lab machines.
*/