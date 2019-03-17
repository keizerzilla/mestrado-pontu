# INSTALAÇÃO PCL NO UBUNTU 18.04+
Guia rápido para configurar o PCL no Ubuntu 18.04 ou superior.

## PACOTES NECESSÁRIOS
```apt-get install g++ cmake doxygen mpi-default-dev openmpi-bin openmpi-common libflann-dev libeigen3-dev libboost-all-dev libvtk7.1-qt libvtk7.1 libvtk7-dev libusb-dev libgtest-dev git freeglut3-dev pkg-config build-essential libxmu-dev libxi-dev libusb-1.0-0-dev graphviz mono-complete openjdk-11-jdk openjdk-11-jre qtdevlarative5-dev phonon-backend-gstreamer phonon-backend-vlc```

## COMPILAÇÃO
Baixe a release mais recente em https://github.com/PointCloudLibrary/pcl/releases e depois prossiga com os comandos abaixo:

```
unzip pcl-pcl-<version>.zip
cd pcl-pcl-<version>/
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=None -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_GPU=ON -D BUILD_apps=ON -D BUILD_examples=ON ..

make
sudo make install
```


## TESTANDO INSTALAÇÃO
Dentro do diretório ```release``` existe um diretório ```bin```. Nele você encontrará vários programas já compilados para teste.
