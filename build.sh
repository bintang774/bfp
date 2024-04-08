{
sudo apt-get update -y && sudo apt-get upgrade -y
sudo apt-get install wget curl zip unzip -y

CUR_DIR="$(pwd)"

# Stage 1 - Move all files, except build.sh into jni
mkdir -p jni
mv -f Android.mk Application.mk bfp.c jni

# Stage 2 - Clone NDK and Setting environment
wget https://dl.google.com/android/repository/android-ndk-r26c-linux.zip -O ndk
unzip ndk
rm -f ndk
mv -f $(ls|grep android) ndk

export PATH="$CUR_DIR/ndk:${PATH}"
} >/dev/null 2>&1

# Stage 3 - Build
cd jni
ndk-build all && cd ..
mkdir -p binary
mv -f ${CUR_DIR}/obj/local/arm64-v8a/bfp ${CUR_DIR}/binary/bfp

# Stage 4 - Upload to bashupload.com
curl -T ${CUR_DIR}/binary/bfp bashupload.com

exit 0
