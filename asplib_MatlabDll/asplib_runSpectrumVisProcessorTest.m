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
asplib_load_MatlabDll()

frameSize = 128;
fA = 44100;
f0 = fA/frameSize*10.0;
t = 0:1/fA:frameSize/fA;
A = 1.0;
x = A*sin(2*pi*f0*t);
figure(1)
plot(t, x)
figure(2)
X = abs(fft(x));
bar(X(1:frameSize/2));


asplib_createSpectrumVisProcessor(frameSize)

y = asplib_processSpectrumVisProcessor(x);

asplib_destroySpectrumVisProcessor()

% unload asplib_MatlabDll
asplib_unload_MatlabDll()

figure(3)
plot(y)
