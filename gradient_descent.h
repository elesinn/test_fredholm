#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

#include <iostream>
#include <cmath>
#include <QVector>
#include <stdlib.h>
#include <iomanip>

using namespace std;

const int snum = 50;
const int xnum = 50;

///K(x,s) - ядро
class kxs
{
public:
    double  operator() (double x, double s)
    {
        return 1 / (1 + 100 * (s - x)*(s - x));

    }
};


///z(x)
class kz
{
public:
    double  operator() (double s)
    {
        switch (4)
        {
            case 1:
               return 20*s*(s-0.5)*(s-0.5);
            case 2:
               return 4*s*(1 - s);
            case 3:
               return 1 / (100 * s*s + 1);
            case 4:
               return atan(s);
            case 5:
               return  ((exp(-((s - 0.3)*(s - 0.3)) / 0.06) + exp(-((s - 0.3)*(s - 0.3)) / 0.06)) / 0.09550408) - 0.052130913;
            case 6:
               return exp(-((s - 0.5)*(s-0.5))/0.06);
        }
    }
};

///Оперетор A*
QVector<double> A_link(kxs &k, double a, double b, QVector<double> z, QVector<double> s,QVector<double> delta)
{
    double h, c;
    QVector<double> sum(snum);
    QVector<double> I(snum);
    h = (b - a) / xnum;
    for (int j = 0; j<snum; j++){
        for (int i = 0; i<xnum; i++){
            c = a + i*h;
            sum[j] += k(s[j], c)*z[j];
        }
    }
    for (int i = 0; i<snum; i++){
        I[i] = h*((k(s[i], a)*z[i] + k(s[i], b)*z[i]) / 2 + sum[i])+delta[i];
    }
    return I;
}


///Оперетор A
QVector<double> A(kxs &k, double a, double b, QVector<double> s, QVector<double> a_link,QVector<double> delta)
{
    double h, c;
    QVector<double> sum(xnum);
    QVector<double> I(xnum);
    h = (b - a) / snum;
    for (int j = 0; j<xnum; j++){
        for (int i = 0; i<snum; i++){
            c = a + i*h;
            sum[j] += k(s[j], c)*a_link[i];
        }
    }
    for (int i = 0; i<xnum; i++){
        I[i] = h*((k(s[i], a)*a_link[0] + k(s[i], b)*a_link[snum - 1]) / 2 + sum[i])+delta[i];
    }
    return I;

}

///Скалярное произведение
double scalar(double a, double b, QVector<double> f, QVector<double> g)
{
    double h;
    double sum = 0;
    double I;

    h = (b - a) / xnum;

    for (int i = 0; i<xnum; i++){
        sum += f[i] * g[i];
    }
    I = h*((f[0] * g[0] + f[xnum - 1] * g[xnum - 1]) / 2 + sum);
    return I;

}

///Умножение на число
QVector<double> mult_numb(QVector<double> v1, double v2, double vec_size)
{
    QVector<double> r(vec_size);
    for (int i = 0; i<vec_size; i++)
    {
        r[i] = v1[i] * v2;
    }
    return r;
}
///Разность
QVector<double> sub(QVector<double> v1, QVector<double> v2, double vec_size)
{
    QVector<double> r(vec_size);
    for (int i = 0; i<vec_size; i++)
    {
        r[i] = v1[i] - v2[i];
    }
    return r;
}
///Сумма
QVector<double> sum(QVector<double> v1, QVector<double> v2, double vec_size)
{
    QVector<double> r(vec_size);
    for (int i = 0; i<vec_size; i++)
    {
        r[i] = v1[i] + v2[i];
    }
    return r;
}

