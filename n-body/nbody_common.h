// nbody_common.h
// Спільні структури та допоміжні функції для всіх версій симуляції
// (naive O(N^2), OpenMP, Barnes-Hut). Тримаємо в одному місці, щоб
// фізика (leapfrog, енергія) була ідентична в усіх версіях —
// це критично для чесного порівняння продуктивності.

#pragma once
#include <vector>
#include <cmath>
#include <fstream>
#include <random>

struct Particle {
    double x, y;      // позиція
    double vx, vy;    // швидкість
    double ax, ay;    // прискорення
    double mass;
};

const double G = 1.0;
// Softening length. Значення 0.05 обране не довільно: при малому EPS
// (напр. 1e-3) частинки, що стартують у стані спокою, під час "холодного
// колапсу" на початку симуляції проходять надто близькі зближення, які
// фіксований крок часу (DT) не встигає коректно відпрацювати — це вносить
// штучну енергію в систему (перевірено емпірично: EPS=1e-3 давав дрейф
// енергії ~25x, EPS=0.05 — дрейф ~0.4%). Це стандартна проблема "cold
// collapse" у N-body симуляціях з фіксованим кроком часу.
const double EPS = 0.05;

// Генерує N частинок у випадкових позиціях в межах [-1, 1] x [-1, 1]
inline std::vector<Particle> makeRandomParticles(int n, unsigned seed = 42) {
    std::vector<Particle> particles(n);
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> pos_dist(-1.0, 1.0);
    std::uniform_real_distribution<double> mass_dist(0.5, 2.0);

    for (auto& p : particles) {
        p.x = pos_dist(rng);
        p.y = pos_dist(rng);
        p.vx = 0.0;
        p.vy = 0.0;
        p.ax = 0.0;
        p.ay = 0.0;
        p.mass = mass_dist(rng);
    }
    return particles;
}

// Leapfrog (kick-drift-kick) — на відміну від Ейлера, симплектичний
// метод: енергія коливається навколо сталого значення, а не "втікає"
// монотонно. Стандарт для N-body симуляцій у реальних дослідженнях.
//
// Схема:
//   v(t + dt/2) = v(t) + a(t) * dt/2        [kick]
//   x(t + dt)   = x(t) + v(t + dt/2) * dt   [drift]
//   ... перерахувати a(t+dt) ...
//   v(t + dt)   = v(t + dt/2) + a(t+dt) * dt/2   [kick]
//
// computeAccel — функція, яка рахує ax/ay для всіх частинок
// (передається ззовні, бо в naive версії й у Barnes-Hut вона різна).
template <typename AccelFunc>
void leapfrogStep(std::vector<Particle>& particles, double dt, AccelFunc computeAccel) {
    int n = static_cast<int>(particles.size());

    // half-kick
    for (int i = 0; i < n; i++) {
        particles[i].vx += 0.5 * particles[i].ax * dt;
        particles[i].vy += 0.5 * particles[i].ay * dt;
    }
    // drift
    for (int i = 0; i < n; i++) {
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
    }
    // перерахувати прискорення в нових позиціях
    computeAccel(particles);
    // half-kick
    for (int i = 0; i < n; i++) {
        particles[i].vx += 0.5 * particles[i].ax * dt;
        particles[i].vy += 0.5 * particles[i].ay * dt;
    }
}

// Повна енергія системи: кінетична + потенціальна.
// Має лишатись приблизно сталою протягом симуляції — це стандартна
// перевірка коректності N-body коду (незалежно від алгоритму
// обчислення сил).
inline double totalEnergy(const std::vector<Particle>& particles) {
    int n = static_cast<int>(particles.size());
    double kinetic = 0.0;
    double potential = 0.0;

    for (int i = 0; i < n; i++) {
        double v2 = particles[i].vx * particles[i].vx + particles[i].vy * particles[i].vy;
        kinetic += 0.5 * particles[i].mass * v2;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dx = particles[j].x - particles[i].x;
            double dy = particles[j].y - particles[i].y;
            double dist = std::sqrt(dx * dx + dy * dy + EPS * EPS);
            potential -= G * particles[i].mass * particles[j].mass / dist;
        }
    }

    return kinetic + potential;
}

inline void savePositions(const std::vector<Particle>& particles, const std::string& filename) {
    std::ofstream out(filename);
    out << "x,y,mass\n";
    for (auto& p : particles) {
        out << p.x << "," << p.y << "," << p.mass << "\n";
    }
}
