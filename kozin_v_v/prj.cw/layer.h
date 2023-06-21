#ifndef LAYER_H
#define LAYER_H

#include <string>

typedef unsigned char byte;

/**
 * @brief Класс Layer представляет слой изображения.
 */
class Layer
{
public:
    /**
     * @brief Конструктор класса Layer.
     */
    Layer();

    std::string name; /**< Имя слоя. */

    /**
     * @brief Проверяет видимость слоя.
     * @return true, если слой видим, иначе false.
     */
    bool isVisible() const { return _visible; }

    /**
     * @brief Изменяет видимость слоя.
     */
    void toggleVisibility()
    {
        _visible = !_visible;
    }

    /**
     * @brief Возвращает размер данных слоя.
     * @return Размер данных слоя.
     */
    int dataSize() const;

    /**
     * @brief Устанавливает размер слоя и инициализирует его пикселями.
     * @param size Размер слоя в виде массива [ширина, высота].
     * @param pixel Массив пикселей для инициализации слоя.
     */
    void setSize(int size[2], const byte pixel[]);

    /**
     * @brief Устанавливает размер слоя и инициализирует его пикселями.
     * @param w Ширина слоя.
     * @param h Высота слоя.
     * @param pixel Массив пикселей для инициализации слоя.
     */
    void setSize(int w, int h, const byte pixel[]);

    /**
     * @brief Устанавливает значение пикселя слоя по указанным координатам.
     * @param x Координата X пикселя.
     * @param y Координата Y пикселя.
     * @param pixel Массив значений пикселя [R, G, B, A].
     */
    void setPixel(int x, int y, const byte pixel[]);

    int size[2] = {0, 0}; /**< Размер слоя в виде массива [ширина, высота]. */
    int bpp = 4; /**< Количество байт на пиксель. */
    unsigned char* data = nullptr; /**< Данные слоя. */

    /**
     * @brief Создает слой с размером и пикселями по умолчанию.
     * @param size Размер слоя в виде массива [ширина, высота].
     * @param pixel Массив пикселей для инициализации слоя.
     * @return Указатель на созданный слой.
     */
    static Layer* defaultLayer(int size[2], const byte pixel[]);

    /**
     * @brief Создает слой из файла.
     * @param filename Имя файла.
     * @return Указатель на созданный слой.
     */
    static Layer* fromFile(const char* filename);

private:
    bool _visible = true; /**< Флаг видимости слоя. */
};

#endif // LAYER_H
