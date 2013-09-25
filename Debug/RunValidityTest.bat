del ValidityTest\* /Q

start .\ScaleSpace.exe -I "ValidityData/Edge" -s 2 1 -p cl -o "ValidityTest/in_edge" -m edge
start .\ScaleSpace.exe -I "ValidityData/Edge" -s 2 1 -p cv -o "ValidityTest/in_edge" -m edge
start .\ScaleSpace.exe -I "ValidityData/Edge" -s 2 1 -p cv_gpu -o "ValidityTest/in_edge" -m edge

start .\ScaleSpace.exe -I "ValidityData/Blob" -s 2 1 -p cl -o "ValidityTest/in_blob2" -m blob
start .\ScaleSpace.exe -I "ValidityData/Blob" -s 2 1 -p cv -o "ValidityTest/in_blob2" -m blob
start .\ScaleSpace.exe -I "ValidityData/Blob" -s 2 1 -p cv_gpu -o "ValidityTest/in_blob2" -m blob

start .\ScaleSpace.exe -I "ValidityData/Corner" -s 2 1 -p cl -o "ValidityTest/in_corner" -m corner
start .\ScaleSpace.exe -I "ValidityData/Corner" -s 2 1 -p cv -o "ValidityTest/in_corner" -m corner
start .\ScaleSpace.exe -I "ValidityData/Corner" -s 2 1 -p cv_gpu -o "ValidityTest/in_corner" -m corner

start .\ScaleSpace.exe -I "ValidityData/Ridge" -s 2 1 -p cl -o "ValidityTest/in_ridge" -m ridge
start .\ScaleSpace.exe -I "ValidityData/Ridge" -s 2 1 -p cv -o "ValidityTest/in_ridge" -m ridge
start .\ScaleSpace.exe -I "ValidityData/Ridge" -s 2 1 -p cv_gpu -o "ValidityTest/in_ridge" -m ridge

start .\ScaleSpace.exe -I "ValidityData/Pure" -s 2 1 -p cl -o "ValidityTest/in_pure"
start .\ScaleSpace.exe -I "ValidityData/Pure" -s 2 1 -p cv -o "ValidityTest/in_pure"
start .\ScaleSpace.exe -I "ValidityData/Pure" -s 2 1 -p cv_gpu -o "ValidityTest/in_pure"
