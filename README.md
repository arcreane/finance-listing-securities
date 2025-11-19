# Euronext_Surveillance

Secure authentication module checking the validity of the identifier (C++ / Crow), granting the role and managing access with SQLite

## Dependence & link
- [GCC](https://sourceforge.net/projects/mingw/files/Installer/mingw-get-setup.exe/download)
- [Cmake](https://cmake.org/download/) 
- [QT](https://www.qt.io/download-qt-installer-oss)

## Install
After installing all the dependencie :
````
git clone https://github.com/AdamYv/Euronext_Surveillance.git
````

## Compile
The easiest way is to use clion , but you can try in the terminal like this :  

*Generate the make file*
````
cmake  \  
  -DCMAKE_BUILD_TYPE=Debug \
  -DCROW_INCLUDE_DIR=../crow/include \
  -DSQLITE3_INCLUDE_DIR=/usr/include \
  -DSQLITE3_LIBRARY=/usr/lib64/libsqlite3.so
````

*Compile*
````
make
````

*Exec*
````
./Euronext_surveillance
````

## Expected output 
![image](https://github.com/user-attachments/assets/8eed190a-92e4-4dae-97b5-3d4812e45eaa)



