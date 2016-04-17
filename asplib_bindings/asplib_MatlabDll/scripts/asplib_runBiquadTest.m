% this script is used to create and test Biquads from asplib

%/*
% * Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
% * https://github.com/AchimTuran/asplib
% *
% * This file is part of asplib (Achim's Signal Processing LIBrary)
% *
% * asplib (Achim's Signal Processing LIBrary) is free software:
% * you can redistribute it and/or modify it under the terms of the
% * GNU General Public License as published by the Free Software Foundation,
% * either version 3 of the License, or (at your option) any later version.
% *
% * asplib (Achim's Signal Processing LIBrary) is distributed
% * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
% * without even the implied warranty of MERCHANTABILITY or FITNESS
% * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
% *
% * You should have received a copy of the GNU General Public License
% * along with asplib (Achim's Signal Processing LIBrary).
% * If not, see <http://www.gnu.org/licenses/>.
% *
% */

% reset workspace
clc
clear all;

% load asplib_MatlabDll
asplib_load_MatlabDll

% initialize asplib
fA = 44100;
FrameSize = 1024*20;
Channels = 1;
asplib_init(fA, Channels, FrameSize);

% create Biquads
MaxFrequencyBands = 10;
asplib_createBiquad(MaxFrequencyBands);

% create test signal
x = zeros(1, FrameSize);
x(1) = 1.0;
y_cut = zeros(1, FrameSize);
y_boost = zeros(1, FrameSize);

% set all Biquad Gain values to -12dB
asplib_setBiquadGains(-12);
y_cut = asplib_processBiquads(x);

% set all Biquad Gain values to +12dB
asplib_setBiquadGains(12);
y_boost = asplib_processBiquads(x);

% unload asplib_MatlabDll
asplib_unload_MatlabDll

% transform to frequency domain
% cut EQ
Y_cut=fft(y_cut);
Amplitude_cut = 20*log10(abs(Y_cut(1:FrameSize/2)));    %[dB]
Phase_cut = angle(Y_cut(1:FrameSize/2))*180/pi;         %Degree [°]
% boost EQ
Y_boost=fft(y_boost);
Amplitude_boost = 20*log10(abs(Y_boost(1:FrameSize/2)));    %[dB]
Phase_boost = angle(Y_boost(1:FrameSize/2))*180/pi;         %Degree [°]

% calculate frequency axis steps
df = fA/length(Y_cut);
freq = 0:df:df*(length(Y_cut)/2-1);

% plot results
figure(1);
subplot(2,1,1)
semilogx(freq, Amplitude_cut, 'b', 'Linewidth', 1.5)
title('Amplitude response of Constant-Q EQ from asplib Biquads-Module','FontWeight','normal','FontName','Arial','FontSize', 16)
xlabel('frequency [Hz]');
ylabel('Amplitude [dB]');
xlim([0 fA/2])
hold on;
semilogx(freq, Amplitude_boost, 'r', 'Linewidth', 1.5)
h = legend('Gain=-12dB (cut EQ)', 'Gain=+12dB (boost EQ)');
set(h,'location','northwest','orientation','vertical','FontWeight','normal','FontName','Arial','FontSize',12);
grid on;
box on;
hold off;


subplot(2,1,2)
semilogx(freq, Phase_cut, 'b', 'Linewidth', 1.5)
title('Phase response of Constant-Q EQ from asplib Biquads-Module','FontWeight','normal','FontName','Arial','FontSize', 16)
xlabel('frequency [Hz]');
ylabel('Phase [°]');
xlim([min(freq) max(freq)])
hold on;
semilogx(freq, Phase_boost, 'r', 'Linewidth', 1.5)
h = legend('Gain=-12dB (cut EQ)', 'Gain=+12dB (boost EQ)');
set(h,'location','northwest','orientation','vertical','FontWeight','normal','FontName','Arial','FontSize',12);
grid on;
hold off;
