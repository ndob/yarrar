#/bin/sh
echo "Trying to build in headless mode..."
cd Xcode
SKIP_PACKAGE_SCRIPT=1 xcodebuild -project Yarrar.xcodeproj/
cd ..
echo "Build done."
