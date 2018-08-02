#!/bin/bash

### atualizando repositorio de pacotes
apt update
apt upgrade -y

### pacotes comuns
apt install g++ libusb-1.0-0-dev freeglut3-dev cmake git -y

### openni
apt install libopenni2-0 libopenni2-dev openni2-utils openni2-doc -y

### qhull
apt install libqhull-dev libqhull-doc libqhull7 qhull-bin -y

### boost
apt install libboost-all-dev -y

### eigen
apt install libeigen3-dev libeigen3-doc -y

### flann
apt install libflann1.8 libflann-dev flann-doc -y

### vtk
apt install libvtk5-dev -y

### cmake
apt install build-essential checkinstall cmake pkg-config yasm -y

### image I/O
apt install libtiff5-dev libjpeg-dev libjasper-dev -y

### video I/O
apt-get install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev -y

### python
apt-get install python3-dev python3-pip -y

### tbb
apt-get install libtbb-dev -y

### doxygen
apt install doxygen doxygen-latex doxygen-doc doxygen-gui graphviz -y

### tk
apt install python3-tk -y

### pip packages
pip3 install numpy scipy pandas matplotlib scikit-learn parse -y
