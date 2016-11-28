#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(parar()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(iniciar()));

    primeiraVez = true;
    parado = true;

    MainWindow::MakePlotCarga();
    MainWindow::MakePlotDescarga();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parar()
{
    parado = true;
}

void MainWindow::iniciar()
{

    if(primeiraVez)
    {
        minhaThread = std::thread(&MainWindow::RunPlotCarga, this);
        minhaThread2 = std::thread(&MainWindow::RunPlotDescarga, this);
        primeiraVez = false;
    }
    parado = false;
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

void MainWindow::RunPlotCarga(){
    static unsigned int porcentagemBat;

    while(true){
        if(!parado){

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

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void MainWindow::RunPlotDescarga(){
    static double tempoDescarga;

    while(true){
        if(!parado){

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

            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
}

