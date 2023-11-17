
#include "Json.h"
#include<stack>

enum expectings{key, value, object, array, object_array, unidentified, unidentified_array};

JSONObject::JSONObject(std::string& source) // ???
{
    int start = source.find_first_of('{', 0);
    if (start == -1)
        throw new std::invalid_argument("source");

    int actual_end = find_block_end_obj(source, start);

    std::string k;
    expectings current = key;
    while (start < actual_end) {
        start++;
        if (current == key) {
            current = unidentified;
            if (has_next_key(source, start))
                k = pick_val(source, start);
            else
                return;
        }
        else if (current == value) {
            current = key;
            this->name_to_value.emplace(std::make_pair(k, pick_val(source, start)));
        }
        else if (current == object) {
            JSONObject tmp(source, start);
            this->name_to_object.emplace(std::make_pair(k, tmp));
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_array(source, start);
            this->name_to_values.emplace(std::make_pair(k, tmp));
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_array(source, start);
            this->name_to_objects.emplace(std::make_pair(k, tmp));
        }
        else if (current == unidentified) {
            for (; start < source.size(); start++) {
                if (source[start] == '\"')
                    current = value;
                else if (source[start] == '{')
                    current = object;
                else if (source[start] == '[')
                    current = unidentified_array;
                else
                    continue;

                break;
            }
        }
        else if (current == unidentified_array) {
            int arr_end = find_block_end_array(source, start);
            if (arr_end == -1 || arr_end >= actual_end)
                return;

            int pos_obj = source.find_first_of('{', start);
            int pos_val = source.find_first_of('\"', start);
            bool v = pos_val != -1 && pos_val < arr_end;
            bool o = pos_obj != -1 && pos_obj < arr_end;
            if (!v && !o)
                return;

            if (pos_obj < pos_val) {
                if (o) {
                    current = object_array;
                }
                else if (v) {
                    current = array;
                }
            }
            else if (v)
                current = array;
            else
                current = object_array;
        }
    }
}

