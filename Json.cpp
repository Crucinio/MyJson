#include "Json.h"

#include<stack>
#include<stdexcept>
#include<fstream>

enum expecting { key, value, number, object, array, object_array, number_array, boolean_array, unidentified, unidentified_array };

JSONObject::JSONObject(std::string& source) // ???
{
    size_t begin = source.find_first_of('{', 0);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expecting current = key;
    while (has_next_key(source, ++begin)) {
        k = pick_val(source, begin); // pick the key
        begin++;

        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == 't' && source.size() - begin - 3 > 0 && source.substr(begin, 4) == "true") {
                current = key;
                this->key_to_bool[k] = true;
                begin += 3;
            }
            else if (source[begin] == 'f' && source.size() - begin - 4 > 0 && source.substr(begin, 5) == "false") {
                current = key;
                this->key_to_bool[k] = false;
                begin += 4;
            }
            else if ((source[begin] >= '0' && source[begin] <= '9') || (source[begin] == '-' && source.size() - 1 > begin && source[begin + 1] >= '0' && source[begin + 1] <= '9')) {
                current = number;
            }
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                throw new std::invalid_argument("Key does not correspond to anything! Key = " + k);

            break;
        } // identify target field using control symbols \", {, [ or T&F

        if (current == unidentified_array) {
            size_t tmp = begin;
            for (; tmp < source.size(); tmp++) {
                if (source[tmp] == '\"' || source[tmp] == ']')
                    current = array;
                else if ((source[tmp] == 't' && source.size() - tmp - 3 > 0 && source.substr(tmp, 4) == "true")
                    || (source[tmp] == 'f' && source.size() - tmp - 4 > 0 && source.substr(tmp, 5) == "false")) {
                    current = boolean_array;
                }
                else if (source[tmp] == '{')
                    current = object_array;
                else if ((source[tmp] == '-' && source[tmp + 1] >= '0' && source[tmp + 1] <= '9') || (source[tmp] >= '0' && source[tmp] <= '9'))
                    current = number_array;
                else
                    continue;

                if (tmp == source.size())
                    throw new std::invalid_argument("Array is not closed! Key = " + k);

                break;
            } // if the target is an array, determine the type of the array using control symbols \", {, T&F
        }

        switch (current) {
        case value:
            this->key_to_value[k] = pick_val(source, begin);
            break;
        case number:
            this->key_to_num[k] = pick_num(source, begin);
            break;
        case object:
            this->key_to_object[k] = JSONObject(source, begin);
            break;
        case array:
            this->key_to_value_list[k] = pick_val_list(source, begin);
            break;
        case object_array:
            this->key_to_object_list[k] = pick_obj_list(source, begin);
            break;
        case number_array:
            this->key_to_num_list[k] = pick_num_list(source, begin);
            break;
        case boolean_array:
            this->key_to_boolean_list[k] = pick_bool_list(source, begin);
            break;
        }
    }
}

