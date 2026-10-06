// Та сама O(N^2) модель, що й nbody_sequential.cpp, але з
// розпаралеленим обчисленням прискорень через OpenMP.

#include <iostream>
#include <chrono>
#include <omp.h>
#include "nbody_common.h"

//# Запуск на 1 потоці (для прямого порівняння з nbody_sequential)
//OMP_NUM_THREADS=1 ./nbody_omp

//# Запуск на 4 або 8 потоках
//OMP_NUM_THREADS=4 ./nbody_omp
//OMP_NUM_THREADS=8 ./nbody_omp

void computeAccelerations(std::vector<Particle>& particles) {
    int n = static_cast<int>(particles.size());

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        double ax = 0.0, ay = 0.0;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            double dx = particles[j].x - particles[i].x;
            double dy = particles[j].y - particles[i].y;
            double distSqr = dx * dx + dy * dy + EPS * EPS;
            double invDist3 = 1.0 / (distSqr * std::sqrt(distSqr));
            ax += G * particles[j].mass * dx * invDist3;
            ay += G * particles[j].mass * dy * invDist3;
        }
        particles[i].ax = ax;
        particles[i].ay = ay;
    }
}

int main() {
    const int N = 1000;
    const int STEPS = 500;
    const double DT = 0.001;
    const int ENERGY_LOG_INTERVAL = 20;

    auto particles = makeRandomParticles(N);
    computeAccelerations(particles);

    std::ofstream energyLog("./data/energy_openmp.csv");
    energyLog << "step,energy\n";
    energyLog << 0 << "," << totalEnergy(particles) << "\n";

//omp_get_max_threads(): виводить максимальну кількість апаратних потоків (логічних ядер), які за замовчуванням задіює рантайм.

    std::cout << "OpenMP threads available: " << omp_get_max_threads() << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (int s = 1; s <= STEPS; s++) {
        leapfrogStep(particles, DT, computeAccelerations);
        if (s % ENERGY_LOG_INTERVAL == 0) {
            energyLog << s << "," << totalEnergy(particles) << " \n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "[openmp] N=" << N << ", steps=" << STEPS
              << ", time=" << elapsed.count() << " s";

    savePositions(particles, "./data/positions_openmp.csv");
    return 0;
}
