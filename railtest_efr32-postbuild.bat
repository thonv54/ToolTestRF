@echo off
setlocal enableextensions enabledelayedexpansion
SET allParams=%*
SET params=%allParams:' '=","%
SET params=%params:'="%
set /a count=0
for %%i in (%params%) do (
  set /a count+=1
  set param[!count!]=%%i
)

java -jar "C:\SiliconLabs\SimplicityStudio\v4\plugins\com.silabs.external.jython_2.7.0.201705012047-102\external_jython\2.7.0\jython-2.7.0.jar"  "C:\Users\RocketBoy\SimplicityStudio\v4_workspace\railtest_efr32\./railtest_efr32-postbuild.py" %param[1]%  %param[2]%  "" "" %param[3]% 