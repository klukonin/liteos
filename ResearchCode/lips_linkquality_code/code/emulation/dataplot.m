% figure(1);
% plot(1:length(iris_rssi), iris_rssi(:,1), 'xb');
% axis([0 580000 0 260]);
% set(gca, 'fontsize', 14);
% xlabel('packets');
% ylabel('RSSI value');


% %%
% d_micaz_rssi = abs(micaz_rssi(10:length(micaz_rssi)-1,2) - micaz_rssi(11:length(micaz_rssi),1));
% d_micaz_lqi = abs(micaz_lqi(10:length(micaz_lqi)-1,2) - micaz_lqi(11:length(micaz_lqi),1));
% d_micaz_prr = abs(micaz_prr(10:length(micaz_prr)-1,2) - micaz_prr(11:length(micaz_prr),1))*100.0;
% 
% [cdf_micaz_rssi, x_m_r] = ecdf(d_micaz_rssi); 
% [cdf_micaz_prr, x_m_p] = ecdf(d_micaz_prr);
% [cdf_micaz_lqi, x_m_l] = ecdf(d_micaz_lqi);
% 
% figure(2); hold on;
% plot(x_m_r, cdf_micaz_rssi, 'r-', 'linewidth', 2);
% plot(x_m_l, cdf_micaz_lqi, 'b--', 'linewidth', 2);
% plot(x_m_p, cdf_micaz_prr, 'k-.', 'linewidth', 2);
% axis([0 10 0 1]);
% grid on;
% set(gca, 'fontsize', 14);
% xlabel('prediction error');
% ylabel('percentage');
% legend('RSSI', 'LQI', 'PRR (%)');

% %%
% d_iris_rssi = abs(iris_rssi(10:length(iris_rssi)-1,2) - iris_rssi(11:length(iris_rssi),1));
% d_iris_prr = abs(iris_prr(10:length(iris_prr)-1,2) - iris_prr(11:length(iris_prr),1))*100.0;
% 
% [cdf_iris_rssi, x_m_r] = ecdf(d_iris_rssi);
% [cdf_iris_prr, x_m_p] = ecdf(d_iris_prr);
% 
% figure(3); hold on;
% plot(x_m_r, cdf_iris_rssi, 'r-', 'linewidth', 2);
% plot(x_m_p, cdf_iris_prr, 'k--', 'linewidth', 2);
% axis([0 150 0 1]);
% grid on;
% set(gca, 'fontsize', 14);
% xlabel('prediction error');
% ylabel('percentage');
% legend('RSSI', 'PRR (%)');

%%
d_micaz_rssi = abs(micaz_rssi(10:length(micaz_rssi)-1,2) - micaz_rssi(11:length(micaz_rssi),1));
d_micaz_lqi = abs(micaz_lqi(10:length(micaz_lqi)-1,2) - micaz_lqi(11:length(micaz_lqi),1));
d_micaz_prr = abs(micaz_prr(10:length(micaz_prr)-1,2) - micaz_prr(11:length(micaz_prr),1))*100.0;

[cdf_micaz_rssi, x_m_r] = ecdf(d_micaz_rssi); 
[cdf_micaz_prr, x_m_p] = ecdf(d_micaz_prr);
[cdf_micaz_lqi, x_m_l] = ecdf(d_micaz_lqi);
d_iris_rssi = abs(iris_rssi(10:length(iris_rssi)-1,2) - iris_rssi(11:length(iris_rssi),1));
d_iris_prr = abs(iris_prr(10:length(iris_prr)-1,2) - iris_prr(11:length(iris_prr),1))*100.0;

[cdf_iris_rssi, x_i_r] = ecdf(d_iris_rssi);
[cdf_iris_prr, x_i_p] = ecdf(d_iris_prr);

figure(4); hold on;
plot(x_m_r, cdf_micaz_rssi, 'r-', 'linewidth', 2);
plot(x_m_l, cdf_micaz_lqi, 'b--', 'linewidth', 2);
plot(x_m_p, cdf_micaz_prr, 'k-.', 'linewidth', 2);
plot(x_i_r, cdf_iris_rssi, 'r-.', 'linewidth', 2);
plot(x_i_p, cdf_iris_prr, 'k--', 'linewidth', 2);
axis([0 200 0 1]);