#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<fstream>
#include<algorithm>
#include<iomanip>
using namespace std;

/**
 * vector<string>
 * * * 用来存储文法的非终结符.
 * * * eg: vector<string> tuiName = { "E","A","T","B","F" };
 * 便于位置与vector<vector<string>> tui对应
 * * */
vector<string> tuiName = { "E","A","T","B","F" };

/**
 * set<string>.
 *存储非终结符，便于便利查找
 */
set<string> fFu = { "E","T","A","B","F" };

/**
 * set<string>存储终结符，便于便利查找.
 */
set<string> tFu = { "i","+","*","(",")","ε" };//epsilon和ε都是空串
vector<string> tableTop = { "i","+","*","(",")","#" };
vector<string> Etui = { "TA" };
vector<string> Atui = { "+TA","ε" };
vector<string> Ttui = { "FB" };
vector<string> Btui = { "*FB","ε" };
vector<string> Ftui = { "(E)","i" };

/**
 * vector<vector<string>>
 * * 用来存储文法的产生式.
 * * eg: vector<string> Etui = { "TA" };
 * */
vector<vector<string>> tui = { Etui,Atui,Ttui,Btui,Ftui };

/**
 * map<string, bool>
 * * * * 用来存储文法的生成式是否含有ε.
 * * * * eg: map<string, bool> tuiHav = { {"E",false},{"A",true},{"T",false},{"B",true},{"F",false} };
 * *
 * * * */
map<string, bool> tuiHav;

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
///*后来发现没用
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

void initALL();
void FindFIRSTset();
void FindFOLLOWset();
void doAnalysisTable();

void printMapSet(const map<string, set<string>>& Diy);
void printHav(map<string, bool> Have);
void printAnT();


int main()
{
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
	return 0;
}

/**
 * 初始化.
 * *
 */
void initALL()
{
	FIRST_f.resize(tuiName.size());
	FOLLOW_f_Follow.resize(tuiName.size());
	for (int i = 0; i < tuiName.size(); i++)
	{
		//1.初始化 tuiHav
		if (find(tui[i].begin(), tui[i].end(), "ε") != tui[i].end())
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

				if (tmp == "ε")
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
			if (tuii != "ε")
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
								if (k != "ε")
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
								if (k != "ε")
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
			if (tuii != "ε")
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
									if (tuiZ == "ε")
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
			if (tzi != "ε")
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
			if (tzi != "ε")
			{
				if (tui[i].size() == 1 || tui[i].size() == 2 && tuiHav[tuiName[i]])
				{
					string tmp = "";
					//不含有ε的生成式只有一个
					for (auto tuii : tui[i])
					{
						if (tuii != "ε")
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
						if (tuii != "ε")
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
				AnalysisTable[k][ttt].insert("ε");
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
					cout << setw(5) << tmp << "->" << tuii;
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
