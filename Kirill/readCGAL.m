function Mesh=readCGAL(filename)

file=fopen(filename,'r');
for i=1:3
hdr=fgets(file)
end
Mesh.N_nodes=fscanf(file,'%d\n',1);

for i=1:Mesh.N_nodes
    for j=1:4
       Mesh.nodes(i,j)=fscanf(file,'%f',1);
    end
end

hdr=fgets(file)
hdr=fgets(file)

Mesh.N_triangles=fscanf(file,'%d\n',1);
for i=1:Mesh.N_triangles
    for j=1:4
       Mesh.triangles(i,j)=fscanf(file,'%d',1);
    end
end

hdr=fgets(file)
hdr=fgets(file)

Mesh.N_tetra=fscanf(file,'%d\n',1);
for i=1:Mesh.N_tetra
    for j=1:5
       Mesh.tetra(i,j)=fscanf(file,'%d',1);
    end
end

fclose(file);