git clone https://github.com/DiCode77/TriLogic.git --recurse-submodules

cd TriLogic;

./vcpkg/bootstrap-vcpkg.sh;

./vcpkg/vcpkg install;

mkdir build && cd build;

cmake .. -G "Xcode";

xcodebuild -configuration Release;
