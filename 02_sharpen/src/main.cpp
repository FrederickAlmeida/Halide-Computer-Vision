// make bin/sharpen.generator mkdir -p bin g++ src/registration/HalideSharpen.cpp $HALIDE_ROOT/tools/GenGen.cpp -std=c++20 -fno-rtti -I$HALIDE_ROOT/include -I$HALIDE_ROOT/tools -Iinclude -Ibin -L$HALIDE_ROOT/lib -lHalide -lpthread -ldl -lz -ltinfo -o bin/sharpen.generator
// g++ src/registration/HalideSharpen.cpp $HALIDE_ROOT/tools/GenGen.cpp -std=c++20 -fno-rtti -I$HALIDE_ROOT/include -I$HALIDE_ROOT/tools -Iinclude -Ibin -L$HALIDE_ROOT/lib -lHalide -lpthread -ldl -lz -ltinfo -o bin/sharpen.generator
// g++ src/main.cpp bin/sharpen_simple.o bin/sharpen_full.o -std=c++20 -fno-rtti -I$HALIDE_ROOT/include -I$HALIDE_ROOT/tools -Iinclude -Ibin -L$HALIDE_ROOT/lib -lHalide -lpthread -ldl -lz -ltinfo -ljpeg `libpng-config --cflags --ldflags` -o bin/main
#include <iostream>
#include "HalideBuffer.h"
#include "halide_image_io.h"
#include "sharpen_simple.h"
#include "sharpen_full.h"

using std::cout;
using std::endl;
using std::string;

using namespace Halide::Runtime;
using namespace Halide::Tools;

int main(int argc, char ** argv) {
    if (argc < 4) {
        cout << "Usage: bin/main <path/to/input> <path/to/output/simple> <path/to/output/full>";
        return -1;
    }

    const string path_input = argv[1];
    const string path_output_simple = argv[2];
    const string path_output_full = argv[3];

    Buffer<uint8_t> img_input = load_image(path_input);
    Buffer<uint8_t> output_simple = Buffer<u_int8_t>::make_with_shape_of(img_input);
    Buffer<uint8_t> output_full = Buffer<u_int8_t>::make_with_shape_of(img_input);

    sharpen_simple(img_input, output_simple);
    sharpen_full(img_input, output_full);

    save_image(output_simple, path_output_simple);
    save_image(output_full, path_output_full);

    return 0;
}