JSONObject::JSONObject(std::string& source, size_t& begin) // ???
{
    begin = source.find_first_of('{', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    std::string k;
    expecting current = key;
    while (has_next_key(source, ++begin)) {
        k = pick_val(source, begin); // pick the key
        begin++;

        for (; begin < source.size(); begin++) {
            if (source[begin] == '\"')
                current = value;
            else if (source[begin] == 't' && source.size() - begin - 3 > 0 && source.substr(begin, 4) == "true") {
                current = key;
                this->key_to_bool[k] = true;
                begin += 3;
            }
            else if (source[begin] == 'f' && source.size() - begin - 4 > 0 && source.substr(begin, 5) == "false") {
                current = key;
                this->key_to_bool[k] = false;
                begin += 4;
            }
            else if ((source[begin] >= '0' && source[begin] <= '9') || (source[begin] == '-' && source.size() - 1 > begin && source[begin + 1] >= '0' && source[begin + 1] <= '9')) {
                current = number;
            }
            else if (source[begin] == '{')
                current = object;
            else if (source[begin] == '[')
                current = unidentified_array;
            else
                continue;

            if (begin == source.size())
                throw new std::invalid_argument("Key does not correspond to anything! Key = " + k);

            break;
        } // identify target field using control symbols \", {, [ or T&F

        if (current == unidentified_array) {
            size_t tmp = begin;
            for (; tmp < source.size(); tmp++) {
                if (source[tmp] == '\"' || source[tmp] == ']')
                    current = array;
                else if ((source[tmp] == 't' && source.size() - tmp - 3 > 0 && source.substr(tmp, 4) == "true")
                    || (source[tmp] == 'f' && source.size() - tmp - 4 > 0 && source.substr(tmp, 5) == "false")) {
                    current = boolean_array;
                }
                else if (source[tmp] == '{')
                    current = object_array;
                else if ((source[tmp] == '-' && source[tmp + 1] >= '0' && source[tmp + 1] <= '9') || (source[tmp] >= '0' && source[tmp] <= '9'))
                    current = number_array;
                else
                    continue;

                if (tmp == source.size())
                    throw new std::invalid_argument("Array is not closed! Key = " + k);

                break;
            } // if the target is an array, determine the type of the array using control symbols \", {, T&F
        }

        // after identifying the target, pick it and insert/rewrite
        switch (current) {
        case value:
            this->key_to_value[k] = pick_val(source, begin);
            break;
        case number:
            this->key_to_num[k] = pick_num(source, begin);
            break;
        case object:
            this->key_to_object[k] = JSONObject(source, begin);
            break;
        case array:
            this->key_to_value_list[k] = pick_val_list(source, begin);
            break;
        case object_array:
            this->key_to_object_list[k] = pick_obj_list(source, begin);
            break;
        case number_array:
            this->key_to_num_list[k] = pick_num_list(source, begin);
            break;
        case boolean_array:
            this->key_to_boolean_list[k] = pick_bool_list(source, begin);
            break;
        }
    }
}

int JSONObject::find_block_end_array(const std::string& source, size_t curpos)
{
    curpos++;
    bool counts = true;
    while (curpos < source.size()) {
        if (source[curpos] == '\"') {
            if (source[curpos - 1] != '\\')
                counts = !counts;
        }
        else if (source[curpos] == ']' && counts)
            return curpos;

        curpos++;
    }

    return -1;
}

std::string JSONObject::pick_val(std::string& source, size_t& begin) // (OK)
{
    if (begin + 1 >= source.size())
        throw new std::invalid_argument("begin");

    begin = source.find_first_of('\"', begin);
    if (begin + 1 > source.size() || begin == -1)
        throw new std::invalid_argument("source");

    begin++;
    std::string word;
    while (begin < source.size()) {
        char cur = source[begin];
        if (cur == '\\') {
            if (begin + 1 < source.size()) {
                char next = source[begin + 1];
                switch (next) {
                case '\\' :word.push_back(cur);
                    break;
                case '\"':word.push_back('\"');
                    break;
                case 'b':word.push_back('\b');
                    break;
                case 't':word.push_back('\t');
                    break;
                case 'f':word.push_back('\f');
                    break;
                case 'n':word.push_back('\n');
                    break;
                case 'r':word.push_back('\r');
                    break;
                default: word.push_back(cur);
                    begin--;
                }

                begin += 2;
            }
            else {
                throw new std::invalid_argument("source");
            }
        }
        else if (cur == '\"')
            break;
        else {
            word.push_back(cur);
            begin++;
        }
    }

    if (begin >= source.size())
        throw new std::invalid_argument("source");

    return word;
}

std::list<std::string> JSONObject::pick_val_list(std::string& source, size_t& begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    size_t end = find_block_end_array(source, begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    std::list<std::string> values;
    begin = source.find_first_of('\"', begin);
    while (begin < end && begin != -1)
    {
        std::string value = pick_val(source, begin);
        values.push_back(value);
        begin++;
        begin = source.find_first_of('\"', begin);
    }

    begin = end;
    return values;
}

long double JSONObject::pick_num(std::string& source, size_t& begin)
{
    size_t start = source.find_first_not_of(" ,", begin);
    size_t end = source.find_first_of(" ,}", start);
    std::string num = source.substr(start, end - start);
    if (!tryParseLDouble(num) || end == -1)
        throw new std::invalid_argument("non-numeric");

    begin = end - 1;
    return std::stold(num);
}

std::list<long double> JSONObject::pick_num_list(std::string& source, size_t& begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    size_t check = source.find_first_of('}', begin);

    size_t end = source.find_first_of("]", begin);
    if (end == -1)
        throw new std::invalid_argument("source::no_closing_bracket");

    if (check < end)
        throw new std::invalid_argument("source has invalid brackets");

    begin++;
    std::list<long double> nums;

    size_t start = source.find_first_not_of(" ,", begin);
    while (start != -1 && start != end) {
        size_t num_end = source.find_first_of(" ,]", start);
        std::string num = source.substr(start, num_end - start);
        if (!tryParseLDouble(num))
            throw new std::invalid_argument("non-numeric");

        nums.push_back(std::stold(num));
        start = source.find_first_not_of(" ,", num_end);
    }

    begin = end;
    return nums;
}

std::list<JSONObject> JSONObject::pick_obj_list(std::string& source, size_t& begin)
{
    size_t pos1 = source.find_first_of('{', begin);
    size_t pos2 = source.find_first_of(']', begin);

    if (pos2 == -1)
        throw new std::invalid_argument("invalid source array. Position = " + std::to_string(begin));

    std::list<JSONObject> tmp;
    if (pos1 == -1 || pos2 < pos1) {
        begin = pos2;
        return tmp;
    }

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

std::list<bool> JSONObject::pick_bool_list(std::string& source, size_t& begin)
{
    begin = source.find_first_of('[', begin);
    if (begin == -1)
        throw new std::invalid_argument("source");

    size_t end = source.find_first_of(']', begin);
    if (end == -1)
        throw new std::invalid_argument("source");

    std::list<bool> bools;
    while (begin != end)
    {
        if (source[begin] == 't' && source.size() - begin - 3 > 0 && source.substr(begin, 4) == "true") {
            bools.push_back(true);
            begin += 3;
        }
        else if (source[begin] == 'f' && source.size() - begin - 4 > 0 && source.substr(begin, 5) == "false") {
            bools.push_back(false);
            begin += 4;
        }
        
        begin++;
    }

    return bools;
}

bool JSONObject::has_next_key(std::string& source, size_t& pos)
{
    if (pos > source.size() - 1)
        throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

    size_t end = source.find_first_of('}', pos);
    if (end == -1)
        throw new std::invalid_argument("Source file does not contain closing bracket for current object. Latest position = " + std::to_string(pos));

    size_t begin = source.find_first_of('\"', pos);
    if (begin == -1 || begin > end) {
        pos = end;
        return false;
    }

    pos = begin;
    return true;
}

bool JSONObject::tryParseLDouble(std::string num)
{
    try {
        long double res = std::stold(num);
        return true;
    }
    catch (std::exception e) {
        return false;
    }
}

JSONObject& JSONObject::get_object(std::string key) { return key_to_object.at(key); }

std::list<JSONObject>& JSONObject::get_obj_list(std::string key) { return key_to_object_list.at(key); }

std::list<std::string>& JSONObject::get_val_list(std::string key) { return key_to_value_list.at(key); }

std::list<bool>& JSONObject::get_bool_list(std::string key) { return key_to_boolean_list.at(key); }

std::list<long double>& JSONObject::get_num_list(std::string key) { return key_to_num_list.at(key); }

std::string& JSONObject::get_value(std::string key) { return key_to_value.at(key); }

bool JSONObject::get_bool(std::string key) { return key_to_bool.at(key); }

long double JSONObject::get_num(std::string key) { return key_to_num.at(key); }

const std::unordered_map<std::string, JSONObject>& JSONObject::get_name_to_object() const { return key_to_object; }

const std::unordered_map<std::string, std::list<JSONObject> >& JSONObject::get_name_to_objects() const { return key_to_object_list; }

const std::unordered_map<std::string, std::list<std::string> >& JSONObject::get_name_to_values() const { return key_to_value_list; }

const std::unordered_map<std::string, std::list<long double>>& JSONObject::get_name_to_nums() const { return key_to_num_list; }

const std::unordered_map<std::string, std::list<bool>>& JSONObject::get_name_to_bools() const { return key_to_boolean_list; }

const std::unordered_map<std::string, std::string>& JSONObject::get_name_to_value() const { return key_to_value; }

const std::unordered_map<std::string, long double>& JSONObject::get_name_to_num() const { return key_to_num; }

const std::unordered_map<std::string, bool>& JSONObject::get_name_to_bool() const { return key_to_bool; }

bool JSONObject::is_in_values(std::string key) const
{
    if (key_to_value.find(key) == key_to_value.end())
        return false;

    return true;
}

bool JSONObject::is_in_objects(std::string key) const
{
    if (key_to_object.find(key) == key_to_object.end())
        return false;

    return true;
}

bool JSONObject::is_in_bools(std::string key) const
{
    if (key_to_bool.find(key) == key_to_bool.end())
        return false;

    return true;
}

bool JSONObject::is_in_nums(std::string key) const
{
    if (key_to_num.find(key) == key_to_num.end())
        return false;

    return true;
}

bool JSONObject::is_in_val_lists(std::string key) const
{
    if (key_to_value_list.find(key) == key_to_value_list.end())
        return false;

    return true;
}

bool JSONObject::is_in_obj_lists(std::string key) const
{
    if (key_to_object_list.find(key) == key_to_object_list.end())
        return false;

    return true;
}

bool JSONObject::is_in_bool_lists(std::string key) const
{
    if (key_to_boolean_list.find(key) == key_to_boolean_list.end())
        return false;

    return true;
}

bool JSONObject::is_in_num_lists(std::string key) const
{
    if (key_to_num_list.find(key) == key_to_num_list.end())
        return false;

    return true;
}

bool JSONObject::empty_values() const { return key_to_value.empty(); }

bool JSONObject::empty_objects() const { return key_to_object.empty(); }

bool JSONObject::empty_nums() const { return key_to_num.empty(); }

bool JSONObject::empty_bools() const { return key_to_bool.empty(); }

bool JSONObject::empty_val_lists() const { return key_to_value_list.empty(); }

bool JSONObject::empty_obj_lists() const { return key_to_object_list.empty(); }

bool JSONObject::empty_num_lists() const { return key_to_num_list.empty(); }

bool JSONObject::empty_bool_lists() const { return key_to_boolean_list.empty(); }

bool JSONObject::empty() const
{
    return key_to_value.size() == 0 &&
        key_to_object.size() == 0 &&
        key_to_value_list.size() == 0 &&
        key_to_object_list.size() == 0;
}

void JSONObject::set_value(std::string key, std::string value) { key_to_value[key] = value; }

void JSONObject::set_value(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    start++;
    std::string value = pick_val(source, start);
    key_to_value[key] = value;
}

void JSONObject::set_bool(std::string key, bool value) { key_to_bool[key] = value; }

void JSONObject::set_num(std::string key, long double value) { key_to_num[key] = value; }

void JSONObject::set_object(std::string key, JSONObject obj)
{
    key_to_object[key].clear();
    key_to_object[key] = obj;
}

void JSONObject::set_object(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_object[key].clear();
    key_to_object[key] = JSONObject(source, ++start);
}

void JSONObject::set_obj_list(std::string key, std::list<JSONObject> list)
{
    key_to_object_list[key].clear();
    for (auto i : list)
        key_to_object_list[key].push_back(i);
}

void JSONObject::set_obj_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_object_list[key].clear();
    key_to_object_list[key] = pick_obj_list(source, ++start);
}

void JSONObject::set_val_list(std::string key, std::list<std::string> list)
{
    key_to_value_list[key].clear();
    for (auto i : list)
        key_to_value_list[key].push_back(i);
}

void JSONObject::set_val_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_value_list[key] = pick_val_list(source, ++start);
}

void JSONObject::set_bool_list(std::string key, std::list<bool> list)
{
    key_to_boolean_list[key].clear();
    for (auto i : list)
        key_to_boolean_list[key].push_back(i);
}

void JSONObject::set_bool_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_boolean_list[key] =  pick_bool_list(source, ++start);
}

