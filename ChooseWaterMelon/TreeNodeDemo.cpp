#include "Tree_Node.cpp"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include<cmath>
#include <set>
#include <sstream>

using namespace std;
//�����������飬��ſ��ܵ����ԣ��������
vector<string> data_Attributes;//���ڱ����ݼ���˵���ǣ�ɫ�� ���� ���� ���� �겿 ���� ���
//��������Զ�Ӧ������ֵ
map<string, vector<string>> data_AttValues;//���磺ɫ��={���� �ں� ǳ��}
//����ʣ�����ԣ���������������Ҫ���ں����㷨�ĵݹ飩
vector<string> remain_Attributes;//ɫ�� ���� ���� ���� �겿 ����
//�������ݱ�,����-����ֵ��ȫ�����ݵ�����ֵ����ͬһ�����飩
map<string, vector<string>>data_Table;//���ű�


//���������������ݼ�
void data_Input(){
    //�������ԣ�ɫ�� ���� ���� ���� �겿 ���� �ùϣ�
    string input_Line,temp_Attributes;
    cout << "���������ԣ�" << endl;
    //��ȡһ�����ݣ�Ȼ��󶨵�������istringstream
    getline(cin, input_Line);
    istringstream input_Attributes(input_Line);
    //�����������ݣ��ո������������������������
    while (input_Attributes >> temp_Attributes){
        data_Attributes.push_back(temp_Attributes);
    }
    //�޳�����������
    remain_Attributes = data_Attributes;
    remain_Attributes.pop_back();
    //������������
    int N = 0;
    cout << "����������������" << endl;
    cin >> N;
    cin.ignore();//���cin�������е����µĻ��з�
    //��������(����ֵ��
    cout << "������������" << endl;
    //һ��N��ѵ������
    for (int j = 0; j < N; j++){
        string temp_AttValues;
        //��ȡһ������ֵ����
        getline(cin, input_Line);
        istringstream input_AttValues(input_Line);
        //��������ֵ���뵽���ݱ�data_table��
        for (int i = 0; i < data_Attributes.size(); i++){
            input_AttValues >> temp_AttValues;
            data_Table[data_Attributes[i]].push_back(temp_AttValues);
        }
    }

    //���ɸ����Զ�Ӧ������ֵ����ӳ��data_AttValues
    for (int i = 0; i < data_Attributes.size(); i++){
        //ͨ��set�ṹ��ͳ�����������и����Զ�Ӧ������ֵ�����п��ܵ�ȡֵ
        //�磺��ɫ�󡱵Ŀ���ȡֵΪ������ �ں� ǳ��
        set<string> attValues;
        for (int j = 0; j < N; j++){
            //ע�⣺data_Attributes[i]����ĳ������
            //��data_Table[data_Attributes[i]]��һ������
            string temp = data_Table[data_Attributes[i]][j];
            //�����ظ�����ֵ��set�ǲ�������
            attValues.insert(temp);
        }
        for (set<string>::iterator it = attValues.begin(); it != attValues.end(); it++){
            //�����п��ܵ�����ֵ����data_AttValues[data_Attributes[i]]
            data_AttValues[data_Attributes[i]].push_back(*it);
        }

    }
}

int main(){
    //����
    data_Input();
    Tree_Node myTree(data_Table, data_Attributes, data_AttValues, remain_Attributes);
    //����ѵ��
    myTree.generate_ChildNode();
    //����Ԥ������������Ԥ��
    vector<string> predict_Sample;
    string input_Line, temp;
    cout << "���������Խ���Ԥ��:" << endl;
    getline(cin, input_Line);
    istringstream input_Sample(input_Line);
    while (input_Sample >> temp){
        //������Ԥ�����������Զ�����predict_Sample���Ա㴫��
        predict_Sample.push_back(temp);
    }
    cout << endl;
    // Ԥ����
    cout << "������Ϊ��" << myTree.findClass(predict_Sample) << endl;
    return 0;
}
