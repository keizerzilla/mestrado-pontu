# INSTALAÇÃO OPENCV NO UBUNTU 16.04+
Guia rápido para configurar o OpenCV no Ubuntu 16.04 ou superior.

## PACOTES NECESSÁRIOS
### cmake
```apt-get install build-essential checkinstall cmake pkg-config yasm```
### image I/O
```apt-get install libtiff5-dev libjpeg-dev libjasper-dev```
### video I/O
```apt-get install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev```
### python
```apt-get install python3-dev python-numpy```
### tbb
```apt-get install libtbb-dev```

## COMPILAÇÃO
Baixa a release mais recente em https://github.com/opencv/opencv/releases e depois prossiga com os comandos abaixo:

```
unzip opencv-<version>.zip
cd opencv-<version>/
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_GTK=ON -D WITH_OPENGL=ON ..
make
sudo make install
```

## TESTE
Note que dentro do diretório ```release``` existe um diretório ```bin```. Nele você encontrará vários programas já compilados para teste. Entre em ```bin``` e execute ```./opencv_test_core```. Se nenhum erro for disparado até o final da execução, sua instalação do OpenCV foi um sucesso. Se ocorrer algum erro, use o Google para achar a solução.
