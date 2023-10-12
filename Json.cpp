
#include "Json.h"

// парсит текст в JSON-объект
// может парсить вложенные JSON-объекты, массивы объектов, массивы значений НО! не может парсить вложенные массивы
// замечание: нет обработки вложенных кавычек и иных знаков \char ("на двери была надпись "Закрыто" ")
JSONObject::JSONObject(std::string text)
{
    auto block_counter = (int)std::min(count(text.begin(), text.end(), '{'), count(text.begin(), text.end(), '}'));


    size_t pos1 = text.find_first_of("{");
    size_t pos2 = 0;
    while (block_counter > 0)
    {
        pos1 = text.find_first_of("\"", pos1);
        if (pos1 == -1)
            return;

        pos1++;
        pos2 = text.find_first_of("\"", pos1);
        if (pos2 == -1)
            return;

        std::string key = text.substr(pos1, pos2 - pos1);
        std::string value = "";
        pos1 = pos2 + 1;
        size_t check_pos = text.find_first_of("{[", pos1);

        if (check_pos > text.find_first_of("\"", pos1)) // if it is a default name to value data
        {
            pos1 = text.find_first_of("\"", pos1);
            if (pos1 == -1)
                return;

            pos1++;
            pos2 = text.find_first_of("\"", pos1);
            if (pos2 == -1)
                return;

            value = text.substr(pos1, pos2 - pos1);
            pos1 = pos2 + 1;
            name_to_value.emplace(std::make_pair(key, value));
        }

        else if (text[check_pos] == '{') // if it is a beginning of a JSONObject
        {
            size_t end_pos = find_block_end_1(text, (int)check_pos);
            JSONObject tmp(text.substr(check_pos, end_pos - check_pos + 1));
            name_to_object.emplace(std::make_pair(key, tmp));
            pos1 = end_pos + 1;
        }
        else if (text[check_pos] == '[') // if it is a beginning of an array
        {
            size_t end_pos = find_block_end_2(text, (int)check_pos);
            std::string array = text.substr(check_pos, end_pos - check_pos + 1);
            if(array.find_first_of("{}", 0) == -1)
                insert_array(key, array);
            else
                insert_object_array(key, array);

            pos1 = end_pos + 1;
        }
    }
}

JSONObject::JSONObject(const JSONObject& other) // конструктор копирования
{   
    for (const auto& i : other.name_to_values)
    {
        std::list<std::string> tmp;
        for (const auto& j : i.second)
            tmp.push_back(j);
    }

    for (const auto& i : other.name_to_object)
    {
        JSONObject tmp(i.second);
        name_to_object.emplace(std::make_pair(i.first, tmp));
    }

    for (const auto& i : other.name_to_objects)
    {
        std::list<JSONObject> tmp1;
        for (const auto& j : i.second)
        {
            JSONObject tmp2(j);
            tmp1.push_back(tmp2);
        }
        name_to_objects.emplace(std::make_pair(i.first, tmp1));
    }

    for (const auto& i : other.name_to_value)
        name_to_value.emplace(std::make_pair(i.first, i.second));
}

const JSONObject& JSONObject::get_object(const std::string& key) const
{
    return name_to_object.at(key);
}

const std::list<JSONObject>& JSONObject::get_objects(const std::string& key) const
{
    return name_to_objects.at(key);
}

const std::list<std::string>& JSONObject::get_values(const std::string& key) const
{
    return name_to_values.at(key);
}

const std::string& JSONObject::get_value(const std::string& key) const
{
    return name_to_value.at(key);
}

const std::unordered_map<std::string, JSONObject>& JSONObject::get_name_to_object() const
{
    return name_to_object;
}

const std::unordered_map<std::string, std::list<JSONObject> >& JSONObject::get_name_to_objects() const
{
    return name_to_objects;
}

const std::unordered_map<std::string, std::list<std::string> >& JSONObject::get_name_to_values() const
{
    return name_to_values;
}

const std::unordered_map<std::string, std::string>& JSONObject::get_name_to_value() const
{
    return name_to_value;
}

