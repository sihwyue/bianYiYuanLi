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

string epsilon = "��";
string tuiDaoFu = "��";
/**
 * set<string>.
 *�洢���ս�������ڱ�������
 */
set<string> fFu;

/**
 * set<string>�洢�ս�������ڱ�������.
 */
set<string> tFu;//epsilon�ͦŶ��ǿմ�
vector<string> tableTop;


/**
 * vector<vector<string>>
 * * �����洢�ķ��Ĳ���ʽ.
 * * eg: vector<string> Etui = { "TA" };
 * */
vector<vector<string>> tui;

/**
 * map<string, bool>
 * * * * �����洢�ķ�������ʽ�Ƿ��Ц�.
 * * * * eg: map<string, bool> tuiHav = { {"E",false},{"A",true},{"T",false},{"B",true},{"F",false} };
 * *
 * * * */
map<string, bool> tuiHav;
/**
 * vector<string>
 * * * �����洢�ķ��ķ��ս��.
 * * * eg: vector<string> tuiName = { "E","A","T","B","F" };
 * ����λ����vector<vector<string>> tui��Ӧ
 * * */
vector<string> tuiName;


/**
* map<string,set<string>>
 * ������FIRST���ǲ�Ϊ�յģ����Բ���Ҫ��ʼ��.
 */
map<string, set<string>> FIRSTsetMap;

/**
* map<string, bool>
 * �洢FIRST�Ƿ��Ц�.
 */
map<string, bool> FIRSTHave;



/**
 * vector<set<string>>
 * �����洢���ս��������һ���ս������������FIRST����.
 * eg: set<string> FIRST_E_f;
 */
vector<set<string>> FIRST_f;

/**
* map<string,set<string>>
 * ������FOLLOW���ǲ�Ϊ�յ�
 */
map<string, set<string>> FOLLOWsetMap;

///**
// * vector<set<string>>
// * * �����洢��һ���������������FOLLOW����.
// * * eg: set<string> FOLLOW_E_f_FIRST;
// *
// */
//vector<set<string>> FOLLOW_f_FIRST = { FOLLOW_E_f_FIRST,FOLLOW_A_f_FIRST,FOLLOW_T_f_FIRST,FOLLOW_B_f_FIRST,FOLLOW_F_f_FIRST };

/**
 * vector<set<string>>
 * * �����洢�ڶ����������������FOLLOW����.
 * * eg: set<string> FOLLOW_E_f_FOLLOW;
 */
vector<set<string>> FOLLOW_f_Follow;



/**
* map<string, map<string, set<string>>>
 * �����洢���ķ�����.
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

	cout << "-----------------FIRST����---------------" << endl;
	printMapSet(FIRSTsetMap);
	cout << "-----------------------------------------" << endl;

	cout << "----------�ķ�����ʽ�Ƿ��Ц�-----------" << endl;
	printHav(tuiHav);
	cout << "-----------------------------------------" << endl;

	cout << "-------------FIRST�Ƿ��Ц�-------------" << endl;
	printHav(FIRSTHave);
	cout << "-----------------------------------------" << endl;
	FindFOLLOWset();

	cout << "-----------------FOLLOW����--------------" << endl;
	printMapSet(FOLLOWsetMap);
	cout << "-----------------------------------------" << endl;

	doAnalysisTable();
	cout << "-----------------������------------------" << endl;
	printAnT();
	cout << "-----------------------------------------" << endl;
	writeOut();
	return 0;
}

/**
 * ��ȡ�ķ�.
 * �����±�֤
 * 1.���ս��˳�򣬼�ס����һ�����ȥ�ļ�Ϊ��ʼ����
 * 2.��֤ÿһ��ֻ��һ���մ�"��"
 * 3.ÿһ���Ƴ��Ĳ�ͬ������"|"�ָ�
 * 4.���ս�����ǵ�����д��ĸ
 * 5.�ս�����ǵ����ַ���ASCII��
 * * *
 */
void readTui()
{
	//��ȡ�ķ�
	//1.���ļ�
	ifstream input("input.txt");

	if (!input)
	{
		cout << "open file error" << endl;
		return;
	}

	//2.��ʼ�����Ȱ���20�����ս�����ж�ȡ
	tui = vector<vector<string>>(20);

	//3.��ȡ�ķ�
	string tmp;
	while (getline(input, tmp))
	{
		int i = 0;//����,������д���

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
	//4.��ʼ��tableTop
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
 * ��ʼ��.
 * *
 */
void initALL()
{
	FIRST_f.resize(tuiName.size());
	FOLLOW_f_Follow.resize(tuiName.size());

	//1.��ʼ�� tuiHav
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

		//2.��ʼ�� FIRSTsetMap
		if (!i)
		{
			FOLLOWsetMap[tuiName[i]] = { "#" };
		}
		else
		{
			FOLLOWsetMap[tuiName[i]] = {};
		}
		//3.��ʼ��FIRST���ϵĸ�������
		FIRST_f[i] = {};
		//4.��ʼ��FOLLOW���ϵĸ�������
		FOLLOW_f_Follow[i] = {};
	}

}


/**
 * ��FIRST����.
 *
 */
void FindFIRSTset()
{
	for (int i = 0; i < tuiName.size(); i++)
	{
		//��ʼ�� FIRSTHave ��Ĭ�϶������Ц�
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
						//���������ڷ��ս�� eg: E ->TA  FIRST(E) += FIRST(T)
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
						//�������ս��  eg: E ->i  FIRST(E) += i
						FIRSTsetMap[tuiName[i]].insert(tmp.substr(0, 1));
						size_TM_after = FIRSTsetMap[tuiName[i]].size();
					}
				//֤�������
				if ((size_f_after - size_f_before) || (size_TM_after - size_TM_before))
				{
					flag = true;
				}
			}
		}
	}

}
/**
 * ��FOLLOW����.
 *
 */
