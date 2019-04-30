#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

void Read2DUCharImg(char *filename, unsigned char **img, int dim_x, int dim_y) {
    FILE *fp;
    int i;

    fp = fopen(filename, "rb");

    for (i = 0; i < dim_y; i++){
        fread(img[i], sizeof(unsigned char), dim_x, fp);
    }

    fclose(fp);
}

void Write2DUCharImg(char *filename, unsigned char **img, int dim_x, int dim_y)
{
    int i;
    FILE *fp;
    fp = fopen(filename, "wb");

    for (i = 0; i < dim_y; i++)
        fwrite(img[i], sizeof(unsigned char), dim_x, fp);
    fclose(fp);
}

unsigned char **UCalloc2d(int i_size, int j_size)
{
    unsigned char **array;
    int i;

    array = (unsigned char **)calloc(i_size, sizeof(unsigned char *));

    for (i = 0; i < i_size; i++)
        array[i] = (unsigned char *)calloc(j_size, sizeof(unsigned char));

    return(array);
}

void UCfree2d(unsigned char **array, int i_size)
{
    int i;

    for (i = 0; i < i_size; i++)
        free(array[i]);

    free(array);
}

void feature5x5(unsigned char **orig, int dim_x, int dim_y, string filename){
    std::ofstream ofs;
    ofs.open(filename + ".txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    std::ofstream outfile;
    outfile.open(filename + ".txt", std::ios_base::app);
    bool zeros = true;
    bool back = false;
    string out;
    int count = 0;

    for (int i = 0; i < dim_y; i++) {
        for (int j = 0; j < dim_x; j++) {
            if (orig[i][j] == 0){
                out = "-1 ";
            }else{
                out = "1 ";
            }
            //Not on outer edges
            if(i >= 2 && j >= 2 && i < dim_y - 2 && j < dim_x - 2){
                //printf("%d", orig[i][j]);

                for (int k = i - 2; k < i + 3; k++) {
                    for (int l = j - 2; l < j + 3; l++) {
                        if(orig[k][l] == 255){
                            out = out + "" + std::to_string(count) + ":" + std::to_string(1) + " ";
                        }
                        count++;
                    }
                }
                count = 0;
                out = out + "\n";
                printf("%s", out.c_str());
                outfile << out;
            }else{//in this case we know our image has black on the borders
                out = out + "\n";
                printf("%s", out.c_str());
                outfile << out;
            }
        }
    }
}

void predicted(int dim_x, int dim_y, string filename){
    unsigned char **pcircle;
    char output[100];
    pcircle = UCalloc2d(dim_y, dim_x);
    std::ifstream in(filename);
    std::string str;
    int i = 0;
    int j = 0;
    while (std::getline(in, str))
    {
        printf("%s\n", str.c_str());
        stringstream val(str);
        int x = 0;
        val >> x;
        if(x == -1){
            pcircle[i][j] = (unsigned char)0;
        }else if(x == 1){
            pcircle[i][j] = (unsigned char)255;
        }
        if(j < 45, i < 45){
            j++;
        }else{
            j = 0;
            i++;
        }
    }

    string filepath = "circle_predicted.raw";
    strcpy(output, filepath.c_str());
    Write2DUCharImg(output, pcircle, dim_x, dim_y);
    //UCfree2d(pcircle, dim_y);

}

int main(int argc, char *argv[]){


    unsigned char **circle;
    char circlefilename[100], pcirclefilename[100];
    FILE *fp;
    int dim_x_circle, dim_y_circle;

    string circlename = "circle45x45";

    //circle        45 x 45

    dim_x_circle = 45; dim_y_circle = 45;

    circle = UCalloc2d(dim_y_circle, dim_x_circle);

    strcpy(circlefilename, "/home/quincy/CLionProjects/untitled1/circle45x45.raw");
    strcpy(pcirclefilename, "/home/quincy/CLionProjects/untitled1/testresult.txt");

    Read2DUCharImg(circlefilename, circle, dim_x_circle, dim_y_circle);

    feature5x5(circle, dim_x_circle, dim_y_circle, circlename);
    predicted(dim_x_circle, dim_y_circle, pcirclefilename);

    UCfree2d(circle, dim_y_circle);

    return EXIT_SUCCESS;
}