clear;
clf;
filename='Output_CLT.h5';

%h5disp(filename);

R=h5read(filename,'/R');
Z=h5read(filename,'/Z');

BR=h5read(filename,'/BR');
BZ=h5read(filename,'/BZ');
BPhi=h5read(filename,'/BPhi');


JR=h5read(filename,'/JR');
JZ=h5read(filename,'/JZ');
JPhi=h5read(filename,'/JPhi');

Psi=h5read(filename,'/Psi');
rho=h5read(filename,'/rho');
p=h5read(filename,'/p');

[R,Z]=meshgrid(R,Z);

set(gcf,'Units','points','position',[50 20 800 500]);
%B
hax=axes('Position',[0.06 0.71 0.28 0.25],'FontSize',13); 
contourf(R,Z,BR,20,'LineStyle','none');
title( '$B_R$');

hax=axes('Position',[0.38 0.71 0.28 0.25],'FontSize',13); 
contourf(R,Z,BZ,20,'LineStyle','none');
title( '$B_Z$');

hax=axes('Position',[0.7 0.71 0.28 0.25],'FontSize',13); 
contourf(R,Z,BPhi,20,'LineStyle','none');
title( '$B_\varphi$');

%J
hax=axes('Position',[0.06 0.38 0.28 0.25],'FontSize',13); 
contourf(R,Z,JR,20,'LineStyle','none');
title( '$J_R$');

hax=axes('Position',[0.38 0.38 0.28 0.25],'FontSize',13); 
contourf(R,Z,JZ,20,'LineStyle','none');
title( '$J_Z$');

hax=axes('Position',[0.7 0.38 0.28 0.25],'FontSize',13); 
contourf(R,Z,JPhi,20,'LineStyle','none');
title( '$J_\varphi$');

%Psi
hax=axes('Position',[0.06 0.05 0.28 0.25],'FontSize',13); 
contourf(R,Z,Psi,20,'LineStyle','none');
title( '$\psi$');

%Pressure
hax=axes('Position',[0.38 0.05 0.28 0.25],'FontSize',13); 
contourf(R,Z,p,20,'LineStyle','none');
title( '$P$');

%rho (density)
hax=axes('Position',[0.7 0.05 0.28 0.25],'FontSize',13); 
contourf(R,Z,rho,20,'LineStyle','none');
title( '$\rho$');




filename='2D_equ'
  set(gcf, 'PaperPositionMode','auto');
  filename_out=sprintf('%s.png',filename);
  print(gcf,filename_out,'-dpng','-r600');
  
  filename_out=sprintf('%s.eps',filename);
  set(gcf, 'Renderer', 'painters');
  print(gcf,filename_out,'-depsc2');
  
  

% set(gcf, 'PaperPositionMode','auto');
set(gcf,'PaperUnits','centimeters'); 
set(gcf,'PaperSize',[28 20]); 
filename_out=sprintf('%s.pdf',filename);
set(gcf, 'Renderer', 'painters');
print(gcf,filename_out,'-dpdf');
  