///Норма
double norm(double a, double b, QVector<double> f)
{
    double h;
    double sum = 0;
    double I;

    h = (b - a) / xnum;

    for (int i = 0; i<xnum; i++){
        sum += f[i] * f[i];
    }
    I = h*((f[0] * f[0] + f[xnum - 1] * f[xnum - 1]) / 2 + sum);
    return sqrt(I);
}
///R
double R(QVector<double> lambda, QVector<double> s, double alpha, double a, double b, QVector<double> u,QVector<double> delta)
{
    kxs k;
    double res;
    QVector<double> t1;
    QVector<double> t2;
    t1 = A_link(k, a, b, lambda, s,delta);
    t2 = A(k, a, b, s, t1,delta);
    double t3 = scalar(a, b, lambda, t2);
    res = (-0.25)*t3 - scalar(a, b, lambda, u) - alpha*norm(a, b, lambda)*norm(a, b, lambda);
    return res;
}
///Градиент R
QVector<double> gradR(QVector<double> lambda, QVector<double> s, double a, double b, double alpha, QVector<double> u,QVector<double> delta){
    kxs k;
    QVector<double> t1;
    QVector<double> t2;
    t1 = A_link(k, a, b, lambda,s,delta);
    t2 = A(k, a, b, s, t1,delta);
    QVector<double> t3;
    QVector<double> t4;
    t3 = mult_numb(t2, -0.5, xnum);
    t4 = sub(t3, u, xnum);
    QVector<double> t5;
    double zs = alpha * 2;
    t5 = mult_numb(lambda, zs, xnum);
    QVector<double> res;
    res = sub(t4, t5, xnum);
    return res;
}
///Одномерная функция для нахождения шага
double funcgrad(double beta, QVector<double> lambda, QVector<double> grad, QVector<double> &s, double a, double b, double alpha, QVector<double> u,QVector<double> delta)
{
    QVector<double> res1(xnum);
    double res3;
    for (int i = 0; i<xnum; i++){
        res1[i] = lambda[i] + beta*grad[i];
    }

    res3 = R(res1, s, alpha, a, b, u,delta);
    return res3;
}
///нахождение шага с помощью золотого сечения
double zoloto(double a, double b, QVector<double> grad, QVector<double> lambda, QVector<double> &s, double alpha, QVector<double> u,QVector<double> delta)
{
    double eps = 0.00001;
    double x1 = a + (1 - (sqrt(5) - 1) / 2)*(b - a);
    double x2 = a + ((sqrt(5) - 1) / 2)*(b - a);
    double y1 = funcgrad(x1, lambda, grad, s, -1, 1, alpha, u,delta);
    double y2 = funcgrad(x2, lambda, grad, s, -1, 1, alpha, u,delta);


    while (fabs((b - a))>eps)
    {

        if (y1 > y2)
        {
            b = x2;
            x2 = x1;
            x1 = a + (1 - (sqrt(5) - 1) / 2)*(b - a);
            y2 = y1;
            y1 = funcgrad(x1, lambda, grad, s, -1, 1, alpha, u,delta);
        }
        else
        {
            a = x1;
            x1 = x2;
            x2 = a + ((sqrt(5) - 1) / 2)*(b - a);
            y1 = y2;
            y2 = funcgrad(x2, lambda, grad, s, -1, 1, alpha, u,delta);
        }


    }
    return (a + b) / 2;
}
///Вычисление нового значения lambda
QVector<double> New(QVector<double> &lambda, QVector<double> grad, double beta)
{
    QVector<double> res(xnum);
    for (int i = 0; i<xnum; i++){
        res[i] = lambda[i] + beta*grad[i];
    }
    return res;
}
///Поиск шага по формуле
double step(QVector<double> lambda, QVector<double> grad, QVector<double> &s, double a, double b, double alpha, QVector<double>  u,QVector<double> delta){
    kxs k;
    double res1;
    QVector<double> t1(xnum);
    QVector<double> t2(xnum);
    QVector<double> t3(xnum);
    QVector<double> t4(xnum);
    t1 = A_link(k, a, b, grad, s,delta);
    t2 = A(k, a, b, s, t1,delta);
    t3 = A_link(k, a, b, lambda, s,delta);
    t4 = A(k, a, b, s, t3,delta);
    QVector<double> t5(xnum);
    QVector<double> t6(xnum);
    res1 =
        (0.25*scalar(a, b, lambda, t2) +
        0.25*scalar(a, b, grad, t4) +
        scalar(a, b, grad, u) +
        2 * alpha*scalar(a, b, grad, lambda)) /
        (-0.5*(scalar(a, b, grad, t2)) -
        2 * alpha*sqrt(norm(a, b, grad)));
    return res1;
}

///Поиск шага с помощью метода дихотомии
double diht(double a, double b, QVector<double> grad, QVector<double> lambda, QVector<double> &s, double alpha, QVector<double> u,QVector<double> delta)
{
    double eps = 0.000001;

    while ((b - a)>eps)
    {
        double x = (a + b) / 2;
        double x1 = x - eps / 2;
        double x2 = x + eps / 2;
        double y1 = funcgrad(x1, lambda, grad, s, -1, 1, alpha, u,delta);
        double y2 = funcgrad(x2, lambda, grad, s, -1, 1, alpha, u,delta);
        if (y1<y2)
            a = x;
        else
            b = x;
    }
    return (a + b) / 2;
}



QVector<double> GradDown(QVector<double> lambda, QVector<double> s, double a, double b, QVector<double> u,QVector<double> delta)
{
    double eps = 0.000001;
    QVector<double> curr = lambda;
    QVector<double> last;
    QVector<double> grad;
    double resul;
    double it = 1;
    double alpha;
    double beta;
    do
    {
        last = curr;
        alpha = pow(it,-1.0/6.0);
        beta = 10*pow(it,-3.0/5.0);
        grad = gradR(curr, s, a, b, alpha, u,delta);
        curr = New(curr, grad, beta);
        it++;
        //cout << "iter=" << it << endl;
        resul=fabs(R(last, s, alpha, a, b, u,delta) - R(curr, s, alpha, a, b, u,delta));
       // cout << "iter=" << resul << endl;
    } while   (resul >eps);
   // cout << "iter=" << it << endl;
    return curr;

}



#endif // GRADIENT_DESCENT_H

