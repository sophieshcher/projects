// benchmark.cpp
//
// Порівнює час ОДНОГО обчислення прискорень (не повної симуляції)
// для naive O(N^2) та Barnes-Hut O(N log N) на різних N.
// Обидва запускаються однопотоково (OMP_NUM_THREADS=1), щоб порівняння
// відображало саме алгоритмічну складність, а не ефект паралелізму —
// той окремо вимірюється через nbody_openmp.cpp.
//
// Результат — positions не потрібні, тільки CSV з часом виконання,
// щоб побудувати графік N vs час для обох алгоритмів і знайти точку
// перетину (crossover point), де Barnes-Hut починає вигравати.

#include <iostream>
#include <chrono>
#include <vector>
#include "nbody_barneshut_algo.h"

// Naive O(N^2) — однопотокова версія (та сама фізика, що й у
// nbody_sequential.cpp)
void computeAccelerationsNaive(std::vector<Particle>& particles) {
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

// Вимірює середній час одного виклику computeAccel (усереднено по REPEATS запусках)
template <typename Func>
double timeIt(Func computeAccel, std::vector<Particle> particles, int repeats) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < repeats; r++) {
        computeAccel(particles);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count() / repeats;
}

int main() {
    std::vector<int> testSizes = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};

    std::ofstream out("benchmark_results.csv");
    out << "N,naive_seconds,barneshut_seconds\n";

    std::cout << "N\tnaive (s)\tbarnes-hut (s)\n";

    for (int n : testSizes) {
        auto particles = makeRandomParticles(n);

        // Для великих N naive стає надто повільним для багатьох повторів —
        // адаптивно зменшуємо кількість повторів
        int repeats = (n <= 1000) ? 5 : 1;

        double tNaive = timeIt(computeAccelerationsNaive, particles, repeats);
        double tBH = timeIt(computeAccelerationsBarnesHut, particles, repeats);

        std::cout << n << "\t" << tNaive << "\t" << tBH << "\n";
        out << n << "," << tNaive << "," << tBH << "\n";

        // Якщо naive стає надто повільним (>5с на виклик) — далі не тестуємо,
        // щоб бенчмарк не тривав вічно; Barnes-Hut продовжуємо окремо не варто
        // ускладнювати, для навчального проєкту достатньо побаченого тренду
        if (tNaive > 5.0) {
            std::cout << "(naive став надто повільним, зупиняюсь)\n";
            break;
        }
    }

    return 0;
}
