// nbody_sequential.cpp
// Наївна O(N^2) N-body симуляція. Leapfrog-інтегратор + трекінг енергії
// для перевірки фізичної коректності.

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

int main() {
    const int N = 1000;
    const int STEPS = 500;
    const double DT = 0.001;
    const int ENERGY_LOG_INTERVAL = 20;   // рахувати енергію не щокроку — дорого

    auto particles = makeRandomParticles(N);

    // початкове прискорення потрібне перед першим leapfrog-кроком
    computeAccelerations(particles);

    std::ofstream energyLog("energy_sequential.csv");
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
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "[sequential] N=" << N << ", steps=" << STEPS
              << ", time=" << elapsed.count() << " s\n";

    savePositions(particles, "positions_sequential.csv");
    return 0;
}
