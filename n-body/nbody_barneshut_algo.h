// nbody_barneshut_algo.h
//
// Barnes-Hut N-body алгоритм: O(N log N) замість O(N^2).
//
// Ідея: замість того, щоб для кожної частинки рахувати силу від УСІХ
// інших (O(N^2)), будуємо просторове дерево (у 2D — квадродерево):
// рекурсивно ділимо площину на квадранти, доки в кожному листі не
// залишиться одна частинка. Далі для кожної частинки обходимо дерево
// зверху вниз: якщо ціле піддерево виглядає як "маленька пляма" з її
// точки зору (s/d < theta, де s — розмір вузла, d — відстань до нього),
// рахуємо його вплив як ОДНЕ тіло (маса = сума, позиція = центр мас).
// theta — параметр точності: менше = точніше, але повільніше (0.5 —
// типове значення).
//
// Винесено в окремий заголовок, щоб перевикористовувати і в основній
// програмі (nbody_barneshut.cpp), і в бенчмарку (benchmark.cpp).

#pragma once
#include <algorithm>
#include <limits>
#include "nbody_common.h"

const double THETA = 0.5;      // критерій розкриття вузла (0.5 — типове значення)
const int MAX_DEPTH = 40;      // запобіжник від нескінченної рекурсії при майже
                                // однакових позиціях частинок (на практиці з
                                // випадковими даними майже ніколи не спрацьовує)

struct QuadNode {
    double cx, cy, halfSize;               // центр і половина розміру квадрата
    double mass = 0.0;
    double comX = 0.0, comY = 0.0;         // центр мас піддерева
    int particleIndex = -1;                // індекс частинки, якщо це лист з однією частинкою
    bool isLeaf = true;
    bool isEmpty = true;
    QuadNode* children[4] = {nullptr, nullptr, nullptr, nullptr};

    QuadNode(double cx_, double cy_, double halfSize_)
        : cx(cx_), cy(cy_), halfSize(halfSize_) {}

    ~QuadNode() {
        for (auto c : children) delete c;
    }

    // Визначає, в якому з 4 квадрантів лежить точка (0=NE, 1=NW, 2=SW, 3=SE)
    int getQuadrant(double x, double y) const {
        if (x >= cx) return (y >= cy) ? 0 : 3;
        else         return (y >= cy) ? 1 : 2;
    }

    void subdivide() {
        double h = halfSize / 2.0;
        children[0] = new QuadNode(cx + h, cy + h, h);  // NE
        children[1] = new QuadNode(cx - h, cy + h, h);  // NW
        children[2] = new QuadNode(cx - h, cy - h, h);  // SW
        children[3] = new QuadNode(cx + h, cy - h, h);  // SE
    }

    void insert(int idx, const std::vector<Particle>& particles, int depth = 0) {
        const Particle& p = particles[idx];

        if (isEmpty) {
            particleIndex = idx;
            isLeaf = true;
            isEmpty = false;
            mass = p.mass;
            comX = p.x;
            comY = p.y;
            return;
        }

        if (isLeaf) {
            if (depth > MAX_DEPTH) {
                // Запобіжник: майже співпадаючі позиції — просто зливаємо маси,
                // не намагаючись ділити простір нескінченно глибоко.
                double newMass = mass + p.mass;
                comX = (comX * mass + p.x * p.mass) / newMass;
                comY = (comY * mass + p.y * p.mass) / newMass;
                mass = newMass;
                return;
            }
            // Лист вже зайнятий іншою частинкою — ділимо вузол і переносимо
            // стару частинку в дочірній квадрант
            int oldIdx = particleIndex;
            subdivide();
            isLeaf = false;
            particleIndex = -1;
            int qOld = getQuadrant(particles[oldIdx].x, particles[oldIdx].y);
            children[qOld]->insert(oldIdx, particles, depth + 1);
        }

        // Вузол тепер внутрішній — вставляємо нову частинку у відповідний квадрант
        int q = getQuadrant(p.x, p.y);
        children[q]->insert(idx, particles, depth + 1);

        // Оновлюємо сукупну масу й центр мас цього вузла (інкрементальне
        // зважене середнє — коректно незалежно від того, скільки частинок
        // вже було додано раніше)
        double newMass = mass + p.mass;
        comX = (comX * mass + p.x * p.mass) / newMass;
        comY = (comY * mass + p.y * p.mass) / newMass;
        mass = newMass;
    }

    // Рекурсивно обходить дерево й накопичує прискорення частинки idx
    void computeForce(int idx, const std::vector<Particle>& particles,
                       double& ax, double& ay) const {
        if (isEmpty) return;

        if (isLeaf) {
            if (particleIndex == idx) return;   // не рахуємо силу від самої себе
            double dx = comX - particles[idx].x;
            double dy = comY - particles[idx].y;
            double distSqr = dx * dx + dy * dy + EPS * EPS;
            double invDist3 = 1.0 / (distSqr * std::sqrt(distSqr));
            ax += G * mass * dx * invDist3;
            ay += G * mass * dy * invDist3;
            return;
        }

        double dx = comX - particles[idx].x;
        double dy = comY - particles[idx].y;
        double distSqr = dx * dx + dy * dy + EPS * EPS;
        double dist = std::sqrt(distSqr);
        double s = halfSize * 2.0;   // розмір вузла

        if (s / dist < THETA) {
            // Вузол достатньо далеко й компактний — трактуємо як одне тіло
            double invDist3 = 1.0 / (distSqr * dist);
            ax += G * mass * dx * invDist3;
            ay += G * mass * dy * invDist3;
        } else {
            // Занадто близько/велике — заходимо всередину
            for (auto c : children) {
                if (c) c->computeForce(idx, particles, ax, ay);
            }
        }
    }
};

// Знаходить квадратний bounding box, що містить усі частинки (з невеликим запасом)
void computeBounds(const std::vector<Particle>& particles, double& cx, double& cy, double& halfSize) {
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (auto& p : particles) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }

    cx = (minX + maxX) / 2.0;
    cy = (minY + maxY) / 2.0;
    halfSize = std::max(maxX - minX, maxY - minY) / 2.0 * 1.001 + 1e-6;  // невеликий запас
}

void computeAccelerationsBarnesHut(std::vector<Particle>& particles) {
    double cx, cy, halfSize;
    computeBounds(particles, cx, cy, halfSize);

    QuadNode root(cx, cy, halfSize);
    int n = static_cast<int>(particles.size());
    for (int i = 0; i < n; i++) {
        root.insert(i, particles);
    }

    // Дерево вже побудоване (спільне, тільки читаємо) — обчислення сили
    // для кожної частинки незалежне, тож можна безпечно розпаралелити.
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < n; i++) {
        double ax = 0.0, ay = 0.0;
        root.computeForce(i, particles, ax, ay);
        particles[i].ax = ax;
        particles[i].ay = ay;
    }
}
