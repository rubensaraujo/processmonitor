#include "sys/types.h"
#include "sys/sysinfo.h"
#include "iostream"
#include <unistd.h>

using namespace std;


int main(){
    double ram, vram, usedvram, usedram, percentRam, percentVram;

    struct sysinfo memInfo;
    FILE* saida;

    //saida = fopen("saidaMem.txt", "w");

    while(true){
        saida = fopen("saidaMem.txt", "w");
        sysinfo (&memInfo);
        long long totalVirtualMem = memInfo.totalram;
        //Add other values in next statement to avoid int overflow on right hand side...
        totalVirtualMem += memInfo.totalswap;
        totalVirtualMem *= memInfo.mem_unit;
        vram=(totalVirtualMem/1024)/1024;
        cout << "TOTAL VRAM: "<< vram << endl;

            long long totalPhysMem = memInfo.totalram;
        //Multiply in next statement to avoid int overflow on right hand side...
        totalPhysMem *= memInfo.mem_unit;
        ram = (totalPhysMem/1024)/1024;
        cout << "TOTAL RAM: "<< ram << endl;


        long long physMemUsed = memInfo.totalram - memInfo.freeram;
        //Multiply in next statement to avoid int overflow on right hand side...
        physMemUsed *= memInfo.mem_unit;
        usedram = (physMemUsed/1024)/1024;
        cout << "USED RAM: "<< usedram<<endl;

        long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
        //Add other values in next statement to avoid int overflow on right hand side...
        virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
        virtualMemUsed *= memInfo.mem_unit;
        usedvram = (virtualMemUsed/1024)/1024;
        cout << "USED VRAM: " << usedvram << endl;

        percentRam = (usedram * 100) / ram;
        percentVram = (usedvram * 100) / vram;

        cout << "Porcentagem RAM: " << percentRam << endl;
        cout << "Porcentagem VRAM: " << percentVram << endl;
        fprintf(saida, "%.1f\n", percentRam);
        fprintf(saida, "%.1f\n", percentVram);

        fclose(saida);
        sleep(1);
    }    
    return 0;
}



