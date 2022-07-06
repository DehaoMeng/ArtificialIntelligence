#include "Tree_Node.cpp"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include<cmath>
#include <set>
#include <sstream>

using namespace std;
//定义属性数组，存放可能的属性，包括类别
vector<string> data_Attributes;//对于本数据集来说就是：色泽 根蒂 敲声 纹理 脐部 触感 类别
//定义各属性对应的属性值
map<string, vector<string>> data_AttValues;//比如：色泽={青绿 乌黑 浅白}
//定义剩余属性，不包括类别（这个主要用于后面算法的递归）
vector<string> remain_Attributes;//色泽 根蒂 敲声 纹理 脐部 触感
//定义数据表,属性-属性值（全部数据的属性值放在同一个数组）
map<string, vector<string>>data_Table;//整张表


//输入数据生成数据集
void data_Input(){
    //输入属性（色泽 根蒂 敲声 纹理 脐部 触感 好瓜）
    string input_Line,temp_Attributes;
    cout << "请输入属性：" << endl;
    //获取一行数据，然后绑定到数据流istringstream
    getline(cin, input_Line);
    istringstream input_Attributes(input_Line);
    //将数据流内容（空格不输出）输入数据属性数组中
    while (input_Attributes >> temp_Attributes){
        data_Attributes.push_back(temp_Attributes);
    }
    //剔除类别这个属性
    remain_Attributes = data_Attributes;
    remain_Attributes.pop_back();
    //定义样本数量
    int N = 0;
    cout << "请输入样本数量：" << endl;
    cin >> N;
    cin.ignore();//清空cin缓冲区中的留下的换行符
    //输入数据(属性值）
    cout << "请输入样本：" << endl;
    //一共N个训练样本
    for (int j = 0; j < N; j++){
        string temp_AttValues;
        //获取一行属性值输入
        getline(cin, input_Line);
        istringstream input_AttValues(input_Line);
        //将各属性值输入到数据表data_table中
        for (int i = 0; i < data_Attributes.size(); i++){
            input_AttValues >> temp_AttValues;
            data_Table[data_Attributes[i]].push_back(temp_AttValues);
        }
    }

    //生成各属性对应的属性值集的映射data_AttValues
    for (int i = 0; i < data_Attributes.size(); i++){
        //通过set结构来统计所有样本中各属性对应的属性值的所有可能的取值
        //如：“色泽”的可能取值为：青绿 乌黑 浅白
        set<string> attValues;
        for (int j = 0; j < N; j++){
            //注意：data_Attributes[i]代表某个属性
            //而data_Table[data_Attributes[i]]是一个数组
            string temp = data_Table[data_Attributes[i]][j];
            //若有重复属性值，set是不会插入的
            attValues.insert(temp);
        }
        for (set<string>::iterator it = attValues.begin(); it != attValues.end(); it++){
            //将所有可能的属性值存入data_AttValues[data_Attributes[i]]
            data_AttValues[data_Attributes[i]].push_back(*it);
        }

    }
}

int main(){
    //输入
    data_Input();
    Tree_Node myTree(data_Table, data_Attributes, data_AttValues, remain_Attributes);
    //进行训练
    myTree.generate_ChildNode();
    //输入预测样例，进行预测
    vector<string> predict_Sample;
    string input_Line, temp;
    cout << "请输入属性进行预测:" << endl;
    getline(cin, input_Line);
    istringstream input_Sample(input_Line);
    while (input_Sample >> temp){
        //将输入预测样例的属性都存入predict_Sample，以便传参
        predict_Sample.push_back(temp);
    }
    cout << endl;
    // 预测结果
    cout << "分类结果为：" << myTree.findClass(predict_Sample) << endl;
    return 0;
}
