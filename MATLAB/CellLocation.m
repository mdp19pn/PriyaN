function [LuminalPositions, MyoepithelialPositions, LuminalLocation, MyoepithelialLocation, r, r2] = CellLocation(directory)
%% Importfiles

location = importfile_location(directory);

%% Reshape data file

LuminalPositions = zeros(height(displacement), width(displacement)); % creates new table filled with zero the size of location data file
MyoepithelialPositions = zeros(height(displacement), width(displacement)); % creates new table filled with zero the size of location data file

for i = 1:height(displacement)
    
    for j = 1:width(displacement)-5
        
        if displacement(i,j) == "Luminal"
            LuminalPositions(i,j) = str2double(displacement(i,j));
            LuminalPositions(i,j+1) = str2double(displacement(i,j+1));
            LuminalPositions(i,j+2) = str2double(displacement(i,j+2));
            LuminalPositions(i,j+3) = str2double(displacement(i,j+3));
            LuminalPositions(i,j+4) = str2double(displacement(i,j+4));
            LuminalPositions(i,j+5) = str2double(displacement(i,j+5));
        
        elseif displacement(i,j) == "Myoepithelial"
            
            MyoepithelialPositions(i,j) = str2double(displacement(i,j));
            MyoepithelialPositions(i,j+1) = str2double(displacement(i,j+1));
            MyoepithelialPositions(i,j+2) = str2double(displacement(i,j+2));
            MyoepithelialPositions(i,j+3) = str2double(displacement(i,j+3));
            MyoepithelialPositions(i,j+4) = str2double(displacement(i,j+4));
            MyoepithelialPositions(i,j+5) = str2double(displacement(i,j+5));
        
        end
    
    end

end

%% Compute Statistics

LuminalLocation = LuminalPositions(:,any(LuminalPositions)); % removes collumns containing only zeros
MyoepithelialLocation = MyoepithelialPositions(:,any(MyoepithelialPositions)); % removes collumns containing only zeros

for row = 1:height (LuminalLocation)
    for counter = 1:(width(LuminalLocation)-1)/4
        j = (counter * 4)-2;
        r(row,counter) = LuminalLocation(row,j+2);
    end
end

for row = 1:height (MyoepithelialLocation)
    for counter = 1:(width(MyoepithelialLocation)-1)/4
        j = (counter * 4)-2;
        r2(row,counter) = MyoepithelialLocation(row,j+2);
    end
end

%% Plot location of LE and ME cells
luminal = r(end,:);
myo = r2(end,:);
g = [luminal myo];
C = [zeros(1,length(luminal)) ones(1,length(myo))];

boxplot(g,C, 'Labels', {'LE Cells', 'ME Cells'})

ylabel ('Position of Cell from the ECM (a.u.)')

folder = '~/Desktop/';
exportgraphics(gca, 'CellPosition.pdf');

return;

end

