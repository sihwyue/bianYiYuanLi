#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include<cstdio>

using namespace std;


vector<string> typeChinese = { "������","�û���ʶ��","�޷�������","�޷��Ÿ�����","������","�޷�ʶ����ַ�" };

/**
 * ����ö������.
 * */
enum MyEnum
{
	kdType = 0, idType, numIntType, numFlType, OperType, InviType,
};
/**
 * ���屣���ּ���.
 * �����ֵ�����Ϊkeyword : 0
 */
struct ukeyword {
	string value;
	int type; // ����
	int length;  // �洢����
};

// ������,����Ϊkeyword : 0
unordered_set<string> keywords = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum",
			"extern", "float", "for", "goto", "if", "int","input", "long","main","register", "return", "short", "signed",
			"sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while" };

vector<ukeyword> keywordlist; // �����ֱ�

/**
 * �����ʶ������ṹ��.
 * ������������Ϊid  :1
 */
struct Identifier {
	string value;
	int type; // ����
	int length;  // �洢����
};

vector<Identifier> idlist; // �û���ʶ����


/**
 * �����޷���������������ṹ��.
 * ����Ϊnum_int ��2
 */
struct UnsignedInteger {
	string value;
	int type; // ����
	int length;  // �洢����
};

vector<UnsignedInteger> uintlist; // �޷�������������

/**
 * �����޷��Ÿ�������������ṹ��.
 * ����Ϊnum_float:3
 */
struct UnsignedFloat {
	string value;
	int type; // ����
	int length;  // �洢����
};

vector<UnsignedFloat> ufdlist; // �޷��Ÿ�����������



/**
 * �������������ṹ��.
 * ����Ϊ Operator:4.
 */
