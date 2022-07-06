#ifndef TOPOLOGICAL_CPP_TREE_NODE_H
#define TOPOLOGICAL_CPP_TREE_NODE_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include<cmath>
using namespace std;
class Tree_Node {
public:
    //构造函数，参数依次为：数据集表（西瓜数据表）、西瓜所有的属性包括类别、每个属性可能的取值构成的表、剩余的未被划分的属性
    Tree_Node(map<string, vector<string>> temp_Table, vector <string> temp_Attribute,map <string, vector<string>> data_AttValues, vector <string> temp_remain);
    //生成子节点
    void generate_ChildNode();
    //计算信息增益 寻找最优划分属性
    string findBestAttribute();
    //计算信息熵
    double calc_Entropy(map <string, vector<string>> temp_Table);
    //设置节点的属性
    void set_NodeAttribute(string atttribute);
    //根据所给属性，对数据进行分类
    string findClass(vector <string> attributes);

private:
    //属性个数，不包括类别
    int attribute_Num;
    //本节点的属性
    string node_Attribute;
    //数据集属性
    vector <string> data_Attribute;
    //本节点的所有属性值
    vector <string> node_AttValues;
    //剩余属性集
    vector <string> remain_Attributes;
    //子节点,本节点属性对应的属性值与子节点地址进行一一映射
    //为空说明该节点为叶节点
    map<string, Tree_Node *> childNode;
    //样本集合表
    map <string, vector<string>> MyDateTable;
    //定义各属性对应的属性值
    map <string, vector<string>> data_AttValues;
};


#endif //TOPOLOGICAL_CPP_TREE_NODE_H
