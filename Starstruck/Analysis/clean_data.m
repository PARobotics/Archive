function dat = clean_data(dat, threshold);
%
% remove the jumpy data out of the raw data array
%
if length(threshold) ~= size(dat,2)
  error('threshold size has to match dat size')
end

da = abs(diff(dat));
irmv = [];
for icol=1:length(threshold)
  irmv=union(irmv, find(da(:,icol)>threshold(icol)));
end
dat(irmv+1,:) = [];

