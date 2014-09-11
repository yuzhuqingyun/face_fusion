#include "stdafx.h"
#include <iostream>
#include "fusion_laplacian.h"

using namespace std;
/************************************************************************/
/* 说明：
*金字塔从下到上依次为 [0,1，...，level-1] 层
*blendMask 为图像的掩模
*maskGaussianPyramid为金字塔每一层的掩模
*resultLapPyr 存放每层金字塔中直接用左右两图Laplacian变换拼成的图像
*/
/************************************************************************/
//参考可见 http://blog.csdn.net/abcjennifer/article/details/7628655

