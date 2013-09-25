del PerformanceTest\* /Q

.\ScaleSpace.exe -I "PerformanceData/cos" -p cl -o "PerformanceTest/lena_edge" -s 2 11 -m edge --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv -o "PerformanceTest/lena_edge" -s 2 11 -m edge --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv_gpu -o "PerformanceTest/lena_edge" -s 2 11 -m edge --no-show -q --no-first-image

.\ScaleSpace.exe -I "PerformanceData/cos" -p cl -o "PerformanceTest/lena_blob" -s 2 11 -m blob --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv -o "PerformanceTest/lena_blob" -s 2 11 -m blob --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv_gpu -o "PerformanceTest/lena_blob" -s 2 11 -m blob --no-show -q --no-first-image

.\ScaleSpace.exe -I "PerformanceData/cos" -p cl -o "PerformanceTest/lena_corner" -s 2 11 -m corner --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv -o "PerformanceTest/lena_corner" -s 2 11 -m corner --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv_gpu -o "PerformanceTest/lena_corner" -s 2 11 -m corner --no-show -q --no-first-image

.\ScaleSpace.exe -I "PerformanceData/cos" -p cl -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv_gpu -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge --no-show -q --no-first-image

.\ScaleSpace.exe -I "PerformanceData/cos" -p cl -o "PerformanceTest/lena_pure" -s 2 11 --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv -o "PerformanceTest/lena_pure" -s 2 11 --no-show -q --no-first-image
.\ScaleSpace.exe -I "PerformanceData/cos" -p cv_gpu -o "PerformanceTest/lena_pure" -s 2 11 --no-show -q --no-first-image