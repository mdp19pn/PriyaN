function [numberofcells] = CellCount_Graph(directory)
%% Importfiles

cells = importfile_displacement(directory);

celltype = cells(:,1:5:end);
numberofcells (:,1) = transpose(sum(transpose(celltype()) == 'Luminal'));
numberofcells (:,2)= transpose(sum(transpose(celltype()) == 'Myoepithelial'));
numberofcells (:,3) = transpose(sum(transpose(numberofcells())));

%% Plot number of LE and ME cells
x = [1 2];
y = [numberofcells(end,1),numberofcells(end,2)];

b = bar(x,y)
ylabel('Number of Cells at t = 96')
ax = gca;
ax.XTick = [1 2]; 
ax.XTickLabels = {'LE Cells', 'ME Cells'};
ax.XTickLabelRotation = 45;

b.FaceColor = 'flat';
b.CData(1,:) = [.4 .8 .3];
b.CData(2,:) = [.8 .1 .2];

folder = '~/Desktop/';
exportgraphics(gca, 'CellCount.pdf');
movefile('CellCount.pdf', folder);

return;

end