void JSONObject::set_num_list(std::string key, std::list<long double> list)
{
    key_to_num_list[key].clear();
    for (auto i : list)
        key_to_num_list[key].push_back(i);
}

void JSONObject::set_num_list(std::string& source)
{
    size_t start = 0;
    std::string key = pick_val(source, start);
    key_to_num_list[key] = pick_num_list(source, ++start);
}

void JSONObject::erase_value(std::string key) { key_to_value.erase(key); }

void JSONObject::erase_object(std::string key) { key_to_object.erase(key); }

void JSONObject::erase_num(std::string key) { key_to_num.erase(key); }

void JSONObject::erase_bool(std::string key) { key_to_bool.erase(key); }

void JSONObject::erase_val_list(std::string key) { key_to_value_list.erase(key); }

void JSONObject::erase_obj_list(std::string key) { key_to_object_list.erase(key); }

void JSONObject::erase_bool_list(std::string key) { key_to_boolean_list.erase(key); }

void JSONObject::erase_num_list(std::string key) { key_to_num_list.erase(key); }

void JSONObject::clear_bools() { key_to_bool.clear(); }

void JSONObject::clear_nums() { key_to_num.clear(); }

void JSONObject::clear_values() { key_to_value.clear(); }

void JSONObject::clear_objects() { key_to_object.clear(); }

