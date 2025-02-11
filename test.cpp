#include <iostream>
#include <numeric>
#include <vector>
#include <iomanip>
using namespace std;


class Car {
public:
    int number;
    int capacity;

    Car() {};

    void setCapacity(int capacity_) {
        capacity = capacity_;
    }
    int getCapacity() {
        return capacity;
    }

    virtual void info() = 0;
    virtual ~Car() {}
};

class Bus: public Car {
public:
    int capacity = 30;

    Bus() {};
    int getCapacity() { return capacity; }

    void info() {
        std::cout << "Bus" << std::endl;
    }
};

class Taxi : public Car {
public:
    int capacity = 4;

    Taxi() {};
    int getCapacity() { return capacity; }

    void info() {
        std::cout << "Taxi" << std::endl;
    }
};

int FillBus(int sum_of_passengers) {
    Bus bus;
    int leftovers = 0;
    leftovers = bus.getCapacity() - abs(sum_of_passengers);
    return leftovers;
};

class CarFactory {
public:
    virtual Car* CreateCar() = 0;
    ~CarFactory() {}
};

class BusFactory: public CarFactory {
public:
    Car* CreateCar() {
        return new Bus;
    }
};

class TaxiFactory : public CarFactory {
public:
    Car* CreateCar() {
        return new Taxi;
    }
};

int FillTaxi(int sum_of_passengers) {
    Taxi taxi;
    int leftovers = 0;
    leftovers = taxi.getCapacity() - abs(sum_of_passengers);
    return leftovers;
};

// Синглтон не подходит по причине, что водителей будет вызвано больше одного
class Driver {
private:
    string lisence;
    string fullName;
public:
    Driver() {};
    Driver(string lisence_, string fullName_) {
        lisence = lisence_;
        fullName = fullName_;
    };

    void setLisence(string lisence_) {
        lisence = lisence_;
    }
    string getLisence() { return lisence; }
    void setFullName(string fullName_) {
        fullName = fullName_;
    }
    string getFullName() {
        return fullName
            ;
    }
    virtual void info() = 0;
    virtual ~Driver() {}
};

class DriverFactory {
public:
    DriverFactory() {};
    virtual Driver* CreateDriver() = 0;
    ~DriverFactory() {}
};

class BusDriver : public Driver {
public:
    BusDriver() {};
    void info() {
        std::cout << "BusDriver" << std::endl;
    }
};

class BusDriverFactory : public DriverFactory {
public:
    BusDriverFactory() {};
    Driver* CreateDriver() {
        return new BusDriver;
    }
};

class TaxiDriver : public Driver {
public:
    TaxiDriver() {};
    void info() {
        std::cout << "TaxiDriver" << std::endl;
    }
};

class TaxiDriverFactory : public DriverFactory {
public:
    TaxiDriverFactory() {};
    Driver* CreateDriver() {
        return new TaxiDriver;
    }
};

