function [p,E] = cal_motor_power(t,vcmd, ang,rpm);
% [p,E] = cal_motor_power(t,vcmd, ang,rpm);
% calculate the power (p) and Energy (E) delivered from the motor
% rpm, p, E are all normalized to single motor at 100 rpm (standard gear)
% 
% example: for 5 motors geared at 4.8
%  
% [p,E] = cal_motor_power(t,vcmd, ang,rpm/4.8);
% p = 5*p;
% E = 5*E;

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
E = t*0; p = t*0;
for istep=2:length(ang)
  % scale    = (vcmd(istep)+vcmd(istep-1))/2/127; 
  scale = 1;
  if (vcmd(istep)*rpm(istep)>=0)
    p(istep) = interp1(motor(:,2),motor(:,3),abs(rpm(istep)));   %average power of istep
    scale    = (vcmd(istep)+vcmd(istep-1))/2/127; 
    p(istep) = p(istep)*abs(scale);
  else
    % p(istep) = 0;
    p(istep) = interp1(motor(:,2),motor(:,3),abs(rpm(istep)));   %average power of istep
    scale    = (vcmd(istep)+vcmd(istep-1))/2/127; 
    p(istep) = -p(istep)*abs(scale);
  end
  % if isnan(p(istep)), p(istep) = 0; end
  E(istep) = E(istep-1)+p(istep)*(t(istep)-t(istep-1))/1000;
end
