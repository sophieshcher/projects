// nbody_sequential.cpp
// Класичний прямий розв'язок задачі $N$ тіл. Тут кожна частинка взаємодіє з кожною іншою, що дає асимптотику O(N^2). Він потрібен для перевірки точності та порівняння швидкодії з оптимізованими версіями.

#include <iostream>
#include <chrono>
#include "nbody_common.h"

void computeAccelerations(std::vector<Particle>& particles) {
    int n = static_cast<int>(particles.size());

    for (int i = 0; i < n; i++) {
        particles[i].ax = 0.0;
        particles[i].ay = 0.0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            double dx = particles[j].x - particles[i].x;
            double dy = particles[j].y - particles[i].y;
            double distSqr = dx * dx + dy * dy + EPS * EPS;
            double invDist3 = 1.0 / (distSqr * std::sqrt(distSqr));
            particles[i].ax += G * particles[j].mass * dx * invDist3;
            particles[i].ay += G * particles[j].mass * dy * invDist3;
        }
    }
}
//тут використовується повна матриця взаємодій $N \times N$, а не симетрія $\vec{F}_{ij} = -\vec{F}_{ji}$ (через $j > i$). Для послідовного коду це означає вдвічі більше обчислень, проте такий підхід суттєво легше переноситься на багатопотоковість без гонок даних (data races).

int main() {
    const int N = 1000;
    const int STEPS = 500;
    const double DT = 0.001;
    const int ENERGY_LOG_INTERVAL = 20;   

    auto particles = makeRandomParticles(N);

    computeAccelerations(particles);

    std::ofstream energyLog("./data/energy_sequential.csv");
    energyLog << "step,energy\n";
    energyLog << 0 << "," << totalEnergy(particles) << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (int s = 1; s <= STEPS; s++) {
        leapfrogStep(particles, DT, computeAccelerations);
        if (s % ENERGY_LOG_INTERVAL == 0) {
            energyLog << s << "," << totalEnergy(particles) << "\n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_sec = std::chrono::duration<double>(end - start).count();

    energyLog.flush();
    energyLog.close();

    savePositions(particles, "./data/positions_sequential.csv");

    std::cout << "[sequential] N=" << N << ", steps=" << STEPS
              << ", time=" << time_sec << " s" << std::endl;

    return 0;
}