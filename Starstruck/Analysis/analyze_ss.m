function [t_dmp,dmp,t_pos,pos] = analyze_ss(rawFile)
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

if (rawFile(end-3)=='.'); 
  testname=rawFile(1:end-4); 
else
  testname=rawFile;
end
lbl = sprintf('%s',testname);
datFile1 = [testname,'.pos'];       % pos time history data
% datFile2 = [testname,'.raw'];       % raw time history data
% datFile3 = [testname,'.lmk'];       % landmark position data
datFile4 = [testname,'.dmp'];       % landmark position data
datFile5 = [testname,'.lft'];       % landmark position data

% note that we are using dos command grep or egrep -- matlab has to be started from cygwin 
% this create a new file with only useful data in it
dos(['grep  -iaE "^W " ' rawFile '| gawk ''{if (NF==6) print $2,$3,$4,$5,$6} '' > ' datFile1]);
% dos(['grep  -iaE "^W " ' rawFile '| gawk ''{if (NF==5) print $2,$3,$4,$5} '' > ' datFile1]);
% dos(['grep  -iaE "^R " ' rawFile '| gawk ''{if (NF==5) print $2,$3,$4,$5} '' > ' datFile2]);
dos(['grep  -iaE "^D " ' rawFile '| gawk ''{if (NF==5) print $2,$3,$4,$5} '' > ' datFile4]);
dos(['grep  -iaE "^L " ' rawFile '| gawk ''{if (NF==4) print $2,$3,$4} '' > ' datFile5]);
% dos(['grep  -iaE "^K" ' rawFile '| gawk ''{if (NF==5) print $0} | sed -e ''s/K//g''  > ' datFile3]);
% dos(['grep  -iaE "LMK|^K" ' rawFile ]);

% load the data
pos = load(datFile1);
if (length(pos)>0)
pos = clean_data(pos,1000*ones(size(pos,2),1));    %clean the data 
pos(:,1) = pos(:,1)-pos(1,1);   % time from zero
t_pos = pos(:,1)/1000;          % sep time from data
pos   = pos(:,2:5);
figure(1)
  % plot the angles (from gyro, from raw yro, from raw wheel)
  subplot(3,1,1);box on; grid on; hold on; ylabel('Field X,Y','fontweight','b'); 
  title(lbl, 'interpreter','none','fontweight','b');
  plot(t_pos,pos(:,1)/10,'r+-');
  plot(t_pos,pos(:,2)/10,'b--p');
  legend({'x','y'});

  subplot(3,1,2);box on; grid on; hold on; ylabel('Field Angle (deg)','fontweight','b');
  plot(t_pos,pos(:,3)/10,'b-o');

  subplot(3,1,3);box on; grid on; hold on; ylabel('VCMD','fontweight','b');
  plot(t_pos,pos(:,4),'b-o');
  xlabel('TIME in sec','fontweight','b')
  posfig = get(0,'ScreenSize');
  set(gcf,'pos',[10 70 posfig(3)-20 posfig(4)-150]);
end
% raw = load(datFile2);
% lmk = load(datFile3);
dmp = load(datFile4);
if length(dmp) >0; 
  dmp = clean_data(dmp,[200,400,100,200]);    %clean the data 
  % dmp(:,1) = dmp(:,1)-dmp(1,1); 
  if length(pos)==0
    dmp(:,1) = dmp(:,1)-dmp(1,1); 
  else
    dmp(:,1) = dmp(:,1)-pos(1,1); 
  end

  t_dmp = dmp(:,1)/1000;          % sep time from data
  dmp = [dmp(:,3:end)/10 dmp(:,2)];            % in deg and rpm
  figure(2)
  box on; grid on; hold on; 
  plot(t_dmp,dmp(:,1),'b-p');
  plot(t_dmp,dmp(:,2)*3.125,'r-p');
  plot(t_dmp,dmp(:,3),'g-p');
  title(lbl, 'interpreter','none','fontweight','b');
  xlabel('TIME in sec','fontweight','b')
  legend({'dmp angle','dmp speed % of expected','motor power'})
  posfig = get(0,'ScreenSize');
  set(gcf,'pos',[10 70 posfig(3)-20 posfig(4)-150]);
end
% t_raw = raw(:,1)/1000;          % sep time from data
% raw   = raw(:,2:4);


% plot the pos time history data and verify using raw data
% convert raw to pos to verify pos results
if (0==1)
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
end   
   
   
lft = load(datFile5);
if (length(lft)>0)
% pos = clean_data(pos,1000*ones(size(pos,2),1));    %clean the data 
lft(:,1) = lft(:,1)-lft(1,1);   % time from zero
t_lft = lft(:,1)/1000;          % sep time from data
h  = lft(:,2)
v  = lft(:,3)
figure(3)
  % plot the angles (from gyro, from raw yro, from raw wheel)
  subplot(2,1,1);box on; grid on; hold on; ylabel('Lift h','fontweight','b'); 
  title(lbl, 'interpreter','none','fontweight','b');
  plot(t_lft,h,'r-+');

  subplot(2,1,2);box on; grid on; hold on; ylabel('Lift v','fontweight','b');
  plot(t_lft,v,'b-+');
  xlabel('TIME in sec','fontweight','b')
  posfig = get(0,'ScreenSize');
  set(gcf,'pos',[10 70 posfig(3)-20 posfig(4)-150]);
end
 


