#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

enum MyEnum
{
	kdType = 0, idType, numIntType, numFlType, OperType, InviType,
};
/**
 * 定义保留字集合.
 * 保留字的类型为keyword : 0
 */
struct ukeyword {
	string value;
	int type; // 类型
	int length;  // 存储长度
};

// 保留字,类型为keyword : 0
unordered_set<string> keywords = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum",
			"extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed",
			"sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while" };

vector<ukeyword> keywordlist; // 保留字表

/**
 * 定义标识符表项结构体.
 * 变量名字类型为id  :1
 */
struct Identifier {
	string value;
	int type; // 类型
	int length;  // 存储长度
};

vector<Identifier> idlist; // 用户标识符表


/**
 * 定义无符号整数常数表项结构体.
 * 类型为num_int ：2
 */
struct UnsignedInteger {
	string value;
	int type; // 类型
	int length;  // 存储长度
};

vector<UnsignedInteger> uintlist; // 无符号整数常数表

/**
 * 定义无符号浮点数常数表项结构体.
 * 类型为num_float:3
 */
struct UnsignedFloat {
	string value;
	int type; // 类型
	int length;  // 存储长度
};

vector<UnsignedFloat> ufdlist; // 无符号浮点数常数表



/**
 * 定义操作符表项结构体.
 * 类型为 Operator:4.
 */
struct uOperator {
	string value;
	int type; // 类型
	int length;  // 存储长度
};
vector<uOperator> operatorlist; // 操作符表
unordered_set<string> Operators = { "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "sizeof", "new", "->", ".", ":", "?", "::", ",", ";", "(", ")", "[", "]", "{", "}" };

/**
 * 定义无法识别的字符的结构体
 * 类型为 Invalid:5.
 */
struct
	uInvalid {
	string value;
	int type; // 类型
	int length;  // 存储长度
};
vector<uInvalid> invalidlist; // 无法识别的字符表

/**
 * output输出结构.
 */
struct uOutput
{
	string value;
	int type; // 类型
	// 构造函数接受两个参数
	uOutput(const string& v, int t) : value(v), type(t) {}
};
vector<uOutput> outputlist;//输出表


// 函数声明
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

		//根据空格先找出有几个待辨别的“单词”。
		while (iss >> word_ge)
		{
			word_list.emplace_back(word_ge);
		}





		for (auto wd : word_list)
		{
			//操作符和保留字不需要判断是否是标识符
			if (isKeyword(wd) || isOperator(wd))
			{
				continue;
			}
			else
			{

				//判断是否是用户标识符
				bool flag = true;
				if (isalpha(wd[0]) || wd[0] == '_')//认为单独的下划线是一个标识符
				{
					for (int i = 1; i < wd.size(); i++)
					{
						if (!(isalpha(wd[i]) || wd[i] == '_' || isdigit(wd[i])))
						{
							flag = false;
							break;
						}
					}
					if (flag)
					{
						addToIdList(wd);
						continue;
					}
					flag = true;
				}

				//判断是否是无符号整数
				if (isdigit(wd[0]))
				{
					for (int i = 1; i < wd.size(); i++)
					{
						if (!isdigit(wd[i]))
						{
							flag = false;
							break;
						}
					}
					if (flag)
					{
						addToUintList(wd);
						continue;
					}
					flag = true;

				}
				//判断是否是无符号浮点数
				if (isdigit(wd[0]))
				{
					int dot_count = 0;
					for (int i = 1; i < wd.size() && dot_count < 2; i++)
					{
						if (!isdigit(wd[i]) && wd[i] != '.')
						{
							flag = false;
							break;
						}
						if (wd[i] == '.')
						{
							dot_count++;
						}

					}
					if (dot_count < 2 && flag)
					{
						addToUfdList(wd);
						continue;
					}
					flag = true;
				}

				//无法识别的字符
				addToInvalidList(wd);

			}
		}
	}

	//打印
	printikeyword();
	printId();
	printUint();
	printUfd();
	printInvalid();
	printOut();
	
	//写入文件
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
 * 检查是否是保留字，如果是，就存储到keywordlist里面.
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
				return true; // 已存在，不添加
			}
		}
		keywordlist.push_back({ word, kdType, (int)word.size() });
		return true;
	}
	return false;
}


/**
 * 添加到用户标识符表.
 * struct Identifier {
 *   string name;
 *   int type; // 类型
 *   int length;  // 存储长度
 *};
 *   vector<Identifier> idlist; // 用户标识符表
 * \param name
 */
void addToIdList(const string& name) {
	// 检查是否已存在
	addOutput(name, idType);
	for (const auto& id : idlist) {
		if (id.value == name) {
			return; // 已存在，不添加
		}
	}
	// 添加新标识符
	idlist.emplace_back(Identifier{ name, idType, (int)name.size() });
}

/**
 * .
 * struct UnsignedInteger {
 *    string value;
 *    int type; // 类型
 *    int length;  // 存储长度
 *};

 *    vector<UnsignedInteger> uintlist;
 * \param value
 */
 // 添加到无符号整数常数表
