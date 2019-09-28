#ifndef CLOGICBASE
#define CLOGICBASE
//�����߼����߼��� 
//�������ݻ�������������ɾ������ѯ��
class CLogicBase
{
public:
	CLogicBase(void) {}
	~CLogicBase(void) {}
public:

	///���������һ������
	///@param  arr[] ����, data Ҫ��ӵ�����, ArrLen ����ĳ���
	///@return true ��ӳɹ���false �������ˣ���Ӳ��ɹ�
	static bool AddOneDataToArr(BYTE arr[], BYTE data, BYTE ArrLen)
	{
		for (int i = 0;i < ArrLen;i++)
		{
			if (arr[i] == 255)
			{
				arr[i] = data;
				return true;
			}
		}
		return false;
	}

	///��������ɾ��һ������
	///@param  arr[] ����, data Ҫɾ��������, ArrLen ����ĳ���
	///@return true ��ӳɹ���false �������ˣ���Ӳ��ɹ�
	static bool DeleteOneDataFromArr(BYTE arr[], BYTE data, BYTE ArrLen)
	{
		for (int i = 0;i < ArrLen;i++)
		{
			if (arr[i] == data)
			{
				arr[i] = 255;
				return true;
			}
		}
		return false;
	}

	///��ȡ��ɫ�ӵ��������
	///@param  data0 ɫ��1, data1 ɫ��2, data2 ɫ��3
	static void GetSeziData(BYTE *data0 = NULL, BYTE *data1 = NULL, BYTE *data2 = NULL)
	{
		srand((unsigned)time(NULL));
		if (data0 != NULL)
		{
			*data0 = rand() % 6 + 1;
		}
		if (data1 != NULL)
		{
			*data1 = rand() % 6 + 1;
		}
		if (data2 != NULL)
		{
			*data2 = rand() % 6 + 1;
		}
	}

	///��ȡ�������ЧԪ�ظ���
	///@param  arr[] Ŀ������,data ��Чֵ, ArrLen ���鳤��
	///@return ��ЧԪ�ظ���
	static int GetArrInvalidNum(BYTE arr[], BYTE data, BYTE ArrLen)
	{
		int num = 0;
		for (int i = 0;i < ArrLen;i++)
		{
			if (arr[i] != data)
			{
				num++;
			}
		}
		return num;
	}

	///�ж��������Ƿ����ĳ��Ԫ��
	///@param  arr[] Ŀ������,data ��Чֵ, ArrLen ���鳤��
	///@return ��ЧԪ�ظ���
	static bool IsHaveInArr(BYTE arr[], BYTE data, BYTE ArrLen)
	{
		for (int i = 0;i < ArrLen;i++)
		{
			if (arr[i] == data)
			{
				return true;
			}
		}
		return false;
	}
	///�ж��������Ƿ����ĳ��Ԫ��
	///@param  arr[] Ŀ������,data Ҫ���ҵ�Ԫ��, ArrLen ���鳤��
	///@return ��ЧԪ�ظ���
	static int GetNumInArr(BYTE arr[], BYTE data, BYTE ArrLen)
	{
		int num = 0;
		for (int i = 0;i < ArrLen;i++)
		{
			if (arr[i] == data)
			{
				num++;
			}
		}
		return num;
	}
	///���ƣ�GetPaiNum
	///��������ȡ���������Ч������
	///@param  pai[] Ҫ����������, count ��Ч�Ƶ�����
	///@return 
	static int GetPaiNum(BYTE pai[], BYTE count)
	{
		int num = 0;
		for (int i = 0;i < count;i++)
		{
			if (pai[i] != 255)
			{
				num++;
			}
		}
		return num;
	}
	///ð������
	//big������true���Ӵ�С����false����С��������
	static void MaoPaoSort(BYTE a[], int count, bool big)
	{
		if (a == NULL || count <= 1)
			return;
		BYTE byTemp;
		for (int i = 0; i < count - 1; i++)
			for (int j = 0; j < count - i - 1; j++)
			{
				if (big)
				{
					if (a[j] < a[j + 1])
					{
						byTemp = a[j + 1];
						a[j + 1] = a[j];
						a[j] = byTemp;

						/*a[j + 1] = a[j] ^ a[j + 1];
						a[j] = a[j] ^ a[j + 1];
						a[j + 1] = a[j] ^ a[j + 1];*/
					}
				}
				else
				{
					if (a[j] > a[j + 1])
					{
						byTemp = a[j + 1];
						a[j + 1] = a[j];
						a[j] = byTemp;

						/*a[j + 1] = a[j] ^ a[j + 1];
						a[j] = a[j] ^ a[j + 1];
						a[j + 1] = a[j] ^ a[j + 1];*/
					}
				}

			}
	}

	///ѡ������
	//big������true���Ӵ�С����false����С��������
	static void SelectSort(BYTE a[], int count, bool big)
	{
		if (a == NULL || count <= 1)
			return;
		int iMin = 0, iMax = 0;
		BYTE byTemp;
		if (big)
		{
			for (int i = 0; i < count - 1; i++)
			{
				iMax = i;
				for (int j = i + 1; j < count; j++)
					if (a[j] > a[iMax])
						iMax = j;
				if (iMax != i)
				{
					byTemp = a[iMax];
					a[iMax] = a[i];
					a[i] = byTemp;

					/*a[i] = a[i] ^ a[iMax];
					a[iMax] = a[i] ^ a[iMax];
					a[i] = a[i] ^ a[iMax];*/
				}
			}
		}
		else
		{
			for (int i = 0; i < count - 1; i++)
			{
				iMin = i;
				for (int j = i + 1; j < count; j++)
					if (a[j] < a[iMin])
						iMin = j;
				if (iMin != i)
				{
					byTemp = a[iMin];
					a[iMin] = a[i];
					a[i] = byTemp;

					/*a[i] = a[i] ^ a[iMin];
					a[iMin] = a[i] ^ a[iMin];
					a[i] = a[i] ^ a[iMin];*/
				}
			}
		}
	}

