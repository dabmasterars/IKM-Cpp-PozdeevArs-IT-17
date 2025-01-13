#include <iostream>
#include <algorithm>
using namespace std;
bool numbercheck(string &x){//СОСТОИТ ЛИ СТРОКА ТОЛЬКО ИЗ ЦИФР
    return x.find_first_not_of("0123456789") == string::npos;
}
bool isGreaterOrEqual(string a, string b) {//БОЛЬШЕ ИЛИ РАВНО
    if (a.size() != b.size())
        return a.size() > b.size();
    return a >= b;
}
string add(string a, string b){//СЛОЖЕНИЕ, КАК В СТОЛБИК
    int maxi = max(a.length(), b.length());
    int mini = min(a.length(), b.length());
    int carry = 0, sum;
    int num1, num2;
    int i=0;
    string res = "";//ХРАНЕНИЕ ИТОГОВОЙ СУММЫ
    while((a[i]=='0' || b[i]=='0') && i<mini){//ЕСЛИ a ИЛИ b РАВНЫ НУЛЮ, ПЕРЕПИСЫВАЕМ b ИЛИ a СООТВЕТСТВЕННО
        if(a[i]=='0') res = res + b[i];
        else res = res + a[i];
        i++;
    }
    for(; i<maxi; i++){
        num1 = 0; num2 = 0;//ЦИФРЫ ДЛЯ СЛОЖЕНИЯ
        if(i<a.length()) num1 = (int)a[i] - 48;//ЕСЛИ ИНДЕКС МЕНЬШЕ ДЛИНЫ a, ПОЛУЧАЕМ ТЕКУЩУЮ ЦИФРУ ИЗ ЧИСЛА a, ПРЕОБРАЗУЕМ ЕЁ В ЧИСЛО, 48 - ЭТО 0 В ASCII
        if(i<b.length()) num2 = (int)b[i] - 48;//ЕСЛИ ИНДЕКС МЕНЬШЕ ДЛИНЫ b, ПОЛУЧАЕМ ТЕКУЩУЮ ЦИФРУ ИЗ ЧИСЛА b, ПРЕОБРАЗУЕМ ЕЁ В ЧИСЛО, 48 - ЭТО 0 В ASCII
        sum = num1 + num2 + carry;//СУММА ЦИФР ПЛЮС ПЕРЕНОС
        carry = sum/10;//ЕСЛИ СУММА >=10, СЧИТАЕМ ПЕРЕНОС
        res = res + (char)(sum%10 + 48);//ПОСЛЕДНЯЯ ЦИФРА СУММЫ ПРЕОБРАЗУЕТСЯ В СИМВОЛ И ДОБАВЛЯЕТСЯ В ИТОГОВУЮ СУММУ
    }
    if(carry>0)res = res + (char)(carry + 48);//ЕСЛИ ОСТАЛСЯ ПЕРЕНОС, ДОБАВЛЯЕМ ЕГО В КОНЕЦ РЕЗУЛЬТАТА
    return res;
}
string subtract(string a, string b) {//ВЫЧИТАНИЕ, КАК В СТОЛБИК, ПРЕДПОЛАГАЕМ, ЧТО a >= b
    string res = "";//РЕЗУЛЬТАТ
    int carry = 0;
    reverse(a.begin(), a.end());//ПЕРЕВОРАЧИВАЕМ ДЛЯ УДОБСТВА
    reverse(b.begin(), b.end());
    for (size_t i = 0; i < a.size(); i++) {//ВЫЧИТАЕМ ПО ЦИФРАМ
        int digitA = a[i] - '0';
        int digitB = (i < b.size()) ? (b[i] - '0') : 0;
        int sub = digitA - digitB - carry;//ВЫЧИТАНИЕ ЦИФР МИНУС ПЕРЕНОС
        if (sub<0) {//ЕСЛИ РАЗНОСТЬ МЕНЬШЕ 0, ДОБАВЛЯЕМ 10 И ДЕЛАЕМ ПЕРЕНОС
            sub += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        res += (sub + '0');//РАЗНОСТЬ ЦИФР ДОБАВЛЯЕТСЯ В ОБЩУЮ РАЗНОСТЬ
    }
    while (res.size() > 1 && res.back() == '0') {//УДАЛЯЕМ НУЛИ
        res.pop_back();
    }
    reverse(res.begin(), res.end());
    return res;
}
string multiply(string a, string b){//УМНОЖЕНИЕ, КАК В СТОЛБИК
    reverse(a.begin(), a.end());//ПЕРЕВОРАЧИВАЕМ ДЛЯ УДОБСТВА
    reverse(b.begin(), b.end());
    int maxi = a.length();
    int maxj = b.length();
    int j;
    int carry=0; int num;
    string res = "", addStr="", zeroStr ="";//ГЛАВНЫЙ РЕЗУЛЬТАТ, ПРОМЕЖУТОЧНЫЙ РЕЗУЛЬТАТ, СТРОКА ДЛЯ ДОБАВЛЕНИЯ НУЛЕЙ
    for(int i=0; i<maxi; i++){//ПРОХОДИМ ЧЕРЕЗ ВСЕ ЦИФРЫ a
        carry=0;//ПЕРЕНОС СБРАСЫВАЕТСЯ
        for(j =0; j< maxj; j++){//УМНОЖАЕМ a НА ВСЕ ЦИФРЫ b
            num = carry + ((int)a[i] - 48)*((int)b[j] - 48);//УМНОЖАЕМ ЦИФРЫ И ДОБАВЛЯЕМ ПЕРЕНОС
            addStr = addStr + (char)(num%10 + 48);
            carry = num/10;
        }
        if(carry>0)addStr = addStr + (char)(carry + 48);//ЕСЛИ ОСТАЛСЯ ПЕРЕНОС, ДОБВАЛЯЕМ ЕГО
        res = add(addStr, res);//ДОБАВЛЯЕМ ПРОМЕЖУТОЧНЫЙ РЕЗУЛЬТАТ В ГЛАВНЫЙ
        zeroStr = zeroStr + "0";
        addStr = zeroStr;//ДОБАВЛЯЕМ НУЛИ ДЛЯ ПРАВИЛЬНОЙ РАЗРЯДНОСТИ
    }
    reverse(res.begin(), res.end());//РАЗВОРАЧИВАЕМ РЕЗУЛЬТАТ ОБРАТНО
    while(res.length() > 0 && res[0]=='0') res = res.substr(1);
    if(res == "") return "0";
    return res;
}
string divide(string a, string b) {//ДЕЛЕНИЕ, КАК В СТОЛБИК
    string res = "";//РЕЗУЛЬТАТ
    string current = "";//ВРЕМЕННАЯ СТРОКА (ЧАСТЬ ЧИСЛА a)
    for (int i = 0; i < a.size(); i++) {//ПРОХОДИМ ЧЕРЕЗ ВСЕ ЦИФРЫ a
        current += a[i];//ДОБАВЛЯЕМ СИМВОЛ К ДЕЛИМОМУ
        int count = 0;//РАЗНОСТЬ
        while (current.size() > 1 && current[0] == '0') {
            current.erase(current.begin());//УДАЛЯЕМ НУЛИ
        }
        while (current.size() > 0 && (current.size() > b.size() || (current.size() == b.size() && current >= b))) {
            current = subtract(current, b);//ВЫЧИТАЕМ b ИЗ ВРЕМЕННОЙ СТРОКИ
            count++;
        }
        res += to_string(count);//ДОБАВЛЯЕМ СЧЁТЧИК К РЕЗУЛЬТАТУ
    }
    while (res.size() > 1 && res[0] == '0') {//УДАЛЯЕМ НУЛИ
        res.erase(res.begin());
    }
    return res.empty() ? "0" : res;
}
string gcd(string a, string b) {//НОД, НУЖЕН ДЛЯ НАХОЖДЕНИЯ НОК. ИСПОЛЬЗУЕТ АЛГОРИТМ ЕВКЛИДА
    while (b != "0") {//ЕСЛИ b РАВЕН 0, ТО a - НОД
        string temp = b;
        b = subtract(a, b);
        if (isGreaterOrEqual(temp, b)) {//СРАВНИВАЕМ БУФЕР И ЧИСЛО, МЕНЯЕМ МЕСТАМИ ЕСЛИ ПЕРВОЕ МЕНЬШЕ ВТОРОГО
            a = temp;
        } else {
            a = b;
            b = temp;
        }
    }
    return a;
}