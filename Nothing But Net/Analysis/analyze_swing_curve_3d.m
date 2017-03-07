function analyze_swing_curve_3D(GR, rl, K,V,kbrake);
%GR: gear ratio 3 or 4.8
%rl: lenght of rubber band arm in inch (1.5-2.5)
%K:  rubber stiffness ( somewhere between 5-10)
%V:  voltage 100-127
%kbrake: coeffcient of braking when motor speed goes above limit (between 0-10 depend on v_brake)
% example
%   close all; clear all; analyze_swing_curve(3, 2.25, 5,127,5)  % my best guess of our current system
%   close all; clear all; analyze_swing_curve(4.8, 1.5, 6,127,5)  % 4.8 gear ratio with reduced RB arm

% default 
% GR = 4.8; 
% K = 10;
% rl = 2;
% V = 127;
rl = rl*.0254;
W = 1.*.0254;
scale  = V/127;
% kbrake = 5;

motor= [
  1.67	0.00	0.00
  1.59	5.00	0.83
  1.50	10.00	1.57
  1.42	15.00	2.23
  1.34	20.00	2.80
  1.25	25.00	3.28
  1.17	30.00	3.67
  1.09	35.00	3.98
  1.00	40.00	4.20
  0.92	45.00	4.33
  0.84	50.00	4.37
  0.75	55.00	4.33
  0.67	60.00	4.20
  0.58	65.00	3.98
  0.50	70.00	3.67
  0.42	75.00	3.28
  0.33	80.00	2.80
  0.25	85.00	2.23
  0.17	90.00	1.57
  0.08	95.00	0.83
  0.00	100.00	0.00
];                         % torque rpm power curve
nMotor = 5;
m      = 0.15;            % bat weight
L      = 7*0.0254;        % bat length
CM_F   = 0.4;             % between 1/3 to 1
J      = CM_F*m*L^2;
Angle  = [200:-1:-30]';
N      = length(Angle);
E      = zeros(N,1);
E1     = zeros(N,1);
E2     = zeros(N,1);
RPM    = zeros(N,1);
RPM(1) = 10.00;
t      = zeros(N,1);
KE_ratio = 0.8;
% KE_ratio = 1.0;
d      = zeros(N,1);
F      = zeros(N,1);
M      = zeros(N,1);
O      =   [rl 0 0];
for i=2:N
  if (RPM(i-1)/GR>100*scale && i>3) 
    dRPM = RPM(i-1)/GR-100;
    % p    = -ks*dRPM^(-0.5);
    p    = -kbrake*(RPM(i-1)-RPM(i-2))/GR;
  else
    p      = interp1(motor(:,2)*scale,motor(:,3)*scale,RPM(i-1)/GR); % average power of istep
  end
  dt     = abs(Angle(i)-Angle(i-1))/360/RPM(i-1)*60; 
  t(i)   = t(i-1)+dt*1000;                                            % in msec
  
  % rb
  RB_P0 = [rl 0 W/2];
  a      = (Angle(i))*pi/180;
  Onew = rl*[cos(a) sin(a) 0];
  beta = atan(rl*sin(a)/W*2);
  d(i)    = norm(Onew-RB_P0)*2-W;
  F(i)    = K*d(i)*sin(beta);
  torque  = -F(i)*cross(Onew-O, [0 0 1]);
  M(i)    = torque(1);
  
  % power input
  E1(i) = E1(i-1)+max(M(i)*(Angle(i)-Angle(i-1)),0);
  E2(i) = E2(i-1)+p*dt*nMotor*KE_ratio;
  E(i) = E1(i)+E2(i);
  omega  = sqrt(E(i)*2/J);
  RPM(i) = omega*60/2/pi;
end

% post processing
ihit = min(find(Angle<-10));
RPM(isnan(RPM)) = GR*100*scale;
max(RPM)
Angle = -(Angle-180);

pos = get(0,'ScreenSize');
figure(1); set(gcf,'pos',[10 20 pos(3)/2-10 pos(4)-20]);
subplot(2,2,1); hold on; grid on; box on; xlabel('Time in msec');ylabel('RPM');
plot(t,RPM,'b.'); plot(t(ihit),RPM(ihit),'rd')
title(sprintf('RPM_m_a_x=%.1f',max(RPM)));
subplot(2,2,3); hold on; grid on; box on;  xlabel('Time in msec'); ylabel('Angle in degree'); 
plot(t,Angle,'b.');plot(t(ihit),Angle(ihit),'rd');
subplot(2,2,2); hold on; grid on; box on;  xlabel('Angle in degree'); ylabel('RPM')
plot(Angle,RPM,'b.');plot(Angle(ihit),RPM(ihit),'rd');
subplot(2,2,4); hold on; grid on; box on; xlabel('Angle'); ylabel('RB Torque in N.m')
plot(Angle,-M*180/pi,'b.');plot(Angle(ihit),-M(ihit)*180/pi,'rd');

figure(2); hold on; grid on; box on; xlabel('Angle');ylabel('E in jouel');
title(sprintf('GR=%.1f nMotor=%d K_r_b=%d L_r_b=%.1f" V=%d k_b_r_a_k_e=%d',GR,nMotor,K,rl/.0254,V,kbrake));
set(gcf,'pos',[pos(3)/2+10 20 pos(3)/2-10 pos(4)-20]);
plot(Angle,[E,E1,E2]);
plot(Angle(ihit),[E(ihit),E1(ihit),E2(ihit)],'d');
legend('Total','from RB','from Motor');