JSONObject::JSONObject(std::string& source, int& begin) // ???
{
    int start = source.find_first_of('{', begin);
    if (start == -1)
        throw new std::invalid_argument("source");

    int actual_end = find_block_end_obj(source, start);

    std::string k;
    expectings current = key;
    while (start < actual_end) {
        start++;
        if (current == key) {
            current = unidentified;
            if (has_next_key(source, start))
                k = pick_val(source, start);
            else
                return;
        }
        else if (current == value) {
            current = key;
            this->name_to_value.emplace(std::make_pair(k, pick_val(source, start)));
        }
        else if (current == object) {
            JSONObject tmp(source, start);
            this->name_to_object.emplace(std::make_pair(k, tmp));
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_array(source, start);
            this->name_to_values.emplace(std::make_pair(k, tmp));
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_array(source, start);
            this->name_to_objects.emplace(std::make_pair(k, tmp));
        }
        else if (current == unidentified) {
            for (; start < source.size(); start++) {
                if (source[start] == '\"')
                    current = value;
                else if (source[start] == '{')
                    current = object;
                else if (source[start] == '[')
                    current = unidentified_array;
                else
                    continue;

                break;
            }
        }
        else if (current == unidentified_array) {
            int arr_end = find_block_end_array(source, start);
            if (arr_end == -1 || arr_end >= actual_end)
                return ;

            int pos_obj = source.find_first_of('{', start);
            int pos_val = source.find_first_of('\"', start);
            bool v = pos_val != -1 && pos_val < arr_end;
            bool o = pos_obj != -1 && pos_obj < arr_end;
            if (!v && !o)
                return;

            if (pos_obj < pos_val) {
                if (o) {
                    current = object_array;
                }
                else if (v) {
                    current = array;
                }
            }
            else if (v)
                current = array;
            else
                current = object_array;
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

bool JSONObject::is_in_values(const std::string& key) const
{
    if (name_to_value.find(key) == name_to_value.end())
        return false;

    return true;
}

bool JSONObject::is_in_objects(const std::string& key) const
{
    if (name_to_object.find(key) == name_to_object.end())
        return false;

    return true;
}

bool JSONObject::is_in_arrays(const std::string& key) const
{
    if (name_to_values.find(key) == name_to_values.end())
        return false;

    return true;
}

bool JSONObject::is_in_object_arrays(const std::string& key) const
{
    if (name_to_objects.find(key) == name_to_objects.end())
        return false;

    return true;
}

int JSONObject::find_block_end_obj(const std::string& source, int curpos)
{
    int pos = curpos;
    int counter = 1;
    bool counts = true;
    while (counter > 0 && pos + 1 < source.size()) {
        pos++;
        if (source[pos] == '\"') {
            if (pos > curpos && source[pos - 1] == '\\')
                pos++;
            else
                counts = (counts + 1) % 2;
        }
        else if (counts && source[pos] == '}')
            counter--;
        else if (counts && source[pos] == '{')
            counter++;
    }

    if (pos == source.size())
        return -1;

    return pos;
}

int JSONObject::find_block_end_array(const std::string& source, int curpos)
{
    int pos = curpos + 1;
    int counter = 1;
    bool counts = true;
    while (pos < source.size()) {
        if (source[pos] == '\"') {
            if (source[pos - 1] != '\\')
                counts = (counts + 1) % 2;
        }
        else if (counts && source[pos] == ']')
            return pos;

        pos++;
    }

    return -1;
}

std::string JSONObject::pick_val(std::string& source, int& begin) // (OK)
{
    if (begin + 1 >= source.size())
        throw new std::invalid_argument("begin");

    begin = source.find_first_of('\"', begin);
    if (begin + 1 > source.size() || begin == -1)
        throw new std::invalid_argument("source");

    begin++;
    std::stack<char> pile;
    while (begin < source.size()) {
        char cur = source[begin];
        if (cur == '\\') {
            if (begin + 1 < source.size()) {
                char next = source[begin + 1];
                if (next == '\\')
                    pile.push(cur);
                else if (next == '"')
                    pile.push('\"');
                else if (next == 'b')
                    pile.pop();
                else if (next == 't')
                    pile.push('\t');

                begin += 2;
            }
            else {
                throw new std::invalid_argument("source");
            }
        }
        else if (cur == '\"')
            break;
        else {
            pile.push(cur);
            begin++;
        }
    }

    if (begin == source.size())
        throw new std::invalid_argument("source");

    std::string res(pile.size(),'0');
    while (pile.size() > 0) {
        res[pile.size() - 1] = pile.top();
        pile.pop();
    }

    return res;
}

std::list<std::string> JSONObject::pick_val_array(std::string& source, int& begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    int pos1 = begin;
    int end = find_block_end_array(source, begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    std::list<std::string> values;
    while (pos1 < end && pos1 != -1)
    {
        std::string value = pick_val(source, pos1);
        values.push_back(value);
        pos1++;
        pos1 = source.find_first_of('\"', pos1);
    }

    return values;
}

std::list<JSONObject> JSONObject::pick_obj_array(std::string& source, int& begin)
{
    std::list<JSONObject> res;
    int pos1 = source.find_first_of("{", begin);
    while (pos1 != -1){
        int pos2 = find_block_end_obj(source, pos1);
        JSONObject tmp(source, pos1);
        res.push_back(tmp);
        pos1 = pos2 - pos1 + 1;
        pos1 = source.find_first_of("{", pos1);
    }

    return res;
}

bool JSONObject::has_next_key(std::string& source, int pos)
{
    if (pos + 1 >= source.size())
        return false;

    pos = source.find_first_of('\"', pos);
    if (pos + 1 > source.size() || pos == -1)
        return false;

    pos++;
    std::stack<char> pile;
    while (pos < source.size()) {
        char cur = source[pos];
        if (cur == '\\') {
            if (pos + 1 < source.size()) {
                char next = source[pos + 1];
                if (next == '\\')
                    pile.push(cur);
                else if (next == '"')
                    pile.push('\"');
                else if (next == 'b')
                    pile.pop();
                else if (next == 't')
                    pile.push('\t');

                pos += 2;
            }
            else {
                return false;
            }
        }
        else if (cur == '\"')
            break;
        else {
            pile.push(cur);
            pos++;
        }
    }

    if (pos == source.size())
        return false;

    std::string res(pile.size(), '0');
    while (pile.size() > 0) {
        res[pile.size() - 1] = pile.top();
        pile.pop();
    }

    return true;
}


void JSONObject::insert_value(const std::string& key, std::string value) // Wrong real
{
    name_to_value.emplace(std::make_pair(key, value));
}

void JSONObject::insert_object(const std::string& key, JSONObject& obj) // Wrong real
{
    name_to_object.emplace(std::make_pair(key, obj));
}

void JSONObject::insert_object_array(const std::string& key, const std::string& array)
{
    int pos1 = array.find_first_of("{",0);
    int pos2;
    std::list<JSONObject> objects;
    while(pos1 != -1)
    {
        pos2 = find_block_end_obj(array, (int)pos1);
        std::string text = array.substr(pos1, pos2 - pos1 + 1);
        JSONObject object(text);
        objects.push_back(object);
        pos1 = array.find_first_of("{", pos2 + 1);
    }

    name_to_objects.emplace(std::make_pair(key, objects));
}



