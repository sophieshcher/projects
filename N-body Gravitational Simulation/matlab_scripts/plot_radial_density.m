% Read trajectory data
traj = readtable(fullfile('../data/trajectory.csv'));

step_col = traj.step;
x_col    = traj.x;
y_col    = traj.y;

steps = unique(step_col);
first_step = steps(1);
last_step  = steps(end);

% Compute radial distances
r_init  = sqrt(x_col(step_col == first_step).^2 + y_col(step_col == first_step).^2);
r_final = sqrt(x_col(step_col == last_step).^2  + y_col(step_col == last_step).^2);

figure('Color', 'w', 'Position', [150, 150, 720, 480]);
histogram(r_init, 35, 'Normalization', 'pdf', 'FaceColor', [0.2 0.5 0.9], 'FaceAlpha', 0.5);
hold on;
histogram(r_final, 35, 'Normalization', 'pdf', 'FaceColor', [0.9 0.3 0.2], 'FaceAlpha', 0.6);
grid on; grid minor;
xlim([0, 2.5]);
xlabel('Radial Distance from Center, r', 'FontSize', 11, 'FontWeight', 'bold');
ylabel('Probability Density Function, PDF', 'FontSize', 11, 'FontWeight', 'bold');
title('Radial Density Profile: Initial Cloud vs Collapsed Core', 'FontSize', 13);
legend({sprintf('Initial State (Step %d)', first_step), ...
    sprintf('Virialized Core (Step %d)', last_step)}, 'Location', 'northeast');

if ~exist('plots', 'dir'), mkdir('plots'); end
exportgraphics(gcf, fullfile('../plots/radial_density_profile.png'), 'Resolution', 300);
fprintf('Radial density plot saved to plots/radial_density_profile.png\n');