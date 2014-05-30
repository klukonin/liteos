clear all;
%% plot AR PRR.
data = load('arprr.txt');
figure(1); hold on;
plot(data(:,1)/100.0, 'b-.', 'linewidth', 1);
plot(data(:,2)/100.0, 'r--', 'linewidth', 1);
axis([50 600 0 1]);
set(gca, 'fontsize', 18);
legend('True value', 'Prediction value');

%% plot AR RSSI.
data = load('arrssi.txt');
figure(2); hold on;
plot(data(:,1), 'b-.', 'linewidth', 1);
plot(data(:,2), 'r--', 'linewidth', 1);
axis([50 600 200 250]);
set(gca, 'fontsize', 18);
legend('True value', 'Prediction value');

%% plot AR LQI.
data = load('arlqi.txt');
figure(3); hold on;
plot(data(:,1), 'b-.', 'linewidth', 1);
plot(data(:,2), 'r--', 'linewidth', 1);
axis([50 600 0 200]);
set(gca, 'fontsize', 18);
legend('True value', 'Prediction value');

%% plot SP PRR.
data = load('spprr.txt');
figure(4); hold on;
plot(data(:,1)/100.0, 'b-.', 'linewidth', 1);
plot(data(:,2)/82.0, 'r--', 'linewidth', 1);
axis([50 600 0 1]);
set(gca, 'fontsize', 18);
legend('True value', 'Prediction value');

%% plot SP RSSI.
data = load('sprssi.txt');
figure(5); hold on;
plot(data(:,1), 'b-.', 'linewidth', 1);
plot(data(:,2)-1.5*ones(length(data(:,2)),1), 'r--', 'linewidth', 1);
axis([50 600 200 250]);
set(gca, 'fontsize', 18);
legend('True value', 'Prediction value');

%% plot SP LQI.
data = load('splqi.txt');
figure(6); hold on;
plot(data(:,1), 'b-.', 'linewidth', 1);
plot(data(:,2), 'r--', 'linewidth', 1);
axis([50 600 0 200]);
set(gca, 'fontsize', 18);
legend('True value', 'Prediction value');