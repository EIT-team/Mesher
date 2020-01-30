% Make sizing field plots

% needs iso2mesh
V=readinr('../Input.inr');
%scale points to mm, these values are given in .inr header
vx=1;
vy=1;
vz=1;

X=zeros(size(V,1),1);
Y=zeros(size(V,2),1);
Z=zeros(size(V,3),1);

%chose slice to view (middle)
iSlice=54;

[x,z]=find(squeeze(V(:,iSlice,:)));
y=ones(size(x))*iSlice;

x=x/vx;
y=y/vy;
z=z/vz;

figure
plot(x,z,'.');
daspect([1,1,1])
title('Slice through inr');
%% Plot sizing field of planar refinement

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

%% Depth Refinement 

fine_size=2;
coarse_size=5;
elem_fine_per=100;

out=zeros(size(x));

origin=[max(x)/2 max(z)/2];

for iPoint = 1:size(x,1)
    d_el=[x(iPoint),z(iPoint)]-origin;
    d_per=sqrt((d_el(1)/origin(1))*(d_el(1)/origin(1)) + (d_el(2)/origin(2))*(d_el(2)/origin(2)));
    
    if (d_per >= 1 - elem_fine_per/100 )
        
        op=fine_size + (coarse_size - fine_size) * (1-d_per);
        
        if op < fine_size
            op=fine_size;
        end
            out(iPoint)=op;
    else
        out(iPoint) = coarse_size;
    end
    
    
end

scatter(x,z,20,out,'filled')
colormap('summer')
colorbar
daspect([1,1,1]);
caxis([fine_size coarse_size]);
title(sprintf('Depth Sizing field %0.5g%% fine elements',elem_fine_per));
xlabel('x')
ylabel('z');


% saveas(gcf,'2_SF1.png');


