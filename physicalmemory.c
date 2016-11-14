    //**********TOTAL PHYSICAL MEMORY (RAM):

    Same code as in "Total Virtual Memory" and then

    long long totalPhysMem = memInfo.totalram;
    //Multiply in next statement to avoid int overflow on right hand side...
    totalPhysMem *= memInfo.mem_unit;





    //**********PHYSICAL MEMORY CURRENTLY USED:
    
    Same code as in "Total Virtual Memory" and then

    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;





    //**********PHYSICAL MEMORY CURRENTLY UDES BY CURRENT PROCESS:

    Change getValue() in "Virtual Memory currently used by current process" as follows:

    int getValue(){ //Note: this value is in KB!
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];

        while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmRSS:", 6) == 0){
                result = parseLine(line);
                break;
            }
        }
        fclose(file);
        return result;
    }


