function read_T(filename)

% This function loads in a text file of planar averaged T from SOWFA



heights = [5 15 25 35 45 55 65 75 85 95 105 115 125 135 145 155 165 175 185 195 205 215 225 235 245 255 265 275 285 295 305 315 325 335 345 355 365 375 385 395 405 415 425 435 445 455 465 475 485 495 505 515 525 535 545 555 565 575 585 595 605 615 625 635 645 655 665 675 685 695 705 715 725 735 745 755 765 775 785 795 805 815 825 835 845 855 865 875 885 895 905 915 925 935 945 955 965 975 985 995 1005 1015 1025 1035 1045 1055 1065 1075 1085 1095 1105 1115 1125 1135 1145 1155 1165 1175 1185 1195 1205 1215 1225 1235 1245 1255 1265 1275];

% Reads in file and converts to array
T = readtable(filename);
T = table2array(T);

% Get times and dt data
times = T(:,1);
dt    = T(:,2);

% Set averaging period
startAvg = 20000;
endAvg = 25000;

% Find data indices for data to be averaged
idx1 = find(times > startAvg);
idx2 = find(times > endAvg);
i = 1;

% Remove unwanted data
T(idx2(1):end,:) = [];
T(1:idx1(1),:) = [];

% Compute averages 
columnMeans = mean(T,1);

plot(columnMeans(3:end),heights);
xlabel('Potential Temperature, K');
ylabel('Height, m');
title('Time-averaged profile of potential temperature (20000-25000s)');
axis([299 307 0 1300]);
yline(700,'--');
yline(800,'--');