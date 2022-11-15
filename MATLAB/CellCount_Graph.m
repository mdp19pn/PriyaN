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

x=categorical({'LE cells', 'ME cells'});

b = bar(x,y)
ylabel('Number of Cells at End')
ax = gca;

ax.XTickLabels = {'LE Cells', 'ME Cells'};

b.FaceColor = 'flat';
b.CData(1,:) = [.4 .8 .3];
b.CData(2,:) = [.8 .1 .2];

ax.FontSize = 14; 
ax.FontName = "Calibri";

folder = '~/Desktop/';
exportgraphics(gca, 'CellCount.tiff');
movefile('CellCount.tiff', folder);

return;

end

