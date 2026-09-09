// nbody_barneshut.cpp
// Основна програма Barnes-Hut симуляції. Сам алгоритм (квадродерево,
// обчислення сил) винесено в nbody_barneshut_algo.h — щоб той самий
// код можна було перевикористати в benchmark.cpp.

#include <iostream>
#include <chrono>
#include "nbody_barneshut_algo.h"

int main() {
    const int N = 2000;        // Barnes-Hut масштабується краще за naive —
                                // можна сміливо брати більше N
    const int STEPS = 500;
    const double DT = 0.001;
    const int ENERGY_LOG_INTERVAL = 20;

    auto particles = makeRandomParticles(N);
    computeAccelerationsBarnesHut(particles);

    std::ofstream energyLog("energy_barneshut.csv");
    energyLog << "step,energy\n";
    energyLog << 0 << "," << totalEnergy(particles) << "\n";

    // Траєкторії для анімації (Python matplotlib) — зберігаємо кожні
    // TRAJ_INTERVAL кроків, інакше файл буде надто великим
    const int TRAJ_INTERVAL = 5;
    std::ofstream trajLog("trajectory.csv");
    trajLog << "step,id,x,y\n";
    for (int i = 0; i < N; i++) {
        trajLog << 0 << "," << i << "," << particles[i].x << "," << particles[i].y << "\n";
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int s = 1; s <= STEPS; s++) {
        leapfrogStep(particles, DT, computeAccelerationsBarnesHut);
        if (s % ENERGY_LOG_INTERVAL == 0) {
            energyLog << s << "," << totalEnergy(particles) << "\n";
        }
        if (s % TRAJ_INTERVAL == 0) {
            for (int i = 0; i < N; i++) {
                trajLog << s << "," << i << "," << particles[i].x << "," << particles[i].y << "\n";
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "[barnes-hut] N=" << N << ", theta=" << THETA
              << ", steps=" << STEPS << ", time=" << elapsed.count() << " s\n";

    savePositions(particles, "positions_barneshut.csv");
    return 0;
}