void JSONObject::clear_val_lists() { key_to_value_list.clear(); }

void JSONObject::clear_obj_lists() { key_to_object_list.clear(); }

void JSONObject::clear_num_lists() { key_to_num_list.clear(); }

void JSONObject::clear_bool_lists() { key_to_boolean_list.clear(); }

void JSONObject::clear()
{
    key_to_bool.clear();
    key_to_num.clear();
    key_to_value.clear();
    key_to_object.clear();
    key_to_value_list.clear();
    key_to_object_list.clear();
    key_to_boolean_list.clear();
    key_to_num_list.clear();
}

JSONObject JSONObject::read(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    return JSONObject(source);
}

void JSONObject::read_as_field(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    size_t start = 0;
    std::string key = pick_val(source, start);
    JSONObject tmp(source, ++start);
    key_to_object[key] = tmp;
}

void JSONObject::read_and_overwrite(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
        throw new std::invalid_argument("path invalid");

    std::string source;
    std::string line;
    while (std::getline(in, line))
        source += line;

    JSONObject tmp(source);
    for (auto l : tmp.get_name_to_value())
        key_to_value[l.first] = l.second;

    for (auto l : tmp.get_name_to_object())
        key_to_object[l.first] = l.second;

    for (auto l : tmp.get_name_to_values())
        key_to_value_list[l.first] = l.second;

    for (auto l : tmp.get_name_to_objects())
        key_to_object_list[l.first] = l.second;

}

