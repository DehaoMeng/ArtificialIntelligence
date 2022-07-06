#include "Tree_Node.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include<cmath>
using namespace std;
Tree_Node::Tree_Node(map<string, vector<string>> temp_Table,vector<string> temp_Attribute, map<string, vector<string>> data_AttValues, vector<string> temp_remain){
    //全部属性,包括类别
    data_Attribute = temp_Attribute;
    //属性个数，不包括类别
    attribute_Num = (int)temp_Attribute.size() - 1;
    //各属性对应的属性值
    this->data_AttValues = data_AttValues;
    //属性表
    MyDateTable = temp_Table;
    //剩余属性集
    remain_Attributes = temp_remain;
}

//计算信息熵
double Tree_Node::calc_Entropy(map<string, vector<string>> temp_Table){
    map<string, vector<string>> table = temp_Table;
    //数据集中样本的数量
    int sample_Num = (int)temp_Table[data_Attribute[0]].size();
    //计算数据集中的类别数量
    map<string, int> class_Map;
    for (int i = 0; i < sample_Num; i++){
        //data_Attribute[attribute_Num]对应的就是数据集的类别
        string class_String = table[data_Attribute[attribute_Num]][i];
        class_Map[class_String]++;
    }

    map<string, int>::iterator it = class_Map.begin();
    //存放类别及其对应的数量
    //vector<string> m_Class;
    vector<int> n_Class;

    for (; it != class_Map.end(); it++){
        //m_Class.push_back(it->first);
        n_Class.push_back(it->second);
    }
    //计算信息熵
    double Ent = 0;
    for (int k = 0; k < class_Map.size(); k++){
        //比例
        double p = (double) n_Class[k] / sample_Num;
        if (p == 0){
            //规定了p=0时，plogp=0
            continue;
        }
        //c++中只有log和ln，因此需要应用换底公式
        Ent -= p * (log(p) / log(2));//信息熵
    }
    return Ent;
}

//寻找最优划分
string Tree_Node::findBestAttribute(){
    //样本个数
    int N = (int)MyDateTable[data_Attribute[0]].size();
    //定义用于存放最优属性
    string best_Attribute;
    //信息增益
    double gain = 0;
    //对每个剩余属性
    for (int i = 0; i < remain_Attributes.size(); i++){
        //定义信息增益，选取增益最大的属性来划分即为最优划分
        double temp_Gain = calc_Entropy(MyDateTable);//根据公式先将本节点的信息熵初始化给增益
        //对该属性的数据集进行分类（获取各属性值的数据子集）
        string temp_Att = remain_Attributes[i];//假设选取的属性
        vector<string> remain_AttValues;//属性可能的取值
        for (int j = 0; j < data_AttValues[temp_Att].size(); j++){
            remain_AttValues.push_back(data_AttValues[temp_Att][j]);
        }

        //对每个属性值求信息熵
        for (int k = 0; k < remain_AttValues.size(); k++){
            //属性值
            string temp_AttValues = remain_AttValues[k];
            int sample_Num = 0;//该属性值对应样本数量
            //定义map用来存放该属性值下的数据子集
            map<string, vector<string>>sub_DataTable;
            for (int l = 0; l < MyDateTable[temp_Att].size(); l++){
                if (temp_AttValues == MyDateTable[temp_Att][l]){
                    sample_Num++;
                    //将符合条件的训练集存入sub_DataTable
                    for (int m = 0; m < data_Attribute.size(); m++){
                        sub_DataTable[data_Attribute[m]].push_back(MyDateTable[data_Attribute[m]][l]);
                    }
                }
            }
            //累加每个属值的信息熵
            temp_Gain -= (double)sample_Num / N * calc_Entropy(sub_DataTable);
        }
        //比较寻找最优划分属性
        if (temp_Gain > gain){
            gain = temp_Gain;
            best_Attribute = temp_Att;
        }
    }
    return best_Attribute;
}

void Tree_Node::set_NodeAttribute(string attribute){
    //设置节点的属性
    this->node_Attribute = attribute;
}

