function idx = find_perturbation(Mesh)

% set perturbation at position (100, 100, 100) with radius 15

idx = [];
idxcount = 1;

for i=1:size(Mesh.tetra,1)
    count = 1;
    for j=1:4
        quadrasum = 0;
        for k=1:3
            quadrasum = quadrasum + (Mesh.nodes(Mesh.tetra(i,j),k)-100)^2;
        end
        if quadrasum < 15^2
            count = count +1;
        end
    end
    if count == 4
        idx(idxcount)=i;
        idxcount = idxcount + 1;
    end
end