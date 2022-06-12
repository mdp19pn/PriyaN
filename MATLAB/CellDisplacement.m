function [LuminalMeanDisplacement, MyoepithelialMeanDisplacement] = CellDisplacement(directory)
%% Importfiles

displacement = importfile_displacement(directory);
time = double(importfile_time(directory));

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

LuminalPositions = LuminalPositions(:,any(LuminalPositions)); % removes collumns containing only zeros
MyoepithelialPositions = MyoepithelialPositions(:,any(MyoepithelialPositions)); % removes collumns containing only zeros

%% Calculate displacement LE
for j = 1:(width(LuminalPositions)-2)/5 % Columns corresponding to x-position of each cell
    for i = 1:(height(LuminalPositions)-1)            
        xLE_0 = LuminalPositions(i,5*j);
        xLE_1 = LuminalPositions(i+1,5*j);
        xLEDisp(i,1) = xLE_1-xLE_0;   % Calculate distance between x and x+1 timestep
        
        yLE_0 = LuminalPositions(i,5*j+1);
        yLE_1 = LuminalPositions(i+1,5*j+1);
        yLEDisp(i,1) = yLE_1-yLE_0; % Calculate distance between y and y+1 timestep
        
        zLE_0 = LuminalPositions(i,5*j+2);
        zLE_1 = LuminalPositions(i+1,5*j+2);
        zLEDisp(i,1) = zLE_1-zLE_0;   % Calculate distance between z and z+1 timestep
        
        LEtotDisp(i,1) = time(i,1);  % Set first column as time
        
        LEnode1Disp(i,1) = sqrt(xLEDisp(i,1)^2 + yLEDisp(i,1)^2 + zLEDisp(i,1)^2);  % Calculate total distance travelled
        
        LEtotDisp(i,j+1) = LEnode1Disp(i,1);
    end
end

LEtotDisp = double(LEtotDisp);

%% Calculate displacement ME
for j = 1:(width(MyoepithelialPositions)-2)/5 % Columns corresponding to x-position of each cell
    for i = 1:(height(MyoepithelialPositions)-1)
        xME_0 = MyoepithelialPositions(i,5*j);
        xME_1 = MyoepithelialPositions(i+1,5*j);
        xMEDisp(i,1) = xME_1-xME_0;   % Calculate distance between x and x+1 timestep
        
        yME_0 = MyoepithelialPositions(i,5*j+1);
        yME_1 = MyoepithelialPositions(i+1,5*j+1);
        yMEDisp(i,1) = yME_1-yME_0; % Calculate distance between y and y+1 timestep
        
        zME_0 = MyoepithelialPositions(i,5*j+2);
        zME_1 = MyoepithelialPositions(i+1,5*j+2);
        zMEDisp(i,1) = zME_1-zME_0;   % Calculate distance between z and z+1 timestep
            
        MEtotDisp(i,1) = time(i,1);  % Set first column as time
            
        MEnode1Disp(i,1) = sqrt(xMEDisp(i,1)^2 + yMEDisp(i,1)^2 + zMEDisp(i,1)^2);  % Calculate total distance travelled
            
        MEtotDisp(i,j+1) = MEnode1Disp(i,1);
    end
end

MEtotDisp = double(MEtotDisp);

%% Average Cell Type Displacement
% Averaging distance for each time step of Luminal Cells
LuminalMeanDisplacement(:,1) = time(:,1)

for n = 1:height(LEtotDisp)
    LuminalMeanDisplacement(n,2) = mean(LEtotDisp(n, (LEtotDisp(n,2:end) ~= 0)));
end

% Averaging distance for each time step of Myoepithelial Cells
MyoepithelialMeanDisplacement(:,1) = time(:,1)

for n = 1:height(MEtotDisp)
    MyoepithelialMeanDisplacement(n,2) = mean(MEtotDisp(n, (MEtotDisp(n,2:end) ~= 0)));
end

return;
end

