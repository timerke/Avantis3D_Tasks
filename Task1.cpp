/*
Задание 1.
Написать код на С++ для определения кратчайшего расстояния между двумя
отрезками в 3D пространстве. Отрезки заданы координатами концов.
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

    // Метод вычисляет оптимальный параметр e, соответствующий точке p на прямой,
    // проходящей через отрезок s. С помощью параметра e и параметрических уравнений
    // на прямой можно вычислить точку, ближайшую к точке p
    static double calculate_optimal_e(Segment& s, Point* p)
    {
        double e = -((s.a->x - s.b->x) * (s.a->x - p->x) +
            (s.a->y - s.b->y) * (s.a->y - p->y) +
            (s.a->z - s.b->z) * (s.a->z - p->z)) /
            (s.k_x * (s.a->x - s.b->x) + s.k_y * (s.a->y - s.b->y) + s.k_z * (s.a->z - s.b->z));
        return e;
    }

    // Метод вычисляет расстояние между прямыми a и b, проходящими через отрезки s1 и s2 соотвественно.
    // Метод возвращает true, если расстояние между прямыми a и b равно расстоянию между отрезками s1 и s2
    static bool calculate_distance_between_lines(Segment& s1, Segment& s2)
    {
        double m11 = -(s1.k_x * s1.k_x + s1.k_y * s1.k_y + s1.k_z * s1.k_z);
        double m12 = s1.k_x * s2.k_x + s1.k_y * s2.k_y + s1.k_z * s2.k_z;
        double m21 = -(s1.k_x * s2.k_x + s1.k_y * s2.k_y + s1.k_z * s2.k_z);
        double m22 = s2.k_x * s2.k_x + s2.k_y * s2.k_y + s2.k_z * s2.k_z;
        double n1 = s1.k_x * (s1.a->x - s2.a->x) + s1.k_y * (s1.a->y - s2.a->y) + s1.k_z * (s1.a->z - s2.a->z);
        double n2 = s2.k_x * (s1.a->x - s2.a->x) + s2.k_y * (s1.a->y - s2.a->y) + s2.k_z * (s1.a->z - s2.a->z);
        //
        double e1, e2; // параметры для оптимальных точек на прямых a и b соответственно
        Point* p1, * p2; // оптимальные точки на прямых a и b соответственно
        if (m12 * m21 - m11 * m22 != 0)
        {
            // Если отрезки s1 и s2 неколлинеарные
            std::cout << "\nОтрезки AB и CD лежат на неколлинеарных прямых a и b соответственно:\n";
            e1 = (m12 * n2 - m22 * n1) / (m12 * m21 - m11 * m22); // оптимальный параметр для точки на прямой a
            e2 = (m21 * n1 - m11 * n2) / (m12 * m21 - m11 * m22); // оптимальный параметр для точки на прямой b
        }
        else
        {
            // Если отрезки s1 и s2 коллинеарны друг другу
            std::cout << "\nОтрезки AB и CD лежат на коллинеарных прямых a и b соответственно:\n";
            e1 = calculate_optimal_e(s1, s2.a); // оптимальный параметр для точки на прямой a, соотвествующей точке A отрезка s2
            e2 = calculate_optimal_e(s1, s2.b); // оптимальный параметр для точки на прямой a, соотвествующей точке B отрезка s2
            double e3 = calculate_optimal_e(s2, s1.a); // оптимальный параметр для точки на прямой b, соотвествующей точке A отрезка s1
            double e4 = calculate_optimal_e(s2, s1.b); // оптимальный параметр для точки на прямой a, соотвествующей точке B отрезка s1
            if (0 <= e1 && e1 <= 1)
            {
                // Если оптимальная точка на прямой a, соотвествующая точке A отрезка s2, лежит внутри отрезка s1,
                // выбираем ее
                e2 = 0;
            }
            else if (0 <= e2 && e2 <= 1)
            {
                // Если оптимальная точка на прямой a, соотвествующая точке B отрезка s2, лежит внутри отрезка s1,
                // выбираем ее
                e1 = e2;
                e2 = 1;
            }
            else if (0 <= e3 && e3 <= 1)
            {
                // Если оптимальная точка на прямой b, соотвествующая точке A отрезка s1, лежит внутри отрезка s2,
                // выбираем ее
                e1 = 0;
                e2 = e3;
            }
            else if (0 <= e4 && e4 <= 1)
            {
                // Если оптимальная точка на прямой b, соотвествующая точке B отрезка s1, лежит внутри отрезка s2,
                // выбираем ее
                e1 = 1;
                e2 = e4;
            }
            else
            {
                e2 = 0;
            }
        }
        p1 = set_point(s1, e1); // оптимальная точка на прямой a
        p2 = set_point(s2, e2); // оптимальная точка на прямой b
        std::cout << "\tточка на прямой a:\n";
        printf("\t\tx= %f;\n", p1->x);
        printf("\t\ty= %f;\n", p1->y);
        printf("\t\tz= %f;\n", p1->z);
        std::cout << "\tточка на прямой b:\n";
        printf("\t\tx= %f;\n", p2->x);
        printf("\t\ty= %f;\n", p2->y);
        printf("\t\tz= %f;\n", p2->z);
        // Вычисляется расстояние между оптимальными точками
        printf("\tрасстояние между прямыми a и b равно %f.\n", distance_between_points(p1, p2));
        // Удаляем мусор
        delete p1;
        delete p2;
        // Определяем, лежат ли оптимальные (ближайшие) точки прямых a и b внутри отрезков s1 и s2
        if (0 <= e1 && e1 <= 1 && 0 <= e2 && e2 <= 1)
        {
            // Ближайшие точки прямых a и b лежат внутри отрезков s1 и s2
            return true;
        }
        else
        {
            // Ближайшие точки прямых a и b не лежат внутри отрезков s1 и s2
            return false;
        }
    }

    // Метод вычисляет расстояние между двумя точками
    static double distance_between_points(Point* p1, Point* p2)
    {
        return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2) + pow(p1->z - p2->z, 2));
    }

    // Метод вычисляет расстояние между участками двух отрезков s1 и s2.
    // n - количество точек на каждом отрезке, между которыми будут вычисляться расстояния;
    // e1_min - оптимальный параметр для точки на отрезке s1, ближайшей к отрезку s2;
    // e1_left, e1_right - нижняя и верхняя границы параметра e для точек на отрезке s1;
    // e2_min - оптимальный параметр для точки на отрезке s2, ближайшей к отрезку s1;
    // e2_left, e2_right - нижняя и верхняя границы параметра e для точек на отрезке s2
    static double calculate_local_distance(int n,
        Segment& s1, double* e1_min, double e1_left, double e1_right,
        Segment& s2, double* e2_min, double e2_left, double e2_right)
    {
        double d_min; // минимальное найденное расстояние между точками отрезков s1 и s2
        for (int i = 0; i < n; i++) // перебор точек на отрезке s1
        {
            double e1 = e1_left + (e1_right - e1_left) * i / (n - 1);
            Point* p1 = set_point(s1, e1); // точка на отрезке s1
            for (int j = 0; j < n; j++) // перебор точек на отрезке s2
            {
                double e2 = e2_left + (e2_right - e2_left) * j / (n - 1);
                Point* p2 = set_point(s2, e2); // точка на отрезке s2
                double d = distance_between_points(p1, p2);
                delete p2; // удаляем мусор
                if ((i == 0 && j == 0) || d < d_min)
                {
                    d_min = d;
                    *e1_min = e1;
                    *e2_min = e2;
                }
            }
            delete p1; // удаляем мусор
        }
        return d_min;
    }

    // Метод вычисляет расстояние между двумя отрезками s1 и s2
    static void calculate_distance_between_segments(Segment& s1, Segment& s2)
    {
        const double ACCURACY = 0.0000001; // точность, с которой нужно вычислить расстояние между отрезками
        int n = 50; // начальное количество точек на каждом из отрезков, между которыми будут вычислены расстояния
        double d1, d2; // расстояния между отрезками, вычисленные при n и 2n количестве точек на отрезках
        double e1_min, e2_min; // оптимальные параметры для ближайших точек на отрезках s1 и s2
        double e1_left = 0.0, e1_right = 1.0; // нижняя и верхняя границы параметра e на отрезке s1
        double e2_left = 0.0, e2_right = 1.0; // нижняя и верхняя границы параметра e на отрезке s2
        do
        {
            // Вычисляем расстояние между n точками на отрезках s1 и s2
            d1 = calculate_local_distance(n, s1, &e1_min, e1_left, e1_right, s2, &e2_min, e2_left, e2_right);
            // Удваиваем количество точек на отрезках
            n *= 2;
            // Вычисляем расстояние между удвоенным количеством точек на отрезках
            d2 = calculate_local_distance(n, s1, &e1_min, e1_left, e1_right, s2, &e2_min, e2_left, e2_right);
            // Определяем новые границы параметра e на отрезке s1 (около вычисленного оптимального)
            double e_left_new = e1_min - (e1_right - e1_left) / (n - 1);
            double e_right_new = e1_min + (e1_right - e1_left) / (n - 1);
            e1_left = e_left_new < 0 ? 0 : e_left_new;
            e1_right = 1 < e_right_new ? 1 : e_right_new;
            // Определяем новые границы параметра e на отрезке s2 (около вычисленного оптимального)
            e_left_new = e2_min - (e2_right - e2_left) / (n - 1);
            e_right_new = e2_min + (e2_right - e2_left) / (n - 1);
            e2_left = e_left_new < 0 ? 0 : e_left_new;
            e2_right = 1 < e_right_new ? 1 : e_right_new;
        } while (ACCURACY < abs(d1 - d2)); // пользуемся правилом Рунге для оценки точности вычисления
        std::cout << "\nРасстояние между отрезками AB и CD:\n";
        Point* p1, * p2; // оптимальные точки на отрезках s1 и s2
        p1 = set_point(s1, e1_min); // оптимальная точка на отрезке s1
        p2 = set_point(s2, e2_min); // оптимальная точка на отрезке s2
        std::cout << "\tточка на отрезке AB:\n";
        printf("\t\tx= %f;\n", p1->x);
        printf("\t\ty= %f;\n", p1->y);
        printf("\t\tz= %f;\n", p1->z);
        std::cout << "\tточка на отрезке CD:\n";
        printf("\t\tx= %f;\n", p2->x);
        printf("\t\ty= %f;\n", p2->y);
        printf("\t\tz= %f;\n", p2->z);
        // Вычисляется расстояние между оптимальными точками
        printf("\tрасстояние между отрезками AB и CD равно %f.\n", distance_between_points(p1, p2));
        // Удаляем мусор
        delete p1;
        delete p2;
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
    std::cout << "Задание 1.\nНаписать код на С++ для определения кратчайшего расстояния между двумя отрезками\n" <<
        "в 3D пространстве. Отрезки заданы координатами концов.\n";
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
    std::cout << "Введите координаты концов отрезка CD:\n";
    Point* c = input((char*)"C"); // точка C
    Point* d = input((char*)"D"); // точка D
    //
    Segment ab(a, b); // создаем отрезок AB
    Segment cd(c, d); // создаем отрезок CD
    // Вычисляем расстояние между прямыми, проходящими через отрезки AB и CD
    bool is_same = Segment::calculate_distance_between_lines(ab, cd);
    if (is_same)
    {
        // Если расстояние между прямыми, проходящими через отрезки, то же, что и расстояние между отрезками
        std::cout << "\nРасстояние между отрезками AB и CD такое же, что и между прямыми a и b.\n";
    }
    else
    {
        // Если расстояние между прямыми, проходящими через отрезки, не равно расстоянию между отрезками
        Segment::calculate_distance_between_segments(ab, cd);
    }
    // Удаляем мусор
    delete a;
    delete b;
    delete c;
    delete d;
    std::cout << std::endl;
    system("pause");
}