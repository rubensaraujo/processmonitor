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
        //minhaThread2 = std::thread(&MainWindow::MakePlotDescarga, this);
        primeiraVez = false;
    }
    parado = false;

}


void MainWindow::MakePlotCarga(){
    ui->plotCarga->addGraph(); // blue line
    ui->plotCarga->graph(0);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plotCarga->xAxis->setTicker(timeTicker);
    ui->plotCarga->axisRect()->setupFullAxesBox();
    ui->plotCarga->yAxis->setRange(0, 100);


}
void MainWindow::RunPlotCarga(){
    std::string dado;
    std::ifstream readDado;
    //dado = "teste";
    while(true){
        if(!parado){
            //std::system("cat /sys/class/power_supply/BAT1/uevent | grep POWER_SUPPLY_CAPACITY= ");
            readDado.open("/sys/class/power_supply/BAT1/uevent/capacity");
            while (!readDado.eof()) {


                getline(readDado, dado);
                std::cout << dado << '\n';


             }
            readDado.close();
            //std::cout << dado << '\n';

            static QTime time(QTime::currentTime());
            double tempo = time.elapsed()/1000.0;

            static double lastPointKey = 0;

            if (tempo-lastPointKey > 0.001)
            {

              ui->plotCarga->graph(0)->addData(tempo, 50);

              lastPointKey = tempo;
            }

            ui->plotCarga->xAxis->setRange(tempo, 8, Qt::AlignRight);
            ui->plotCarga->replot();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void MainWindow::MakePlotDescarga(){

}

