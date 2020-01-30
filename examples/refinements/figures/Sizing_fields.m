% Make sizing field plots

% needs iso2mesh
V=readinr('../Input.inr');

X=zeros(size(V,1),1);
Y=zeros(size(V,2),1);
Z=zeros(size(V,3),1);

%chose slice to view (middle)
iSlice=54;

[x,z]=find(squeeze(V(:,iSlice,:)));
y=ones(size(x))*iSlice;

figure
plot(x,z,'.');
daspect([1,1,1])
title('Slice through inr');
%% Plot sizing field of planar refinement

%scale points to mm, these values are given in .inr header
vx=1;
vy=1;
vz=1;

x=x/vx;
y=y/vy;
z=z/vz;

upper_bound=max(z)*vx;

fine_size=2;
coarse_size=5;
height=100;
elem_fine_per=10;

out=zeros(size(x));

for iPoint = 1:size(x,1)
    d=abs(z(iPoint)-height);
    d_per=d/upper_bound;
    
    if (d_per <= elem_fine_per/100 )
        
        out(iPoint)=fine_size;
        
    else
       op=fine_size+abs((coarse_size-fine_size)*(d_per - (elem_fine_per) /100));
       
       if op > coarse_size
           op=coarse_size;
       end
       out(iPoint)=op;
       
    end
    
    
end

scatter(x,z,20,out,'filled')
colormap('summer')
colorbar
daspect([1,1,1]);
caxis([fine_size coarse_size]);
title(sprintf('Planar Sizing field, height=%0.5g, %0.5g%% fine elements',height,elem_fine_per));
xlabel('x')
ylabel('z');

% saveas(gcf,'5_SF1.png');




