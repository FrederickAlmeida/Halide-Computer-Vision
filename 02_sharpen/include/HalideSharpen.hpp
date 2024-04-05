#ifndef HALIDE_SHARPEN_HPP
#define HALIDE_SHARPEN_HPP

#include "Halide.h"

using namespace Halide;
using namespace Halide::ConciseCasts;

enum Mode {
    SIMPLE = 0,
    FULL
};

class HalideSharpen : public Generator<HalideSharpen> {
    public:
        // adicionar inputs
        Input<Buffer<uint8_t>> img_input{"img_input", 3};

        // adicionar GeneratorParams
        GeneratorParam<enum Mode> mode{"mode", SIMPLE, {
            {"simple", SIMPLE},
            {"full", FULL}
        }};

        // adicionar outputs
        Output<Buffer<uint8_t>> img_output{"img_output", 3};

        void generate() {
            // adicionar padding e converter para int16
            img_bound = BoundaryConditions::constant_exterior(img_input, 0);
            img_bound_i16(x, y, c) = i16(img_bound(x, y, c));

            Expr result;
            switch (mode) {
                case SIMPLE:
                    result = 5 * img_bound_i16(x, y, c) 
                            - img_bound_i16(x, y - 1, c) - img_bound_i16(x, y + 1, c)
                            - img_bound_i16(x - 1, y, c) - img_bound_i16(x + 1, y, c);

                    break;
                
                case FULL:
                    result = 9 * img_bound_i16(x, y, c) 
                            - img_bound_i16(x, y - 1, c) - img_bound_i16(x, y + 1, c)
                            - img_bound_i16(x - 1, y, c) - img_bound_i16(x + 1, y, c) 
                            - img_bound_i16(x - 1, y - 1, c) - img_bound_i16(x + 1, y - 1, c)
                            - img_bound_i16(x - 1, y + 1, c) - img_bound_i16(x + 1, y + 1, c);
                default:
                    break;
            }

            img_output(x, y, c) = u8(clamp(result, 0, 255));
        }

    private:
        Var x{"x"}, y{"y"}, c{"c"};
        Func img_bound{"img_bound"}, img_bound_i16{"img_bound_i16"};

};

#endif
