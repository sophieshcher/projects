% Read energy log files
seq_data = readtable('../data/energy_sequential.csv');
bh_data  = readtable('../data/energy_barneshut.csv');

% Initial total energy values
E0_seq = seq_data.energy(1);
E0_bh  = bh_data.energy(1);

% Relative energy error: dE = |E(t) - E0| / |E0|
rel_err_seq = abs((seq_data.energy - E0_seq) / E0_seq);
rel_err_bh  = abs((bh_data.energy  - E0_bh)  / E0_bh);

figure('Color', 'w', 'Position', [150, 150, 750, 500]);
plot(seq_data.step, rel_err_seq, 'r-', 'LineWidth', 1.8);
hold on;
plot(bh_data.step, rel_err_bh, 'b--', 'LineWidth', 1.8);
grid on;
grid minor;

xlabel('Simulation Step', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('|E(t) - E_0| / |E_0|', 'FontSize', 12, 'FontWeight', 'bold');
title('Total Energy Conservation (Leapfrog Drift)', 'FontSize', 14);
legend({'Direct Summation', 'Barnes-Hut (\theta = 0.5)'}, 'Location', 'northwest', 'FontSize', 11);

% Export figure
exportgraphics(gcf, '../plots/energy_conservation.png', 'Resolution', 300);
fprintf('Energy plot saved: energy_conservation.png\n');