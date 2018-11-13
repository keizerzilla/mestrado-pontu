#!/bin/bash

cloud=../../datasets/bosphorus-outlier-densit200/nonneutral/bs000_E_HAPPY_0.xyz
endpoints=../../dump/endpoints.xyz
slice=../../dump/slice.xyz
cloudpcd=../../datasets/bosphorus-outlier-densit200/nonneutral/bs000_E_HAPPY_0.pcd
endpointspcd=../../dump/endpoints.pcd
slicepcd=../../dump/slice.pcd

clear
make
../../bin/alydist $cloud $slice $endpoints
echo ">>> convertendo slice"
pcl_xyz2pcd $slice $slicepcd
echo ">>> convertendo endpoints"
pcl_xyz2pcd $endpoints $endpointspcd
echo ">>> visualizando"
#pcl_viewer $cloudpcd -ps 2 -fc 127,127,127 $slicepcd -ps 6 -fc 0,0,255 $endpointspcd -ps 8 -fc 255,0,0
pcl_viewer $slicepcd -ps 6 -fc 0,0,255 $endpointspcd -ps 8 -fc 255,0,0
