#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

class CPU {
private:
	vector<bool> core_availability;
	int max_core;
	int num_cpu;
	int quantum_cycles;
	int batch_process_freq;
	int delays_per_exec;

public:
	CPU() {

	}

	void CpuReport() {

	}
};