void Tree_Node::generate_ChildNode(){
    //样本个数
    int N = (int)MyDateTable[data_Attribute[0]].size();
    //将数据集中类别种类和数量放入map里面，只需判断最后一列即可
    map<string,int> category;
    for (int i = 0; i < N; i++){
        vector<string> temp_Class;
        temp_Class = MyDateTable[data_Attribute[attribute_Num]];
        category[temp_Class[i]]++;
    }
    //第一种情况
    //只有一个类别，标记为叶节点
    if (1 == category.size()){
        map<string, int>::iterator it = category.begin();
        node_Attribute = it->first;
        return;
    }
    //第二种情况
    //先判断所有属性是否取相同值
    bool isAllSame = false;
    for (int i = 0; i < remain_Attributes.size(); i++){
        isAllSame = true;
        vector<string> temp;
        temp = MyDateTable[remain_Attributes[i]];
        for (int j = 1; j < temp.size(); j++){
            //只要有一个不同，即可退出
            if (temp[0] != temp[j])
            {
                isAllSame = false;
                break;
            }
        }
        if (isAllSame == false){
            break;
        }
    }
    //若属性集为空或者样本中的全部属性取值相同
    if (remain_Attributes.empty()||isAllSame){
        //找出数量最多的类别及其出现的个数，并将该节点标记为该类
        map<string, int>::iterator it = category.begin();
        node_Attribute = it->first;
        int max = it->second;
        it++;
        for (; it != category.end(); it++){
            int num = it->second;
            if (num > max){
                node_Attribute = it->first;
                max = num;
            }
        }
        return;
    }
    //第三种情况
    //从remian_attributes中划分最优属性
    string best_Attribute = findBestAttribute();
    //将本节点设置为最优属性
    node_Attribute = best_Attribute;
    //对最优属性的每个属性值
    for (int i = 0; i < data_AttValues[best_Attribute].size(); i++){
        string best_AttValues = data_AttValues[best_Attribute][i];
        //计算属性对应的数据集D
        //定义map用来存放该属性值下的数据子集
        map<string, vector<string>> sub_DataTable;
        for (int j = 0; j < MyDateTable[best_Attribute].size(); j++){
            //寻找最优属性在数据集中属性值相同的数据样本
            if (best_AttValues == MyDateTable[best_Attribute][j]){
                //找到对应的数据集，存入子集中sub_DataTable（该样本的全部属性都要存入）
                for (int k = 0; k < data_Attribute.size(); k++){
                    sub_DataTable[data_Attribute[k]].push_back(MyDateTable[data_Attribute[k]][j]);
                }
            }
        }
        //若子集为空，将分支节点（子节点）标记为叶节点，类别为MyDateTable样本最多的类
        if (sub_DataTable.empty()){
            //生成子节点
            Tree_Node * p = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, remain_Attributes);
            //找出样本最多的类，作为子节点的属性
            map<string, int>::iterator it = category.begin();
            string childNode_Attribute = it->first;
            int max_Num = it->second;
            it++;
            for (; it != category.end(); it++){
                if (it->second > max_Num){
                    max_Num = it->second;
                    childNode_Attribute = it->first;
                }
            }
            //设置子叶节点属性
            p->set_NodeAttribute(childNode_Attribute);
            //将子节点存入childNode，预测样本的时候会用到
            childNode[best_AttValues] = p;
        }
        else{//若不为空，则从剩余属性值剔除该属性，调用generate_ChildNode继续往下细分
            vector<string> child_RemainAtt;
            child_RemainAtt = remain_Attributes;
            //找出child_RemainAtt中的与该最佳属性相等的属性
            vector<string>::iterator it = child_RemainAtt.begin();
            for (; it != child_RemainAtt.end(); it++){
                if (*it == best_Attribute){
                    break;
                }
            }
            //删除
            child_RemainAtt.erase(it);
            //生成子节点
            Tree_Node * pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
            //将子节点存入childNode
            childNode[best_AttValues] = pt;
            //子节点再调用generate_ChildNode函数
            pt->generate_ChildNode();
        }
    }

}
//输入为待预测样本的所有属性集合
string Tree_Node::findClass(vector<string> attributes){
    //若存在子节点
    if (childNode.size() != 0){
        //找出输入的样例中与本节点属性对应的属性值，以便寻找下个节点，直到找到叶节点
        string attribute_Value;
        for (int i = 0; i < data_AttValues[node_Attribute].size(); i++){
            for (int j = 0; j < attributes.size(); j++){
                //data_AttValues[node_Attribute]为属性node_Attribute对应的所有可能的取值集合
                if (attributes[j] == data_AttValues[node_Attribute][i]){
                    //找到了样例对应的属性值
                    attribute_Value = attributes[j];
                    break;
                }
            }
            //找到后就没必要继续循环了
            if (!attribute_Value.empty()){
                break;
            }
        }
        //找出该属性值对应的子节点的地址，以便进行访问
        Tree_Node *p = childNode[attribute_Value];
        return p->findClass(attributes);//递归寻找，直到找到叶节点为止
    }
    else{//不存在子节点说明已经找到分类，类别为本节点的node_Attribute
        return node_Attribute;
    }
}