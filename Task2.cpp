/*
Задание 2.
Написать код на С++ для определения точки пересечения отрезка и
треугольника в 3D пространстве. Отрезок задан координатами концов,
треугольник задан координатами всех трех углов.
*/

#include <iostream>
#include <clocale>
#include <math.h>

// Класс для точек в 3D пространстве
class Point
{
public:
    double x, y, z; // координаты точки
    // Конструктор. Аргументами являются координаты точки
    Point(double x = 0, double y = 0, double z = 0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

// Перегружаем оператор равенства для класса Point
bool operator == (Point p1, Point p2)
{
    return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}

// Класс для отрезка в 3D пространстве
class Segment
{
public:
    Point* a, * b; // точки - концы отрезка
    double k_x, k_y, k_z; // координаты направляющего вектора отрезка: k - вектор от точки а к точке b

    // Конструктор. Аргументами являются концы отрезка
    Segment(Point* a, Point* b)
    {
        this->a = a;
        this->b = b;
        k_x = b->x - a->x;
        k_y = b->y - a->y;
        k_z = b->z - a->z;
    }

    // Метод вычисляет координаты x, y, z точки на отрезке с помощью параметрических уравнений:
    // x = a_x + k_x * e, y = a_y + k_y * e, z = a_z + k_z * e,
    // e - параметр (если точка лежит внутри отрезка, то 0 <= e <= 1);
    // a_x, a_y, a_z - координаты начала отрезка, точки a.
    // Аргументами являются отрезок s и параметр e
    static Point* set_point(Segment& s, double e)
    {
        // Вычисляются координаты x, y, z точки
        double x = s.a->x + s.k_x * e;
        double y = s.a->y + s.k_y * e;
        double z = s.a->z + s.k_z * e;
        // Создается точка
        Point* p = new Point(x, y, z);
        return p;
    }
};

// Класс для треугольника в 3D пространстве
class Triangle
{
public:
    Point* a, * b, * c; // точки - вершины треугольника
    double k_x, k_y, k_z, k; // коэффициенты в уравнении плоскости треугольника

    // Конструктор. Аргументами являются вершины треугольника
    Triangle(Point* a, Point* b, Point* c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        //
        calculate_plane();
    }
    
    // Метод вычисляет коэффициенты в уравнении плоскости, в которой лежит треугольник.
    // Уравнение плоскости выглядит так: k1 * x + k2 * y + k3 * z = k.
    // Нам известны три точки на этой плоскости, потому нам известна матрица коэффициентов системы.
    // Решим систему и найдем коэффициенты k1, k2, k3 с помощью метода Гаусса-Зейделя
    void calculate_plane()
    {
        k_x = (b->y - a->y) * (c->z - a->z) - (c->y - a->y) * (b->z - a->z);
        k_y = (c->x - a->x) * (b->z - a->z) - (b->x - a->x) * (c->z - a->z);
        k_z = (b->x - a->x) * (c->y - a->y) - (c->x - a->x) * (b->y - a->y);
        k = k_x * a->x + k_y * a->y + k_z * a->z;
    }

    // Метод проверяет, лежит ли точка p на плоскости
    bool is_on(Point* p)
    {
        return k_x * p->x + k_y * p->y + k_z * p->z == k;
    }

    // Метод вычисляет расстояние между точками p1 и p2
    double calculate_distance(Point* p1, Point* p2)
    {
        return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2) + pow(p1->z - p2->z, 2));
    }

    // Метод вычисляет по теореме косинусов косинус угла p1p2p3
    double calculate_angle(Point* p1, Point* p2, Point* p3)
    {
        double d12 = calculate_distance(p1, p2); // длина стороны p1p2
        double d13 = calculate_distance(p1, p3); // длина стороны p1p3
        double d23 = calculate_distance(p2, p3); // длина стороны p2p3
        double cos = (d12 * d12 + d23 * d23 - d13 * d13) / (2 * d12 * d23); // косинус угла p1p2p3
        return cos;
    }

