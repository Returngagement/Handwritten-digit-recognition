#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>
#include<io.h>                          
#include<string.h>
#include<string>

using namespace std;
using namespace cv;


void  ergodicImage(string filename, string name);//遍历文件并保存文件信息
string Image_compress(string Imagename);         //图片像素二值化（压缩）
int distance(string str1, string str2);          //欧氏距离
int turn(char a);
void compare();
void Basyes();



void main()
{
	const char* trainfile = "C:\\Users\\13296\\Desktop\\c++\\train-images";
	ergodicImage(trainfile, "train-num.txt");
	const char* testfile = "C:\\Users\\13296\\Desktop\\c++\\test-images";
	ergodicImage(testfile, "test-num.txt");
	compare();
	//Basyes();

}

void  ergodicImage(string filename, string name)             //遍历图片
{
	string firstfilename = filename + "\\*.bmp";
	struct _finddata_t fileinfo;
	intptr_t handle;                       //本来应该是long型，但一直报错，经查资料改为intptr_t
	string openname = "C:\\Users\\13296\\Desktop\\c++\\" + name;
	ofstream file;
	file.open(openname, ios::out);
	handle = _findfirst(firstfilename.c_str(), &fileinfo);            
	if (_findfirst(firstfilename.c_str(), &fileinfo) != -1)               //遍历所有后缀为.bmp的图片
	{
		do
		{
			file << fileinfo.name << ":" << Image_compress(filename + "\\" + fileinfo.name) << endl;
		} while (!_findnext(handle, &fileinfo));
		file.close();
		_findclose(handle);
	}

}

string Image_compress(string Imagename)//图片压缩
{
	Mat Image = imread(Imagename);
	cvtColor(Image, Image, COLOR_BGR2GRAY);
	int pixel[28][28];
	for (int row = 0; row < Image.rows; row++)
		for (int col = 0; col < Image.cols; col++)
		{
			pixel[row][col] = Image.at<uchar>(row, col);
		}                                                       //像素采集


	//图片压缩
	int x = 0, y = 0;

	string img_pixel = "";
	for (int k = 1; k < 50; k++)
	{
		int sign = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				if (pixel[x + i][y + j] > 127) sign += 1;
			}
		if (sign >= 6)                                 //阈值为6.在4*4中有6个像素点大于127，压缩二值为1
			img_pixel += '1';
		else
			img_pixel += '0';
		y = (y + 4) % 28;                        //纵向进行遍历压缩二值化
		if (k % 7 == 0)
		{
			x += 4;
			y = 0;
		}
	}
	return img_pixel;
}




int distance(string str1, string str2)  //比对两个字符串有多少个不一样
{
	int counts = 0;
	for (int i = 0; i < str1.length(); i++)
	{
		if (str1[i] == str2[i]) continue;
		else counts++;
	}
	return counts;
}
int turn(char a)
{
	stringstream str;
	int f = 1;
	str << a;
	str >> f;
	str.clear();
	return f;
}


void compare()
{
	ifstream test_data;
	ifstream train_data;
	string test_bmp1 = "";         //读取的二值化数据
	string test_bmp2 = "";
	string train_bmp1 = "";
	string train_bmp2 = "";
	bool refuse = false;//拒绝识别标识
	int recognize_num = 0;//识别的结果
	int num_refuse = 0, num_false = 0, num_true = 0;//拒绝识别，识别正确、错误的数量
	test_data.open("C:\\Users\\13296\\Desktop\\c++\\test-num.txt");
	for (int k = 0; k < 200; k++)
	{
		int sign = 49;//设置欧式距离阈值
		getline(test_data, test_bmp1);//遍历文件，每次读取一行
		test_bmp2 = test_bmp1;                          //对原始数据进行保留，后面通过test_bmp[2]确定正在识别的数字
		if (test_bmp1.length() == 57) test_bmp1.erase(0, 8);//确定图片二值开始的位置
		else test_bmp1.erase(0, 9);
		train_data.open("C:\\Users\\13296\\Desktop\\c++\\train-num.txt");
		for (int i = 0; i < 1000; i++)
		{
			getline(train_data, train_bmp1);
			train_bmp2 = train_bmp1;
			if (train_bmp1.length() == 57) train_bmp1.erase(0, 8);
			else train_bmp1.erase(0, 9);
			if (distance(train_bmp1, test_bmp1) < sign)   //找出最小距离
			{
				refuse = false;                              
				sign = distance(train_bmp1, test_bmp1);
				recognize_num = turn(train_bmp2[0]);

			}
			else if (distance(train_bmp1, test_bmp1) == sign && recognize_num != turn(train_bmp2[0]))
				//距离相等，但识别的数字不同
			{
				refuse = true;
				continue;
			}
		}
		train_data.close();//注意打开关闭的位置，没读取一次test数据就要打开关闭一次
		if (!refuse)
		{
			if (recognize_num == turn(test_bmp2[0]))
			{
				num_true++;
				cout << "识别为：" << recognize_num << endl;
			}
			else
			{
				num_false++;
				cout << "识别错误" << endl;
			}
		}
		else
		{
			num_refuse++;
			cout << "拒绝识别" << endl;
		}
	}
	test_data.close();
	double t = num_true / 200.0, f = num_false / 200.0, r = num_refuse / 200.0;
	cout << "正确率为：" << t << endl;
	cout << "错误率为：" << f << endl;
	cout << "拒绝识别率：" << r << endl;

}

