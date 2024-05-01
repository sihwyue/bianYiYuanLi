#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<fstream>
#include<algorithm>
#include<iomanip>
#include<string>
#include<sstream>
using namespace std;

string epsilon = "ε";
string tuiDaoFu = "→";
/**
 * set<string>.
 *存储非终结符，便于便利查找
 */
set<string> fFu;

/**
 * set<string>存储终结符，便于便利查找.
 */
set<string> tFu;//epsilon和ε都是空串
vector<string> tableTop;


/**
 * vector<vector<string>>
 * * 用来存储文法的产生式.
 * * eg: vector<string> Etui = { "TA" };
 * */
vector<vector<string>> tui;

/**
 * map<string, bool>
 * * * * 用来存储文法的生成式是否含有ε.
 * * * * eg: map<string, bool> tuiHav = { {"E",false},{"A",true},{"T",false},{"B",true},{"F",false} };
 * *
 * * * */
map<string, bool> tuiHav;
/**
 * vector<string>
 * * * 用来存储文法的非终结符.
 * * * eg: vector<string> tuiName = { "E","A","T","B","F" };
 * 便于位置与vector<vector<string>> tui对应
 * * */
vector<string> tuiName;


/**
* map<string,set<string>>
 * 经分析FIRST都是不为空的，所以不需要初始化.
 */
map<string, set<string>> FIRSTsetMap;

/**
* map<string, bool>
 * 存储FIRST是否含有ε.
 */
map<string, bool> FIRSTHave;



/**
 * vector<set<string>>
 * 用来存储非终结符包含另一非终结符，辅助计算FIRST集合.
 * eg: set<string> FIRST_E_f;
 */
vector<set<string>> FIRST_f;

/**
* map<string,set<string>>
 * 经分析FOLLOW都是不为空的
 */
map<string, set<string>> FOLLOWsetMap;

///**
// * vector<set<string>>
// * * 用来存储第一种情况，辅助计算FOLLOW集合.
// * * eg: set<string> FOLLOW_E_f_FIRST;
// *
// */
//vector<set<string>> FOLLOW_f_FIRST = { FOLLOW_E_f_FIRST,FOLLOW_A_f_FIRST,FOLLOW_T_f_FIRST,FOLLOW_B_f_FIRST,FOLLOW_F_f_FIRST };

/**
 * vector<set<string>>
 * * 用来存储第二种情况，辅助计算FOLLOW集合.
 * * eg: set<string> FOLLOW_E_f_FOLLOW;
 */
vector<set<string>> FOLLOW_f_Follow;



/**
* map<string, map<string, set<string>>>
 * 用来存储最后的分析表.
 */
map<string, map<string, set<string>>> AnalysisTable;
void readTui();
void initALL();
void FindFIRSTset();
void FindFOLLOWset();
void doAnalysisTable();



void printMapSet(const map<string, set<string>>& Diy);
void printHav(map<string, bool> Have);
void printAnT();
void writeOut();

int main()
{
	readTui();

	initALL();

	FindFIRSTset();

	cout << "-----------------FIRST集合---------------" << endl;
	printMapSet(FIRSTsetMap);
	cout << "-----------------------------------------" << endl;

	cout << "----------文法生成式是否含有ε-----------" << endl;
	printHav(tuiHav);
	cout << "-----------------------------------------" << endl;

	cout << "-------------FIRST是否含有ε-------------" << endl;
	printHav(FIRSTHave);
	cout << "-----------------------------------------" << endl;
	FindFOLLOWset();

	cout << "-----------------FOLLOW集合--------------" << endl;
	printMapSet(FOLLOWsetMap);
	cout << "-----------------------------------------" << endl;

	doAnalysisTable();
	cout << "-----------------分析表------------------" << endl;
	printAnT();
	cout << "-----------------------------------------" << endl;
	writeOut();
	return 0;
}

/**
 * 读取文法.
 * 有以下保证
 * 1.非终结符顺序，记住，第一个存进去的即为开始符号
 * 2.保证每一行只有一个空串"ε"
 * 3.每一行推出的不同串都用"|"分割
 * 4.非终结符都是单个大写字母
 * 5.终结符都是单个字符，ASCII码
 * * *
 */