void JSONObject::write(std::string file_path)
{
    std::ofstream out(file_path);
    out << to_string(0);
    out.close();
}

std::string JSONObject::format(std::string value)
{
    std::string res = "\"";
    for (int i = 0; i < value.size(); i++) {
        if (value[i] == '\"')
            res += "\\\"";
        else if (value[i] == '\'')
            res += "\\\'";
        else if (value[i] == '\t')
            res += "\\t";
        else if (value[i] == '\v')
            res += "\\v";
        else if (value[i] == '\b')
            res += "\\b";
        else if (value[i] == '\n')
            res += "\\n";
        else if (value[i] == '\\')
            res += "\\\\";
        else
            res += value[i];
    }

    res += "\"";
    return res;
}

std::string JSONObject::to_string(int tabs)
{
    std::string spacing(tabs, '\t');
    std::string res = "{\n";

    for (auto& t : key_to_value) {
        res += spacing + format(t.first) + " : " + format(t.second) + '\n';
    }

    for (auto& t : key_to_num) {
        // postfix zeros
        std::string numeric = std::to_string(t.second);
        numeric.erase(numeric.find_last_not_of('0') + 1, numeric.size());
        if (numeric.back() == '.')
            numeric.pop_back();

        res += spacing + format(t.first) + " : " + numeric + '\n';
    }

    for (auto& t : key_to_bool) {
        if (t.second)
            res += spacing + format(t.first) + " : true\n";
        else
            res += spacing + format(t.first) + " : false\n";
    }

    for (auto& t : key_to_value_list) {
        res += spacing + format(t.first) + " : [\n";
        for (auto& inner : t.second) {
            res += spacing + "\t" + format(inner) + "\n";
        }

        res += spacing + "\t]\n";
    }

    for (auto& t : key_to_num_list) {
        res += spacing + format(t.first) + " : [\n";
        for (auto& inner : t.second) {
            // postfix zeros
            std::string numeric = std::to_string(inner);
            numeric.erase(numeric.find_last_not_of('0') + 1, numeric.size());
            if (numeric.back() == '.')
                numeric.pop_back();

            res += spacing + "\t" + numeric + "\n";
        }

        res += spacing + "\t]\n";
    }

    for (auto& t : key_to_boolean_list) {
        res += spacing + format(t.first) + " : [\n";
        for (auto& inner : t.second) {
            if (inner)
                res += spacing + "\ttrue\n";
            else
                res += spacing + "\tfalse\n";
        }

        res += spacing + "\t]\n";
    }

    for (auto& obj : key_to_object) {
        res += spacing + format(obj.first) + " : " + obj.second.to_string(tabs + 1) + "\n";
    }

    for (auto& t : key_to_object_list) {
        res += spacing + format(t.first) + " : [\n";
        for (auto& obj : t.second) {
            res += spacing + '\t' + obj.to_string(tabs + 1) + "\n";
        }

        res += "\t]\n";
    }

    res += spacing + "}";
    return res;
}


