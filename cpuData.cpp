#include "sys/types.h"
#include "sys/sysinfo.h"
#include "iostream"
#include <unistd.h>

using namespace std;


static unsigned long long lastTotalUser[5], lastTotalUserLow[5], lastTotalSys[5], lastTotalIdle[5];

void init(){
    FILE* file = fopen("/proc/stat", "r");

    fseek(file, 3, SEEK_CUR);
    for (int i = 0; i < 5; ++i)
    {
        //fseek(file, 3, SEEK_CUR);
        fscanf(file, "%llu %llu %llu %llu", &lastTotalUser[i], &lastTotalUserLow[i], 
        &lastTotalSys[i], &lastTotalIdle[i]);
        fseek(file, +24, SEEK_CUR);
    }
    
    fclose(file);
}

void getCurrentValue(){
    double percent[5];
    FILE* file;
    FILE* saida;
    unsigned long long totalUser[5], totalUserLow[5], totalSys[5], totalIdle[5], total[5];

    //init();

    file = fopen("/proc/stat", "r");
    saida = fopen("saidaCPU.txt", "w");
    
    fseek(file, 3, SEEK_CUR);
    for (int i = 0; i < 5; ++i)
    {
        //fseek(file, 3, SEEK_CUR);
        fscanf(file, "%llu %llu %llu %llu", &totalUser[i], &totalUserLow[i], 
        &totalSys[i], &totalIdle[i]);
        fseek(file, +24, SEEK_CUR);
    }
    
    fclose(file);

    for (int i = 0; i < 5; ++i)
    {
        if (totalUser[i] < lastTotalUser[i] || totalUserLow[i] < lastTotalUserLow[i] ||
        totalSys[i] < lastTotalSys[i] || totalIdle[i] < lastTotalIdle[i]){
            //Overflow detection. Just skip this value.
            percent[i] = -1.0;
        }
        else{
            total[i] = (totalUser[i] - lastTotalUser[i]) + (totalUserLow[i] - lastTotalUserLow[i]) +
                (totalSys[i] - lastTotalSys[i]);
            percent[i] = total[i];
            total[i] += (totalIdle[i] - lastTotalIdle[i]);
            percent[i] /= total[i];
            percent[i] *= 100;
        }

        lastTotalUser[i] = totalUser[i];
        lastTotalUserLow[i] = totalUserLow[i];
        lastTotalSys[i] = totalSys[i];
        lastTotalIdle[i] = totalIdle[i];    
        
        cout << percent[i] << endl;
    }
    
    for (int i = 1; i < 5; ++i)
    {
        fprintf(saida, "%.1f\n", percent[i]);
    }
    
    fclose(saida);
}

int main(){
    while(true){
        init();
        sleep(1);
        getCurrentValue();
    }    

    return 0;
}