	//������
	static void HeapAdjustSmall(BYTE * a, int i, int length) //�����󶥶ѣ���С��������
	{
		int nChild;
		int nTemp;
		for (; 2 * i + 1 < length; i = nChild)
		{
			//�ӽ���λ��=2*�������λ�ã�+1
			nChild = 2 * i + 1;
			//�õ��ӽ���нϴ�Ľ��
			if (nChild<length - 1 && a[nChild + 1]>a[nChild])
				++nChild;
			//����ϴ���ӽ����ڸ������ô�ѽϴ���ӽ�������ƶ����滻���ĸ����
			if (a[i] < a[nChild])
			{
				nTemp = a[i];
				a[i] = a[nChild];
				a[nChild] = nTemp;
			}
			else break;
		}
	}
	static void HeapSortSmall(BYTE * a, int length)
	{
		int i;
		//�������е�ǰ�벿��Ԫ�أ�������֮���һ��Ԫ�������е�����Ԫ�أ��Ƚ����󶥶ѣ�
		//length/2-1�����һ����Ҷ�ڵ㣬�˴�"/"Ϊ����
		for (i = length / 2 - 1; i >= 0; --i)
			HeapAdjustSmall(a, i, length);

		//�����һ��Ԫ�ؿ�ʼ�����н��е��������ϵ���С�����ķ�Χֱ����һ��Ԫ��
		for (i = length - 1; i > 0; --i)
		{
			//�ѵ�һ��Ԫ�غ͵�ǰ�����һ��Ԫ�ؽ�����
			//��֤��ǰ�����һ��λ�õ�Ԫ�ض��������ڵ��������֮������
			a[i] = a[0] ^ a[i];
			a[0] = a[0] ^ a[i];
			a[i] = a[0] ^ a[i];
			//������С����heap�ķ�Χ��ÿһ�ε�����ϱ�֤��һ��Ԫ���ǵ�ǰ���е����ֵ
			HeapAdjustSmall(a, 0, i);
		}
	}

	static void HeapAdjustBig(BYTE * a, int i, int length) //����С���ѣ��Ӵ�С����
	{
		int nChild;
		int nTemp;
		for (; 2 * i + 1 < length; i = nChild)
		{
			//�ӽ���λ��=2*�������λ�ã�+1
			nChild = 2 * i + 1;
			//�õ��ӽ���нϴ�Ľ��
			if (nChild < length - 1 && a[nChild + 1] < a[nChild])
				++nChild;
			//����ϴ���ӽ����ڸ������ô�ѽϴ���ӽ�������ƶ����滻���ĸ����
			if (a[i] > a[nChild])
			{
				nTemp = a[i];
				a[i] = a[nChild];
				a[nChild] = nTemp;
			}
			else break;
		}
	}
	static void HeapSortBig(BYTE * a, int length)
	{
		int i;
		//�������е�ǰ�벿��Ԫ�أ�������֮���һ��Ԫ�������е�����Ԫ�أ��Ƚ����󶥶ѣ�
		//length/2-1�����һ����Ҷ�ڵ㣬�˴�"/"Ϊ����
		for (i = length / 2 - 1; i >= 0; --i)
			HeapAdjustBig(a, i, length);

		//�����һ��Ԫ�ؿ�ʼ�����н��е��������ϵ���С�����ķ�Χֱ����һ��Ԫ��
		for (i = length - 1; i > 0; --i)
		{
			//�ѵ�һ��Ԫ�غ͵�ǰ�����һ��Ԫ�ؽ�����
			//��֤��ǰ�����һ��λ�õ�Ԫ�ض��������ڵ��������֮������
			a[i] = a[0] ^ a[i];
			a[0] = a[0] ^ a[i];
			a[i] = a[0] ^ a[i];
			//������С����heap�ķ�Χ��ÿһ�ε�����ϱ�֤��һ��Ԫ���ǵ�ǰ���е����ֵ
			HeapAdjustBig(a, 0, i);
		}
	}

	static void HeapSort(BYTE a[], int length, bool big)
	{
		if (length <= 1)
		{
			return;
		}
		if (big)
		{
			HeapSortBig(a, length);
		}
		else
		{
			HeapSortSmall(a, length);
		}
	}

	/**
	* @brief ��ȡ���г�������·�������ַ���ĩβ���ӷ�б��
	*/
	static char * G_GetAppPath()
	{
		static char s_Path[MAX_PATH];
		static bool s_bIsReady = false;
		if (!s_bIsReady)
		{
			ZeroMemory(s_Path, sizeof(s_Path));
			DWORD dwLength = GetModuleFileNameA(GetModuleHandle(NULL), s_Path, sizeof(s_Path));
			char *p = /*_tcsrchr*/strrchr(s_Path, '\\');
			*p = '\0';
			s_bIsReady = true;
		}
		return s_Path;
	}
};
#endif