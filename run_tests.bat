@echo off
chdir "%~dp0/build/test"
call ctest
chdir "%~dp0"
PAUSE