#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
using namespace std;

class HoughTransform{
    public:
        int numRows, numCols, minVal, maxVal, houghDist, houghAngle, angleInDegree;
        int **imgAry;
        int **cartesianHoughAry;
        int **polarHoughAry;
        double angleInRadians;
        int offSet;
    
    HoughTransform(int nr, int nc, int minv, int maxv){
        numRows = nr;
        numCols = nc;
        minVal = minv;
        maxVal = maxv;
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
            cout << dist << " " << distInt << endl;
            cartesianHoughAry[distInt][angleInDegree]++;
            dist = polarDist(x, y, angleInRadians);
            cout << dist << endl;
            distInt = (int)dist;
            polarHoughAry[distInt][angleInDegree]++;
            angleInDegree++;
        }
        cout << "Hello" << endl;
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

    void prettyPrint(int **arr, ofstream& out){
        //algo in previous projects

        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
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
    hgh.prettyPrint(hgh.imgAry, outFile1);

    //step 2
    hgh.buildHoughSpace();
    
    cout << "Hello" << endl;

    //step 3
    hgh.prettyPrint(hgh.cartesianHoughAry, outFile1);
    hgh.prettyPrint(hgh.polarHoughAry, outFile1);

    //step 4
    inFile.close();
    outFile1.close();

    return 0;
}