function new_electrodes = rotate_electrodes(electrodes, axis, angle)

% Centre points around origin
centres = mean(electrodes);
num_electrodes = size(electrodes,1);
translation_matrix = repmat(centres,num_electrodes,1);
electrodes = electrodes - translation_matrix;

% Create rotation matrix
if (axis == 'x')
    rot_matrix = [  1           0           0
                    0           cosd(angle)  -sind(angle)
                    0           sind(angle)  cosd(angle)
                    ];
                
end

if (axis == 'y')
    rot_matrix = [  cosd(angle)  0           sind(angle)
                    0           1           0
                    -sind(angle) 0           cosd(angle)
                    ];
                
end

if (axis == 'z')
    rot_matrix = [  cosd(angle)  -sind(angle) 0
                    sind(angle)  cosd(angle)  0
                    0           0           1
                    ];
                
end


%Rotate around origin
new_electrodes = electrodes * rot_matrix;

% Move back to original locations
new_electrodes = new_electrodes + translation_matrix;
