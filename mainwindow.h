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
    float lerCPU0();
    float lerCPU1();
    float lerCPU2();
    float lerCPU3();
    float lerVRAM();
    float lerRAM();
    void RunPlotDescarga();
    void RunPlotCPU();
    void RunPlotRAM();
    double lerBateria2();
private slots:
    void MakePlotCarga();
    void MakePlotDescarga();
    void MakePlotCPU();
    void MakePlotRAM();
    void pararTabEnergia();
    void iniciarTabEnergia();
    void pararTabRecursos();
    void iniciarTabRecursos();

private:
    Ui::MainWindow *ui;
    std::thread minhaThread, minhaThread2, minhaThread3, minhaThread4;
    //std::thread minhaThread2;
    bool primeiraVezEnergia, primeiraVezRecursos;
    bool paradoEnergia, paradoRecursos;
};

#endif // MAINWINDOW_H
