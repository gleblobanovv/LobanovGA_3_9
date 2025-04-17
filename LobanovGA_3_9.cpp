#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

const int MAX_PLANETS = 100;

class Planet {
public:
    char name[50];
    double radius;
    double mass;
    char system[50];

    void inputFromConsole();
    void print();
    void saveToFile(FILE* file);
};

void Planet::inputFromConsole() {
    char buffer[100];

    printf("Введите название планеты: ");
    scanf("%s", name);

    printf("Введите радиус: ");
    while (scanf("%lf", &radius) != 1) {
        printf("Неверный ввод. Введите радиус ещё раз: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    printf("Введите массу: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%lf", &mass);

    printf("Введите систему: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%49s", system);
}

void Planet::print() {
    printf("%-12s %10.2lf %15.2lf %s\n", name, radius, mass, system);
}

void Planet::saveToFile(FILE* file) {
    fprintf(file, "%s %.2lf %.2lf %s\n", name, radius, mass, system);
}

// Сравнение 
int compareByName(const void* a, const void* b) {
    return strcmp(((Planet*)a)->name, ((Planet*)b)->name);
}

int compareByMass(const void* a, const void* b) {
    double diff = ((Planet*)a)->mass - ((Planet*)b)->mass;
    return (diff > 0) - (diff < 0);
}

int compareByRadius(const void* a, const void* b) {
    double diff = ((Planet*)a)->radius - ((Planet*)b)->radius;
    return (diff > 0) - (diff < 0);
}

// Загрузка из файла 
int loadFromFile(const char* filename, Planet planets[]) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || file == nullptr) {
        printf("Файл не найден или ошибка чтения: %s\n", filename);
        return 0;
    }

    int count = 0;
    while (!feof(file) && count < MAX_PLANETS) {
        if (fscanf(file, "%49s %lf %lf %49s",
            planets[count].name,
            &planets[count].radius,
            &planets[count].mass,
            planets[count].system) == 4) {
            count++;
        }
    }

    fclose(file);
    return count;
}

int main() {
    setlocale(LC_ALL, "RUS");

    Planet planets[MAX_PLANETS];
    int count = 0;

    count = loadFromFile("planets.txt", planets);
    printf("Загружено планет из файла: %d\n", count);

    // Добавление новых планет
    char choice;
    do {
        printf("Добавить новую планету? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            if (count >= MAX_PLANETS) {
                printf("Достигнут предел количества планет.\n");
                break;
            }
            planets[count].inputFromConsole();
            count++;
        }
    } while (choice == 'y' || choice == 'Y');

    int param, order;
    printf("\nВыберите параметр сортировки:\n1 - Имя\n2 - Масса\n3 - Радиус\nВаш выбор: ");
    scanf("%d", &param);
    printf("Выберите порядок сортировки:\n1 - По возрастанию\n2 - По убыванию\nВаш выбор: ");
    scanf("%d", &order);

    // Применение сортировки
    if (param == 1)
        qsort(planets, count, sizeof(Planet), compareByName);
    else if (param == 2)
        qsort(planets, count, sizeof(Planet), compareByMass);
    else if (param == 3)
        qsort(planets, count, sizeof(Planet), compareByRadius);

    if (order == 2) { // Обратный порядок
        for (int i = 0; i < count / 2; i++) {
            Planet temp = planets[i];
            planets[i] = planets[count - 1 - i];
            planets[count - 1 - i] = temp;
        }
    }

    printf("\nОтсортированные планеты:\n");
    for (int i = 0; i < count; i++) {
        planets[i].print();
    }

    FILE* outFile;
    errno_t err = fopen_s(&outFile, "sorted_planets.txt", "w");
    if (err != 0 || outFile == nullptr) {
        printf("Ошибка сохранения файла.\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        planets[i].saveToFile(outFile);
    }

    fclose(outFile);
    printf("\nДанные записаны в sorted_planets.txt\n");

    return 0;
}