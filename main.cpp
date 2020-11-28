/**
 * Вариант задания 10.
 * Условие задания:  Найти все возможные тройки компланарных векторов. Входные
 * данные: множество не равных между собой векторов (x, y, z), где x, y, z –
 * числа. Оптимальное количество потоков выбрать самостоятельно.
 * Выполнил студент БПИ199 Кочарян Тигран Самвелович.
 */

#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <omp.h>

/**
 * Класс для хранения координат вектора с возможностью вывода в виде строки.
 */
class Vector {
public:
    int x;
    int y;
    int z;

    /**
     * Конструктор, принимающий координаты.
     * @param cordX
     * @param cordY
     * @param cordZ
     */
    Vector(int cordX, int cordY, int cordZ) :
            x(cordX), y(cordY), z(cordZ) {}

    /**
     * Пустой конструктор для инициализации коллекций.
     */
    Vector() {}

    /**
     * Перевод координаты вектора в виде строки для вывода.
     * @return
     */
    std::string toString() {
        std::ostringstream os;
        os << "(" << x << " " << y << " " << z << ") ";
        return os.str();
    }
};

/**
 * Проверка трех входных параметров-векторов на компланарность.
 * Смешанное произведение == 0 для компланарных векторов.
 * @param a
 * @param b
 * @param c
 * @return
 */
bool isCoplanar(Vector a, Vector b, Vector c) {
    int value = (a.x * b.y * c.z) + (a.y * b.z * c.x) + (a.z * b.x * c.y) -
                (a.z * b.y * c.x) - (a.x * b.z * c.y) - (b.x * a.y * c.z);
    return value == 0;
}

/**
 * Проверка текущего потока считывания файла на корректные данные. В случае, если данные некорректные,
 * программа завершается с соответствующей ошибкой.
 * @param in
 * @param filename
 */
void isIncorrect(std::ifstream &in, const std::string &filename) {
    if (in.fail()) {
        std::cout << "Wrong Data in file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

/**
 * Функция для запуска потоков.
 * Проверяет на компланарность i+1 и i+2 векторов между собой.
 * @param i
 * @param vectorCount
 * @param vectors_
 */
void threadFunction(int i, int vectorCount, std::vector<Vector>* vectors_) {
    std::vector<Vector>& vectors = *vectors_;
    for (int j = i + 1; j < vectorCount; ++j) {
        for (int k = j + 1; k < vectorCount; ++k) {
            if (isCoplanar(vectors[i], vectors[j], vectors[k])) {
                std::ostringstream os;
                os << "\nThread " << omp_get_thread_num() << " calculated coplanar vectors: " << vectors[i].toString() <<
                   vectors[j].toString() << vectors[k].toString();
                std::cout << os.str();
            }
        }
    }
}

/**
 * Точка входа в программу.ы
 * @param argsNumber
 * @param args
 * @return
 */
int main(int argsNumber, char **args) {
    /**
    * Если количество аргументов не равно 2, то у нас отсутствует название файла
    * с входными данными => сообщение об ошибке.
    */
    if (argsNumber != 2) {
        std::cout << "Wrong Console Data" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::ifstream in{args[1]};
    int vectorCount, x, y, z;

    /**
     * Если файл существует, то продолжаем работу.
     * В противном случае, закрываем программу с сообщением об ошибке.
     */
    if (!in.good()) {
        std::cout << "Error opening file: " << args[1] << std::endl;
        std::exit(EXIT_FAILURE);
    } else {
        in >> vectorCount;
        if(vectorCount < 1) {
            std::cout << "Wrong Vector Size in file: " <<args[1] << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::vector<Vector> vectors;

        /**
         * Построчное считывание векторов из файла с проверкой на корректность.
         */
        for (int i = 0; i < vectorCount; ++i) {
            in >> x;
            in >> y;
            in >> z;
            isIncorrect(in, args[1]);
            vectors.push_back(Vector(x, y, z));
        }

        /**
         * Вывод всех векторов в консоль для удобного взаимодействия и проверки.
         */
        std::cout << "The input vectors are:" << std::endl;
        for (int i = 0; i < vectorCount; ++i) {
            std::cout << "Vector #" << i + 1 << " " << vectors[i].toString() << std::endl;
        }

        /**
         * Создание потоков для цикла с помощью #pragma omp parallel for
         */
        #pragma omp parallel for
        for (int i = 0; i < vectorCount; ++i) {
            threadFunction(i, vectorCount, &vectors);
        }
        std::cout<<"\n";
    }
}
