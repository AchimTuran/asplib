::/*
:: * Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
:: * https://github.com/AchimTuran/asplib
:: *
:: * This file is part of asplib (Achim's Signal Processing LIBrary)
:: *
:: * asplib (Achim's Signal Processing LIBrary) is free software:
:: * you can redistribute it and/or modify it under the terms of the
:: * GNU General Public License as published by the Free Software Foundation,
:: * either version 3 of the License, or (at your option) any later version.
:: *
:: * asplib (Achim's Signal Processing LIBrary) is distributed
:: * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
:: * without even the implied warranty of MERCHANTABILITY or FITNESS
:: * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
:: *
:: * You should have received a copy of the GNU General Public License
:: * along with asplib (Achim's Signal Processing LIBrary).
:: * If not, see <http://www.gnu.org/licenses/>.
:: *
:: */


:: %1% apslib root path
:: %2% asplib MatlabDll path
:: %3% Matalb workspace folder
:: %4% asplib_MatlabDll file path

@ECHO OFF

IF "%1%"=="" GOTO ERROR
IF "%2%"=="" GOTO ERROR
IF "%3%"=="" GOTO ERROR
IF "%4%"=="" GOTO ERROR

:: set path variables
SET ASPLIB_ROOT=%1%
SET ASPLIB_MATLABDLL_ROOT=%2%
SET MATLAB_WORKSPACE=%3%\asplib_MatlabDll
SET ASPLIB_TARGET_DLL=%4%

:: show path variables on the console
ECHO asplib root path: %ASPLIB_ROOT%
ECHO asplib MatlabDll sources path: %ASPLIB_MATLABDLL_ROOT%
ECHO matlab workspace path: %MATLAB_WORKSPACE%
ECHO asplib_MatlabDll Target: %ASPLIB_TARGET_DLL%

ECHO ----copy source files from project path to matlab path----

:: delete old files and create folders if needed
IF NOT EXIST %MATLAB_WORKSPACE% ( 
	MKDIR %MATLAB_WORKSPACE%
) ELSE (
	DEL %MATLAB_WORKSPACE%\*.* /s /q
	ECHO deleting old asplib_MatlabDll files
)

IF NOT EXIST %MATLAB_WORKSPACE%\bin ( 
	MKDIR %MATLAB_WORKSPACE%\bin
) ELSE (
	DEL %MATLAB_WORKSPACE%\bin\*.* /s /q
	ECHO deleting old asplib_Matlab.dll
)

:: copy relevant files to matlab workspace
::/A for ASCI-files and /B for binary files
COPY /A %ASPLIB_MATLABDLL_ROOT%\asplib_MatlabDll.h %MATLAB_WORKSPACE%\bin\asplib_MatlabDll.h
COPY /A %ASPLIB_MATLABDLL_ROOT%\typedefs.h %MATLAB_WORKSPACE%\bin\typedefs.h
COPY /B %ASPLIB_TARGET_DLL% %MATLAB_WORKSPACE%\bin\asplib_Matlab.dll

COPY /A %ASPLIB_MATLABDLL_ROOT%\*.m %MATLAB_WORKSPACE%\*.m

ECHO ----finish copying source files from project path to matlab path----
GOTO END

:: process errors
:ERROR
ECHO Error! Invalid path arguments!
SET errStr="syntax is COPYFiles.bat <apslib root path> <asplib MatlabDll path> <Matalb workspace folder>"
ECHO %errStr%

:END
