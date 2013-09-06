Rem Test validity
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cl -o "ValidityTest/in_edge" -m edge
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv -o "ValidityTest/in_edge" -m edge
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv_gpu -o "ValidityTest/in_edge" -m edge

.\ScaleSpace.exe -i in.bmp -s 2 1 -p cl -o "ValidityTest/in_blob2" -m blob
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv -o "ValidityTest/in_blob2" -m blob
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv_gpu -o "ValidityTest/in_blob2" -m blob

.\ScaleSpace.exe -i in.bmp -s 2 1 -p cl -o "ValidityTest/in_corner" -m corner
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv -o "ValidityTest/in_corner" -m corner
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv_gpu -o "ValidityTest/in_corner" -m corner

.\ScaleSpace.exe -i in.bmp -s 2 1 -p cl -o "ValidityTest/in_ridge" -m ridge
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv -o "ValidityTest/in_ridge" -m ridge
.\ScaleSpace.exe -i in.bmp -s 2 1 -p cv_gpu -o "ValidityTest/in_ridge" -m ridge
 
.\ScaleSpace.exe -i lena.jpg -p cl -o "ValidityTest/lena"
.\ScaleSpace.exe -i lena.jpg -p cv -o "ValidityTest/lena"
.\ScaleSpace.exe -i lena.jpg -p cv_gpu -o "ValidityTest/lena"

Rem Test performance

.\ScaleSpace.exe -i lena.jpg -p cl -o "PerformanceTest/lena_edge" -s 2 11 -m edge
.\ScaleSpace.exe -i lena.jpg -p cv -o "PerformanceTest/lena_edge" -s 2 11 -m edge
.\ScaleSpace.exe -i lena.jpg -p cv_gpu -o "PerformanceTest/lena_edge" -s 2 11 -m edge

.\ScaleSpace.exe -i lena.jpg -p cl -o "PerformanceTest/lena_blob" -s 2 11 -m blob
.\ScaleSpace.exe -i lena.jpg -p cv -o "PerformanceTest/lena_blob" -s 2 11 -m blob
.\ScaleSpace.exe -i lena.jpg -p cv_gpu -o "PerformanceTest/lena_blob" -s 2 11 -m blob

.\ScaleSpace.exe -i lena.jpg -p cl -o "PerformanceTest/lena_edge" -s 2 11 -m corner
.\ScaleSpace.exe -i lena.jpg -p cv -o "PerformanceTest/lena_edge" -s 2 11 -m corner
.\ScaleSpace.exe -i lena.jpg -p cv_gpu -o "PerformanceTest/lena_edge" -s 2 11 -m corner

.\ScaleSpace.exe -i lena.jpg -p cl -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge
.\ScaleSpace.exe -i lena.jpg -p cv -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge
.\ScaleSpace.exe -i lena.jpg -p cv_gpu -o "PerformanceTest/lena_ridge" -s 2 11 -m ridge