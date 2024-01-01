![MyJson (3)](https://github.com/Crucinio/MyJson/assets/109169672/f3740dd1-cb71-44b4-bddc-78b0caab9bac)
<p style="text-align: center;">This project is a versetile tool for working with JSON data</p>
<h3 style="text-align: center;">About</h3>
- This project is developped to make a versetile tool for reading JSON files and strings that also provides ability to add new key-val pairs via reading from other sources or just adding them through setters.
- JSONObject currently supports processing numeric, string, boolean and JSONObject values, as well as corresponding arrays and infinetely nested objects.
- It can also process files and strings that could not be processed by some other parsers due to sensitivity to minor syntax issues like commas, unnecessary symbols between values and keys.
### Install
Download Json.h, Json.cpp and include it in the files you need.
    Note that the lib was tested only on MSVC compiler
### Usage
- Initialising object:
![image](https://github.com/Crucinio/MyJson/assets/109169672/fc35cf9f-8f59-481e-b821-97a5e6ee5517)
- Accessing values
![image](https://github.com/Crucinio/MyJson/assets/109169672/c253bb8c-cb11-4b1f-9772-1bb14bdc52b3)
- Setting new value with premade pair:
![image](https://github.com/Crucinio/MyJson/assets/109169672/6f1eda0f-89ec-4986-bd8a-76e8068c65d3)
- Using setter with string source as argument:
![image](https://github.com/Crucinio/MyJson/assets/109169672/7a5c2e62-fcf4-4096-b0f0-9c11d708282c)
The example above obviously contains unnecessary symbols, but still works fine due to insensetivity to minor syntax problems.
- There are clear, empty and erase(by key) implemented as well.

#### Currently working on
- Writing in bin and txt files (write, rewrite);
- Reading from bin files (read, reread));
- to_string method;
- documentation (since 31.12.2023);
