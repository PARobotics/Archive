function v= analyze(dataName)
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
% dos(['grep  -iaE "test-"  ' inFile ' | grep -iaE ang= | gawk ''{print $5,$8,$9,$10,$11}'' >tmp1']);
dos(['grep  -iaE "test-"  ' inFile ' | grep -iaE ang= | gawk ''{print $5,$8,$9,$10}'' >tmp1']);
dos(['grep  -iaE "L_ANG_MAX"  ' inFile ' | gawk ''{print $4}'' >tmp2']);
dos(['grep  -iaE "vhit"  ' inFile ' | gawk ''{print $4,$7}'' >tmp3']);
dos(['grep  -iaE " , A ="  ' inFile ' | gawk ''{print $4,$12}'' >tmp4']);

v1 = load('tmp1');
v2 = load('tmp2');
v3 = load('tmp3');
v4 = load('tmp4');
fprintf('size v1 = %d\n',size(v1,1));
fprintf('size v2 = %d\n',size(v2,1));
fprintf('size v3 = %d\n',size(v3,1));
fprintf('size v4 = %d\n',size(v4,1));
n = min([size(v1,1),size(v2,1),size(v3,1),size(v4,1),])
v = [[1:length(v1)]' v2(1:n,:) v4(1:n,:) v3(1:n,:) v1(1:n,:)];
fprintf(' #  target  dtMore  rpm actual  vhit  actual    V_150    V_90     V45   Y/N  \n');
% fprintf('%2d  %5d  %5d  %5d  %8.1f %8.1f %8.1f %8.1f   %3d\n',v')
fprintf('%2d  %5d  %5d  %5d %5d  %5d  %8d %8.1f %8.1f  %8.1f \n',v')
% fprintf('%2d  %5d  %8.1f %8.1f %8.1f %8.1f \n',v')