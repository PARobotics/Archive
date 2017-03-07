function [t,vcmd,vbat] = a_single(dataName,ITEST, xrange,isFilter,marker,isPower)
%
% clear all, close all, analyze(dataName,ITEST, xrange,isFilter)
% clear all, close all, analyze(dataName,'', [0 800],1)
%   dataName: the file name (in data folder ) w/o extension
%   ITEST:    index of test to view (use '' to view all)
%   xrange:   plot range of x-axis such as [0 800] or [0 10000] in msec
%   isFilter: =1 to view filtered rpm =0 to not filter
%   marker:   =1 to view/print results with marked values
%   isPower:  =1 to plot motor power =0 no

inFile  = [dataName,'.txt'];
datFile = [dataName,'.dat'];
dos(['grep  -iaE "^B |^set feed down|pb end"  ' inFile ' | sed -e ''s/pb end/99999 0 0 0/g'' |sed -e ''s/B //g''  |sed -e ''s/pb bail out //g''  > ' datFile]);

dat = load(datFile);
it = find(dat(:,1)==99999);
dat(:,1) = dat(:,1)-dat(1,1);
ntest = length(it);
if nargin<3, xrange = [0 1000]; end
if (nargin<2 || length(ITEST)==0 ), ITEST = 1:ntest; end
if (nargin<4 ), isFilter = 0; end
if (nargin<5 ), marker = 0; end
if (nargin<6 ), isPower = 0; end

ls = {'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',... 
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--',...
      'rd-','gd-','bd-','kd-','md-','yd-','cd-','k--'};
% figure;
sync = 0;
res = [];
for itest = ITEST
  if (itest==1)
    id = 1:it(1)-1;
  else
    id = it(itest-1)+1:it(itest)-1;
  end
  d = dat(id,:);
  t = dat(id,1)-dat(id(1),1);
  vbat = dat(id,2)/1000;
  vcmd = dat(id,3);
  ang  = dat(id,4);
  rpm = dXdt(ang,t)*1000/360*60;
  if isFilter==1
    rpm = matfiltfilt(mean(diff(t)), .015, 2, rpm);
  end

  if (sync==1)
    drpm = [0; diff(rpm)];
    iii = min(find(ang.*sign(drpm)>80==1));
    t = t-t(iii);
  end

  
  if marker>0
    dv = diff(vcmd);
    idpt(1) = min(find(abs(rpm)>5))-1;
    idpt(2) = min(find(vcmd ~= vcmd(1)))-1;
    idpt(4) = min(find(dv<-80))+1;
    idpt(3) = min(find(vcmd == vcmd(idpt(4)-1)));
    [tmp,idpt(5)] = max(ang);
    idpt(6) = min(find(ang(20:end)<-1))+19-3  ;
    [tmp,idpt(7)] = min(ang);
  end
 
  
  figure(1);
  lbl = sprintf('%s test-%d AVGBAT=%.2f ',dataName, itest, vbat(1));

   figure;box on; grid on; hold on; xlabel('ANG'),ylabel('VCMD'); title(lbl);  xlim([-160 270])
   plot(ang,vcmd,ls{itest});
       xlim([0 250]);


   figure; subplot(2,3,1);box on; grid on; hold on; xlabel('TIME'),ylabel('VCMD'); xlim(xrange); title(lbl);
   plot(t,vcmd,ls{itest});

   subplot(2,3,4);box on; grid on; hold on; xlabel('TIME'),ylabel('AVG BATT'); xlim(xrange); ylim([6.4,8.4])
   % plot(t,vbat.*vcmd/vcmd(1),ls{itest});
   plot(t,vbat,ls{itest});


    % plot(ang,vcmd,ls{itest}); xlabel('angle in deg')
  if marker>0
    for i=1:length(idpt)
      text(t(idpt(i)),vcmd(idpt(i)),['\leftarrow ',sprintf('%.0f',vcmd(idpt(i)))])
      fprintf(sprintf('    t(%d)=%4.0f vbat=%4d vcmd=%4d ang=%4d rpm=%5.1f\n', ... 
        i,t(idpt(i)), vbat(idpt(i)), vcmd(idpt(i)),ang(idpt(i)), rpm(idpt(i))))
    end
  end
  

  subplot(2,3,3);box on; grid on; hold on; xlabel('TIME'),ylabel('ANG');xlim(xrange);  title(lbl);  
  plot(t,ang,ls{itest});
  
  if marker>0
    for i=1:length(idpt)
      text(t(idpt(i)),ang(idpt(i)),['\leftarrow ',sprintf('%.0f@%d',ang(idpt(i)),t(idpt(i)))])
    end
  end
  
  subplot(2,3,6);box on; grid on; hold on; xlabel('TIME'),ylabel('RPM');xlim(xrange); 
  plot(t,rpm,ls{itest});
  if marker>0
    for i=1:length(idpt)
      text(t(idpt(i)),rpm(idpt(i)),['\leftarrow ',sprintf('%.0f@%d',rpm(idpt(i)),t(idpt(i)))])
    end
  end
  

  figure;box on; grid on; hold on; xlabel('ANG'),ylabel('RPM');
  xlim([0 250]); 
  % ylim([0 250])
  plot(ang,rpm,ls{itest});
  if marker>0
    for i=1:length(idpt)
      text(ang(idpt(i)),rpm(idpt(i)),['\leftarrow ',sprintf('[%.0f %.0f]',ang(idpt(i)),rpm(idpt(i)))])
    end
  end
  pos = get(0,'ScreenSize');
  
  % set the size of screen
  set(gcf,'pos',[10 20 pos(3)-20 pos(4)-120]);
  % plot power if needed
  if isPower==1
    [p,E] = cal_motor_power(t,vcmd, ang,rpm/4.8);
    E = 5*E;
    figure(2); 
    subplot(2,1,1);box on; grid on; hold on; xlabel('t'),ylabel('P');
    plot(t,p,ls{itest})
    subplot(2,1,2);box on; grid on; hold on; xlabel('t'),ylabel('E');
    plot(t,E,ls{itest})
  end  
  
  [tmp,imax] = max(ang(end:-1:1));
  [tmp,imin] = min(ang);
  imax=length(ang)-imax+2;
  id = imax:imin;
  tt = interp1(ang(id)-1e-6*[1:length(id)]',t(id),[160 100 55 10]','linear');
  [ang(id),t(id)];
  V_avg(1) = 150/(tt(end)-tt(1))*1000*60/360;
  V_avg(2) = 90/(tt(end)-tt(2))*1000*60/360;
  V_avg(3) = 45/(tt(end)-tt(3))*1000*60/360;
  
  % fprintf('%s test-%d volt=%2d ang= %.1f rpm= %.0f \n',dataName, itest, vbat(1), max(ang), max(-rpm));
  fprintf('%s test-%d volt=%4.2f ang= %.1f rpm= %.0f %.1f %.1f %.1f \n',dataName, itest, vbat(1), max(ang), max(-rpm),V_avg);
  % fprintf('%d %.1f %.1f %.1f %.1f\n',max(ang), max(-rpm),V_avg);

  
end