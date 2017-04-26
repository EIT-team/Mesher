function mesh = dilate_mesh_layer(mesh, sigma_to_dilate, neighbouring_elements)

% Calculate adjacent cells to each element, if needed.
if nargin < 3
    neighbouring_elements = faceneighbors(double(Mesh.Tetra(:,1:4)));
end


% Indexes of elements in layer to expand
layer_idxs = find(mesh.Sigma == sigma_to_dilate);
% dilated.Tetra = mesh.Tetra(layer_idxs,:);

% When using tetrahedral mesh, need to peform the dilation operation 3
% times in order to avoid 'spiky' boundaries between layers.
num_dilations = 3;

for i = 1:num_dilations
    
    % Select only the elements that are to be set to the desired sigma -
    % those that are adjacent to an existing cell with that value, or that
    % already have the desired sigma.
    
    % faceneighbors will return 0 where there is no neighbouring element
    % i.e. it is at the boundary. Remove these elements from the array and
    % change it to a vector.
    tetras_to_dilate = neighbouring_elements(layer_idxs,:);
    tetras_to_dilate = tetras_to_dilate(:);
    tetras_to_dilate = tetras_to_dilate( tetras_to_dilate > 0);
        
    % Change sigma values of desired elements
    mesh.Sigma(tetras_to_dilate) = sigma_to_dilate;
    
    % Update with newly dilated elements
    layer_idxs = find(mesh.Sigma == sigma_to_dilate);
    
end