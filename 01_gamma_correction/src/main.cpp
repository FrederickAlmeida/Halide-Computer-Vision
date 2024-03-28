// g++ -std=c++20 src/main.cpp -I$HALIDE_ROOT/include -I$HALIDE_ROOT/tools -L$HALIDE_ROOT/lib -lHalide -ljpeg `libpng-config --cflags --ldflags` -lz -ldl -ltinfo -o bin/main
// bin/main input/fread_anao.png 0 a.png
#include <iostream>
#include "Halide.h"
#include "halide_image_io.h"

using namespace std;
using namespace Halide;

using std::string;
using namespace Halide::Tools;
using namespace Halide::ConciseCasts;

int main(int argc, char **argv) {
    if (argc < 4) {
        cout << "Usage: bin/main <path/to/input> <gamma> <path/to/output>" << endl;
        return -1;
    }

    const string path_input = argv[1];
    const float gamma = atof(argv[2]);
    const string path_output = argv[3];

    Buffer<uint8_t> img_input = Tools::load_image(path_input);

    Buffer<uint8_t> img_output(
        img_input.width(),
        img_input.height(),
        img_input.channels()
    );

    Var x{"x"}, y{"y"}, c{"c"};

    Func img_f32{"img_32"};
    img_f32(x, y, c) = f32(img_input(x, y, c) / 255.0f);

    Func transformed{"transformed"};
    transformed(x, y, c) = pow(img_f32(x, y, c), gamma);

    Func gamma_corrected{"gamma_corrected"};
    gamma_corrected(x, y, c) = u8(255.0f * clamp(transformed(x, y, c), 0.0f, 1.0f));

    img_output = gamma_corrected.realize(
        {
            img_input.width(),
            img_input.height(),
            img_input.channels()
        }
    );

    Tools::save_image(img_output, path_output);

    return 0;
}