%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ISS Project
% Michal Ormos | xormos00@stud.fit.vutbr.cz
% 18.12.2015
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

iptsetpref('UseIPPL', false);
step = imread('xormos00.bmp');
step1_org = imread('xormos00.bmp');
step_out = imread('xormos00.bmp');

%ZAOSTRENI OBRAZU
%%%%%%%%%%%%%%%%%
step1_output = 'step1.bmp'
matrix = [ -0.5 -0.5 -0.5; -0.5 5 -0.5; -0.5 -0.5 -0.5];
step1_final = imfilter(step_out, matrix); % focusing
imwrite(step1_final, step1_output); % saving 

% OTOCENIE OBRAZU
%%%%%%%%%%%%%%%%%
step_out = 'step2.bmp'; % output name to variable
rotate_step2 = fliplr(step1_final); %rotation
imwrite(rotate_step2, step_out); % saving picture

% MEDIANOVY FILTER
%%%%%%%%%%%%%%%%%%
step_out = 'step3.bmp';
median_step3 = medfilt2(rotate_step2, [5 5]); % median filter
imwrite(median_step3, step_out); % saving

% ROZMAZANIE OBRAZU
%%%%%%%%%%%%%%%%%%%
step_out = 'step4.bmp';
blurring_matrix = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1] /49;
blurring_step4 = imfilter(median_step3, blurring_matrix);
imwrite(blurring_step4, step_out); % saving

% CHYBA V OBRAZE
%%%%%%%%%%%%%%%%
compare_step = fliplr(blurring_step4);
compare_step_double = im2double(compare_step);
step4_org_double = im2double(step);
error = 0;

step_size = size(step);
xstep_size = min(step_size);
ystep_size = max(step_size);

for(i = 1: xstep_size)
    for(j = 1: ystep_size)
        error = error + abs(step4_org_double(i, j) - compare_step_double(i, j));
    end;
end;

chyba = (error / (xstep_size * ystep_size)) * 255

% ROZTAZENI HISTORGAMU
%%%%%%%%%%%%%%%%%%%%%%
blurring_step4_double = im2double(blurring_step4);
step_out = 'step5.bmp';

min_step = min(blurring_step4_double);
inputL = min(min_step);
max_step = max(blurring_step4_double);
inputH = max(max_step);

outputL = 0.0;
outputH = 1.0;

histogram = imadjust(blurring_step4, [inputL inputH], [outputL, outputH]);
imwrite(histogram, step_out);



% SPOCITANIE STREDNEJ HODNOTY A SMERODATNE ODCHYLKY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
blurring_step4_double = im2double(blurring_step4);
mean_no_hist = mean2(blurring_step4_double) * 255
std_no_hist = std2(blurring_step4_double) * 255

histogram_double = im2double(histogram);
mean_hist = mean2(histogram_double) * 255
std_hist = std2(histogram_double) * 255

% KVANTIZACE OBRAZU
%%%%%%%%%%%%%%%%%%%
N = 2; a = 0; b = 255;

step_out = 'step6.bmp';
quantization_out = zeros(xstep_size, ystep_size);
tmp = double(histogram);
for(i = 1: xstep_size)
    for(j = 1: ystep_size)
        quantization_out(i, j) = round(((2^N)-1)*(tmp(i, j)-a)/(b-a))*(b-a)/((2^N)-1) + a;
    end;
end;

quantization_out_final = uint8(quantization_out);
imwrite(quantization_out_final, step_out);



