function [para,d,res] = analyze(dataName,ITEST, xrange,isFilter)
%
% clear all, close all, analyze(dataName,ITEST, xrange,isFilter)
% clear all, close all, analyze(dataName,'', [0 800],1)
%   dataName: the file name (in data folder ) w/o extension
%   ITEST:    index of test to view (use '' to view all)
%   xrange:   plot range of x-axis such as [0 800] or [0 10000] in msec
%   isFilter: =1 to view filtered rpm =0 to not filter
inFile  = [dataName,'.txt'];
datFile = [dataName,'.dat'];
dos(['grep  -iaE "^B |^set feed down|pb end"  ' inFile ' | sed -e ''s/pb end/99999 0 0 0/g'' |sed -e ''s/B //g''  |sed -e ''s/pb bail out //g''  > ' datFile]);
dos(['grep -iaE "SSS  " ' inFile ' | gawk -e ''{print $2,$3,$4,$5}'' > tmp2.dat']);
dos(['grep -iaE "SSS v=" ' inFile ' | gawk -e ''{print $3, $8}'' > tmp1.dat']);
p1 = load('tmp1.dat');
p2 = load('tmp2.dat');
para = [p1,p2];
dat = load(datFile);
it = find(dat(:,1)==99999);
dat(:,1) = dat(:,1)-dat(1,1);
ntest = length(it);
if nargin<3, xrange = [0 1000]; end
if (nargin<2 || length(ITEST)==0 ), ITEST = 1:ntest; end
if (nargin<4 ), isFilter = 0; end

ls = {'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',... 
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--'};
figure;
res = [];
for itest = ITEST
  lbl = sprintf('%s test-%d volt=%d target=%d g=%d w_l=%d w_r=%d bat=%d',dataName, itest,para(itest,:));
  if (itest==1)
    id = 1:it(1)-1;
  else
    id = it(itest-1)+1:it(itest)-1;
  end
  % t = dat(id,1)-dat(id(1),1);
  d = dat(id,:);
  t = dat(id,1);
  v = dat(id,2);
  rpm = dat(id,4);
  if (isFilter==1)
    rpm_F = matfiltfilt(mean(diff(t)),.03,2,rpm);
  else
   rpm_F = rpm;
  end
  ang = dat(id,3);
  % i0 = min(find(ang>=100));
  % t = t-t(i0)+200;
subplot(2,2,1);box on; grid on; hold on; xlabel('t'),ylabel('vol'); xlim(xrange); title(lbl);

  plot(t,v,ls{itest});
subplot(2,2,2);box on; grid on; hold on; xlabel('t'),ylabel('ang');xlim(xrange); 

  plot(t,ang,ls{itest});
subplot(2,2,4);box on; grid on; hold on; xlabel('t'),ylabel('rpm');xlim(xrange); 

  plot(t,rpm_F,ls{itest});
subplot(2,2,3);box on; grid on; hold on; xlabel('ang'),ylabel('rpm');

  
  plot(ang,rpm_F,ls{itest});
  res = [res;  max(ang), -min(rpm_F)];
  
  fprintf('%s test-%d volt=%d target=%d pull=%d hit_rpm=%d g=%d w_l=%d w_r=%d bat=%d \n',dataName, itest,para(itest,1:2), max(ang), -min(rpm_F),para(itest,3:6));

end