bool JSONObject::is_in_values(const std::string& key) // если такой ключ хранится в простых значениях, возвращает true, иначе - false
{
    if (name_to_value.find(key) == name_to_value.end())
        return false;

    return true;
}

bool JSONObject::is_in_objects(const std::string& key) // если такой ключ хранится в простых JSON-объектах, возвращает true, иначе - false
{
    if (name_to_object.find(key) == name_to_object.end())
        return false;

    return true;
}

bool JSONObject::is_in_arrays(const std::string& key) // если такой ключ хранится в массивах значений, возвращает true, иначе - false
{
    if (name_to_values.find(key) == name_to_values.end())
        return false;

    return true;
}

bool JSONObject::is_in_object_arrays(const std::string& key) // если такой ключ хранится в массивах JSON-объектов, возвращает true, иначе - false
{
    if (name_to_objects.find(key) == name_to_objects.end())
        return false;

    return true;
}

int JSONObject::find_block_end_1(const std::string& text, int curpos) const// вспомогательная функция для нахождения позиции конца блока ОБЪЕКТА
{
    size_t pos1 = curpos;
    int block_c = 1;
    while (block_c > 0)
    {
        pos1++;
        pos1 = text.find_first_of("{}", pos1);
        if (text[pos1] == '{')
            block_c++;
        else
            block_c--;
    }

    return (int)pos1;
}

int JSONObject::find_block_end_2(const std::string& text, int curpos) const// вспомогательная функция для нахождения позиции конца блока МАССИВА
{
    size_t pos1 = curpos;
    int block_c = 1;
    while (block_c > 0)
    {
        pos1++;
        pos1 = text.find_first_of("[]", pos1);
        if (text[pos1] == '[')
            block_c++;
        else
            block_c--;
    }

    return (int)pos1;
}

void JSONObject::operator=(const JSONObject& other) // присваивание/копирование
{
    for (const auto& i : other.name_to_values)
    {
        std::list<std::string> tmp;
        for (const auto& j : i.second)
            tmp.push_back(j);
    }

    for (const auto& i : other.name_to_object)
    {
        JSONObject tmp(i.second);
        name_to_object.emplace(std::make_pair(i.first, tmp));
    }

    for (const auto& i : other.name_to_objects)
    {
        std::list<JSONObject> tmp1;
        for (const auto& j : i.second)
        {
            JSONObject tmp2(j);
            tmp1.push_back(tmp2);
        }
        name_to_objects.emplace(std::make_pair(i.first, tmp1));
    }

    for (const auto& i : other.name_to_value)
        name_to_value.emplace(std::make_pair(i.first, i.second));
}

void JSONObject::insert_value(const std::string& key, std::string value)
{
    name_to_value.emplace(std::make_pair(key, value));
}

void JSONObject::insert_object(const std::string& key, JSONObject& obj)
{
    name_to_object.emplace(std::make_pair(key, obj));
}

void JSONObject::insert_object_array(const std::string& key, const std::string& array)
{
    size_t pos1 = array.find_first_of("{",0);
    size_t pos2;
    std::list<JSONObject> objects;
    while(pos1 != -1)
    {
        pos2 = find_block_end_1(array, (int)pos1);
        std::string text = array.substr(pos1, pos2 - pos1 + 1);
        JSONObject object(text);
        objects.push_back(object);
        pos1 = array.find_first_of("{", pos2 + 1);
    }

    name_to_objects.emplace(std::make_pair(key, objects));

}

void JSONObject::insert_array(const std::string& key, const std::string& array)
{
    size_t pos2;
    size_t pos1 = 0;
    pos1 = array.find_first_of("\"", pos1);
    std::list<std::string> values;
    while(pos1 != -1)
    {
        pos2 = array.find_first_of("\"",pos1 + 1);
        std::string value = array.substr(pos1 + 1, pos2 - pos1);
        values.push_back(value);
        pos1 = array.find_first_of("\"", pos2 + 1);
    }

    name_to_values.emplace(std::make_pair(key, values));
}
