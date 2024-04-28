#include <iostream>

using namespace std;


template <class T>
class CompareDefault{
    public:
    bool operator ()(const T& l, const T& r){
        return l < r;
    }
};

struct Date{
    int day, month, year, delta;
    Date(): day(0), month(0), year(0), delta(0){}
    Date(int day_, int month_, int year_, int delta_):
    day(day_), month(month_), year(year_), delta(delta_){}
};

class DateCompare{
    public:
    bool operator()(const Date& l, const Date& r){
        if (l.year == r.year){
            if (l.month == r.month){
                if (l.day == r.day){
                    return l.delta < r.delta;
                }else{
                    return l.day < r.day;
                }
            }else{
                return l.month < r.month;
            }
        }else{
            return l.year < r.year;
        }
    }
};


template <class T, class Compare>
void merge(T* l, int l_size, T* r, int r_size, T* target, Compare cmp){
    int pl = 0;
    int pr = 0;
    int pt = 0;
    while (pl < l_size && pr < r_size){
        if (cmp(l[pl], r[pr])){
            target[pt] = l[pl];
            pl++;
        }else{
            target[pt] = r[pr];
            pr++;
        }
        pt++;
    }
    while (pl < l_size){
        target[pt] = l[pl];
        pl++;
        pt++;
    }
    while (pr < r_size){
        target[pt] = r[pr];
        pr++;
        pt++;
    }
}

template <class T, class Compare = CompareDefault<T>>
void mergesort(T* mas, int mas_size, Compare cmp = Compare()){
    if (mas_size <= 1){
        return;
    }
    int m = mas_size / 2;
    mergesort(mas, m, cmp);
    mergesort(mas + m, mas_size - m, cmp);
    T* buffer = new T[mas_size];

    merge(mas, m, mas + m, mas_size - m, buffer, cmp);
    for (int i = 0; i < mas_size; i++){
        mas[i] = buffer[i];
    }
    delete [] buffer;
}


int task(Date* mas, int mas_size){
    DateCompare cmp;
    mergesort(mas, mas_size, cmp);
    int max_res = 0;
    int cur_res = 0;
    for (int i = 0; i < mas_size; i++){
        cur_res += mas[i].delta;
        max_res = max(max_res, cur_res);
    }
    return max_res;
}


int main(){
    int n = 0;
    cin >> n;
    Date* mas = new Date[n * 2];
    DateCompare cmp;
    int date_count = 0;
    for (int i = 0; i < n; i++){
        int day_birth = 0;
        int month_birth = 0;
        int year_birth = 0;
        cin >> day_birth >> month_birth >> year_birth;
        int day_death = 0;
        int month_death = 0;
        int year_death = 0;
        cin >> day_death >> month_death >> year_death;
        Date date_18(day_birth, month_birth, year_birth + 18, 1);
        Date date_80(day_birth, month_birth, year_birth + 80, -1);
        Date date_death(day_death, month_death, year_death, -1);
        if (cmp(date_18, date_death)){
            if (cmp(date_80, date_death)){
                mas[date_count++] = date_18;
                mas[date_count++] = date_80;
            }else{
                mas[date_count++] = date_18;
                mas[date_count++] = date_death;
            }
        }
    }
    Date* new_mas = new Date[date_count];
    for (int i = 0; i < date_count; i++){
        new_mas[i] = mas[i];
    }
    delete [] mas;
    mas = new_mas;

    cout << task(mas, date_count);

    delete [] mas;
    return 0;
}