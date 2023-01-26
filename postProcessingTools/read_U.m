function read_U(filename)

% Copy and paste heights from the text file into this heights array
heights = [5 15 25 35 45 55 65 75 85 95 105 115 125 135 145 155 165 175 185 195 205 215 225 235 245 255 265 275 285 295 305 315 325 335 345 355 365 375 385 395 405 415 425 435 445 455 465 475 485 495 505 515 525 535 545 555 565 575 585 595 605 615 625 635 645 655 665 675 685 695 705 715 725 735 745 755 765 775 785 795 805 815 825 835 845 855 865 875 885 895 905 915 925 935 945 955 965 975 985 995 1005 1015 1025 1035 1045 1055 1065 1075 1085 1095 1105 1115 1125 1135 1145 1155 1165 1175 1185 1195 1205 1215 1225 1235 1245 1255 1265 1275];

% Once this is done delete the first two rows of the U.txt file

z0 = 0.1;  % Surface roughness 
heights = heights./z0; 


% Converts the text file into an array 
U = readtable(filename);
U = table2array(U);

% Create time and dt vectors 
times = U(:,1);
dt = U(:,2);

% Set averaging period
startAvg = 20000;
endAvg = 25000;

% Find data indices for data to be averaged
idx1 = find(times > startAvg);
idx2 = find(times > endAvg);

% Remove unwanted data
U(idx2(1):end,:) = [];
U(1:idx1(1),:) = [];

% Create some arrays for the velocity components 
ux = zeros(1,length(heights));
uy = zeros(1,length(heights));
uz = zeros(1,length(heights));

% Time average the velocities 
for i = 1:length(heights)
    ux(i) = mean(U(:,i*3+0));
    uy(i) = mean(U(:,i*3+1));
    uz(i) = mean(U(:,i*3+2));
end

% Do some plots

%plot(ux,heights)
%xline(8,'--')
%yline(80,'--')
%axis([min(ux) max(ux) min(heights) max(heights)]);
%xlabel('U_x, m/s')
%ylabel('Height, m')
%title('Time-averaged plot of velocity in the x-direction')

figure
semilogx(heights,ux)
xline(8,'--')
yline(80,'--')
axis([min(heights) max(heights) min(ux) max(ux)]);
ylabel('U_x, m/s')
xlabel('z/z0, m')
title('Time-averaged plot of velocity in the x-direction')



%figure
%plot(uy,heights)
%xline(8,'--')
%yline(80,'--')
%axis([min(uy) max(uy) min(heights) max(heights)]);
%xlabel('U_y, m/s')
%ylabel('Height, m')
%title('Time-averaged plot of velocity in the y-direction')

%figure
%plot(uz,heights)
%xline(8,'--')
%yline(80,'--')
%axis([min(uz) max(uz) min(heights) max(heights)]);
%xlabel('U_z, m/s')
%ylabel('Height, m')
%title('Time-averaged plot of velocity in the z-direction')