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

private:
    Ui::MainWindow *ui;
private slots:
    void save_as(bool);
    void ray_trace(bool);
};

#endif // MAINWINDOW_H

/*
refractions
obj files
solid surfaces for opengl draw
materials read from file?


*/