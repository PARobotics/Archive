function dat=analyze_move(dataName,IdxTest)
inFile  = [dataName,'.txt'];
datFile = [dataName,'.dat'];
dos(['grep  -iaE "MOVE_END|^W |^G " ' inFile ' | sed -e ''s/MOVE_END/99999 0 0 0/g'' |sed -e ''s/G //g''  |sed -e ''s/W //g''  > ' datFile]);

% load data and extra data combination

dat = load(datFile);
it = find(dat(:,1)==99999);
ntest = length(it);
if nargin<2; IdxTest=1:ntest; end
ls = {'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--','rd-','gd-','bd-','kd-','md-','yd-','cd-','k--'};
xrange = [0 1600];
figure;
for itest = IdxTest
  if (itest==1)
    id = 1:2:it(1)-1;
  else
    id = it(itest-1)+1:2:it(itest)-1;
  end
  t  = dat(id,1);
  v  = dat(id,2);
  w  = dat(id,3);
  ws = dat(id,4);
  g  = dat(id+1,3);
  gs = dat(id+1,4);
  
  subplot(3,2,1);box on; grid on; hold on; xlim(xrange); title('Volt'); xlim(xrange); 
  plot(t,v,ls{itest});
  subplot(3,2,3);box on; grid on; hold on; xlim(xrange); title('Wheel (0.1 inch)'); xlim(xrange); 
  plot(t,w,ls{itest});
  subplot(3,2,5);box on; grid on; hold on; xlabel('t');xlim(xrange); title('Wheel Speed (0.1 inch/second)'); xlim(xrange); 
  plot(t,ws,ls{itest});
  subplot(3,2,4);box on; grid on; hold on; xlabel('t');title('Gyro');xlim(xrange);
  plot(t,g,ls{itest});
  subplot(3,2,6);box on; grid on; hold on; xlabel('t');title('Gyro Speed');xlim(xrange);
  plot(t,gs,ls{itest});
  subplot(3,2,2);box on; grid on; hold on;   xlabel('Dist in .1 inch');   ylabel('speed in .1 inch');
  plot(w,ws,ls{itest});  
  % subplot(3,2,2);box on; grid on; hold on; xlim(xrange); title('Volt'); xlim(xrange); 
  % plot(t,v,ls{itest});
  % subplot(3,2,4);box on; grid on; hold on; xlim(xrange); title('gyro (0.1 deg)'); xlim(xrange); 
  % plot(t,g,ls{itest});
  % subplot(3,2,6);box on; grid on; hold on; xlabel('t'),xlim(xrange); title('gyro Speed (deg/second)'); xlim(xrange); 
  % plot(t,gs,ls{itest});
end