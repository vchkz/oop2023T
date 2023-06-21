#ifndef IMAGE_H
#define IMAGE_H


#include "layer.h"
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cmath>
#include <algorithm>

/**
 * @brief Класс Image представляет изображение, состоящее из слоев.
 */
class Image {
public:
    /**
     * @brief Конструктор класса Image
     */
    Image();

    /**
     * @brief Деструктор класса Image
     */
    virtual ~Image();

    std::string name; /**< Имя изображения */
    std::string fullPath; /**< Полный путь к изображению */
    int size[2] = {256, 256}; /**< Размер изображения */
    bool isOpen = false; /**< Флаг, указывающий, открыто ли изображение */
    bool maskIsCreated = false; /**< Флаг, указывающий, создана ли маска */

    /**
     * @brief Добавляет новый слой в изображение
     */
    void addLayer();

    /**
     * @brief Загружает изображение из файла
     * @param filename Имя файла
     */
    void fromFile(const char *filename);

    /**
     * @brief Загружает изображение из cv::Mat
     * @param image Изображение в формате cv::Mat
     * @param name Имя изображения
     */
    void fromMAT(const cv::Mat &image, const std::string &name);

    /**
     * @brief Проверяет, были ли внесены изменения в изображение
     * @return true, если внесены изменения, иначе false
     */
    bool isDirty() const { return _dirty; }

    /**
     * @brief Устанавливает флаг изменений для изображения
     * @param dirty Флаг изменений
     */
    void setDirty(bool dirty) { _dirty = dirty; }

    /**
     * @brief Проверяет, были ли внесены изменения, требующие сохранения
     * @return true, если внесены изменения, требующие сохранения, иначе false
     */
    bool isDirtyForSaving() const { return _dirtyForSaving; }

    /**
     * @brief Устанавливает флаг изменений, требующих сохранения
     * @param dirty Флаг изменений, требующих сохранения
     */
    void setDirtyForSaving(bool dirty) { _dirtyForSaving = dirty; }

    /**
     * @brief Удаляет курсор
     */
    void deleteCursor();

    /**
     * @brief Изменяет размер курсора
     * @param x Изменение размера
     */
    void resizeCursor(int x);

    float brushColor[4] = {0.0f, 0.0f, 0.0f, 1.0f}; /**< Цвет кисти */
    float eraseColor[4] = {0.0f, 0.0f, 0.0f, 0.0f}; /**< Цвет ластика */
    float alpha = 0.7f; /**< Значение прозрачности */

    /**
     * @brief Обработка движения мыши
     * @param x Координата X
     * @param y Координата Y
     */
    void mouseMove(int x, int y);

    /**
     * @brief Обработка нажатия правой кнопки мыши
     */
    void rightMouseButton();

    /**
     * @brief Обработка нажатия левой кнопки мыши
     */
    void leftMouseButton();

    /**
     * @brief Обработка отпускания кнопки мыши
     */
    void mouseButtonUp();

    /**
     * @brief Отмена последнего действия
     */
    void Undo();

    /**
     * @brief Отрисовка курсора
     * @param x Координата X
     * @param y Координата Y
     */
    void drawCursor(int x, int y);

    /**
     * @brief Создание текстуры из изображения
     */
    void texImage();

    /**
     * @brief Получение маски
     * @return Указатель на маску
     */
    Layer *getMask();

    /**
     * @brief Получение размера стека отмены
     * @return Размер стека отмены
     */
    int UndoStackSize();

    unsigned char indexInDirectory = 0; /**< Индекс в каталоге */



private:
    std::vector<Layer *> _layers; /**< Список слоев */
    std::vector<Layer *> _undoStack; /**< Стек отмены */
    bool _dirty = true; /**< Флаг, указывающий на изменения в изображении */
    bool _dirtyForSaving = false; /**< Флаг, указывающий на необходимость сохранения изменений */
    int _lastCursorPosition[2] = {40, 40}; /**< Последнее положение курсора */
    int _cursorSize = 10; /**< Размер курсора */
    bool _isPainting = false; /**< Флаг, указывающий на режим рисования */
    bool _isErasing = false; /**< Флаг, указывающий на режим стирания */

    /**
     * @brief Выполняет рисование на слое.
     *
     * @param layer Указатель на слой, на котором выполняется рисование
     * @param color Цвет для рисования в формате RGBA
     * @param from Начальная позиция для рисования
     * @param to Конечная позиция для рисования
     * @param brushWidth Ширина кисти
     */
    void paint(Layer *layer, float color[], const glm::vec2 &from, const glm::vec2 &to, float brushWidth);

    /**
     * @brief Помещает текущее состояние слоя в стек отмены.
     */
    void toCtrlZStack();

    GLuint _glindex = 0; /**< Идентификатор текстуры OpenGL */
    unsigned char *_data = nullptr; /**< Данные изображения */



};

#endif // IMAGE_H
