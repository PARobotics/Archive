function [t_pos,pos,raw] = analyze_autoa(rawFile)
%
% clear all, close all, analyze(dataName,ITEST, xrange,isFilter)
%   rawFile: the filename of the raw file coped from debugStream window of RobotC

%  1. debug stream is written by the following -- write out both pos and raw data every 20 msec
%  writeDebugStreamLine("W %4d %3d %4d %4d", tnow - wheel_t0, DRV.Pos[0],DRV.Pos[1],DRV.Pos[2]);
%  writeDebugStreamLine("R %4d %3d %4d %4d", tnow - wheel_t0, DRV.raw[0],DRV.raw[1],DRV.raw[2]);
% 2. also write out landmark data to allow tracking landmarks
%   void output_pos(int lmk_id) {
%     writeDebugStreamLine("K %4d %3d %4d %4d", time1[T1] - wheel_t0, DRV.Pos[0],DRV.Pos[1],DRV.Pos[2]);
%  }
%
% we will use these data to 
%  (1) test how accurate we are able to track the drive
%  (2) track landmark positions for fine adjusment

% default line style in case it is not given
% if nargin <2, ls = 'r-+'; end
% rawFile = 'auto_a_2.txt';
ls = 'b-';

chk_dmp = 1;

if (rawFile(end-3)=='.'); 
  testname=rawFile(1:end-4); 
else
  testname=rawFile;
end
lbl = sprintf('%s',testname);
datFile1 = [testname,'.pos'];       % pos time history data
datFile2 = [testname,'.raw'];       % raw time history data
datFile3 = [testname,'.lmk'];       % landmark position data
datFile4 = [testname,'.dmp'];       % landmark position data
datFile1

% note that we are using dos command grep or egrep -- matlab has to be started from cygwin 
% this create a new file with only useful data in it
dos(['grep  -iaE "^W " ' rawFile '| gawk ''{if (NF==5) print $2,$3,$4,$5} '' > ' datFile1]);
dos(['grep  -iaE "^R " ' rawFile '| gawk ''{if (NF==5) print $2,$3,$4,$5} '' > ' datFile2]);
dos(['grep  -iaE "^D " ' rawFile '| gawk ''{if (NF==5) print $2,$3,$4,$5} '' > ' datFile4]);
dos(['grep  -iaE "^K" ' rawFile '| gawk ''{if (NF==5) print $0} | sed -e ''s/K//g''  > ' datFile3]);
dos(['grep  -iaE "LMK|^K" ' rawFile ]);

% load the data
pos = load(datFile1);
raw = load(datFile2);
lmk = load(datFile3);
dmp = load(datFile4);
if length(dmp) == 0; chk_dmp=0; end
pos(:,1) = pos(:,1)-pos(1,1);   % time from zero
raw(:,1) = raw(:,1)-pos(1,1);
lmk(:,1) = lmk(:,1)-pos(1,1);
if (chk_dmp==1)
  dmp(:,1) = dmp(:,1)-pos(1,1); 
  t_dmp = dmp(:,1)/1000;          % sep time from data
  dmp = dmp(:,3:4)/10;            % in deg and rpm
end
t_pos = pos(:,1)/1000;          % sep time from data
pos   = pos(:,2:4);
t_raw = raw(:,1)/1000;          % sep time from data
raw   = raw(:,2:4);


% plot the pos time history data and verify using raw data
% convert raw to pos to verify pos results
tick2inch     = 4 * pi/36.;     % tick to 0.1 inch
pi2d          = 180/pi*10;  
DX            = 140;            % 12" between the wheel
pos_raw = 0*raw;
pos_raw(1,:) = pos(1,:);        % initial position from pos
pos_raw(1,4:6) = pos_raw(1,1:3);
draw = diff(raw,1);             % change in raw in every increment
dt   = diff(t_raw);
speed = zeros(length(t_raw),2);
for it = 1:length(t_raw)-1
  % cal left and right wheel dist increment
  dw_l = draw(it,1)*tick2inch;
  dw_r = draw(it,2)*tick2inch;
  dd = (dw_l+dw_r)/2;
  
  % cal angle increment fom gyro and from wheel and update angle
  dang = draw(it,3);
  dang2 = (dw_l-dw_r)/DX * 180/pi*10;      % estimate us left and right wheel disp
  
  pos_raw(it+1,3) = pos_raw(it,3)+dang;
  pos_raw(it+1,6) = pos_raw(it,6)+dang2;

  % calculate position increment and update position  
  s    = sin(pos_raw(it+1,3)/pi2d);
  c    = cos(pos_raw(it+1,3)/pi2d);
  s2   = sin(pos_raw(it+1,6)/pi2d);
  c2   = cos(pos_raw(it+1,6)/pi2d);  
%  s    = sin(pos_raw(it,3)/pi2d);
%  c    = cos(pos_raw(it,3)/pi2d);
  pos_raw(it+1,1) = pos_raw(it,1) + dd*c;
  pos_raw(it+1,2) = pos_raw(it,2) + dd*s;
  pos_raw(it+1,4) = pos_raw(it,4) + dd*c2;
  pos_raw(it+1,5) = pos_raw(it,5) + dd*s2;  
  % calculate speed
  speed(it,1) = dd/dt(it)/10;              % in inch per second
  speed(it,2) = dang/dt(it)/10;         % in degree / second
 end
 
 
figure(1)
% plot the angles (from gyro, from raw yro, from raw wheel)
subplot(2,1,1);box on; grid on; hold on; ylabel('Field X,Y','fontweight','b'); 
title(lbl, 'interpreter','none','fontweight','b');
plot(t_pos,pos(:,1)/10,'b-');
plot(t_raw, pos_raw(:,1)/10,'r-');
plot(t_raw, pos_raw(:,4)/10,'g-');
plot(t_pos,pos(:,2)/10,'b--');
plot(t_raw, pos_raw(:,2)/10,'r--');
plot(t_raw, pos_raw(:,5)/10,'g--');
legend({'Pos[1]','x-raw-gyro','x-raw-wheel','Pos[2]','y-raw-gyro','y-raw-wheel'});

subplot(2,1,2);box on; grid on; hold on; ylabel('Field Angle (deg)','fontweight','b');
plot(t_pos,pos(:,3)/10,'b-');
plot(t_raw, pos_raw(:,3)/10,'r-');
plot(t_raw, pos_raw(:,6)/10,'g-');
legend({'Pos[3]','gyro-raw','wheel-raw'})
xlabel('TIME in sec','fontweight','b')


if (chk_dmp==1)
figure(3)
  subplot(2,1,1);box on; grid on; hold on; 
  plot(t_dmp,dmp(:,1));
  ylabel('Dumper angle deg','fontweight','b')
  subplot(2,1,2);box on; grid on; hold on; 
  plot(t_dmp,dmp(:,2)*3.125);
  xlabel('TIME in sec','fontweight','b')
  ylabel('Dumper speed % wrt 3.125GR','fontweight','b')
end

posfig = get(0,'ScreenSize');
set(gcf,'pos',[10 70 posfig(3)-20 posfig(4)-150]);

figure(2);
subplot(2,1,1);box on;  grid on; hold on;
plot(t_raw, speed(:,1));
ylabel('speed inch/sec','fontweight','b')
title(lbl, 'interpreter','none','fontweight','b');

subplot(2,1,2);box on;  grid on; hold on;
plot(t_raw, speed(:,2));
xlabel('TIME in sec','fontweight','b')
ylabel('ang speed deg/sec','fontweight','b')
set(gcf,'pos',[10 70 posfig(3)-20 posfig(4)-150]);


%print out market data


