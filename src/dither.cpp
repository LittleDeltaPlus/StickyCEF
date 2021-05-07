//
// Created by lildeltaplus on 08/03/2021.
//
#include <vector>

unsigned char SaturatedAdd(int val1, int val2){
    int tmp = val1 + val2;
    if (tmp > 255)
        return char(255);
    else if (tmp < 0)
        return char(0);
    else
        return char(tmp);
}

std::vector<unsigned char> DitherImg(std::vector<unsigned char> input, int width, int height) {
    std::vector<unsigned char> output(input);
    int err;
    int a,b,c,d;

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(input[i*width+j] > 127)
            {
                err = input[i*width+j] - 255;
                output[i*width+j] = 255;
            }
            else
            {
                err = output[i*width+j] - 0;
                output[i*width+j] = 0;
            }

            a = (err * 7) / 16;
            b = (err * 1) / 16;
            c = (err * 5) / 16;
            d = (err * 3) / 16;

            if((i != (height-1)) && (j != 0) && (j != (width - 1)))
            {
                output[i*width+j+1] = SaturatedAdd(int(output[i*width+j+1]),a);
                output[(i+1)*width+j+1] = SaturatedAdd(int(output[(i+1)*width+j+1]),b);
                output[(i+1)*width+j] = SaturatedAdd(int(output[(i+1)*width+j]),c);
                output[(i+1)*width+j-1] = SaturatedAdd(int(output[(i+1)*width+j-1]),d);
            }
        }
    }
    return output;
}