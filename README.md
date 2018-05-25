# BlueVM
BlueVM is a stack-based virtual machine. It is fast and simple to use.

## Building
CMake variables:
 * `LINK_LIBM` - set this to TRUE/1 if our compiler requires us to link libm in order to use `math.h` header
 * `BUILD_EXAMPLES` - should we automatically build all the examples? note: make sure you already built library
 * `BLUEVM_PATH` - root directory of our library. only necessary if we are building examples (so that we can find .a/.lib files)

## Examples 
Check the `examples` directory.