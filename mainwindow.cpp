#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(pararTabEnergia()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(iniciarTabEnergia()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(iniciarTabRecursos()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(pararTabRecursos()));

    primeiraVezEnergia = true;
    primeiraVezRecursos = true;
    paradoEnergia = true;
    paradoRecursos = true;

    MainWindow::MakePlotCarga();
    MainWindow::MakePlotDescarga();
    MainWindow::MakePlotCPU();
    MainWindow::MakePlotRAM();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pararTabEnergia()
{
    paradoEnergia = true;
}

void MainWindow::pararTabRecursos()
{
    paradoRecursos = true;
}

void MainWindow::iniciarTabEnergia()
{

    if(primeiraVezEnergia)
    {
        minhaThread = std::thread(&MainWindow::RunPlotCarga, this);
        minhaThread2 = std::thread(&MainWindow::RunPlotDescarga, this);
        primeiraVezEnergia = false;
    }
    paradoEnergia = false;
}

void MainWindow::iniciarTabRecursos()
{

    if(primeiraVezRecursos)
    {
        minhaThread3 = std::thread(&MainWindow::RunPlotCPU, this);
        minhaThread4 = std::thread(&MainWindow::RunPlotRAM, this);
        primeiraVezRecursos = false;
    }
    paradoRecursos = false;
}

int MainWindow::lerBateria(){
    FILE* file = fopen("/sys/class/power_supply/BAT1/capacity", "r");
    unsigned int porcentagem;

    fscanf(file, "%d", &porcentagem);

    fclose(file);

    return porcentagem;
}

double MainWindow::lerBateria2(){
    FILE* file = fopen("/sys/class/power_supply/BAT1/energy_full", "r");
    FILE* file2 = fopen("/sys/class/power_supply/BAT1/energy_now", "r");
    FILE* file3 = fopen("/sys/class/power_supply/BAT1/power_now", "r");
    double energyFull, energyNow, powerNow;
    double tempoDesc = 0.0;
    double teste = 0.0;

    fscanf(file, "%lf", &energyFull);
    fscanf(file2, "%lf", &energyNow);
    fscanf(file3, "%lf", &powerNow);
    fclose(file);

    teste = (energyFull - energyNow) / powerNow ;

    tempoDesc = teste * 60.0;

    return tempoDesc;
}


float MainWindow::lerCPU0(){
    //printf("chegou aqui\n");
    //system("/.cpuData");
    FILE* file = fopen("/home/rubens/QtProjects/ProcessMonitor/saidaCPU.txt", "r");
    float porcentagemCpu0;

    fscanf(file, "%f", &porcentagemCpu0);

    //printf("%f\n", porcentagemCpu0);
    fclose(file);

    return porcentagemCpu0;
}
float MainWindow::lerCPU1(){
    //system("/.cpuData");
    FILE* file = fopen("/home/rubens/QtProjects/ProcessMonitor/saidaCPU.txt", "r");
    float porcentagemCpu1;


    fseek(file, 1, SEEK_CUR);
    fscanf(file, "%f", &porcentagemCpu1);

    fclose(file);

    return porcentagemCpu1;
}
float MainWindow::lerCPU2(){
    FILE* file = fopen("/home/rubens/QtProjects/ProcessMonitor/saidaCPU.txt", "r");
    float porcentagemCpu2;


    fseek(file, 2, SEEK_CUR);
    fscanf(file, "%f", &porcentagemCpu2);

    fclose(file);

    return porcentagemCpu2;
}
float MainWindow::lerCPU3(){
    FILE* file = fopen("/home/rubens/QtProjects/ProcessMonitor/saidaCPU.txt", "r");
    float porcentagemCpu3;

    fseek(file, 3, SEEK_CUR);
    fscanf(file, "%f", &porcentagemCpu3);

    fclose(file);

    return porcentagemCpu3;
}
float MainWindow::lerRAM(){
    FILE* file = fopen("/home/rubens/QtProjects/ProcessMonitor/saidaMem.txt", "r");
    float porcentagemRAM;

    fscanf(file, "%f", &porcentagemRAM);

    fclose(file);

    return porcentagemRAM;
}

float MainWindow::lerVRAM(){
    FILE* file = fopen("/home/rubens/QtProjects/ProcessMonitor/saidaMem.txt", "r");
    float porcentagemVRAM;

    fseek(file, 1, SEEK_CUR);
    fscanf(file, "%f", &porcentagemVRAM);

    fclose(file);

    return porcentagemVRAM;
}



void MainWindow::MakePlotCarga(){
    ui->plotCarga->addGraph()->setPen(QPen(QColor(40, 110, 255)));
    ui->plotCarga->graph(0);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotCarga->xAxis->setTicker(timeTicker);
    ui->plotCarga->axisRect()->setupFullAxesBox();
    ui->plotCarga->yAxis->setRange(0, 110);

    ui->plotCarga->xAxis->setLabel("Tempo de Execução");
    ui->plotCarga->yAxis->setLabel("% Bateria");

}
void MainWindow::MakePlotDescarga(){
    ui->plotDescarga->addGraph()->setPen(QPen(QColor(255, 110, 40)));
    ui->plotDescarga->graph(0);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotDescarga->xAxis->setTicker(timeTicker);
    ui->plotDescarga->axisRect()->setupFullAxesBox();
    ui->plotDescarga->yAxis->setRange(0.0, 190.0);

    ui->plotDescarga->xAxis->setLabel("Tempo de Execução");
    ui->plotDescarga->yAxis->setLabel("Tempo de Descarga (em min)");
}

