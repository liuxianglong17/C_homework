#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include"head.h"
extern int board[15][15];
extern int coordinate[2];
extern int roaming;
extern unsigned long long ZobristTable[15][15][2];//÷ɭ��ת�Ĺ�ϣ��ֵ��
extern unsigned long long hashValue;//÷ɭ��ת�㷨�£����̵Ĺ�ϣֵ
unsigned long long hashing_value2[depth_of_hashing][3]; 

long int Searching_Hashing(int step_count, bool my_turn, long temp_score, bool write)
{
	int black = 0;
	int white = 1;
	int chess;
	unsigned long long int location;
	if ((step_count % 2) == 0)
	{
		chess = black;
	}
	else
	{
		chess = white;
	}


	location = hashValue % depth_of_hashing;//����һ���ض��������ڵ�λ��

	if (!write)//ֻ��ģʽ
	{

		if (hashing_value2[location][0] != 0)//��������ϣֵ��Ϊ0
		{
			if (hashing_value2[location][0] != hashValue && hashing_value2[location][0] != 0)
				//��������ϣֵ������Ĺ�ϣֵ���ȣ�˵��ȡģ���㽫������ͬ�Ĺ�ϣֵת��������ͬ��λ��
			{
				printf("dismatch!\n");
			}
			if (my_turn)
			{//Ŀǰ[1]��¼�����ҷ��ĵ÷�
				if (hashing_value2[location][1] != 0)
				{
					return (long)hashing_value2[location][1];
				}
			}
			else//[2]��¼���ǶԷ��ĵ÷�
			{
				if (hashing_value2[location][2] != 0)
				{
					return (long)hashing_value2[location][2];
				}

			}



		}



		return 0;

	}
	else//��дģʽ
	{

		if (hashing_value2[location][0] != hashValue && hashing_value2[location][0] != 0)
			//��������ϣֵ������Ĺ�ϣֵ���ȣ�˵��ȡģ���㽫������ͬ�Ĺ�ϣֵת��������ͬ��λ��
		{
			printf("dismatch!\n");
		}

		if (my_turn)
		{
			if (temp_score != 0)//���ǼǷ�0�ĵ÷�
			{
				hashing_value2[location][0] = hashValue;
				hashing_value2[location][1] = (unsigned long long)temp_score;
			}
		}
		else
		{
			if (temp_score != 0)

			{
				hashing_value2[location][0] = hashValue;
				hashing_value2[location][2] = (unsigned long long)temp_score;
			}
		}


		return 0; //�������Ҳֻ��ִ��һ�Σ����ִ���ˣ��ͱ�������ѭ��






	}


}