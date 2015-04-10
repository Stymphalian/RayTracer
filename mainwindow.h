#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Models/WorldModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    WorldModel model;
    void keyPressEvent(QKeyEvent *);
    void uploadSettingsToGui();

private:
    Ui::MainWindow *ui;
private slots:
    void save_as(bool);
    void ray_trace(bool);
    void refreshMaterials();

    void handleAntiAliasFlag(int);
    void handleSoftShadowsFlag(int);
    void handleSoftShadowsNumber(int);
    void handleAntiAliasNumber(int);
    void handleRecursionDepth(int);
    void handleNumberThreads(int);
    void handleTimeTakenToRender(qint64);

};

#endif // MAINWINDOW_H
