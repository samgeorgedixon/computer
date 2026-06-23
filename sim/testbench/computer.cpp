#include "computer.h"

#include <verilated.h>
#include "Vcomputer.h"
#include "verilated_fst_c.h"

#include <iostream>
#include <chrono>

VerilatedContext* contextp;
Vcomputer* computer;

VerilatedFstC* tfp; // Trace FST Pointer

bool tfpFinished = false;
int tfpClockCycles = 10000;
std::string tfpFileName = "logs/computer.fst";

uint64_t tfpHalfCycle = 0;
uint64_t secondCycles = 0;
float clockSpeed = 0; // Hz
bool lastClockState = false;

auto startTime = std::chrono::high_resolution_clock::now();

std::string FormatSpeedInHz(float speedHz) {
    char buffer[64];

    if (speedHz >= 1e9) {
        std::sprintf(buffer, "%.3f GHz", speedHz / 1e9);
    }
    else if (speedHz >= 1e6) {
        std::sprintf(buffer, "%.3f MHz", speedHz / 1e6);
    }
    else if (speedHz >= 1e3) {
        std::sprintf(buffer, "%.3f KHz", speedHz / 1e3);
    }
    else {
        std::sprintf(buffer, "%.0f Hz", speedHz);
    }

    return buffer;
}

void Update() {
    lastClockState = computer->clk;
    computer->clk = !computer->clk;

    computer->eval();

    if (lastClockState == 0 && computer->clk == 1) { // On Clock Rising Edge (posedge)
        secondCycles++;
        
        if (tfpClockCycles == 0 && !tfpFinished) {
            tfp->close();
            tfpFinished = true;
            
            std::cout << "FST Trace Finished\n";
        }
        else {
            tfpClockCycles--;
        }
        
        if (!tfpFinished) { // Dump On posedge
            tfpHalfCycle++;
            tfp->dump(tfpHalfCycle);
        }
    }
    if (lastClockState == 1 && computer->clk == 0) { // On Clock Falling Edge (negedge)
        if (!tfpFinished) { // Dump On negedge
            tfpHalfCycle++;
            tfp->dump(tfpHalfCycle);
        }
    }

    // Measure Clock Speed
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - startTime;

    if (elapsed.count() >= 1) {
        clockSpeed = secondCycles / elapsed.count();

        std::cout << "Clock Speed: " << FormatSpeedInHz(clockSpeed) << "\n";

        secondCycles = 0;
        startTime = now;
    }
}

void RunComputer(bool& finished) {
    while (!finished && !contextp->gotFinish()) {
        Update();
    }
}

void SetupComputer() {
    contextp = new VerilatedContext;
    computer = new Vcomputer(contextp);
    
    tfp = new VerilatedFstC;
    contextp->traceEverOn(true);
    computer->trace(tfp, 99);

    tfp->open(tfpFileName.c_str());
    
    std::cout << "---\n Computer Test Running...\n";

    // Reset Logic
    computer->clk = 0;
    computer->r = 1;
    computer->eval(); // Start
    tfp->dump(tfpHalfCycle);
    
    Update(); // Posedge Clock Reset
    
    computer->r = 0;
    Update(); // Negedge Clock Reset
}

void CloseComputer() {
    std::cout << "Computer Test Finished\n";
    
    if (!tfpFinished) {
        tfp->close();
    }
    computer->final();

    delete tfp;
    delete computer;
    delete contextp;
}
