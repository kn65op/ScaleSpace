@echo off

Rem Test validity

echo Testing validity

call RunValidityTest.bat

echo Waiting for finish all validity tests
echo After all new windows close you can stop the timer
echo If you stop it before it can affect performance tests

timeout 10

Rem Test performance

echo Testing performance

call RunPerformanceTest.bat

