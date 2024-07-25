
#include <iostream>
#include <string>
#include <armadillo>
#include "hvector.h"
#include "parsebdf.h"

using namespace arma;
using namespace std;

int main() {



    double tmax = 1; // maximum thickness

    //读取BDF文件获取：节点总数nn，单元总数ne，节点坐标mat coordinates，节点信息mat Nid,单元信息imat Eid，
    GridData gridData = parseBDF("C:\\Users\\Yanjia\\Desktop\\testNastran.bdf",tmax);

 //获取各个节点处的法向量H3i以及H2i、H1i。
    HVector hv=hVector(gridData.Eid,gridData.Nid,gridData.nn);

    gridData.coordinates.print("Coordinates:");
    gridData.Nid.print("Node IDs and Coordinates:");
    gridData.Eid.print("Element IDs and Node IDs:");
    cout<<"节点数："<<gridData.nn<<endl;
    cout<<"单元数："<<gridData.ne<<endl;
    hv.H3i.print("H3i:");
    hv.H2i.print("H2i:");
    hv.H1i.print("H1i:");
    gridData.t.print("t:");


    return 0;
}
