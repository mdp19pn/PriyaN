function [LuminalPositions, MyoepithelialPositions, LuminalLocation, MyoepithelialLocation, r, r2] = CellLocation(directory)
%% Importfiles

location = importfile_location(directory);

location(location=="0")=5597; % replaces all the zeros in the table with "5597" to ensure these collumns don't get removed 

%% Reshape data file

LuminalPositions = zeros(height(location), width(location)); % creates new table filled with zero the size of location data file
MyoepithelialPositions = zeros(height(location), width(location)); % creates new table filled with zero the size of location data file

for i = 1:height(location)
    
    for j = 1:width(location)-5
        
        if location(i,j) == "Luminal"
            LuminalPositions(i,j) = str2double(location(i,j));
            LuminalPositions(i,j+1) = str2double(location(i,j+1));
            LuminalPositions(i,j+2) = str2double(location(i,j+2));
            LuminalPositions(i,j+3) = str2double(location(i,j+3));
            LuminalPositions(i,j+4) = str2double(location(i,j+4));
            LuminalPositions(i,j+5) = str2double(location(i,j+5));
        
        elseif location(i,j) == "Myoepithelial"
            
            MyoepithelialPositions(i,j) = str2double(location(i,j));
            MyoepithelialPositions(i,j+1) = str2double(location(i,j+1));
            MyoepithelialPositions(i,j+2) = str2double(location(i,j+2));
            MyoepithelialPositions(i,j+3) = str2double(location(i,j+3));
            MyoepithelialPositions(i,j+4) = str2double(location(i,j+4));
            MyoepithelialPositions(i,j+5) = str2double(location(i,j+5));
        
        end
    
    end

end


%% Compute Statistics

LuminalLocation = LuminalPositions(:,any(LuminalPositions)); % removes collumns containing only zeros
MyoepithelialLocation = MyoepithelialPositions(:,any(MyoepithelialPositions)); % removes collumns containing only zeros

LuminalLocation(LuminalLocation==5597)=0; % replace "5597" with the original zero
MyoepithelialLocation(MyoepithelialLocation==5597)=0; % replace "5597" with the original zero

for row = 1:height (LuminalPositions)
    for counter = 1:(width(LuminalLocation)-1)/4
        j = (counter * 4)-2;
        r(row,counter) = LuminalLocation(end,j+2);
    end
end


for row = 1:height (MyoepithelialPositions)
    for counter = 1:(width(MyoepithelialLocation)-1)/4
        j = (counter * 4)-2;
        r2(row,counter) = MyoepithelialLocation(end,j+2);
    end
end

%% Plot location of LE and ME cells
% luminal = r(end,:);
% myo = r2(end,:);
% g = [luminal myo];
% C = [zeros(1,length(luminal)) ones(1,length(myo))];
% 
% boxplot(g,C, 'Labels', {'LE Cells', 'ME Cells'})
% 
% ylabel ('Position of Cell from the ECM (a.u.)')
% 
% ax = gca;
% ax.FontSize = 14; 
% ax.FontName = "Calibri";
% 
% folder = '~/Desktop/';
% exportgraphics(gca, 'CellPosition.tiff');
% movefile('CellPosition.tiff', folder);

%Calculate Mean
% LEaverage = mean(r,2);
% MEaverage = mean(r2,2);
% 
% LEaverageEnd = LEaverage(end,:);
% MEaverageEnd = MEaverage(end,:);

%% Plot average location of LE and ME cells

% y = cat(2, LEaverageEnd, MEaverageEnd);
% x=categorical({'LE cells', 'ME cells'});
% 
% figure
% 
% b = bar(x,y);
% 
% ylabel('Average Position of Cells from the ECM (a.u.)') 
% ax.XTickLabels = {'LE Cells', 'ME Cells'};
% 
% b.FaceColor = 'flat';
% b.CData(1,:) = [.4 .8 .3];
% b.CData(2,:) = [.8 .1 .2];
% 
% ax = gca;
% ax.FontSize = 14; 
% ax.FontName = "Calibri";
% 
% folder = '~/Desktop/';
% exportgraphics(gca, 'AverageCellLocation.tiff');
% movefile('AverageCellLocation.tiff', folder);


luminal = r(end,:);
LE1 = luminal.';
LE2 = repmat({'LE cells'},size(LE1,1),1);

myo = r2(end,:);
ME1 = myo.';
ME2 = repmat({'ME cells'},size(ME1,1),1);

x = categorical(LE2);
c = [0.4 0.8 0.3];
x1 = categorical(ME2);
c1 = [0.8 0.1 0.2];

swarmchart(x,LE1,50,c,'filled')
hold on 
swarmchart(x1,ME1, 50,c1,'filled' )
hold off
ylabel ('Position of cell from the center (a.u.)')

ax = gca;
ax.FontSize = 14; 
ax.FontName = "Calibri";
folder = '~/Desktop/';
exportgraphics(gca, 'CellPosition.tiff');
movefile('CellPosition.tiff', folder);

return;
end