struct uOperator {
	string value;
	int type; // ����
	int length;  // �洢����
};
vector<uOperator> operatorlist; // ��������
unordered_set<string> Operators = { "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "sizeof", "new", "->", ".", ":", "?", "::", ",", ";", "(", ")", "[", "]", "{", "}" };

/**
 * �����޷�ʶ����ַ��Ľṹ��
 * ����Ϊ Invalid:5.
 */
struct
	uInvalid {
	string value;
	int type; // ����
	int length;  // �洢����
};
vector<uInvalid> invalidlist; // �޷�ʶ����ַ���

/**
 * output����ṹ.
 */
struct uOutput
{
	string value;
	int type; // ����
	// ���캯��������������
	uOutput(const string& v, int t) : value(v), type(t) {}
};
vector<uOutput> outputlist;//�����


// ��������
bool isKeyword(const string& word);

void addToIdList(const string& name);
void addToUintList(const string& value);
void addToUfdList(const string& value);
void addToInvalidList(const string& value);
bool isOperator(const string& value);
void addOutput(const string& value, const int& type);

void writeOut(ofstream& output_file);
void writeId(ofstream& idlist_file);
void writeUint(ofstream& uintlist_file);
void writeUfd(ofstream& ufdlist_file);

void printOut();
void printId();
void printUint();
void printUfd();
void printInvalid();
void printikeyword();

void panDuan(const string& wd);
void fenGe(const string& wd);//��һ���ָ�� a<=3,���ָ�� a <= 3


int main() {
	ifstream input("input.txt");
	ofstream output_file("output.txt");
	ofstream idlist_file("idlist.txt");
	ofstream uintlist_file("uintlist.txt");
	ofstream ufdlist_file("ufdlist.txt");

	if (!input || !output_file || !idlist_file || !uintlist_file || !ufdlist_file) {
		cerr << "Error opening files." << endl;
		return 1;
	}

	string word;
	while (getline(input, word)) {
		istringstream iss(word);
		string word_ge;
		vector<string> word_list;

		//���ݿո����ҳ��м��������ġ����ʡ���
		while (iss >> word_ge)
		{
			word_list.emplace_back(word_ge);
		}

		for (auto wd : word_list)
		{
			//��һ���ָ�
			fenGe(wd);
		}
	}

	//��ӡ
	printikeyword();
	printId();
	printUint();
	printUfd();
	printInvalid();
	printOut();

	//д���ļ�
	writeId(idlist_file);
	writeUint(uintlist_file);
	writeUfd(ufdlist_file);
	writeOut(output_file);
	idlist_file.close();
	uintlist_file.close();
	ufdlist_file.close();
	output_file.close();

	return 0;
}

/**
 * ����Ƿ��Ǳ����֣�����ǣ��ʹ洢��keywordlist����.
 *
 * \param word
 * \return
 */
bool isKeyword(const string& word) {

	if (keywords.find(word) != keywords.end())
	{
		addOutput(word, kdType);
		for (const auto& keyd : keywordlist) {
			if (keyd.value == word) {
				return true; // �Ѵ��ڣ������
			}
		}
		keywordlist.push_back({ word, kdType, (int)word.size() });
		return true;
	}
	return false;
}


/**
 * ��ӵ��û���ʶ����.
 * struct Identifier {
 *   string name;
 *   int type; // ����
 *   int length;  // �洢����
 *};
 *   vector<Identifier> idlist; // �û���ʶ����
 * \param name
 */
void addToIdList(const string& name) {
	// ����Ƿ��Ѵ���
	addOutput(name, idType);
	for (const auto& id : idlist) {
		if (id.value == name) {
			return; // �Ѵ��ڣ������
		}
	}
	// ����±�ʶ��
	idlist.emplace_back(Identifier{ name, idType, (int)name.size() });
}

/**
 * .
 * struct UnsignedInteger {
 *    string value;
 *    int type; // ����
 *    int length;  // �洢����
 *};

 *    vector<UnsignedInteger> uintlist;
 * \param value
 */
 // ��ӵ��޷�������������
void addToUintList(const string& value) {
	// ����Ƿ��Ѵ���
	addOutput(value, numIntType);
	for (const auto& num : uintlist) {
		if (num.value == value) {
			return; // �Ѵ��ڣ������
		}
	}
	uintlist.emplace_back(UnsignedInteger{ value, numIntType, (int)value.size() });
}

// 
/**
 *��ӵ��޷��Ÿ����������� .
 * struct UnsignedFloat {
 *    string value;
 *   int type; // ����
 *    int length;  // �洢����
 * };

 *   vector<UnsignedFloat> ufdlist;
 * \param value
 */
void addToUfdList(const string& value) {
	addOutput(value, numFlType);
	double valueToDo = stod(value);
	char buffer[100];
	sprintf(buffer, "%.4f", valueToDo);
	string valueToDoToStr = buffer;


	for (const auto& num : ufdlist) {
		if (num.value == valueToDoToStr) {
			return; // �Ѵ��ڣ������
		}
	}
	ufdlist.emplace_back(UnsignedFloat{ valueToDoToStr, numFlType, (int)valueToDoToStr.size() });

}




/**
 * ��ӷǷ��ַ�.
 * struct uInvalid {
 *	string value;
 *	int type; // ����
 *	int length;  // �洢����
 *};
 *  vector<uInvalid> invalidlist;
 * \param value
 */
void addToInvalidList(const string& value) {
	addOutput(value, InviType);
	for (const auto& invd : invalidlist) {
		if (invd.value == value) {
			return; // �Ѵ��ڣ������
		}
	}
	invalidlist.emplace_back(uInvalid{ value, InviType, (int)value.size() });
}

/**
 *��Ӳ����� .
 * struct uOperator {
 *    string value;
 *	int type; // ����
 *	int length;  // �洢����
 *};
 *  vector<uOperator> operatorlist;
 * \param value
 */
bool isOperator(const string& value) {

	if (Operators.find(value) != Operators.end())
	{
		addOutput(value, OperType);
		for (const auto& ope : operatorlist) {
			if (ope.value == value) {
				return true; // �Ѵ��ڣ������
			}
		}
		operatorlist.emplace_back(uOperator{ value, numIntType, static_cast<int>(value.size()) });
		return true;
	}
	return false;
}


/**
 * ������.
 * struct uOutput
 * {
 *     string value;
 *     int type; // ����
 * };
 * vector<uOutput> outputlist;
 * \param value
 * \param type
 */
void addOutput(const string& value, const int& type) {
	outputlist.push_back(uOutput{ value, type });
}


/**
 * д�뵽output.txt.
 *
 * \param output_file
 */
void writeOut(ofstream& output_file)
{
	output_file << "(Value,\tType)\n";
	for (const auto& ot : outputlist)
	{
		output_file << " ( " << ot.value << " , " << ot.type << " ) " << "\n";

	}
	cout << "������Ѿ�д�뵽output.txt�ļ���" << endl;
	cout << endl;
}

/**
 * д�뵽idlist.txt.
 *
 * \param idlist_file
 */
void writeId(ofstream& idlist_file)
{
	idlist_file << "(Value,\tType,\tLength)\n";
	for (const auto& id : idlist)
	{
		idlist_file << " ( " << id.value << " , " << id.type << " , " << id.length << " ) " << "\n";

	}
	cout << "�û���ʶ�����Ѿ�д�뵽idlist.txt�ļ���" << endl;
	cout << endl;
}

/**
 * д�뵽uintlist.txt.
 *
 * \param uintlist_file
 */
void writeUint(ofstream& uintlist_file)
{
	uintlist_file << "(Value,\tType,\tLength)\n";
	for (const auto& ui : uintlist)
	{
		uintlist_file << " ( " << ui.value << " , " << ui.type << " , " << ui.length << " ) " << "\n";

	}
	cout << "�޷����������Ѿ�д�뵽uintlist.txt�ļ���" << endl;
	cout << endl;
}

/**
 * д�뵽ufdlist.txt.
 *
 * \param ufdlist_file
 */
void writeUfd(ofstream& ufdlist_file)
{
	ufdlist_file << "(Value,\tType,\tLength)\n";
	for (const auto& uf : ufdlist)
	{
		ufdlist_file << " ( " << uf.value << " , " << uf.type << " , " << uf.length << " ) " << "\n";

	}
	cout << "�޷��Ÿ��������Ѿ�д�뵽ufdlist.txt�ļ���" << endl;
	cout << endl;
}

/**
 * ��ӡ�����.
 *
 */
void printOut()
{
	cout << "***********���ڿ�ʼ��ӡ�����***********" << endl;
	cout << "˵����\nType=0��ʾ�����֣�Type=1��ʾ�û���ʶ����Type=2��ʾ�޷�������,\nType=3��ʾ�޷��Ÿ�������Type=4��ʾ��������Type=5��ʾ�޷�ʶ����ַ�" << endl;
	cout << " ( Type ,Value\t)\t��������" << endl;
	for (auto& ou : outputlist)
	{
		printf(" ( %1d ,%5s\t)\t%s\n", ou.type, ou.value.c_str(), typeChinese[ou.type].c_str());
		//printf(" ( %5s,\t%3d )\t%s\n", ou.value.c_str(), ou.type, typeChinese[ou.type].c_str());
		
		//printf(" ( %5s,\t%3d )\t\n", ou.value.c_str(), ou.type);
	}
	cout << "***********��ӡ��������***********" << endl;
	cout << endl;
}

void printikeyword()
{
	cout << "**********���ڿ�ʼ��ӡ���ֹ��ı����ֱ�**********" << endl;
	cout << " (   Value,  Type,  Length )" << endl;
	for (auto& kyd : keywordlist)
	{
		printf(" ( %7s,\t%1d,\t%2d ) \n", kyd.value.c_str(), kyd.type, kyd.length);
		//cout << " ( " << kyd.value << ",\t" << kyd.type << ",\t" << kyd.length << " ) " << endl;
	}
	cout << "************��ӡ���ֹ��ı����ֱ����************" << endl;
	cout << endl;
}
void printId()
{
	cout << "***********���ڿ�ʼ��ӡ�û���ʶ����***********" << endl;
	cout << " (    Value, Type,  Length )" << endl << "------------------------------" << endl;
	for (auto& id : idlist)
	{
		printf(" ( %8s,\t%1d,\t%2d ) \n", id.value.c_str(), id.type, id.length);
	}
	cout << "------------------------------" << endl;
	cout << "***********��ӡ�û���ʶ�������***********" << endl;
	cout << endl;
}
/**
 * ��ӡ�޷�������.
 *
 */
void printUint()
{

	cout << "***********���ڿ�ʼ��ӡ�޷���������***********" << endl;
	cout << " ( Value, Type, Length )" << endl << "-------------------------" << endl;
	for (auto& ui : uintlist)
	{
		printf(" ( %5s, %3d, %5d   ) \n", ui.value.c_str(), ui.type, ui.length);
	}
	cout << "------------------------" << endl;
	cout << "***********��ӡ�޷������������***********" << endl;

	cout << endl;
}

/**
 * ��ӡ�޷��Ÿ�����.
 *
 */
void printUfd()
{

	cout << "***********���ڿ�ʼ��ӡ�޷��Ÿ�������***********" << endl;
	cout << " (  Value,\tType,  Length )" << endl;
	cout << "---------------------------------" << endl;
	for (auto& uf : ufdlist)
	{
		printf(" ( %9s,\t%3d,\t%5d ) \n", uf.value.c_str(), uf.type, uf.length);
	}
	cout << "---------------------------------" << endl;
	cout << "***********��ӡ�޷��Ÿ����������***********" << endl;
	cout << endl;
}

/**
 * ��ӡ�޷�ʶ����ַ�.
 *
 */
void printInvalid()
{
	cout << "***********���ڿ�ʼ��ӡ�޷�ʶ����ַ���***********" << endl;
	cout << " ( Value,\tType,\tLength )" << endl;
	cout << "---------------------------------" << endl;
	for (auto& inv : invalidlist)
	{
		printf(" ( %5s,\t%3d,\t%5d  ) \n", inv.value.c_str(), inv.type, inv.length);
	}
	cout << "---------------------------------" << endl;
	cout << "**************��ӡ�޷�ʶ����ַ������************" << endl;
	cout << endl;
}


/**
 * ��һ���ָ�.
 *
 * \param wd ���ָ���ַ��� ������ a<=3 ���Խ�һ���ָ�� a <= 3
 */
void fenGe(const string& wd)
{

	//�������ͱ����ֲ���Ҫ�ж��Ƿ��Ǳ�ʶ��
	if (isKeyword(wd) || isOperator(wd))
	{
		return;
	}
	else
	{
		string tmp;//�м�����������洢����Լ�ַ�
		string tmp1;//�洢���δ��ж��ַ�
		for (int i = 0; i < wd.size(); i++)
		{
			//�������ֿ�ͷ����ô�Ϳ����Ƿ���㣬�������������ǿ�ѧ������

			//����_����ĸ��ͷ����ô�Ϳ����Ƿ��Ǳ�ʶ��
			//int a=3; 
			tmp1 = wd[i];
			if (Operators.count(tmp1))
			{
				//����������
				if (tmp.size())
				{
					//�洢���ж���
					if (!Operators.count(tmp))
					{
						//Ҳ����˵��tmp�в�����������(������һ�����֡�Ҳ������һ����ĸ������)
						//ȷ�е�˵�����Ĳ��Ǵ���Ĳ�����

						if (isdigit(tmp[0]) && tmp1 == ".")
						{
							//������С�����Ȱ���С���ж�
							tmp.append(tmp1);
							continue;
						}
						//���й�Լ��tmp�����ϡ�αС������Ҳ���ǲ���������tmp1�ǲ�����,���Թ�Լ
						panDuan(tmp);
						tmp.clear();
						tmp.append(tmp1);
						continue;
					}
					//�洢���ǲ�����
					//Ҫ�ж��Ƿ���Գ�Ϊ��ϲ�����
					string tmp2 = tmp + tmp1;//
					if (Operators.count(tmp2))
					{
						//���Գ�Ϊ��ϲ�����
						//�ж��Ƿ���������ϲ�����
						string tmp3 = tmp2 + wd[i + 1];
						if (Operators.count(tmp3))
						{
							panDuan(tmp3);
							tmp.clear();
							i++;
							continue;
						}
						panDuan(tmp2);
						tmp.clear();
						continue;
					}
					else
					{
						//������ϲ�������ֱ�ӹ�Լ
						panDuan(tmp);
						tmp.clear();
						tmp.append(tmp1);
					}
				}
				else
				{
					//tmpΪ�գ�ֱ�Ӵ洢
					tmp.append(tmp1);
				}

			}
			else if (tmp.size() && Operators.count(tmp))
			{
				//tmp����ǲ�������tmp1���ǲ��������ʹ�Լ

				panDuan(tmp);
				tmp.clear();
				tmp.append(tmp1);
			}
			else if (isalpha(tmp1[0]) || isdigit(tmp1[0]) || tmp1 == "_" || (tmp.size() && isdigit(tmp[0]) && tmp1 == "."))
			{
				//tmp�������ĸ�������ֻ���_��tmp1Ҳ����ĸ�������֣��ʼ������
				//δ����������������
				tmp.append(tmp1);
			}
			else
			{
				//tmp�������ĸ�������ֻ���_��tmp1����Ч�ַ����ʹ�Լ
				//��Ч�ַ�
				if (tmp.size())
				{
					//��Լ
					panDuan(tmp);
					tmp.clear();
				}
				//����Ч�ַ�ֱ�ӽ��й�Լ
				panDuan(tmp1);
			}
		}
	}
}

/**
 * �ж��Ƿ��Ǳ����֡���ʶ�����޷����������޷��Ÿ��������޷�ʶ����ַ�.
 * *
 * \param wd
 * */
void panDuan(const string& wd)
{
	//�������ͱ��������ж�Ϊ��ʱ���ͽ����˴洢
	if (isKeyword(wd) || isOperator(wd))
	{
		return;
	}
	//�ж��Ƿ����û���ʶ��
	bool flag = true;
	if (isalpha(wd[0]) || wd[0] == '_')//��Ϊ�������»�����һ����ʶ��
	{
		for (int i = 1; i < wd.size(); i++)
		{
			if (!(isalpha(wd[i]) || wd[i] == '_' || isdigit(wd[i])))
			{
				//������ĸ�����֡��»���,�����û���ʶ��
				flag = false;
				break;
			}
		}
		if (flag)
		{
			//���û���ʶ��
			addToIdList(wd);
			return;
		}
		flag = true;
	}

	//�ж��Ƿ����޷�������
	if (isdigit(wd[0]))
	{
		for (int i = 1; i < wd.size(); i++)
		{
			if (!isdigit(wd[i]))
			{
				//��������
				flag = false;
				break;
			}
		}
		if (flag)
		{
			//���޷�������
			addToUintList(wd);
			return;
		}
		flag = true;

	}
	//�ж��Ƿ����޷��Ÿ�����
	if (isdigit(wd[0]))
	{
		int dot_count = 0, eE = 0;
		for (int i = 1; i < wd.size() && dot_count < 2; i++)
		{
			if (!isdigit(wd[i]) && wd[i] != '.' && wd[i] != 'E' && wd[i] != 'e')
			{
				flag = false;
				break;
			}
			if (wd[i] == '.')
			{
				dot_count++;
			}
			if (wd[i] == 'E' || wd[i] == 'e')
			{
				eE++;
			}

		}
		if (dot_count < 2 && flag && eE < 2)
		{
			addToUfdList(wd);
			return;
		}
		flag = true;
	}

	//�޷�ʶ����ַ�
	addToInvalidList(wd);
}
