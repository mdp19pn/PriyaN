function [x,y] = TimeVsRadius(directory)
%% Importfiles

heterotypicboundaryall = importfile_heterotypicboundaryall(WT);

%% Plot Time vs Total Boundary Length
x=table2array(heterotypicboundaryall(:,1));

y=table2array(heterotypicboundaryall(:,3));

figure
hold on

scatter(x(1:11:960), y(1:11:960), 'k')
pWT = polyfit(x,y,4);
fWT = polyval(pWT,x);
plot(x,fWT, 'k')

hold off

ylabel ('Boundary Length (a.u.)')
xlabel('Time (h)')
%legend({'WT', 'ME B1 KO'})

%% Plot Time vs Radius
x=table2array(heterotypicboundaryall(:,1));

y=table2array(heterotypicboundaryall(:,3));

diameter = y(:,1)./pi;

radius = diameter(:,1)./2;

figure
hold on

scatter(x(1:11:960), radius(1:11:960), 'k')
pWT = polyfit(x,radius,4);
fWT = polyval(pWT,x);
plot(x,fWT, 'k')

hold off

ylabel ('Average Radius (a.u.)')
xlabel('Time (h)')
%legend({'WT', 'ME B1 KO'})

return;
end

