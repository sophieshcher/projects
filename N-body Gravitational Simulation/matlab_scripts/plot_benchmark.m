% Read benchmark results
data = readtable('../data/benchmark_results.csv');

figure('Color', 'w', 'Position', [100, 100, 750, 500]);
h1 = loglog(data.N, data.naive_seconds, 'r-o', 'LineWidth', 2, 'MarkerSize', 6, 'MarkerFaceColor', 'r');
hold on;
h2 = loglog(data.N, data.barneshut_seconds, 'b-s', 'LineWidth', 2, 'MarkerSize', 6, 'MarkerFaceColor', 'b');
grid on;
grid minor;

% Theoretical scaling lines
N_ref = linspace(min(data.N), max(data.N), 100);
C_naive = data.naive_seconds(end) / (data.N(end)^2);
C_bh = data.barneshut_seconds(end) / (data.N(end) * log2(data.N(end)));

loglog(N_ref, C_naive * N_ref.^2, 'r--', 'LineWidth', 1.2);
loglog(N_ref, C_bh * (N_ref .* log2(N_ref)), 'b--', 'LineWidth', 1.2);

xlabel('Number of Particles, N', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Time per Step (s)', 'FontSize', 12, 'FontWeight', 'bold');
title('Scalability: Direct O(N^2) vs Barnes-Hut O(N log N)', 'FontSize', 14);
legend([h1, h2], {'Direct O(N^2)', 'Barnes-Hut O(N log N)'}, 'Location', 'northwest', 'FontSize', 11);

% Export figure
exportgraphics(gcf, '../plots/benchmark_scaling.png', 'Resolution', 300);
fprintf('Benchmark plot saved: benchmark_scaling.png\n');