function [position] = RadiusOfOrganoid(directory)
%% Importfiles

position = importfile_position(directory);

%% Calculate radial distance of cells and detremine max radial position

for row = 1:height (position)
    for counter = 1:(width(position)-1)/5
        j = (counter * 5) - 1;
        r(row,counter) = sqrt(position(row,j)^2 + position(row,j+1)^2 + position(row,j+2)^2);
    end
    [M(row,1), index(row)] = max (r(row,:));
end

%% Plot radial distance of organoid over time

x = position(:,1);
y = M;

ylim([1 2.4])
xlim([0 120])

hold on

scatter(x(1:55:end),y(1:55:end))

hold off

ylabel ('Radius of Organoid (a.u)')
xlabel('Time (h)')

folder = '~/Desktop/';
exportgraphics(gca, 'FigureRadiusofOrganoid.pdf');
movefile('FigureRadiusofOrganoid.pdf', folder);

return;

end