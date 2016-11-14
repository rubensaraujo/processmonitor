#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include <chrono>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void RunPlotCarga();
    int lerBateria();
private slots:
    void MakePlotCarga();
    void MakePlotDescarga();
    void parar();
    void iniciar();

private:
    Ui::MainWindow *ui;
    std::thread minhaThread, minhaThread2;
    //std::thread minhaThread2;
    bool primeiraVez;
    bool parado;
};

#endif // MAINWINDOW_H
