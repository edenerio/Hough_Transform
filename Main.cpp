#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
using namespace std;

class HoughTransform{
    public:
        int numRows, numCols, minVal, maxVal, houghDist, houghAngle, angleInDegree, offSet;
        int **imgAry;
        int **cartesianHoughAry;
        int **polarHoughAry;
        double angleInRadians;
        double offSet;
    
    HoughTransform(int nr, int nc, int minv, int maxv){
        numRows = nr;
        numCols = nc;
        minVal = minv;
        maxVal = maxv;
        houghDist = 2 * sqrt((nr*nr) + (nc*nc));
        houghAngle = 180;
        offSet = sqrt(nr*nc);
        imgAry = new int*[nr];
        cartesianHoughAry = new int*[houghDist];
        polarHoughAry = new int*[houghDist];
        for(int i=0; i<nr; i++){
            imgAry[i] = new int[nc]();
            cartesianHoughAry[i] = new int[houghAngle]();
            polarHoughAry[i] = new int[houghAngle](); 
        }
    }

    ~HoughTransform(){
        delete[] imgAry;
        delete[] cartesianHoughAry;
        delete[] polarHoughAry;
        for(int i=0; i<numRows; i++){
            delete[] imgAry[i];
            delete[] cartesianHoughAry[i];
            delete[] polarHoughAry[i];
        }
    }

    void loadImage(ifstream& in, int **ary){
        for(int i=1; i<numRows; i++){
            for(int j=1; j<numCols; j++){
                in >> ary[i][j];
            }
        }
    }

    void buildHoughSpace(){
        for(int x=0; x<numRows; x++){
            for(int y=0; y<numCols; y++){
                if(imgAry[x][y]){
                    computeSinusoid(x, y);
                }
            }
        }
    }

    void computeSinusoid(int x, int y){
        angleInDegree = 0;
        double dist = 0.0;
        int distInt = 0;
        while(angleInDegree <= 179){
            angleInRadians = angleInDegree / 180.00 * M_PI;
            dist = cartesianDist(x, y, angleInRadians);
            distInt = (int)dist;
            cartesianHoughAry[distInt][angleInDegree]++;
            dist = polarDist(x, y, angleInRadians);
            distInt = (int)dist;
            polarHoughAry[distInt][angleInDegree]++;
            angleInDegree++;
        }
    }

    double cartesianDist(int x, int y, double angle){
        double xdouble = (double)x;
        double ydouble = (double)y;
        double t = angle - atan(ydouble/xdouble) - M_PI/2;
        return sqrt((xdouble*xdouble) + (ydouble*ydouble)) * cos(t) + offSet;
    }

    double polarDist(int x, int y, double angle){
        double xdouble = (double)x;
        double ydouble = (double)y;
        return (xdouble*cos(angle)) + (ydouble*sin(angle)) + offSet;
    }

    void prettyPrint(int **arr, ofstream& out){
        //algo in previous projects

        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(arr[i][j] == 0){
                    out << ". ";
                }else{
                    out << arr[i][j] << " ";
                }
            }
            out << endl;
        }
    }
};

int main(int argc, char *argv[]){

    return 0;
}