void FindFOLLOWset()
{
	//��һ�������FIRST����
	for (auto tuiALL : tui)
	{
		//FIRST  A->aBb a�����ǿմ� FOLLOW(A)���� FIRST(b);
		//��ÿһ������ʽ������FIRST�����ж�,�����õ���auto tuiALL:tui  auto tuii:tuiALLֻ��Ҫ�����ұߣ�
		//һ��������
		for (auto tuii : tuiALL)
		{
			if (tuii != epsilon)
			{
				//�ǿմ�
				if (tuii.size() == 2)
				{
					//
					string tmp0 = tuii.substr(0, 1);
					string tmp1 = tuii.substr(1, 1);
					if (fFu.find(tmp0) != fFu.end())
					{
						//��һ���Ƿ��ս��
						if (tFu.find(tmp1) != tFu.end())
						{
							//�ڶ������ս��
							FOLLOWsetMap[tmp0].insert(tmp1);
							//��Ϊ���ս��ֱ�Ӽ��� tmp0��FOLLOW����
						}
						else
						{
							//�ڶ����Ƿ��ս��
							//ֱ�� ��tmp0����tmp1��FIRST����-{ "��"}
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
						//��֤tmp0�Ƿ��ս��
						if (i + 1 > tuii.size() - 1)
						{
							break;
						}
						tmp1 = tuii.substr(i + 1, 1);
						if (tFu.find(tmp1) != tFu.end())
						{
							//tmp1�Ƿ��ս��
							FOLLOWsetMap[tmp0].insert(tmp1);
							//��Ϊ���ս��ֱ�Ӽ��� tmp0��FOLLOW����
						}
						else
						{
							//tmp1�Ƿ��ս��
							//ֱ�� ��tmp0����tmp1��FIRST����-{ "��"}
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
	//�ڶ��������FOLLOW����  
	//FOLLOW  A->aBb b�����ǿմ� FOLLOW(B)���� FOLLOW(A);

	//1.�ҵ�B�ĵ����п��Ե�A
	for (int i = 0; i < tui.size(); i++)
	{
		for (auto tuii : tui[i])
		{
			if (tuii != epsilon)
			{
				//�ǿմ�
				string tmp0, tmp1;
				for (int j = 0; j < tuii.size(); j++)
				{
					tmp0 = tuii.substr(j, 1);
					if (fFu.find(tmp0) != fFu.end())
					{
						//���ս��
						if (j + 1 >= tuii.size())
						{
							//���һ�����ս������   A ->aB ��B FOLLOW(B)���� FOLLOW(A)
							//Ϊ�˷�ֹ�����Ч�������ȷŵ� vector<set<string>> FOLLOW_f_Follow ����
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
							//�������һ���ַ�
							//�ж���һ���ַ����ս�����Ƿ��ս��
							tmp1 = tuii.substr(j + 1, 1);
							if (fFu.find(tmp1) != fFu.end())
							{
								//���ս��
								//����tmp1�Ĳ���ʽ�Ƿ�"��"
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
									//����"��"
									// �� A->aBb   b=*>"��" FOLLOW(B)���� FEOLLOW(A)
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
								//�ս��
								continue;
							}
						}
					}
				}
			}
		}
	}
	//2.��ʼ���кϲ�

	//cout << endl << "�鿴�������" << endl;
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
 * �ݹ��½�����.
 * ���������������
 */
void doAnalysisTable()
{


	//��ʼ��
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

	//����first�������
	for (int i = 0; i < tuiName.size(); i++)
	{

		for (auto tzi : FIRSTsetMap[tuiName[i]])
		{
			if (tzi != epsilon)
			{
				if (tui[i].size() == 1 || tui[i].size() == 2 && tuiHav[tuiName[i]])
				{
					string tmp = "";
					//�����Цŵ�����ʽֻ��һ��
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
					//���ж����Ч����ʽ���Ա������ƥ��
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

	//����follow�������,FIRST����"��"�İ�FOLLOW����"��"
	for (auto [k, v] : FIRSTHave)
	{
		//k�Ƿ��ս��,v��bool
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
 * ��ӡmap<string,set<string>>.
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
 * ��ӡ�Ƿ�first/tui���Ц�.
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
 * ��ӡ������.
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
 * д���������ļ�.
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
