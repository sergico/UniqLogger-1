::WARNING This file does not honour multi-space delimiters
@echo off
SETLOCAL=ENABLEDELAYEDEXPANSION

if not exist %1 mkdir %1

del /q %1\*.* 