//贝叶斯算法
void Basyes()
{
	string temp;
	ifstream train_data;
	double  num[10] = { 0 };   //数字1的个数
	double proportion[10][49];
	for (int i = 0; i < 49; i++)
	{

		int n = 0;          //二值起始位下标
		train_data.open("C:\\Users\\13296\\Desktop\\c++\\train-num.txt");
		for (int j = 0; j < 1000; j++)
		{
			getline(train_data, temp);
			if (temp.length() == 57)
			{
				n = i + 8;
				if (turn(temp[n]) == 1)
					num[turn(temp[0])]++;     //num数组精准统计各个数字在0-48位上1的个数
				else continue;
			}
			else if (temp.length() == 58)
			{
				n = i + 9;
				if (turn(temp[n]) == 1)
					num[turn(temp[0])]++;
				else continue;

			}
		}
		train_data.close();//关闭文件，每次遍历载重新打开，让getline遍历
		for (int m = 0; m < 10; m++)
		{
			proportion[m][i] = num[m] / 100.0;   //计算第i列的的比例
			num[m] = 0;
		}
	}
	double c_num = 0;//识别正确个数
	double f_num = 0;//识别错误个数
	double r_num = 0;//拒绝识别个数
	ifstream test_data;
	string test_num;
	double px[10];                 //记录后验概率
	test_data.open("C:\\Users\\13296\\Desktop\\c++\\test-num.txt");
	for (int i = 0; i < 200; i++)          //200张测试图片，遍历
	{
		for (int x = 0; x < 10; x++)px[x] = 1.0;
		int m = 0;
		getline(test_data, test_num);
		for (int o = 0; o < 10; o++) {
			for (int j = 0; j < 49; j++)            //每个图片49个二值 
			{

				if (test_num.length() == 57) m = j + 8;
				else m = j + 9;
				if (turn(test_num[m]) == 1)
					px[o] *= 1.0 + proportion[o][j];
				else
					px[o] *= (2.0 - proportion[o][j]);
			}
		}

		int recognize_num = 0;
		bool refused = false;
		double temp_prob = 0;
		for (int i = 0; i < 10; i++)           //比较10概率，确认识别数字
		{

			if (px[i] > temp_prob)
			{
				temp_prob = px[i];
				recognize_num = i;
				refused = false;
			}
			else if (px[i] == temp_prob)
			{
				refused = true;
			}
		}
		if (refused)
		{
			cout << "拒绝识别！" << endl;
			r_num++;
		}
		else
		{
			if (turn(test_num[0]) == recognize_num)
			{
				cout << "识别为：" << recognize_num << endl;
				c_num++;
			}
			else
			{
				cout << "识别错误！" << endl;
				f_num++;
			}
		}
	}
	train_data.close();
	cout << "拒绝识别率为：" << r_num / 200.0 << endl;
	cout << "正确识别率为：" << c_num / 200.0 << endl;
	cout << "错误识别率为：" << f_num / 200.0 << endl;

}             