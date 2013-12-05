clf;
clear;
data=load('psi_xz_data.dat');

 
%set(gcf,'Units','points','position',[0 0 400 400]);
%axes1 = axes('Visible','off','Parent',gcf);
%hold(axes1,'all');

Ra=data(:,4);
Za=data(:,5);
BRa=data(:,3);
Rb=Ra;
Zb=-Za;
BRb=BRa;

s=size(Ra,1);

R(1:s)=Ra(1:s);
R(s+1:2*s)=Rb(1:s);
Z(1:s)=Za(1:s);
Z(1+s:2*s)=Zb(1:s);
BR(1:s)=BRa(1:s);
BR(1+s:2*s)=BRb(1:s);

maxR=max(R);
minR=min(R);
maxZ=max(Z);
minZ=min(Z);
xlin=linspace(minR,maxR,80);
ylin=linspace(minZ,maxZ,80);
[X,Y]=meshgrid(xlin,ylin);

R=R';
Z=Z';
BR=BR';
f=scatteredInterpolant(R,Z,BR,'linear');
BRZ=f(X,Y);

contour(X,Y,BRZ,80);
%axis tight; hold on;
%plot3(R,Z,BR,'.','MarkerSize',4) %nonuniform

%plot(data(:,4),data(:,5),'o','MarkerSize',0.4);



%  set(gcf, 'PaperPositionMode','auto');
%  filename='Nova_BR'
%  filename_out=sprintf('%s.png',filename);
%  print(gcf,filename_out,'-dpng','-r600');
%   set(gcf, 'Renderer', 'painters');
% filename_out=sprintf('%s.eps',filename);
%  print(gcf,filename_out,'-depsc2');