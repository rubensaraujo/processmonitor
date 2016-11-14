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
        //minhaThread = std::thread(&MainWindow::lerBateria, this);
        minhaThread2 = std::thread(&MainWindow::RunPlotCarga, this);
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


void MainWindow::MakePlotCarga(){
    ui->plotCarga->addGraph()->setPen(QPen(QColor(40, 110, 255)));; // blue line
    ui->plotCarga->graph(0);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotCarga->xAxis->setTicker(timeTicker);
    ui->plotCarga->axisRect()->setupFullAxesBox();
    ui->plotCarga->yAxis->setRange(0, 110);

    ui->plotCarga->xAxis->setLabel("Tempo de Execução");
    ui->plotCarga->yAxis->setLabel("% Bateria");

}
void MainWindow::RunPlotCarga(){
    static unsigned int porcentagemBat;

    while(true){
        if(!parado){

            porcentagemBat = lerBateria();

            static QTime time(QTime::currentTime());
            double tempo = time.elapsed()/1000.0;

            static double lastPointKey = 0;

            if (tempo-lastPointKey > 0.001)
            {
              ui->plotCarga->graph(0)->addData(tempo, porcentagemBat);
              lastPointKey = tempo;
            }

            ui->plotCarga->xAxis->setRange(tempo, 8, Qt::AlignRight);
            ui->plotCarga->replot();

            std::this_thread::sleep_for(std::chrono::seconds(2));
            //fclose(file);
        }
    }
}

void MainWindow::MakePlotDescarga(){

}

