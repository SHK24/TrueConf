
///Верхняя граница для генерирования случайных чисел
#define UPPER_BOUND 10

///Нижняя граница для генерирования случайных чисел
#define LOWER_BOUND 0

///Коды ошибок
#define SET_SIZES_ERROR 1
#define SET_CONTENT_ERROR 2

#include <QCoreApplication>
#include <iostream>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>


void printContainer(std::map<unsigned int, unsigned int> * map_container)
{
    for(auto n : *map_container)
        std::cout << n.second <<' ';

    std::cout<<std::endl;
}

void printContainer(std::vector<unsigned int> * vector_container)
{
    for(auto n : *vector_container)
        std::cout << n << ' ';

    std::cout<<std::endl;
}

unsigned int generateRandomNumber(unsigned int lowerBound, unsigned int upperBound)
{
    return ((static_cast<unsigned int>(rand()) % upperBound) + lowerBound);
}

int generateRandomSeq_map(std::map<unsigned int, unsigned int> * resultMap, unsigned int elementsCount)
{
    for(unsigned int i = 0; i < elementsCount;i++)
        resultMap->insert(std::make_pair(i, generateRandomNumber(LOWER_BOUND, UPPER_BOUND)));

    return 0;
}

int generateRandomSeq_vector(std::vector<unsigned int> * resultVector, unsigned int elementsCount)
{
    for(unsigned int i = 0; i < elementsCount;i++)
        resultVector->push_back(generateRandomNumber(LOWER_BOUND,UPPER_BOUND));

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    srand(time(nullptr));

    unsigned int seqSize = 0;

    std::cout << "Enter sequence size: ";
    std::cin >> seqSize;

    ///Контейнеры vector и map для хранения случайных чисел
    std::map<unsigned int, unsigned int> map_container;
    std::vector<unsigned int> vector_container;

    ///Контейнеры множеств каждого из контейнеров vector и map
    std::set<unsigned int> vector_set;
    std::set<unsigned int> map_set;

    ///Вектор содержащий значения которые есть только в Map или только в Vector
    std::vector<int> difference(seqSize);
    ///Итератор для этого вектора
    std::vector<int>::iterator difIt;


    ///-------1.заполнить случайными числами от 1 до 9 значения контейнеров vector[i] и map[i];----------
    generateRandomSeq_map(&map_container, seqSize);
    generateRandomSeq_vector(&vector_container, seqSize);

    std::cout <<"-----------Basic content-----------"<<std::endl;

    std::cout<<"Map content:"<<std::endl;
    printContainer(&map_container);

    std::cout<<"Vector content:"<<std::endl;
    printContainer(&vector_container);

    ///--------------------------------------------------------------------------------------------------

    ///-------2.удалить случайное число элементов (не более 15) в каждом контейнере;---------------------

    ///Итераторы
    auto mapIt    = map_container.begin();
    auto vectorIt = vector_container.begin();

    ///Генерирование случайного числа удаляемых значений
    unsigned char removeCount = static_cast<unsigned char>(generateRandomNumber(0,16));

    while(removeCount > seqSize)
        removeCount = static_cast<unsigned char>(generateRandomNumber(0,16));

    std::cout << std::endl << "Number of items to remove: " << (int)removeCount <<std::endl << std::endl;

    int startValue = 0;

    ///Удаление элементов до тех пор, пока не достигнем ранее сгенерированного значения
    while(startValue < removeCount)
    {
        mapIt = map_container.erase(mapIt);
        vectorIt = vector_container.erase(vectorIt);
        startValue++;
    }

    std::cout <<"-----------Content after remove-----------"<<std::endl;

    std::cout<<"Map content:"<<std::endl;
    printContainer(&map_container);

    std::cout<<"Vector content:"<<std::endl;
    printContainer(&vector_container);

    ///--------------------------------------------------------------------------------------------------

    ///------3.после этого провести синхронизацию, чтобы в vector и map остались только имеющиеся в обоих контейнерах элементы (дубликаты не удалять).-------

    ///Для синхронизации контейнеров заполним множества для каждого из набора чисел

    ///Заполнение множества для Map
    for(auto item : map_container)
        map_set.insert(item.second);

    ///Заполнение множества для Vector
    for(auto n : vector_container)
        vector_set.insert(n);


    ///Определение уникальных значений находящихся в обоих контейнерах
    difIt = std::set_symmetric_difference(vector_set.begin(), vector_set.end(), map_set.begin(), map_set.end(), difference.begin());
    difference.resize(difIt-difference.begin());

    ///Удаление ранее обнаруженных уникальных значений из Vector
    vectorIt = vector_container.begin();

    while(vectorIt != vector_container.end())
    {
        auto it = std::find(difference.begin(), difference.end(), *vectorIt);

        if(it != difference.end())
            vectorIt = vector_container.erase(vectorIt);
        else {
            vectorIt++;
        }
    }

    ///Удаление ранее обнаруженных уникальных значений из Map
    mapIt = map_container.begin();

    while(mapIt != map_container.end())
    {
        auto it = std::find(difference.begin(), difference.end(), ((*mapIt).second));

        if(it != difference.end())
            mapIt = map_container.erase(mapIt);
        else {
            mapIt++;
        }
    }
    ///---------------------------------------------------------------------------------------------------------------------------------------------------

    ///Проверка на синхронность, производится с помощью множеств
    map_set.clear();
    vector_set.clear();

    for(auto item : map_container)
        map_set.insert(item.second);

    for(auto n : vector_container)
        vector_set.insert(n);

    if(map_set.size() != vector_set.size())
    {
        std::cout<<"Set sizes do not match"<<std::endl;
        return SET_SIZES_ERROR;
    }

    if(map_set != vector_set)
    {
        std::cout<<"Content of the sets do not match"<<std::endl;
        return SET_CONTENT_ERROR;
    }

    std::cout <<"-----------Content after sync-----------"<<std::endl;

    std::cout<<"Map content:"<<std::endl;
    printContainer(&map_container);

    std::cout<<"Vector content:"<<std::endl;
    printContainer(&vector_container);

    return 0;
}
