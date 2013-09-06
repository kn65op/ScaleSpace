Rem Test validity
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cl -o "ValidityTest/in_edge" -m edge
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv -o "ValidityTest/in_edge" -m edge
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv_gpu -o "ValidityTest/in_edge" -m edge

start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cl -o "ValidityTest/in_blob2" -m blob
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv -o "ValidityTest/in_blob2" -m blob
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv_gpu -o "ValidityTest/in_blob2" -m blob

start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cl -o "ValidityTest/in_corner" -m corner
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv -o "ValidityTest/in_corner" -m corner
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv_gpu -o "ValidityTest/in_corner" -m corner

start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cl -o "ValidityTest/in_ridge" -m ridge
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv -o "ValidityTest/in_ridge" -m ridge
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv_gpu -o "ValidityTest/in_ridge" -m ridge

start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cl -o "ValidityTest/in_pure"
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv -o "ValidityTest/in_pure"
start .\ScaleSpace.exe -I ValidityData/cos -s 2 1 -p cv_gpu -o "ValidityTest/in_pure"

Rem Test performance

.\ScaleSpace.exe -I PerformanceData/cos -p cl -o "PerformanceTest/lena_edge" -s 2 11 -m edge --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv -o "PerformanceTest/lena_edge" -s 2 11 -m edge --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv_gpu -o "PerformanceTest/lena_edge" -s 2 11 -m edge --no-show

.\ScaleSpace.exe -I PerformanceData/cos -p cl -o "PerformanceTest/lena_blob" -s 2 11 -m blob --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv -o "PerformanceTest/lena_blob" -s 2 11 -m blob --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv_gpu -o "PerformanceTest/lena_blob" -s 2 11 -m blob --no-show

.\ScaleSpace.exe -I PerformanceData/cos -p cl -o "PerformanceTest/lena_corner" -s 2 11 -m corner --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv -o "PerformanceTest/lena_corner" -s 2 11 -m corner --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv_gpu -o "PerformanceTest/lena_corner" -s 2 11 -m corner --no-show

.\ScaleSpace.exe -I PerformanceData/cos -p cl -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv_gpu -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge --no-show

.\ScaleSpace.exe -I PerformanceData/cos -p cl -o "PerformanceTest/lena_pure" -s 2 11 --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv -o "PerformanceTest/lena_pure" -s 2 11 --no-show
.\ScaleSpace.exe -I PerformanceData/cos -p cv_gpu -o "PerformanceTest/lena_pure" -s 2 11 --no-show