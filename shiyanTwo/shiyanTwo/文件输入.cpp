#include<iostream>
#include<vector>
#include<set>
#include<fstream>
#include<sstream>
using namespace std;


//非终结符顺序，记住，第一个存进去的即为开始符号,非终结符
vector<string>tuiName;

set<string> fFu = {};//非终结符
set<string> tFu = {};//epsilon和ε都是空串,终结符
vector<string> tableTop = {};
string epsilon = "ε";
vector<vector<string>> tui;//产生式
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

	tui = vector<vector<string>>(20);//初始化

	//→ 中文字符，占两个格子
	string  tmp;
	while (getline(input, tmp))
	{
		int i = 0;//次序
		//去掉所有空格
		tmp.erase(remove_if(tmp.begin(), tmp.end(), [](const char x)
			{
				if (x < 0 || x>255)
					return false;
				return x == ' ' || isspace(x);
			}), tmp.end());

		//非终结符→
		string tmp1 = tmp.substr(0, tmp.find("→"));
		if (!fFu.count(tmp1))
		{
			//没有重复的
			tuiName.push_back(tmp1);//记录,顺便赋予次序
			fFu.insert(tmp1);//添加到非终结符集合
		}

		//寻找非终结符的次序
		for (i = 0; i < tuiName.size(); i++)
		{
			if (tuiName[i] == tmp1)
			{
				break;
			}
		}
		tui[i] = {};//初始化

		//查看是否有空串
		auto it = tmp.find("ε");
		if (it != string::npos)
		{
			//有空串
			tFu.insert(epsilon);
			//去掉空串
			tmp.replace(it, epsilon.size(), "");//确定只有一个空串
			//添加到产生式
			tui[i].push_back(epsilon);
		}

		tmp = tmp.substr(tmp.find("→") + 2);
		string tmp2;
		stringstream tmps(tmp);
		while (getline(tmps, tmp2, '|'))
		{
			cout << "开始插入" << endl;
			tui[i].push_back(tmp2);
			cout << "插入成功" << endl;
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
	cout << "非终结符" << endl;
	for (auto i : fFu)
	{
		cout << i << " ";
	}
	cout << endl;

	cout << "终结符" << endl;
	for (auto i : tFu)
	{
		cout << i << " ";
	}
	cout << endl;

	cout << "产生式" << endl;
	for (int i = 0; i < tuiName.size(); i++)
	{
		cout << tuiName[i] << "→";
		for (int j = 0; j < tui[i].size(); j++)
		{
			cout << tui[i][j] << "|";
		}
		cout << endl;
	}

	cout << tui.size();
	cout << endl;
}



