
d=readinr('input.inr');
[node,elem]=binsurface(e,4);
elecpos=dlmread('Electrodes.txt');

elecpos=elecpos*10+[-6 -8 -100];
figure

hold on
[node,elem]=binsurface(d,4);
patch('Vertices',node,'faces',elem,'FaceColor','none','EdgeAlpha',0.2);

plot3(elecpos(:,1),elecpos(:,2),elecpos(:,3),'.','MarkerSize',30);
daspect([1,1,1]);
hold off
%%

e=d(70:200,60:150,40:210);
[node,elem]=binsurface(e,4);

elec_pos_new_sc=elecpos-[70,60,40];



figure
hold on
patch('Vertices',node,'faces',elem,'FaceColor','none','EdgeAlpha',0.2);
plot3(elec_pos_new_sc(:,1),elec_pos_new_sc(:,2),elec_pos_new_sc(:,3),'.','MarkerSize',30);
daspect([1,1,1]);

figure
 imagesc(e(:,:,60))
 
 elec_pos_new_sc=[1.25 1.25 1.5].*elec_pos_new_sc*(0.025);

saveinr_EIT(uint8(e),'input2.inr',[1.25 1.25 1.5]);
dlmwrite('Electrodes2.txt',elec_pos_new_sc);