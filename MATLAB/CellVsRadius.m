function [slope] = CellVsRadius(directory)
%% Importfiles
heterotypicboundaryall = importfile_heterotypicboundaryall(directory);
cells = importfile_displacement(directory);

x=table2array(heterotypicboundaryall(:,1));

y=table2array(heterotypicboundaryall(:,3));

celltype = cells(:,1:5:end);
numberofcells (:,1) = transpose(sum(transpose(celltype()) == 'Luminal'));
numberofcells (:,2)= transpose(sum(transpose(celltype()) == 'Myoepithelial'));
numberofcells (:,3) = transpose(sum(transpose(numberofcells())));

%% Plot Number of Cells vs Radius
diameter = y(:,1)./pi;
y = numberofcells(:,3);

figure
hold on

scatter(diameter(1:11:960), y(1:11:960), 'k')
pWT = polyfit(diameter,y,1);
fWT = polyval(pWT,diameter);
total = plot(diameter,fWT, 'k');

hold off

ylabel ('Number of Cells')
xlabel('Average diameter (a.u.)')
Calculate gradient of slope

slope = pWT(1);

return;

end