void addToUintList(const string& value) {
	// 检查是否已存在
	addOutput(value, numIntType);
	for (const auto& num : uintlist) {
		if (num.value == value) {
			return; // 已存在，不添加
		}
	}
	uintlist.emplace_back(UnsignedInteger{ value, numIntType, (int)value.size() });
}

// 
/**
 *添加到无符号浮点数常数表 .
 * struct UnsignedFloat {
 *    string value;
 *   int type; // 类型
 *    int length;  // 存储长度
 * };

 *   vector<UnsignedFloat> ufdlist;
 * \param value
 */
void addToUfdList(const string& value) {
	addOutput(value, numFlType);
	for (const auto& num : ufdlist) {
		if (num.value == value) {
			return; // 已存在，不添加
		}
	}
	ufdlist.emplace_back(UnsignedFloat{ value, numFlType, (int)value.size() });

}




/**
 * 添加非法字符.
 * struct uInvalid {
 *	string value;
 *	int type; // 类型
 *	int length;  // 存储长度
 *};
 *  vector<uInvalid> invalidlist;
 * \param value
 */
void addToInvalidList(const string& value) {
	addOutput(value, InviType);
	for (const auto& invd : invalidlist) {
		if (invd.value == value) {
			return; // 已存在，不添加
		}
	}
	invalidlist.emplace_back(uInvalid{ value, InviType, (int)value.size() });
}

/**
 *添加操作符 .
 * struct uOperator {
 *    string value;
 *	int type; // 类型
 *	int length;  // 存储长度
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
				return true; // 已存在，不添加
			}
		}
		operatorlist.emplace_back(uOperator{ value, numIntType, static_cast<int>(value.size()) });
		return true;
	}
	return false;
}


/**
 * 添加输出.
 * struct uOutput
 * {
 *     string value;
 *     int type; // 类型
 * };
 * vector<uOutput> outputlist;
 * \param value
 * \param type
 */
void addOutput(const string& value, const int& type) {
	outputlist.push_back(uOutput{ value, type });
}


/**
 * 写入到output.txt.
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
	cout << "输出表已经写入到output.txt文件中" << endl;
	cout << endl;
}

/**
 * 写入到idlist.txt.
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
	cout << "用户标识符表已经写入到idlist.txt文件中" << endl;
	cout << endl;
}

/**
 * 写入到uintlist.txt.
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
	cout << "无符号整数常数表已经写入到uintlist.txt文件中" << endl;
	cout << endl;
}

/**
 * 写入到ufdlist.txt.
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
	cout << "无符号浮点数常数表已经写入到ufdlist.txt文件中" << endl;
	cout << endl;
}

/**
 * 打印输出表.
 * 
 */
void printOut()
{
	cout << "***********现在开始打印输出表***********" << endl;
	for (auto &ou : outputlist)
	{
		cout << " ( " << ou.value << " , " << ou.type << " ) " << endl;
	}
	cout << "***********打印输出表结束***********" << endl;
	cout << endl;
}

void printikeyword()
{
	cout << "**********现在开始打印出现过的保留字表**********" << endl;
	for (auto& kyd : keywordlist)
	{
		cout << " ( " << kyd.value << " , " << kyd.type << " , " << kyd.length << " ) " << endl;
	}
	cout << "************打印出现过的保留字表结束************" << endl;
	cout << endl;
}
void printId()
{
	cout << "***********现在开始打印用户标识符表***********" << endl;
	for (auto &id : idlist)
	{
		cout << " ( " << id.value << " , " << id.type << " , " << id.length << " ) " << endl;
	}
	cout << "***********打印用户标识符表结束***********" << endl;
	cout << endl;
}
/**
 * 打印无符号整数.
 * 
 */
void printUint()
{
	cout << "***********现在开始打印无符号整数常数表***********" << endl;
	for (auto &ui : uintlist)
	{
		cout << " ( " << ui.value << " , " << ui.type << " , " << ui.length << " ) " << endl;
	}
	cout << "***********打印无符号整数常数表结束***********" << endl;
	cout << endl;
}

/**
 * 打印无符号浮点数.
 * 
 */
void printUfd()
{
	cout << "***********现在开始打印无符号浮点数常数表***********" << endl;
	for (auto &uf : ufdlist)
	{
		cout << " ( " << uf.value << " , " << uf.type << " , " << uf.length << " ) " << endl;
	}
	cout << "***********打印无符号浮点数常数表结束***********" << endl;
	cout<< endl;
}

/**
 * 打印无法识别的字符.
 * 
 */
void printInvalid()
{
	cout << "***********现在开始打印无法识别的字符表***********" << endl;
	for (auto &inv : invalidlist)
	{
		cout << " ( " << inv.value << " , " << inv.type << " , " << inv.length << " ) " << endl;
	}
	cout << "**************打印无法识别的字符表结束************" << endl;
	cout << endl;
}