    // Метод определяет, лежит ли точка внутри треугольника
    bool is_in(Point* p)
    {
        if (*a == *p || *b == *p || *c == *p)
        {
            // Если точка p совпадает с вершиной треугольника
            return true;
        }
        double cos1 = calculate_angle(a, p, b); // вычисляем косинус угла apb
        double cos2 = calculate_angle(b, p, c); // вычисляем косинус угла bpc
        double cos3 = calculate_angle(c, p, a); // вычисляем косинус угла cpa
        // Посчитаем количество тупых углов
        int i = 0;
        if (cos1 < 0)
        {
            i++;
        }
        if (cos2 < 0)
        {
            i++;
        }
        if (cos3 < 0)
        {
            i++;
        }
        if (2 <= i)
        {
            // Если как минимум три посчитанных угла тупые, то точка p находится внутри треугольника
            return true;
        }
        return false;
    }

    // Метод вычисляет точку пересечения прямой a, проходящей через отрезок s, с плоскостью,
    // в которой лежит треугольник
    void calculate_intersection(Segment& s)
    {
        if (k_x * s.k_x + k_y * s.k_y + k_z * s.k_z == 0)
        {
            // Если прямая и плоскость параллельные
            if (is_on(s.a))
            {
                // Прямая лежит в плоскости
                std::cout << "\nОтрезок AB лежит в плоскости, в которой лежит треугольник CDF.\n";
            }
            else
            {
                // Прямая не пересекает плоскость
                std::cout << "\nОтрезок AB параллелен плоскости, в которой лежит треугольник CDF.\n";
                std::cout << "Отрезок AB не пересекает треугольник CDF.\n";
            }
            return;
        }
        double e = (k - k_x * s.a->x - k_y * s.a->y - k_z * s.a->z)/
            (k_x * s.k_x + k_y * s.k_y + k_z * s.k_z);
        std::cout << "\nПрямая а, проходящая через отрезок AB, пересекает плоскость, в которой лежит треугольник CDF, в точке:\n";
        Point* p = Segment::set_point(s, e); // точка пересечения плоскости и прямой
        printf("\tx= %f;\n", p->x);
        printf("\ty= %f;\n", p->y);
        printf("\tz= %f;\n", p->z);
        if (0 <= e && e <= 1 && is_in(p))
        {
            // Если точка пересечения прямой с плоскостью лежит внутри отрезка и треугольника
            std::cout << "\nОтрезок AB пересекает треугольник CDF в той же точке.\n";
        }
        else
        {
            // Если отрезок и треугольник не пересекаются
            std::cout << "\nОтрезок AB не пересекает треугольник CDF.\n";
        }
        delete p; // удаляем мусор
    }
};

// Функция для ввода координат точки.
// name - имя точки
Point* input(char* name)
{
    double x, y, z; // координаты точки
    printf("\tточка %s:\n", name);
    std::cout << "\t\tx= ";
    std::cin >> x;
    std::cout << "\t\ty= ";
    std::cin >> y;
    std::cout << "\t\tz= ";
    std::cin >> z;
    Point* p = new Point(x, y, z); // создаем точку
    return p;
}

// Функция выводит на экран условие задачи
void print_task()
{
    std::cout << "Задание 2.\nНаписать код на С++ для определения точки пересечения отрезка и треугольника в 3D пространстве.\n" <<
        "Отрезок задан координатами концов, треугольник задан координатами всех трех углов.\n";
}

int main()
{
    setlocale(0, "");
    // Выводим на экран задание
    print_task();
    // Получаем координаты концов отрезков
    std::cout << "\nВведите координаты концов отрезка AB:\n";
    Point* a = input((char*)"A"); // точка A
    Point* b = input((char*)"B"); // точка B
    std::cout << "Введите координаты вершин треугольника CDF:\n";
    Point* c = input((char*)"C"); // точка C
    Point* d = input((char*)"D"); // точка D
    Point* f = input((char*)"F"); // точка F
    //
    Segment ab(a, b); // создаем отрезок AB
    Triangle cdf(c, d, f); // создаем треугольник CDF
    //
    cdf.calculate_intersection(ab);
    // Удаляем мусор
    delete a;
    delete b;
    delete c;
    delete d;
    delete f;
    std::cout << std::endl;
    system("pause");
}