#include "computer.h"

#include <verilated.h>
#include "Vcomputer.h"
#include "verilated_vcd_c.h"

#include <iostream>

VerilatedContext* contextp;
Vcomputer* computer;

VerilatedVcdC* vcd;
bool vcdFinished = false;
int vcdClockCycles = 100000;
std::string vcdFileName = "logs/computer.vcd";

void SetupComputer() {
    contextp = new VerilatedContext;
    computer = new Vcomputer(contextp);
    
    vcd = new VerilatedVcdC;
    contextp->traceEverOn(true);
    computer->trace(vcd, 99);

    vcd->open(vcdFileName.c_str());
}

void CloseComputer() {
    std::cout << "Computer Test Finished\n";
    
    if (!vcdFinished) {
        vcd->close();
    }
    computer->final();

    delete vcd;
    delete computer;
    delete contextp;
}

void Update() {
    computer->eval();

    if (!vcdFinished) {
        vcd->dump(contextp->time());
        if (contextp->time() / 10 >= vcdClockCycles) {
            vcd->close();
            
            vcdFinished = true;

            std::cout << "VCD Finished\n";
        }
    }
    
    contextp->timeInc(1);
}

void RunComputer(bool& finished) {
    while (!finished && !contextp->gotFinish()) {
        Update();
    }
}
