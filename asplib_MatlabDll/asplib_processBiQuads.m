% this script is used to process created BiQuads

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


function [y] = asplib_processBiQuads(signal)
%TEST Summary of this function goes here
%   Detailed explanation goes here
	if not(libisloaded('asplib_MatlabDll'))
		disp('[asplib] asplib_MatlabDll is not loaded! Please run asplib_load_MatlabDll.m first!');
		return;
	end

	mSize = uint32(length(signal));
	pSignal = libpointer('singlePtr', signal);
	
	% ToDo evaluate err
	[err, y] = calllib('asplib_MatlabDll', 'process_BiQuads', pSignal, mSize);
end
