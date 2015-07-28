function Display_Mesh(Mesh)

P=unique(Mesh.triangles(:,4));
figure;
hold on;
for i=1:length(P)
    t=find(Mesh.triangles(:,4)==P(i));
    TR=TriRep(Mesh.triangles(t,1:3), Mesh.nodes(:,1),Mesh.nodes(:,2),Mesh.nodes(:,3));
    h= trimesh(TR);
    if (P(i)==1)
        c='b';
    else if (P(i)==2)
            c='k';
        else 
            c='r';
        end 
    end
    set(h,'EdgeColor',c,'FaceColor','none');
    daspect([1,1,1]);
end