void MainWindow::MakePlotCPU(){
    ui->plotCPU->legend->setVisible(true);
    ui->plotCPU->legend->setFont(QFont("Helvetica",9));

    ui->plotCPU->addGraph();
    ui->plotCPU->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->plotCPU->graph(0)->setName("% CPU0");


    ui->plotCPU->addGraph();
    ui->plotCPU->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->plotCPU->graph(1)->setName("% CPU1");;

    ui->plotCPU->addGraph()->setPen(QPen(QColor(110, 255, 40)));
    ui->plotCPU->graph(2)->setName("% CPU2");

    ui->plotCPU->addGraph()->setPen(QPen(QColor(255, 255, 40)));
    ui->plotCPU->graph(3)->setName("% CPU3");

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotCPU->xAxis->setTicker(timeTicker);
    ui->plotCPU->axisRect()->setupFullAxesBox();
    ui->plotCPU->yAxis->setRange(0, 110);

    ui->plotCPU->xAxis->setLabel("Tempo de Execução");
    ui->plotCPU->yAxis->setLabel("% CPU");

}
void MainWindow::MakePlotRAM(){
    ui->plotMemoria->legend->setVisible(true);
    ui->plotMemoria->legend->setFont(QFont("Helvetica",9));

    ui->plotMemoria->addGraph()->setPen(QPen(QColor(40, 110, 255)));
    ui->plotMemoria->graph(0)->setName("% RAM");
    ui->plotMemoria->addGraph()->setPen(QPen(QColor(255, 110, 40)));
    ui->plotMemoria->graph(1)->setName("% VRAM");

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotMemoria->xAxis->setTicker(timeTicker);
    ui->plotMemoria->axisRect()->setupFullAxesBox();
    ui->plotMemoria->yAxis->setRange(0, 110);

    ui->plotMemoria->xAxis->setLabel("Tempo de Execução");
    ui->plotMemoria->yAxis->setLabel("%");
}

void MainWindow::RunPlotCarga(){
    static unsigned int porcentagemBat;

    while(true){
        if(!paradoEnergia){

            porcentagemBat = lerBateria();

            static QTime time(QTime::currentTime());
            double tempo = time.elapsed()/1000.0;

            static double lastPointKey = 0;

            if (tempo-lastPointKey > 0.002)
            {
              ui->plotCarga->graph(0)->addData(tempo, porcentagemBat);
              lastPointKey = tempo;
            }

            ui->plotCarga->xAxis->setRange(tempo, 8, Qt::AlignRight);
            ui->plotCarga->replot();

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void MainWindow::RunPlotCPU(){
    float porcentagemCPU0, porcentagemCPU1, porcentagemCPU2, porcentagemCPU3;

    while(true){
        if(!paradoRecursos){

            //system("/home/rubens/QtProjects/ProcessMonitor/cpuData");

            //std::this_thread::sleep_for(std::chrono::seconds(2));

            porcentagemCPU0 = lerCPU0();
            porcentagemCPU1 = lerCPU1();
            porcentagemCPU2 = lerCPU2();
            porcentagemCPU3 = lerCPU3();

            static QTime time(QTime::currentTime());
            double tempo = time.elapsed()/1000.0;

            static double lastPointKey = 0;

            if (tempo-lastPointKey > 0.002)
            {
              ui->plotCPU->graph(0)->addData(tempo, porcentagemCPU0);
              ui->plotCPU->graph(1)->addData(tempo, porcentagemCPU1);
              ui->plotCPU->graph(2)->addData(tempo, porcentagemCPU2);
              ui->plotCPU->graph(3)->addData(tempo, porcentagemCPU3);
              lastPointKey = tempo;
            }

            ui->plotCPU->xAxis->setRange(tempo, 8, Qt::AlignRight);
            ui->plotCPU->replot();

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void MainWindow::RunPlotDescarga(){
    static double tempoDescarga;

    while(true){
        if(!paradoEnergia){

            tempoDescarga = lerBateria2();

            static QTime time(QTime::currentTime());
            double tempo = time.elapsed()/1000.0;

            static double lastPointKey = 0;

            if (tempo-lastPointKey > 0.004)
            {
              ui->plotDescarga->graph(0)->addData(tempo, tempoDescarga);
              lastPointKey = tempo;
            }

            ui->plotDescarga->xAxis->setRange(tempo, 8, Qt::AlignRight);
            ui->plotDescarga->replot();

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void MainWindow::RunPlotRAM(){
    float ram, vRam;

    while(true){
        if(!paradoRecursos){

            //system("/home/rubens/QtProjects/ProcessMonitor/memData");

            //std::this_thread::sleep_for(std::chrono::seconds(3));

            ram = lerRAM();
            vRam = lerVRAM();

            static QTime time(QTime::currentTime());
            double tempo = time.elapsed()/1000.0;

            static double lastPointKey = 0;

            if (tempo-lastPointKey > 0.004)
            {
              ui->plotMemoria->graph(0)->addData(tempo, ram);
              ui->plotMemoria->graph(1)->addData(tempo, vRam);
              lastPointKey = tempo;
            }

            ui->plotMemoria->xAxis->setRange(tempo, 8, Qt::AlignRight);
            ui->plotMemoria->replot();

            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
}
