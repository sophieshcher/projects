% Number of threads and measured execution times (s)
threads = [1, 2, 4];
% Replace with your measured values if needed
times   = [2.22, 1.18, 0.65]; 

speedup    = times(1) ./ times;
efficiency = speedup ./ threads;

figure('Color', 'w', 'Position', [100, 100, 850, 380]);

% Subplot 1: Speedup
subplot(1, 2, 1);
plot(threads, speedup, 'b-o', 'LineWidth', 2, 'MarkerSize', 6, 'MarkerFaceColor', 'b');
hold on;
plot(threads, threads, 'k--', 'LineWidth', 1.2);
grid on; grid minor;
xlabel('Number of Threads, p', 'FontSize', 11, 'FontWeight', 'bold');
ylabel('Speedup, S(p)', 'FontSize', 11, 'FontWeight', 'bold');
title('Parallel Speedup (OpenMP)', 'FontSize', 12);
legend({'Measured Speedup', 'Ideal Linear'}, 'Location', 'northwest');

% Subplot 2: Parallel Efficiency
subplot(1, 2, 2);
plot(threads, efficiency * 100, 'r-s', 'LineWidth', 2, 'MarkerSize', 6, 'MarkerFaceColor', 'r');
hold on;
yline(100, 'k--', 'LineWidth', 1.2);
ylim([40, 110]);
grid on; grid minor;
xlabel('Number of Threads, p', 'FontSize', 11, 'FontWeight', 'bold');
ylabel('Parallel Efficiency, E(p) [%]', 'FontSize', 11, 'FontWeight', 'bold');
title('Parallel Efficiency (OpenMP)', 'FontSize', 12);

if ~exist('plots', 'dir'), mkdir('plots'); end
exportgraphics(gcf, fullfile('../plots/openmp_scaling.png'), 'Resolution', 300);
fprintf('OpenMP scaling plot saved to plots/openmp_scaling.png\n');