del ValidityTest\* /Q

start .\ScaleSpace.exe -I "ValidityData/Edge" -s 4 10 -p cl -o "ValidityTest/in_edge" -m edge
start .\ScaleSpace.exe -I "ValidityData/Edge" -s 4 10 -p cv -o "ValidityTest/in_edge" -m edge
start .\ScaleSpace.exe -I "ValidityData/Edge" -s 4 10 -p cv_gpu -o "ValidityTest/in_edge" -m edge

timeout 60

start .\ScaleSpace.exe -I "ValidityData/Blob" -s 4 10 -p cl -o "ValidityTest/in_blob" -m blob
start .\ScaleSpace.exe -I "ValidityData/Blob" -s 4 10 -p cv -o "ValidityTest/in_blob" -m blob
start .\ScaleSpace.exe -I "ValidityData/Blob" -s 4 10 -p cv_gpu -o "ValidityTest/in_blob" -m blob

timeout 60

start .\ScaleSpace.exe -I "ValidityData/Corner" -s 4 10 -p cl -o "ValidityTest/in_corner" -m corner
start .\ScaleSpace.exe -I "ValidityData/Corner" -s 4 10 -p cv -o "ValidityTest/in_corner" -m corner
start .\ScaleSpace.exe -I "ValidityData/Corner" -s 4 10 -p cv_gpu -o "ValidityTest/in_corner" -m corner

timeout 60

start .\ScaleSpace.exe -I "ValidityData/Ridge" -s 4 10 -p cl -o "ValidityTest/in_ridge" -m ridge
start .\ScaleSpace.exe -I "ValidityData/Ridge" -s 4 10 -p cv -o "ValidityTest/in_ridge" -m ridge
start .\ScaleSpace.exe -I "ValidityData/Ridge" -s 4 10 -p cv_gpu -o "ValidityTest/in_ridge" -m ridge

timeout 60

start .\ScaleSpace.exe -I "ValidityData/Pure" -s 4 10 -p cl -o "ValidityTest/in_pure"
start .\ScaleSpace.exe -I "ValidityData/Pure" -s 4 10 -p cv -o "ValidityTest/in_pure"
start .\ScaleSpace.exe -I "ValidityData/Pure" -s 4 10 -p cv_gpu -o "ValidityTest/in_pure"

timeout 60
