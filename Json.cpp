
#include "Json.h"
#include<stack>

enum expectings{key, value, object, array, object_array, unidentified, unidentified_array};

JSONObject::JSONObject(std::string& source) // ???
{
    int begin = source.find_first_of('{', 0);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expectings current = key;
    while (has_next_key(source, ++begin)) {

        // pick the key
        k = pick_val(source, begin);
        begin++;

        // identify target field using control symbols \", { or [
        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                throw new std::invalid_argument("Key does not correspond to anything! Key = " + key);

            break;
        }

        // if the target is an array, determine the type of the array using control symbols \", {
        if (current == unidentified_array) {
            int pos_obj = source.find_first_of('{', begin);
            int pos_val = source.find_first_of('\"', begin);
            int pos_ctrl = source.find_first_of(']', begin);
            if (pos_ctrl == -1)
                throw new std::invalid_argument("Key " + k + "corresponds to invalid array");

            if (pos_ctrl < pos_val && pos_ctrl < pos_obj)
                current = array;
            else {
                bool v = pos_val != -1;
                bool o = pos_obj != -1;
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

        // after identifying the target, pick it and continue
        if (current == value) {
            this->name_to_value[k] = pick_val(source, begin);
        }
        else if (current == object) {
            JSONObject tmp(source, begin);
            this->name_to_object[k] = tmp;
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_array(source, begin);

            this->name_to_values[k] = tmp;
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_array(source, begin);
            this->name_to_objects[k] = tmp;
        }
    }
}

JSONObject::JSONObject(std::string& source, int& begin) // ???
{
    begin = source.find_first_of('{', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expectings current = key;
    while (has_next_key(source, ++begin)) {

        // pick the key
        k = pick_val(source, begin);
        begin++;

        // identify target field using control symbols \", { or [
        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                return;

            break;
        }

        // if the target is an array, determine the type of the array using control symbols \", {
        if (current == unidentified_array) {
            int pos_obj = source.find_first_of('{', begin);
            int pos_val = source.find_first_of('\"', begin);
            int pos_ctrl = source.find_first_of(']', begin);
            if (pos_ctrl == -1)
                throw new std::invalid_argument("Key " + k + "corresponds to invalid array");

            if (pos_ctrl < pos_val && pos_ctrl < pos_obj)
                current = array;
            else {
                bool v = pos_val != -1;
                bool o = pos_obj != -1;
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

        // after identifying the target, pick it and continue
        if (current == value) {
            this->name_to_value[k] = pick_val(source, begin);
        }
        else if (current == object) {
            JSONObject tmp(source, begin);
            this->name_to_object[k] = tmp;
        }
        else if (current == array) {
            std::list<std::string> tmp = pick_val_array(source, begin);

            this->name_to_values[k] = tmp;
        }
        else if (current == object_array) {
            std::list<JSONObject> tmp = pick_obj_array(source, begin);
            this->name_to_objects[k] = tmp;
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

    int end = find_block_end_array(source, begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    std::list<std::string> values;
    while (begin < end && has_next_key(source, begin))
    {
        std::string value = pick_val(source, begin);
        values.push_back(value);
        begin++;
    }

    return values;
}

std::list<JSONObject> JSONObject::pick_obj_array(std::string& source, int& begin)
{
    int pos1 = source.find_first_of('{', begin);
    int pos2 = source.find_first_of(']', begin);

    if (pos2 == -1)
        throw new std::invalid_argument("invalid source array. Position = " + std::to_string(begin));

    std::list<JSONObject> tmp;
    if (pos1 == -1 || pos2 < pos1) {
        begin = pos2;
        return tmp;
    }

    //std::string source = "{\"TestObjects\"[{    }     {}{\"key\"\"val\"}{}    {}{\"objs\"[{}{}]}]}";

    while (pos1 != -1) {
        
        JSONObject obj(source, pos1);
        begin = source.find_first_of(']', pos1);
        if (begin == -1)
            throw new std::invalid_argument("invalid source array. Position = " + std::to_string(begin));

        tmp.push_back(obj);
        pos1++;

        pos1 = source.find_first_of('{', pos1);

        if (pos1 > begin) {
            begin = pos2;
            break;
        }
    }
    return tmp;
}

bool JSONObject::has_next_key(std::string& source, int& pos)
{
    int end = source.find_first_of('}', pos);
    if (end == -1)
        throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

    int begin  = source.find_first_of('\"', pos);
    if (begin == -1 || begin > end) {
        pos = end;
        return false;
    }

    begin++;
    while (begin < source.size()) {
        if (source[begin] == '\"' && source[begin - 1] != '\\') {
            if (source.find_first_of('}', begin) == -1)
                throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

            return true;
        }

        begin++;
    }

    if (begin == source.size())
        return false;
}


void JSONObject::insert_value(const std::string& key, std::string value) // Wrong real
{
    name_to_value.emplace(std::make_pair(key, value));
}

void JSONObject::insert_object(const std::string& key, JSONObject& obj) // Wrong real
{
    name_to_object.emplace(std::make_pair(key, obj));
}


