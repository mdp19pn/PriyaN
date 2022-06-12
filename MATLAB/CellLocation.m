function [location1, location2, percentLuminal, percentMyoepithelial, LuminalAverage, MyoepithelialAverage] = CellLocation(directory)
%% Importfiles

location = importfile_location(directory);

%% Reshape data file

location1 = reshape(location, 5,[])';
location2 = array2table(location1, ...
    'VariableNames',{'Cell Type', 'Cell ID', 'x-pos', 'y-pos', 'z-pos'});

LuminalPositions = zeros(height(location1), width(location1));
MyoepithelialPositions = zeros(height(location1), width(location1));

for i = 1:height(location1)
    if location1(i,1) == "Luminal"
        for j = 1:width(location1)
            LuminalPositions(i,j) = str2double(location1(i,j));
        end
    elseif location1(i,1) == "Myoepithelial"
        for j = 1:width(location1)
            MyoepithelialPositions(i,j) = str2double(location1(i,j));
        end
    end
end

%% Compute Statistics

LuminalPositions = LuminalPositions(any(LuminalPositions,2),:);
MyoepithelialPositions = MyoepithelialPositions(any(MyoepithelialPositions,2),:);

[numRowsLuminal,numColsLuminal] = size(LuminalPositions);
[numRowsMyoepithelial,numColsMyoepithelial] = size(MyoepithelialPositions);
[numRowsTotal,numColsTotal] = size(location2);

Luminal_z0 = 0;
Myoepithelial_z0 = 0;

for i = 1:numRowsLuminal
    if LuminalPositions(i,5) == 0
        Luminal_z0 = Luminal_z0 + 1;
    end
end

percentLuminal = Luminal_z0 / numRowsTotal

for i = 1:numRowsMyoepithelial
    if MyoepithelialPositions(i,5) == 0
        Myoepithelial_z0 = Myoepithelial_z0 + 1;
    end
end

percentMyoepithelial = Myoepithelial_z0 / numRowsTotal

LuminalAverage = mean (LuminalPositions(:,5))

MyoepithelialAverage = mean (MyoepithelialPositions(:,5))

%% Plot location of LE and ME cells
x = [1 2];
y = [LuminalAverage,MyoepithelialAverage];

b = bar(x,y,'FaceColor','[0 0.1 0]')
ylabel('Average Height of Cells from the Subtsrate (a.u.)')
ax = gca;
ax.XTick = [1 2]; 
ax.XTickLabels = {'LE Cells', 'ME Cells'};
ax.XTickLabelRotation = 45;

b.FaceColor = 'flat';
b.CData(1,:) = [.4 .8 .3];
b.CData(2,:) = [.8 .1 .2];

folder = '~/Desktop/';
exportgraphics(gca, 'CellLocation.pdf');
movefile('CellLocation.pdf', folder);

return;

end

