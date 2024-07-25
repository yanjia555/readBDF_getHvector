//
// Created by Yanjia on 2024/5/9.
//




#include "parsebdf.h"
#include <string>
#include <armadillo>

using namespace arma;
using namespace std;



//解析BDF文件的函数
GridData parseBDF(const string &filePath,double tmax){

    ifstream file(filePath);  //打开文件
    string currentLine;            //用于存储每一行数据

    GridData gd;// 创建一个pb对象

    if (!file.is_open()) {// 判断文件是否打开成功
        cerr << "Failed to open file" << endl;// 输出错误信息
        return gd;
    }

    // Count the number of lines in the file
    int numLines = 0;// 初始化numLines为0
    while (getline(file, currentLine)) {// 逐行读取文件

        ++numLines;
    }
    file.clear();// 清除文件状态标志
    file.seekg(0, ios::beg);// 将文件指针重置到文件开头

    // Allocate memory for data storage
    gd.coordinates.set_size(numLines, 3);// 设置矩阵的大小为numLines行3列
    gd.Nid.set_size(numLines, 4);// 设置矩阵的大小为numLines行4列
    gd.Eid.set_size(numLines, 9);// 设置矩阵的大小为numLines行9列

    int nodeCount = 0, elementCount = 0;// 初始化节点和单元的计数器
    string eType = " ";


    while (getline(file, currentLine)) {
        // 检查line是否非空
        if (!currentLine.empty()) {

            size_t pos = currentLine.find('+');
            if (pos != std::string::npos && pos != 0) {// 如果找到了+号且位置不是0，那么拼接当前行和下一行
                currentLine = currentLine.substr(0, pos); // 移除末尾的空格和+号
                // 读取下一行，并且叠加
                std::string nextLine;
                if (std::getline(file, nextLine)) {
                    if (nextLine[0] == '+') {
                        nextLine.erase(0, 1);
                        currentLine += nextLine;

                    }
                }
            }
        }

        stringstream ss(currentLine);// 创建一个stringstream对象
        string token;
        ss >> token;// 读取第一个单词。
        if (token == "GRID") {// 判断是否为GRID行
            // Parse node data
            int nodeId;
            double x, y, z;
            ss >> nodeId >> x >> y >> z;
            gd.Nid(nodeCount, 0) = nodeId;// 设置节点ID
            gd.Nid(nodeCount, 1) = x;// 设置节点x
            gd.Nid(nodeCount, 2) = y;// 设置节点y
            gd.Nid(nodeCount, 3) = z;// 设置节点z
            gd.coordinates(nodeCount, 0) = x;
            gd.coordinates(nodeCount, 1) = y;
            gd.coordinates(nodeCount, 2) = z;
            ++nodeCount;
        } else if (token == "CTRIA3" || token == "CQUAD4" || token == "CQUAD8") {//支持读取壳单元的类型
            // Parse element data
            int elemId, setId, n1, n2, n3, n4, n5, n6, n7, n8 = 0;
            ss >> elemId >> setId >> n1 >> n2 >> n3;
            if (token == "CTRIA3") {
                eType = "CTRIA3";
                gd.Eid(elementCount, 0) = elemId;
                gd.Eid(elementCount, 1) = n1;
                gd.Eid(elementCount, 2) = n2;
                gd.Eid(elementCount, 3) = n3;
            } else if (token == "CQUAD4") {
                eType = "CQUAD4";
                ss >> n4;
                gd.Eid(elementCount, 0) = elemId;
                gd.Eid(elementCount, 1) = n1;
                gd.Eid(elementCount, 2) = n2;
                gd.Eid(elementCount, 3) = n3;
                gd.Eid(elementCount, 4) = n4;
            } else if (token == "CQUAD8") {
                eType = "CQUAD8";
//                gd.Eid.resize(elementCount, 9);
                ss >> n4 >> n5 >> n6 >> n7 >> n8;
                gd.Eid(elementCount, 0) = elemId;
                gd.Eid(elementCount, 1) = n1;
                gd.Eid(elementCount, 2) = n2;
                gd.Eid(elementCount, 3) = n3;
                gd.Eid(elementCount, 4) = n4;
                gd.Eid(elementCount, 5) = n5;
                gd.Eid(elementCount, 6) = n6;
                gd.Eid(elementCount, 7) = n7;
                gd.Eid(elementCount, 8) = n8;
            }
            ++elementCount;
        }
    }

    // Set the total number of nodes and elements
    gd.nn = nodeCount;
    gd.ne = elementCount;

    // Resize matrices to actual sizes
    gd.coordinates.resize(nodeCount, 3);// 重新设置矩阵的大小为实际的节点数量和维度
    gd.Nid.resize(nodeCount, 4);// 重新设置矩阵的大小为实际的节点数量和维度

    if (eType == "CQUAD8") {
        gd.Eid.resize(elementCount, 9);// 重新设置矩阵的大小为实际的单元数量和维度
    } else if (eType == "CQUAD4") {
        gd.Eid.resize(elementCount, 5);// 重新设置矩阵的大小为实际的单元数量和维度
    } else if (eType == "CTRIA3") {
        gd.Eid.resize(elementCount, 4);// 重新设置矩阵的大小为实际的单元数量和维度
    }


    vec y;
    y=gd.Nid.col(2);
    vec t = tmax - abs(y) * 0.0;
    gd.t=t;






    return gd;



}

