#include "stdafx.h"
#include <iostream>
#include "fusion_laplacian.h"

using namespace std;
/************************************************************************/
/* ˵����
*���������µ�������Ϊ [0,1��...��level-1] ��
*blendMask Ϊͼ�����ģ
*maskGaussianPyramidΪ������ÿһ�����ģ
*resultLapPyr ���ÿ���������ֱ����������ͼLaplacian�任ƴ�ɵ�ͼ��
*/
/************************************************************************/
//�ο��ɼ� http://blog.csdn.net/abcjennifer/article/details/7628655

