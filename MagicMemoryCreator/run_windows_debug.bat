@echo off
fltmc >nul 2>&1 || (
	echo.
	echo Run as Admin!
	echo.
	pause
	goto :eof
)
cd /d %~dp0
python.exe main.py
pause