function [numberofcells] = TimeVsNoCells(directory)
%% Importfiles

cells = importfile_displacement(directory);
time = double(importfile_time(directory));

celltype = cells(:,1:5:end);
numberofcells (:,1) = transpose(sum(transpose(celltype()) == 'Luminal'));
numberofcells (:,2)= transpose(sum(transpose(celltype()) == 'Myoepithelial'));
numberofcells (:,3) = transpose(sum(transpose(numberofcells())));

%% Plot Time vs Number of Cells
x = time(:,1);

yLE=numberofcells(:,1);

yME=numberofcells(:,2);

yWT=numberofcells(:,3);

LE = [.4 .8 .3]; 

ME = [.8 .1 .2]; 

figure
hold on

scatter(x(1:55:end), yWT(1:55:end), 'k')
pWT = polyfit(x,yWT,3);
fWT = polyval(pWT,x);
total = plot(x,fWT, 'k');

scatter(x(1:55:end), yLE(1:55:end), [], LE)
pLE = polyfit(x,yLE,3);
fLE = polyval(pLE,x);
LE = plot(x,fLE, 'g');

scatter(x(1:55:end), yME(1:55:end), [], ME)
pME = polyfit(x,yME,3);
fME = polyval(pME,x);
ME = plot(x,fME, 'r');

hold off

ylabel ('Number of Cells')
xlabel('Time (h)')
legend([total, LE, ME],'Total No. of Cells', 'No. of LE Cells', 'No of ME Cells')

folder = '~/Desktop/';
exportgraphics(gca, 'FigureCellsvsTime.pdf');
movefile('FigureCellsvsTime.pdf', folder);

return;

end

