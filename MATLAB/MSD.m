function [xDeltaSq, yDeltaSq, zDeltaSq,totalMSD, sum] = MSD(directory)
%% Importfiles

Displacement = importfile_velocity(directory);
time = double(importfile_time(directory));

%% Calculate MSD cells

Displacement = str2double(Displacement);

for j = 0:((width(Displacement)/7)-1)
    xDisp(:,j+1) = Displacement(:,3+7*j);
    yDisp(:,j+1) = Displacement(:,4+7*j);
    zDisp(:,j+1) = Displacement(:,5+7*j);
end

n = 1;

xDeltaSq = (xDisp(1+n:end,1:width(xDisp)) - xDisp(1:end-n,1:width(xDisp))).^2;
yDeltaSq = (yDisp(1+n:end,1:width(yDisp)) - yDisp(1:end-n,1:width(yDisp))).^2;
zDeltaSq = (zDisp(1+n:end,1:width(zDisp)) - zDisp(1:end-n,1:width(zDisp))).^2;


for i = 1:height(xDeltaSq)
    for j = 1:width(xDeltaSq)
        sum(i,j) = xDeltaSq(i,j) + yDeltaSq(i,j)+ zDeltaSq(i,j);
        totalMSD(i,1) = nanmean(sum(i,:));
    end
end

newRow = zeros(1,size(totalMSD,2));
totalMSD = [totalMSD(1:0, :); newRow; totalMSD(1:end, :)]; 

newRow = zeros(1,size(sum,2));
sum = [sum(1:0, :); newRow; sum(1:end, :)]; 

return;

end

