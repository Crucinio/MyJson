
#ifndef JSON_H
#define JSON_H


#include<unordered_map>
#include<iostream>
#include<string>
#include<list>
#include<assert.h>

class JSONObject
{
private:
    // key - valuse
    std::unordered_map <std::string, std::string> name_to_value;

    // key - object
    std::unordered_map <std::string, JSONObject> name_to_object;

    // key - list<object>
    std::unordered_map <std::string, std::list<JSONObject> > name_to_objects;

    // key - list<value>
    std::unordered_map <std::string, std::list<std::string> > name_to_values;
    
    // assisting parsing function, return index of OBJECT string block end
    int find_block_end_1(const std::string&, int) const;

    // assisting parsing function, return index of ARRAY string block end
    int find_block_end_2(const std::string&, int) const;

public:

    ~JSONObject() = default;
    JSONObject() = default;

    // String constructor
    explicit JSONObject(std::string);

    // Copying
    JSONObject(const JSONObject&);

    // returns object in from name_to_object
    const JSONObject& get_object(const std::string& key) const;

    // returns vector<object> in from name_to_object
    const std::list<JSONObject>& get_objects(const std::string& key) const;

    // returns object in from name_to_object
    const std::list<std::string>& get_values(const std::string& key) const;

    // returns value from name_to_value
    const std::string& get_value(const std::string& key) const;

    const std::unordered_map <std::string, JSONObject>& get_name_to_object() const;
    const std::unordered_map <std::string, std::list<JSONObject> >& get_name_to_objects() const;
    const std::unordered_map <std::string, std::list<std::string> >& get_name_to_values() const;
    const std::unordered_map <std::string, std::string>& get_name_to_value() const;

    // methods that return whether key - X pair exists
    // X = value
    bool is_in_values(const std::string& key);

    // X = object
    bool is_in_objects(const std::string& key);

    // X = list of values
    bool is_in_arrays(const std::string& key);

    // X = list of objects
    bool is_in_object_arrays(const std::string& key);

    // присваивание/копирование
    void operator =(const JSONObject& other);

    // вставить
    // в значения
    void insert_value(const std::string& key, std::string value);

    // в объекты
    void insert_object(const std::string& key, JSONObject& obj);

    // в массивы объектов
    void insert_object_array(const std::string& key, const std::string& array);

    // в массивы значений
    void insert_array(const std::string& key, const std::string& array);
};





#endif // JSON_H
