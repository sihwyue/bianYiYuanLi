#include<string>
#include<iostream>
#include<vector>

using namespace std;

/**
 * ��֤��ѧ������תС��.
 * 
 */
void test01()
{
	string tmp = "2.3E5";
	string tmp1="2.32365989e5";
	double do1 = stod(tmp);
	double do2 = stod(tmp1);
	//cout<<(double)stod(tmp)<<endl;
	//cout<<double(stod(tmp1))<<endl;
	cout<<do1<<endl;	
	cout<<do2<<endl;
	printf("%.2f\n",do1);
	printf("%.2f\n",do2);
}
/**
 * ��֤append����.
 * 
 */
void test02()
{
	string wd = "123456ap";
	string tmp;
	string tmp1;
	for (int i = 0; i < wd.size(); i++)
	{
		//�������ֿ�ͷ����ô�Ϳ����Ƿ���㣬�������������ǿ�ѧ������

		//����_����ĸ��ͷ����ô�Ϳ����Ƿ��Ǳ�ʶ��
		//int a=3; 
		tmp1=wd[i];
		tmp.append(tmp1);
	}
	cout << tmp << endl;
}

/**
 * ��֤string����+������   + char����.
 * 
 */
void test03()
{
	string wd = "56op";
	string wd1 = wd + 'C';
	cout<<wd1<<endl;
	char s = 'm';
	wd1 = wd1 + s;
	cout<<wd1<<endl;
}
int main()
{
	//test01();
	//test02();
	test03();
	return 0;
}
