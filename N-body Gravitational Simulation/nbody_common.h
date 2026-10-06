
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
// енергії ~25x, EPS=0.05 — дрейф ~0.4%). 
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

// Leapfrog (kick-drift-kick) — На відміну від стандартного методу Ейлера або Рунге-Кутти, Leapfrog зберігає структуру фазового простору гамільтонових систем. На практиці це означає, що повна енергія системи не накопичує похибку з часом (не дрейфує), а лише коливається навколо свого справжнього значення.
//Глобальна похибка методу зменшується пропорційно квадрату кроку за часом (\(O(\Delta t^2)\)).
//
// Схема:
//   v(t + dt/2) = v(t) + a(t) * dt/2        [kick]
//   x(t + dt)   = x(t) + v(t + dt/2) * dt   [drift]
//   ... перерахувати a(t+dt) ...
//   v(t + dt)   = v(t + dt/2) + a(t+dt) * dt/2   [kick]
//
// computeAccel — функція, яка рахує ax/ay для всіх частинок

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
// У замкненій системі величина $E$ має залишатися практично сталою. Її відхилення ($\Delta E / E_0$) слугує головним критерієм стабільності та точності симуляції.

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

//вивантажує координати частинок у форматі CSV (x,y,mass) для подальшої візуалізації (наприклад, через Python/Matplotlib для генерації анімації).

inline void savePositions(const std::vector<Particle>& particles, const std::string& filename) {
    FILE* f = std::fopen(filename.c_str(), "w");
    if (!f) {
        std::cerr << "Помилка відкриття файлу: " << filename << "\n";
        return;
    }
    std::fprintf(f, "x,y,mass\n");
    for (const auto& p : particles) {
        std::fprintf(f, "%.6f,%.6f,%.6f\n", p.x, p.y, p.mass);
    }
    std::fclose(f);
}


