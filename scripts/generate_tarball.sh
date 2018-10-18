#! /bin/bash

NAME=$1
ENU_PREFIX=${PREFIX}/${SUBPREFIX}
mkdir -p ${PREFIX}/bin/
#mkdir -p ${PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${ENU_PREFIX}/bin 
mkdir -p ${ENU_PREFIX}/licenses/enumivo
#mkdir -p ${ENU_PREFIX}/include
#mkdir -p ${ENU_PREFIX}/lib/cmake/${PROJECT}
#mkdir -p ${ENU_PREFIX}/cmake
#mkdir -p ${ENU_PREFIX}/scripts

# install binaries 
cp -R ${BUILD_DIR}/bin/* ${ENU_PREFIX}/bin 

# install licenses
cp -R ${BUILD_DIR}/licenses/enumivo/* ${ENU_PREFIX}/licenses

# install libraries
#cp -R ${BUILD_DIR}/lib/* ${ENU_PREFIX}/lib

# install cmake modules
#sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/EnumivoTesterPackage.cmake &> ${ENU_PREFIX}/lib/cmake/${PROJECT}/EnumivoTester.cmake
#sed "s/_PREFIX_/\/${SPREFIX}\/${SSUBPREFIX}/g" ${BUILD_DIR}/modules/${PROJECT}-config.cmake.package &> ${ENU_PREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake

# install includes
#cp -R ${BUILD_DIR}/include/* ${ENU_PREFIX}/include

# make symlinks
#pushd ${PREFIX}/lib/cmake/${PROJECT} &> /dev/null
#ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake ${PROJECT}-config.cmake
#ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/EnumivoTester.cmake EnumivoTester.cmake
#popd &> /dev/null

pushd ${PREFIX}/bin &> /dev/null
for f in `ls ${BUILD_DIR}/bin/`; do
   bn=$(basename $f)
   ln -sf ../${SUBPREFIX}/bin/$bn $bn
done
popd &> /dev/null

tar -cvzf $NAME ./${PREFIX}/*
rm -r ${PREFIX}