int main()
{
    // Очередь пассажиров
    // Пассажиры приходят на остановку с самого утра, т.е время ожидания начинается с нуля минут, т.к первый автобус приходит
    // по расписанию и перед ним нет других автобусов
    //  
    // Заполнять транспортные средства из очереди уменьшая числа в очереди потом
    std::vector<int> passengers;

    int* current_function;
    current_function = nullptr;

    do
    {
        std::cout << endl;
        std::cout << "----------------" << endl;
        std::cout << ' ' << "1" << ' ' << setw(2) << ' ' << "There are more passengers to come" << endl;
        std::cout << ' ' << "2" << ' ' << setw(2) << ' ' << "Board passengers" << endl;
        std::cout << ' ' << "3" << ' ' << setw(2) << ' ' << "There are no more passengers" << endl;
        std::cout << "----------------" << endl;
        std::cout << ' ' << setw(2) << ' ' << "Enter function number" << endl;

        int number_of_function;
        cin >> number_of_function;

        current_function = &number_of_function;

        // Самые первые пассажиры, ждущие автобус по расписанию
        //passengers.push_back(15);

        // Если не будет использоваться первая функция
        int sum_of_passengers = accumulate(passengers.begin(), passengers.end(), 0);

        // Увеличить очередь пассажиров
        if (number_of_function == 1) {
            int number;
            std::cout << "Please enter the number of passengers:\n";
            std::cin >> number;
            passengers.push_back(number);

            // Подсчет суммы элементов всего вектора пассажиров, ждущих автобус
            sum_of_passengers = accumulate(passengers.begin(), passengers.end(), 0);
        }

        // Отсчет времени
        int time_freeze = 0;

        // Вместимость машин
        int bus_capacity = 30;
        int taxi_capacity = 4;

        // Вектор всех машин
        vector<Car*> c;

        // Вектор всех водителей
        vector<Driver*> d;

        // Посадить пассажиров в транспорт
        if (number_of_function == 2) {
            // Вызов транспорта
            do {
                // Автобус ходит каждые 15 минут 
                // Всего водителей 6 на день, то есть маршрут составляет 15*6 минут
                if (time_freeze % 15 == 0) {

                    // Создает автобус
                    BusFactory* bus_factory = new BusFactory;

                    // Создает водителя
                    BusDriverFactory* bus_driver_factory = new BusDriverFactory;

                    // Вызывает автобус с помощью Create()
                    c.push_back(bus_factory->CreateCar());

                    // Вызывает водителя с помощью Create()
                    d.push_back(bus_driver_factory->CreateDriver());

                    // Заполняет автобус
                    // leftovers - остатки пассажиров
                    int leftovers = 0;
                    leftovers = FillBus(sum_of_passengers);

                    // Автобус ждал пассажиров 5 минут
                    time_freeze = time_freeze + 5;

                    // Остатки пассажиров идут обратно в очередь
                    do {
                        passengers.pop_back();
                    } while (passengers.size() >= 1);

                    passengers.push_back(leftovers);
                    sum_of_passengers = accumulate(passengers.begin(), passengers.end(), 0);

                    // Автобус заполнен пассажирами
                    std::cout << "The bus is full\n";
                };

                if (sum_of_passengers == 0) {
                    std::cout << "The bus is full\n";
                    std::cout << "There are no more passengers\n";
                };

                if (sum_of_passengers < 0) {
                    do {

                        int leftovers = 0;

                        if (sum_of_passengers == 0) {

                            // Пассажиров, нуждающихся в машине нет
                            time_freeze = time_freeze + 5;
                            std::cout << "There are no more passengers\n";
                        };

                        if (sum_of_passengers <= 4) {

                            // Водители такси всегда сменяются, в отличие от водителей автобусов
                            // Вызов такси с помощью Create()
                            TaxiFactory* taxi_factory = new TaxiFactory;

                            // Вызов водителя такси с помощью Create()
                            TaxiDriverFactory* taxi_driver = new TaxiDriverFactory;

                            // Добавление вызванного такси в вектор всех машин
                            c.push_back(taxi_factory->CreateCar());
                            // Добавление вызванного водителятакси в вектор всех водителей
                            d.push_back(taxi_driver->CreateDriver());

                            leftovers = FillTaxi(sum_of_passengers);

                            // Следующее такси можно будет вызвать через 5 минут 
                            time_freeze = time_freeze + 5;

                            // Обновление вектора оставшихся пассажиров
                            do {
                                passengers.pop_back();
                            } while (passengers.size() >= 1);

                            // Подсчет суммы вектора всех пассажиров
                            passengers.push_back(leftovers);
                            sum_of_passengers = accumulate(passengers.begin(), passengers.end(), 0);

                            // Такси заполнено пассажирами
                            std::cout << "The taxi is full\n";
                        };

                        if (sum_of_passengers > 4) {

                            TaxiFactory* taxi_factory = new TaxiFactory;
                            c.push_back(taxi_factory->CreateCar());
                            TaxiDriverFactory* taxi_driver = new TaxiDriverFactory;
                            d.push_back(taxi_driver->CreateDriver());

                            leftovers = FillTaxi(sum_of_passengers);
                            time_freeze = time_freeze + 5;

                            do {
                                passengers.pop_back();
                            } while (passengers.size() >= 1);

                            passengers.push_back(leftovers);
                            sum_of_passengers = accumulate(passengers.begin(), passengers.end(), 0);

                            std::cout << "The taxi is full\n";
                        };
                    } while (time_freeze % 15 != 0);
                };
            } while (sum_of_passengers != 0 );
        }
        // Заполнение очереди окончено
    } while (*current_function != 3 );

    system("pause");
}