void readTui()
{
	//读取文法
	//1.打开文件
	ifstream input("input.txt");

	if (!input)
	{
		cout << "open file error" << endl;
		return;
	}

	//2.初始化，先按照20个非终结符进行读取
	tui = vector<vector<string>>(20);

	//3.读取文法
	string tmp;
	while (getline(input, tmp))
	{
		int i = 0;//次序,后面会有大用

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
			tui[i].push_back(tmp2);
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
	//4.初始化tableTop
	for (auto tuii : tFu)
	{
		if (tuii != epsilon)
		{
			tableTop.push_back(tuii);
		}
	}
	tableTop.push_back("#");
	input.close();
}
/**
 * 初始化.
 * *
 */
void initALL()
{
	FIRST_f.resize(tuiName.size());
	FOLLOW_f_Follow.resize(tuiName.size());

	//1.初始化 tuiHav
	for (int i = 0; i < tuiName.size(); i++)
	{
		if (find(tui[i].begin(), tui[i].end(), epsilon) != tui[i].end())
		{
			tuiHav[tuiName[i]] = true;
		}
		else
		{
			tuiHav[tuiName[i]] = false;
		}

		//2.初始化 FIRSTsetMap
		if (!i)
		{
			FOLLOWsetMap[tuiName[i]] = { "#" };
		}
		else
		{
			FOLLOWsetMap[tuiName[i]] = {};
		}
		//3.初始化FIRST集合的辅助集合
		FIRST_f[i] = {};
		//4.初始化FOLLOW集合的辅助集合
		FOLLOW_f_Follow[i] = {};
	}

}


/**
 * 求FIRST集合.
 *
 */
void FindFIRSTset()
{
	for (int i = 0; i < tuiName.size(); i++)
	{
		//初始化 FIRSTHave ，默认都不含有ε
		FIRSTHave[tuiName[i]] = false;
	}
	bool flag = true;
	while (flag)
	{
		flag = false;
		for (int i = 0; i < tui.size(); i++)
		{
			for (auto tmp : tui[i])
			{
				int size_f_before = FIRST_f[i].size(), size_f_after = FIRST_f[i].size();
				int size_TM_before = FIRSTsetMap[tuiName[i]].size(), size_TM_after = FIRSTsetMap[tuiName[i]].size();

				if (tmp == epsilon)
				{
					FIRSTsetMap[tuiName[i]].insert(tmp);
					FIRSTHave[tuiName[i]] = true;
					size_TM_after = FIRSTsetMap[tuiName[i]].size();
				}
				else
					if (fFu.find(tmp.substr(0, 1)) != fFu.end())
					{
						//发现是属于非终结符 eg: E ->TA  FIRST(E) += FIRST(T)
						FIRST_f[i].insert(tmp.substr(0, 1));
						for (auto ff : FIRST_f[i])
						{
							for (auto k : FIRSTsetMap[ff])
								FIRSTsetMap[tuiName[i]].insert(k);
						}
						size_f_after = FIRST_f[i].size();
						size_TM_after = FIRSTsetMap[tuiName[i]].size();
					}
					else
					{
						//发现是终结符  eg: E ->i  FIRST(E) += i
						FIRSTsetMap[tuiName[i]].insert(tmp.substr(0, 1));
						size_TM_after = FIRSTsetMap[tuiName[i]].size();
					}
				//证明有添加
				if ((size_f_after - size_f_before) || (size_TM_after - size_TM_before))
				{
					flag = true;
				}
			}
		}
	}

}
/**
 * 求FOLLOW集合.
 *
 */
void FindFOLLOWset()
{
	//第一种情况加FIRST集合
	for (auto tuiALL : tui)
	{
		//FIRST  A->aBb a可以是空串 FOLLOW(A)包含 FIRST(b);
		//对每一个产生式都进行FIRST集合判断,所以用的是auto tuiALL:tui  auto tuii:tuiALL只需要考虑右边，
		//一遍就能完成
		for (auto tuii : tuiALL)
		{
			if (tuii != epsilon)
			{
				//非空串
				if (tuii.size() == 2)
				{
					//
					string tmp0 = tuii.substr(0, 1);
					string tmp1 = tuii.substr(1, 1);
					if (fFu.find(tmp0) != fFu.end())
					{
						//第一个是非终结符
						if (tFu.find(tmp1) != tFu.end())
						{
							//第二个是终结符
							FOLLOWsetMap[tmp0].insert(tmp1);
							//因为是终结符直接加入 tmp0的FOLLOW集合
						}
						else
						{
							//第二个是非终结符
							//直接 往tmp0加入tmp1的FIRST集合-{ "ε"}
							for (auto k : FIRSTsetMap[tmp1])
							{
								if (k != epsilon)
								{
									FOLLOWsetMap[tmp0].insert(k);
								}
							}
						}
					}
				}
				else if (tuii.size() > 2)
				{
					string tmp0, tmp1;
					for (int i = 0; i < tuii.size(); i++)
					{
						tmp0 = tuii.substr(i, 1);
						while (tFu.find(tmp0) != tFu.end())
						{
							i++;
							if (i >= tuii.size() - 1)
							{
								break;
							}
							tmp0 = tuii.substr(i, 1);
						}
						//保证tmp0是非终结符
						if (i + 1 > tuii.size() - 1)
						{
							break;
						}
						tmp1 = tuii.substr(i + 1, 1);
						if (tFu.find(tmp1) != tFu.end())
						{
							//tmp1是非终结符
							FOLLOWsetMap[tmp0].insert(tmp1);
							//因为是终结符直接加入 tmp0的FOLLOW集合
						}
						else
						{
							//tmp1是非终结符
							//直接 往tmp0加入tmp1的FIRST集合-{ "ε"}
							for (auto k : FIRSTsetMap[tmp1])
							{
								if (k != epsilon)
								{
									FOLLOWsetMap[tmp0].insert(k);
								}
							}

						}
					}
				}

			}
		}
	}
	//第二种情况加FOLLOW集合  
	//FOLLOW  A->aBb b可以是空串 FOLLOW(B)包含 FOLLOW(A);

	//1.找到B的的所有可以的A
	for (int i = 0; i < tui.size(); i++)
	{
		for (auto tuii : tui[i])
		{
			if (tuii != epsilon)
			{
				//非空串
				string tmp0, tmp1;
				for (int j = 0; j < tuii.size(); j++)
				{
					tmp0 = tuii.substr(j, 1);
					if (fFu.find(tmp0) != fFu.end())
					{
						//非终结符
						if (j + 1 >= tuii.size())
						{
							//最后一个非终结符，即   A ->aB 的B FOLLOW(B)包含 FOLLOW(A)
							//为了防止添加无效，所以先放到 vector<set<string>> FOLLOW_f_Follow 存着
							int k = 0;
							for (auto ff : tuiName)
							{
								if (ff == tmp0)
								{
									break;
								}
								k++;
							}
							FOLLOW_f_Follow[k].insert(tuiName[i]);
						}
						else
						{
							//不是最后一个字符
							//判断下一个字符是终结符还是非终结符
							tmp1 = tuii.substr(j + 1, 1);
							if (fFu.find(tmp1) != fFu.end())
							{
								//非终结符
								//查找tmp1的产生式是否含"ε"
								bool flag = false;
								int k = 0;
								for (auto tuiN : tuiName)
								{
									if (tuiN == tmp1)
									{
										break;
									}
									k++;
								}

								for (auto tuiZ : tui[k])
								{
									if (tuiZ == epsilon)
									{
										flag = true;
										break;
									}
								}
								if (flag)
								{
									//含有"ε"
									// 即 A->aBb   b=*>"ε" FOLLOW(B)包含 FEOLLOW(A)
									//FOLLOW_f_Follow[k].insert(tuiName[i]);
									int m = 0;
									for (auto tuiN : tuiName)
									{
										if (tuiN == tmp0)
										{
											break;
										}
										m++;
									}
									FOLLOW_f_Follow[m].insert(tuiName[i]);
								}
							}
							else
							{
								//终结符
								continue;
							}
						}
					}
				}
			}
		}
	}
	//2.开始进行合并

	//cout << endl << "查看并集情况" << endl;
	//for (int i = 0; i < FOLLOW_f_Follow.size(); i++)
	//{
	//	cout<< tuiName[i] << ":";
	//	for (auto Fee :FOLLOW_f_Follow[i])
	//	{
	//					cout << Fee << " ";
	//	}
	//	cout << endl;
	//}

	bool flag = true;
	while (flag)
	{
		flag = false;
		for (int i = 0; i < tuiName.size(); i++)
		{
			string tmp = tuiName[i];
			if (FOLLOW_f_Follow[i].size())
			{
				int size_FM_before = FOLLOWsetMap[tmp].size(), size_FM_after = FOLLOWsetMap[tmp].size();
				for (auto zi : FOLLOW_f_Follow[i])
				{
					for (auto k : FOLLOWsetMap[zi])
					{

						FOLLOWsetMap[tmp].insert(k);
						size_FM_after = FOLLOWsetMap[tmp].size();
					}
				}
				if (size_FM_after - size_FM_before)
				{
					flag = true;
				}
			}
		}
	}
}

/**
 * 递归下降分析.
 * 构建分析表分析表
 */
void doAnalysisTable()
{


	//初始化
	for (int i = 0; i < tuiName.size(); i++)
	{
		for (auto tzi : tFu)
		{
			if (tzi != epsilon)
			{
				AnalysisTable[tuiName[i]][tzi] = {};
			}
		}
	}

	//根据first进行填充
	for (int i = 0; i < tuiName.size(); i++)
	{

		for (auto tzi : FIRSTsetMap[tuiName[i]])
		{
			if (tzi != epsilon)
			{
				if (tui[i].size() == 1 || tui[i].size() == 2 && tuiHav[tuiName[i]])
				{
					string tmp = "";
					//不含有ε的生成式只有一个
					for (auto tuii : tui[i])
					{
						if (tuii != epsilon)
						{
							tmp = tuii;
							break;
						}
					}
					AnalysisTable[tuiName[i]][tzi].insert(tmp);
				}
				else
				{
					//含有多个有效生成式所以必须进行匹配
					string tmp;
					for (auto tuii : tui[i])
					{
						if (tuii != epsilon)
						{
							tmp = tuii.substr(0, 1);
							if (tmp == tzi)
							{
								AnalysisTable[tuiName[i]][tzi].insert(tuii);
								break;
							}
						}
					}
				}
			}

		}
	}

	//根据follow进行填充,FIRST含有"ε"的把FOLLOW加入"ε"
	for (auto [k, v] : FIRSTHave)
	{
		//k是非终结符,v是bool
		if (v)
		{
			for (auto ttt : FOLLOWsetMap[k])
			{
				AnalysisTable[k][ttt].insert(epsilon);
			}
		}
	}
}

/**
 * 打印map<string,set<string>>.
 * *
 */
void printMapSet(const map<string, set<string>>& Diy)
{
	for (auto [k, v] : Diy)
	{
		cout << k << ":";
		for (auto Fol : v)
		{
			cout << Fol << " ";
		}
		cout << endl;
	}
}

/**
 * 打印是否first/tui含有ε.
 *
 */
void printHav(map<string, bool> Have)
{
	for (auto [k, v] : Have)
	{
		cout << k << ":" << v << endl;
	}
}


/**
 * 打印分析表.
 * *
 */
void printAnT()
{

	cout << "**";
	for (auto too : tableTop)
	{
		cout << setw(8) << too;
	}
	cout << endl;
	for (int i = 0; i < tuiName.size(); i++)
	{
		string tmp = tuiName[i];
		cout << tmp << " ";
		for (auto tou : tableTop)
		{
			if (AnalysisTable[tmp][tou].size())
			{
				for (auto tuii : AnalysisTable[tmp][tou])
				{
					if (tuii == epsilon)
					{
						cout << setw(4) << tmp << tuiDaoFu << tuii;
						continue;
					}
					else
					{
						cout << setw(5) << tmp << tuiDaoFu << tuii;
					}
				}
			}
			else
			{
				cout << setw(8) << " ";
			}
		}
		cout << endl;
		cout << endl;
	}
}


/**
 * 写出分析表到文件.
 * * *
 */
void writeOut()
{
	ofstream output("output.txt");
	output << "**";
	for (auto too : tableTop)
	{
		output << setw(8) << too;
	}
	output << endl;
	for (int i = 0; i < tuiName.size(); i++)
	{
		string tmp = tuiName[i];
		output << tmp << " ";
		for (auto tou : tableTop)
		{
			if (AnalysisTable[tmp][tou].size())
			{
				for (auto tuii : AnalysisTable[tmp][tou])
				{
					if (tuii == epsilon)
					{
						output << setw(4) << tmp << tuiDaoFu << tuii;
						continue;
					}
					else
					{
						output << setw(5) << tmp << tuiDaoFu << tuii;
					}
				}
			}
			else
			{
				output << setw(8) << " ";
			}
		}
		output << endl;
		output << endl;
	}
	output.close();
}
