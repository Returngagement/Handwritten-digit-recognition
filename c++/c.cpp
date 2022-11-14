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


void  ergodicImage(string filename, string name);//�����ļ��������ļ���Ϣ
string Image_compress(string Imagename);         //ͼƬ���ض�ֵ����ѹ����
int distance(string str1, string str2);          //ŷ�Ͼ���
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

void  ergodicImage(string filename, string name)             //����ͼƬ
{
	string firstfilename = filename + "\\*.bmp";
	struct _finddata_t fileinfo;
	intptr_t handle;                       //����Ӧ����long�ͣ���һֱ�����������ϸ�Ϊintptr_t
	string openname = "C:\\Users\\13296\\Desktop\\c++\\" + name;
	ofstream file;
	file.open(openname, ios::out);
	handle = _findfirst(firstfilename.c_str(), &fileinfo);            
	if (_findfirst(firstfilename.c_str(), &fileinfo) != -1)               //�������к�׺Ϊ.bmp��ͼƬ
	{
		do
		{
			file << fileinfo.name << ":" << Image_compress(filename + "\\" + fileinfo.name) << endl;
		} while (!_findnext(handle, &fileinfo));
		file.close();
		_findclose(handle);
	}

}

string Image_compress(string Imagename)//ͼƬѹ��
{
	Mat Image = imread(Imagename);
	cvtColor(Image, Image, COLOR_BGR2GRAY);
	int pixel[28][28];
	for (int row = 0; row < Image.rows; row++)
		for (int col = 0; col < Image.cols; col++)
		{
			pixel[row][col] = Image.at<uchar>(row, col);
		}                                                       //���زɼ�


	//ͼƬѹ��
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
		if (sign >= 6)                                 //��ֵΪ6.��4*4����6�����ص����127��ѹ����ֵΪ1
			img_pixel += '1';
		else
			img_pixel += '0';
		y = (y + 4) % 28;                        //������б���ѹ����ֵ��
		if (k % 7 == 0)
		{
			x += 4;
			y = 0;
		}
	}
	return img_pixel;
}




int distance(string str1, string str2)  //�ȶ������ַ����ж��ٸ���һ��
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
	string test_bmp1 = "";         //��ȡ�Ķ�ֵ������
	string test_bmp2 = "";
	string train_bmp1 = "";
	string train_bmp2 = "";
	bool refuse = false;//�ܾ�ʶ���ʶ
	int recognize_num = 0;//ʶ��Ľ��
	int num_refuse = 0, num_false = 0, num_true = 0;//�ܾ�ʶ��ʶ����ȷ�����������
	test_data.open("C:\\Users\\13296\\Desktop\\c++\\test-num.txt");
	for (int k = 0; k < 200; k++)
	{
		int sign = 49;//����ŷʽ������ֵ
		getline(test_data, test_bmp1);//�����ļ���ÿ�ζ�ȡһ��
		test_bmp2 = test_bmp1;                          //��ԭʼ���ݽ��б���������ͨ��test_bmp[2]ȷ������ʶ�������
		if (test_bmp1.length() == 57) test_bmp1.erase(0, 8);//ȷ��ͼƬ��ֵ��ʼ��λ��
		else test_bmp1.erase(0, 9);
		train_data.open("C:\\Users\\13296\\Desktop\\c++\\train-num.txt");
		for (int i = 0; i < 1000; i++)
		{
			getline(train_data, train_bmp1);
			train_bmp2 = train_bmp1;
			if (train_bmp1.length() == 57) train_bmp1.erase(0, 8);
			else train_bmp1.erase(0, 9);
			if (distance(train_bmp1, test_bmp1) < sign)   //�ҳ���С����
			{
				refuse = false;                              
				sign = distance(train_bmp1, test_bmp1);
				recognize_num = turn(train_bmp2[0]);

			}
			else if (distance(train_bmp1, test_bmp1) == sign && recognize_num != turn(train_bmp2[0]))
				//������ȣ���ʶ������ֲ�ͬ
			{
				refuse = true;
				continue;
			}
		}
		train_data.close();//ע��򿪹رյ�λ�ã�û��ȡһ��test���ݾ�Ҫ�򿪹ر�һ��
		if (!refuse)
		{
			if (recognize_num == turn(test_bmp2[0]))
			{
				num_true++;
				cout << "ʶ��Ϊ��" << recognize_num << endl;
			}
			else
			{
				num_false++;
				cout << "ʶ�����" << endl;
			}
		}
		else
		{
			num_refuse++;
			cout << "�ܾ�ʶ��" << endl;
		}
	}
	test_data.close();
	double t = num_true / 200.0, f = num_false / 200.0, r = num_refuse / 200.0;
	cout << "��ȷ��Ϊ��" << t << endl;
	cout << "������Ϊ��" << f << endl;
	cout << "�ܾ�ʶ���ʣ�" << r << endl;

}

//��Ҷ˹�㷨
void Basyes()
{
	string temp;
	ifstream train_data;
	double  num[10] = { 0 };   //����1�ĸ���
	double proportion[10][49];
	for (int i = 0; i < 49; i++)
	{

		int n = 0;          //��ֵ��ʼλ�±�
		train_data.open("C:\\Users\\13296\\Desktop\\c++\\train-num.txt");
		for (int j = 0; j < 1000; j++)
		{
			getline(train_data, temp);
			if (temp.length() == 57)
			{
				n = i + 8;
				if (turn(temp[n]) == 1)
					num[turn(temp[0])]++;     //num���龫׼ͳ�Ƹ���������0-48λ��1�ĸ���
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
		train_data.close();//�ر��ļ���ÿ�α��������´򿪣���getline����
		for (int m = 0; m < 10; m++)
		{
			proportion[m][i] = num[m] / 100.0;   //�����i�еĵı���
			num[m] = 0;
		}
	}
	double c_num = 0;//ʶ����ȷ����
	double f_num = 0;//ʶ��������
	double r_num = 0;//�ܾ�ʶ�����
	ifstream test_data;
	string test_num;
	double px[10];                 //��¼�������
	test_data.open("C:\\Users\\13296\\Desktop\\c++\\test-num.txt");
	for (int i = 0; i < 200; i++)          //200�Ų���ͼƬ������
	{
		for (int x = 0; x < 10; x++)px[x] = 1.0;
		int m = 0;
		getline(test_data, test_num);
		for (int o = 0; o < 10; o++) {
			for (int j = 0; j < 49; j++)            //ÿ��ͼƬ49����ֵ 
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
		for (int i = 0; i < 10; i++)           //�Ƚ�10���ʣ�ȷ��ʶ������
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
			cout << "�ܾ�ʶ��" << endl;
			r_num++;
		}
		else
		{
			if (turn(test_num[0]) == recognize_num)
			{
				cout << "ʶ��Ϊ��" << recognize_num << endl;
				c_num++;
			}
			else
			{
				cout << "ʶ�����" << endl;
				f_num++;
			}
		}
	}
	train_data.close();
	cout << "�ܾ�ʶ����Ϊ��" << r_num / 200.0 << endl;
	cout << "��ȷʶ����Ϊ��" << c_num / 200.0 << endl;
	cout << "����ʶ����Ϊ��" << f_num / 200.0 << endl;

}             