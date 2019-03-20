#include <iostream>

#include <opencv2/opencv.hpp>
using namespace cv;

typedef unsigned char VT;
typedef int B;

template <class T>
struct neighbourhood;

template <class T, class U>
U dee_I_dee_x(neighbourhood<T> hood)
{
  // First 
  U ret = (hood.I_xd_y_t + hood.I_xd_y_td + hood.I_xd_yd_t + hood.I_xd_yd_td)/4
            - (hood.I_x_y_t + hood.I_x_y_td + hood.I_x_yd_t + hood.I_x_yd_td)/4;

  return ret;
}

template <class T, class U>
U dee_I_dee_y(neighbourhood<T> hood)
{
  U ret = (hood.I_x_yd_t + hood.I_x_yd_td + hood.I_xd_yd_t + hood.I_xd_yd_td)/4
            - (hood.I_x_y_t + hood.I_x_y_td + hood.I_xd_y_t + hood.I_xd_y_td)/4;

  return ret;
}

template <class T, class U>
U dee_I_dee_t(neighbourhood<T> hood)
{
  U ret = (hood.I_x_y_td + hood.I_x_yd_td + hood.I_xd_y_td + hood.I_xd_yd_td)/4
            - (hood.I_x_y_t + hood.I_x_yd_t + hood.I_xd_y_t + hood.I_xd_yd_t)/4;

  return ret;
}

template <class T>
struct neighbourhood
{
  T I_x_y_t;
  T I_xd_y_t;
  T I_xd_yd_t;
  T I_x_yd_t;

  T I_x_y_td;
  T I_xd_y_td;
  T I_xd_yd_td;
  T I_x_yd_td;
};

template <class T>
neighbourhood<T> fetch_hood(__uint16_t cols, MatConstIterator_<T> t0, MatConstIterator_<T> t1)
{
  T I_x_y_t = *t0;
  T I_xd_y_t = *(++t0);
  T I_xd_yd_t = *(t0+=cols);
  T I_x_yd_t = *(--t0);
  t0-=cols;

  T I_x_y_td = *t1;
  T I_xd_y_td = *(++t1);
  T I_xd_yd_td = *(t1+=cols);
  T I_x_yd_td = *(--t1);
  t1-=cols;

  neighbourhood<T> ret = {
    I_x_y_t,
    I_xd_y_t,
    I_xd_yd_t,
    I_x_yd_t,

    I_x_y_td,
    I_xd_y_td,
    I_xd_yd_td,
    I_x_yd_td
  };

  return ret;
}

int main()
{
  std::cout << "hi there" << std::endl;
  Mat image, image2;
  image = imread( "1.jpg", IMREAD_COLOR );
  image2 = imread( "2.jpg", IMREAD_COLOR );

  Mat grey, grey2;
  cvtColor(image, grey, COLOR_BGR2GRAY);
  cvtColor(image2, grey2, COLOR_BGR2GRAY);
  Mat res(grey2);

  MatConstIterator_<VT> ptr1 = grey.begin<VT>();
  MatConstIterator_<VT> end1 = grey.end<VT>();
  MatConstIterator_<VT> ptr2 = grey2.begin<VT>();
  MatConstIterator_<VT> end2 = grey2.end<VT>();

  MatIterator_<VT> ptr_res = res.begin<VT>();

  ++ptr1;
  ++ptr_res;
  while (ptr1 != end1-1) 
  {
    neighbourhood<VT> hood = fetch_hood(res.cols, ptr1, ptr2);
    B I_x = dee_I_dee_x<VT, B>(hood);
    B I_y = dee_I_dee_y<VT, B>(hood);
    B I_t = dee_I_dee_t<VT, B>(hood);
    *ptr_res = abs(I_t);
    ++ptr_res;
    ++ptr1;
    ++ptr2;
  }

  imwrite("alpha.jpg", res);

  std::cout << "done" << std::endl;
}
