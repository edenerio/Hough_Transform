#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
using namespace std;

class HoughTransform{
    public:
        int numRows, numCols, minVal, maxVal, houghDist, houghAngle, angleInDegree, cartMax, polMax;
        int **imgAry;
        int **cartesianHoughAry;
        int **polarHoughAry;
        double angleInRadians;
        double offSet;
    //asdf
    HoughTransform(int nr, int nc, int minv, int maxv){
        numRows = nr;
        numCols = nc;
        minVal = minv;
        maxVal = maxv;
        cartMax = 0;
        polMax = 0;
        houghDist = 2 * sqrt((nr*nr)+(nc*nc));
        houghAngle = 180;
        offSet = sqrt((nr*nr)+(nc*nc));
        imgAry = new int*[nr];
        cartesianHoughAry = new int*[houghDist];
        polarHoughAry = new int*[houghDist];
        for(int i=0; i<nr; i++){
            imgAry[i] = new int[nc]();
            cartesianHoughAry[i] = new int[houghAngle]();
            polarHoughAry[i] = new int[houghAngle](); 
        }
        for(int i=nr; i<houghDist; i++){
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

    void loadImage(ifstream& in){
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                in >> imgAry[i][j];
            }
        }
    }

    void buildHoughSpace(){
        for(int x=0; x<numRows; x++){
            for(int y=0; y<numCols; y++){
                if(imgAry[x][y] > 0){
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
            if(cartesianHoughAry[distInt][angleInDegree] > cartMax){
                cartMax = cartesianHoughAry[distInt][angleInDegree];
            }
            dist = polarDist(x, y, angleInRadians);
            distInt = (int)dist;
            polarHoughAry[distInt][angleInDegree]++;
            if(polarHoughAry[distInt][angleInDegree] > polMax){
                polMax = polarHoughAry[distInt][angleInDegree];
            }
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
        double t = angle - atan(ydouble/xdouble) - M_PI/2;
        return (xdouble*cos(t)) + (ydouble*sin(t)) + offSet;
    }

    void prettyPrint(int **arr, ofstream& out, int rs, int cs, int mv){
        //algo in previous projects
        string temp = "";
        int tempInt = 0;
        string mvTemp = to_string(mv);
        int width = mvTemp.length();

        for(int i=0; i<rs; i++){
            for(int j=0; j<cs; j++){
                temp = to_string(arr[i][j]);
                tempInt = temp.length();
                if(arr[i][j] == 0){
                    out << ". ";
                    while(tempInt < width){
                        out << " ";
                        tempInt++;
                    }
                }else{
                    out << arr[i][j] << " ";
                    while(tempInt < width){
                        out << " ";
                        tempInt++;
                    }
                }
            }
            out << endl;
        }
    }
    void addBorder(ofstream& out){
        out << endl;
        for(int i=0; i<houghAngle; i++){
            out << "--";
        }
        out << endl;
    }
};

int main(int argc, char *argv[]){

    if(argc != 3){
        cout << "Invalid arguments, ending program..." << endl;
        return 1;
    }

    //step 0
    ifstream inFile;
    inFile.open(argv[1]);
    
    ofstream outFile1;
    outFile1.open(argv[2]);

    int rows, cols, minv, maxv;
    if(inFile.is_open()){
        inFile >> rows;
        inFile >> cols;
        inFile >> minv;
        inFile >> maxv;
    }

    HoughTransform hgh = HoughTransform(rows, cols, minv, maxv);

    //step 1
    hgh.loadImage(inFile);
    outFile1 << "Image Input:" << endl;
    hgh.prettyPrint(hgh.imgAry, outFile1, rows, cols, maxv);
    hgh.addBorder(outFile1);

    //step 2
    hgh.buildHoughSpace();

    //step 3
    outFile1 << "Cartesian:" << endl;
    hgh.prettyPrint(hgh.cartesianHoughAry, outFile1, hgh.houghDist, hgh.houghAngle, hgh.cartMax);
    hgh.addBorder(outFile1);
    outFile1 << "Polar:"<< endl;
    hgh.prettyPrint(hgh.polarHoughAry, outFile1, hgh.houghDist, hgh.houghAngle, hgh.polMax);
    hgh.addBorder(outFile1);

    //step 4
    inFile.close();
    outFile1.close();

    return 0;
}