#include<iostream>
#include<vector>
#include<set>
#include<fstream>
#include<sstream>
using namespace std;


//���ս��˳�򣬼�ס����һ�����ȥ�ļ�Ϊ��ʼ����,���ս��
vector<string>tuiName;

set<string> fFu = {};//���ս��
set<string> tFu = {};//epsilon�ͦŶ��ǿմ�,�ս��
vector<string> tableTop = {};
string epsilon = "��";
vector<vector<string>> tui;//����ʽ
void printALL();
int main()
{
	ifstream input("input.txt");
	ofstream output("output.txt");
	if (!input || !output)
	{
		cout << "open file error" << endl;
		return 0;
	}

	tui = vector<vector<string>>(20);//��ʼ��

	//�� �����ַ���ռ��������
	string  tmp;
	while (getline(input, tmp))
	{
		int i = 0;//����
		//ȥ�����пո�
		tmp.erase(remove_if(tmp.begin(), tmp.end(), [](const char x)
			{
				if (x < 0 || x>255)
					return false;
				return x == ' ' || isspace(x);
			}), tmp.end());

		//���ս����
		string tmp1 = tmp.substr(0, tmp.find("��"));
		if (!fFu.count(tmp1))
		{
			//û���ظ���
			tuiName.push_back(tmp1);//��¼,˳�㸳�����
			fFu.insert(tmp1);//��ӵ����ս������
		}

		//Ѱ�ҷ��ս���Ĵ���
		for (i = 0; i < tuiName.size(); i++)
		{
			if (tuiName[i] == tmp1)
			{
				break;
			}
		}
		tui[i] = {};//��ʼ��

		//�鿴�Ƿ��пմ�
		auto it = tmp.find("��");
		if (it != string::npos)
		{
			//�пմ�
			tFu.insert(epsilon);
			//ȥ���մ�
			tmp.replace(it, epsilon.size(), "");//ȷ��ֻ��һ���մ�
			//��ӵ�����ʽ
			tui[i].push_back(epsilon);
		}

		tmp = tmp.substr(tmp.find("��") + 2);
		string tmp2;
		stringstream tmps(tmp);
		while (getline(tmps, tmp2, '|'))
		{
			cout << "��ʼ����" << endl;
			tui[i].push_back(tmp2);
			cout << "����ɹ�" << endl;
			for (int j = 0; j < tmp2.size(); j++)
			{
				char tmp3 = tmp2[j];
				if (!isupper(tmp3))
				{
					tFu.insert(tmp2.substr(j, 1));
				}
			}

		}
	}

	printALL();
	return 0;
}

void printALL()
{
	tui.resize(tuiName.size());
	cout << "���ս��" << endl;
	for (auto i : fFu)
	{
		cout << i << " ";
	}
	cout << endl;

	cout << "�ս��" << endl;
	for (auto i : tFu)
	{
		cout << i << " ";
	}
	cout << endl;

	cout << "����ʽ" << endl;
	for (int i = 0; i < tuiName.size(); i++)
	{
		cout << tuiName[i] << "��";
		for (int j = 0; j < tui[i].size(); j++)
		{
			cout << tui[i][j] << "|";
		}
		cout << endl;
	}

	cout << tui.size();
	cout << endl;
}



