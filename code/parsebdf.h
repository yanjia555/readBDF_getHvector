//
// Created by Yanjia on 2024/5/9.
//

#ifndef INC_05_OVERALLPROCESS_PARSEBDF_H
#define INC_05_OVERALLPROCESS_PARSEBDF_H


#include <string>
#include <armadillo>

using namespace arma;
using namespace std;


struct GridData {
    int ne; // number of elements
    int nn; // number of nodes (including mid node)
    mat coordinates; // node coordinates  matrix(x, y, z)
    mat Nid; //  Node IDs and coordinates matrix (node_id, x, y, z)
    imat Eid; // Element IDs and node IDs matrix (ele_id, node1, node2, node3, node4)

    mat H1i; // normal vector of each node point
    mat H2i; // tangent vector of each node point
    mat H3i; // binormal vector of each node point
    mat t; // thickness of each node point
//    mat up_coordinates; // top surface coordinates (x, y, z)
//    mat bot_coordinates; // bottom surface coordinates (x, y, z)
//    ivec lato1; // left side node indices

};

GridData parseBDF(const string &filePath,double tmax);







#endif //INC_05_OVERALLPROCESS_PARSEBDF_H
