#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Интерфейсы, которые видны студентам

// Определение типа byte
typedef unsigned char byte;

// Входной поток
struct IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

// Выходной поток
struct IOutputStream {
    virtual void Write(byte value) = 0;
};

// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
